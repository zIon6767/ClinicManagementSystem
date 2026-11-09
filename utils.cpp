#include "globals.h"
#include <regex>

// Platform detection — used by clearScreen() below.
// _WIN32 is defined by MSVC and MinGW on all Windows targets (32- and 64-bit).
#ifdef _WIN32
#  include <windows.h>   // HANDLE, GetStdHandle, CONSOLE_SCREEN_BUFFER_INFO, etc.
#endif

string getCurrentDate() {
    time_t now = time(nullptr);
    tm*    t   = localtime(&now);
    char   buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", t);
    return string(buf);
}

// clearScreen — Clears the terminal on both Windows and macOS/Linux.
// On Windows, system("cls") invokes the native CLS command, which works in
// both the legacy CMD window and modern Windows Terminal.
// On macOS/Linux, ANSI escape sequence \033[2J (erase screen) + \033[H
// (move cursor to top-left) achieves the same effect.
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[H" << flush;
#endif
}

void printLine(char ch, int width) {
    cout << string(width, ch) << "\n";
}

void printCentered(const string& text, int width) {
    int padding = (width - (int)text.size()) / 2;
    if (padding > 0) cout << string(padding, ' ');
    cout << text << "\n";
}

void printBanner() {
    printLine('=');
    printCentered(CLINIC_NAME);
    printCentered(CLINIC_ADDR);
    printCentered(CLINIC_PHONE);
    printLine('=');
}

int readInt(const string& prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Enter a number between " << minVal << " and " << maxVal << ".\n";
    }
}

double readDouble(const string& prompt, double minVal) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Value must be >= " << minVal << ".\n";
    }
}

string readString(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        size_t start = s.find_first_not_of(" \t");
        size_t end   = s.find_last_not_of(" \t");
        if (start != string::npos) {
            s = s.substr(start, end - start + 1);
            return s;
        }
        cout << "  [!] Input cannot be empty.\n";
    }
}

string toUpper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

bool isValidPhoneNumber(const string& phone) {
    if (phone.empty()) return false;
    if (phone.find_first_not_of("0123456789-") != string::npos) return false;
    return phone.find_first_of("0123456789") != string::npos;
}

bool isValidName(const string& name) {
    bool hasLetter = false;
    for (unsigned char character : name) {
        if (isalpha(character)) {
            hasLetter = true;
        } else if (character != ' ' && character != '-' && character != '\'') {
            return false;
        }
    }
    return hasLetter;
}

bool parseDate(const string& date, int& year, int& month, int& day) {
    regex datePattern(R"(^([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})$)");
    smatch match;
    if (!regex_match(date, match, datePattern)) return false;
    year  = stoi(match[1].str());
    month = stoi(match[2].str());
    day   = stoi(match[3].str());
    return true;
}

bool isValidDate(const string& date) {
    int year, month, day;
    if (!parseDate(date, year, month, day)) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    static const int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2) {
        bool leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
        if (day > (leap ? 29 : 28)) return false;
    } else {
        if (day > daysInMonth[month]) return false;
    }
    return true;
}

bool isFutureOrTodayDate(const string& date) {
    if (!isValidDate(date)) return false;
    string today = getCurrentDate();
    return date >= today;
}

string normalizeDate(const string& date) {
    int year, month, day;
    if (!parseDate(date, year, month, day)) return date;
    ostringstream oss;
    oss << setw(4) << setfill('0') << year << '-'
        << setw(2) << setfill('0') << month << '-'
        << setw(2) << setfill('0') << day;
    return oss.str();
}

void pause() {
    cout << "\n  Press Enter to continue...";
    string dummy;
    getline(cin, dummy);
}

bool patientExists(const string& id, int& idx) {
    for (int i = 0; i < (int)patients.size(); i++)
        if (patients[i].id == id) { idx = i; return true; }
    return false;
}

bool doctorExists(const string& id, int& idx) {
    for (int i = 0; i < (int)doctors.size(); i++)
        if (doctors[i].id == id) { idx = i; return true; }
    return false;
}

bool appointmentExists(const string& id, int& idx) {
    for (int i = 0; i < (int)appointments.size(); i++)
        if (appointments[i].id == id) { idx = i; return true; }
    return false;
}

bool medicineExists(const string& code, int& idx) {
    for (int i = 0; i < (int)medicines.size(); i++)
        if (medicines[i].code == code) { idx = i; return true; }
    return false;
}

string generateId(const string& prefix, int count) {
    ostringstream oss;
    oss << prefix << setw(4) << setfill('0') << (count + 1);
    return oss.str();
}

void clearScheduleGrid() {
    for (int d = 0; d < MAX_DAYS; d++)
        for (int s = 0; s < MAX_SLOTS; s++)
            scheduleGrid[d][s] = "";
}
