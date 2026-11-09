#include "globals.h"
#include <cctype>


void displayPatientRecord(const Patient& patient) {
    printLine();

    cout << left
         << setw(20) << "Patient ID"
         << ": " << patient.id << "\n"

         << setw(20) << "Full Name"
         << ": " << patient.name << "\n"

         << setw(20) << "Age"
         << ": " << patient.age << "\n"

         << setw(20) << "Gender"
         << ": "
         << (patient.gender == 'M' ? "Male" : (patient.gender == 'F' ? "Female" : "Prefer not to say"))
         << "\n"

         << setw(20) << "Phone"
         << ": " << patient.phone << "\n"

         << setw(20) << "Address"
         << ": " << patient.address << "\n"

         << setw(20) << "Blood Type"
         << ": " << patient.bloodType << "\n"

         << setw(20) << "Registered On"
         << ": " << patient.registrationDate << "\n";

    printLine();
}

string normalizePatientPhone(const string& phone) {
    string digits;

    for (char ch : phone) {
        if (isdigit(static_cast<unsigned char>(ch))) {
            digits += ch;
        }
    }

    return digits;
}

bool isValidPatientName(const string& name) {
    if (name.empty())
        return false;

    bool hasLetter = false;
    bool previousSpace = false;

    for (size_t i = 0; i < name.size(); i++) {

        char ch = name[i];

        unsigned char uch =
            static_cast<unsigned char>(ch);

        if (isalpha(uch)) {

            hasLetter = true;
            previousSpace = false;
        }

        else if (ch == ' ') {

            if (previousSpace)
                return false;

            previousSpace = true;
        }

        else if (ch == '-' || ch == '\'') {

            if (i == 0 ||
                i == name.size() - 1)
                return false;

            if (!isalpha(
                    static_cast<unsigned char>(
                        name[i - 1]
                    )
                ) ||
                !isalpha(
                    static_cast<unsigned char>(
                        name[i + 1]
                    )
                )
            ) {
                return false;
            }

            previousSpace = false;
        }

        else {
            return false;
        }
    }

    if (!hasLetter)
        return false;

    if (name.front() == ' ' ||
        name.back() == ' ')
        return false;

    return true;
}


bool isValidPatientPhone(const string& phone) {

    if (phone.empty())
        return false;

    int digitCount = 0;

    for (size_t i = 0;
         i < phone.size();
         i++) {

        char ch = phone[i];

        if (isdigit(
                static_cast<unsigned char>(ch)
            )
        ) {

            digitCount++;
        }

        else if (ch == '+' && i == 0) {

           
        }

        else if (
            ch == '-' ||
            ch == ' ' ||
            ch == '(' ||
            ch == ')'
        ) {

         
        }

        else {

            return false;
        }
    }

    return digitCount >= 9 &&
           digitCount <= 15;
}

bool patientPhoneExists(
    const string& phone,
    int ignoreIndex = -1
) {

    const string normalizedPhone =
        normalizePatientPhone(phone);

    if (normalizedPhone.empty())
        return false;

    for (
        int i = 0;
        i < static_cast<int>(patients.size());
        i++
    ) {

        if (i == ignoreIndex)
            continue;

        if (
            normalizePatientPhone(
                patients[i].phone
            )
            ==
            normalizedPhone
        ) {

            return true;
        }
    }

    return false;
}

string generateNextPatientId() {

    int highestId = 0;

    for (
        const Patient& patient :
        patients
    ) {

        if (patient.id.size() <= 1)
            continue;

        if (
            toupper(
                static_cast<unsigned char>(
                    patient.id[0]
                )
            ) != 'P'
        ) {
            continue;
        }

        try {

            int number =
                stoi(
                    patient.id.substr(1)
                );

            highestId =
                max(
                    highestId,
                    number
                );
        }

        catch (...) {

        }
    }

    ostringstream oss;

    oss
        << 'P'
        << setw(4)
        << setfill('0')
        << (highestId + 1);

    return oss.str();
}

bool patientHasLinkedRecords(
    const string& patientId
) {

    for (
        const Appointment& appointment :
        appointments
    ) {

        if (
            appointment.patientId
            ==
            patientId
        ) {

            return true;
        }
    }


    for (
        const Bill& bill :
        bills
    ) {

        if (
            bill.patientId
            ==
            patientId
        ) {

            return true;
        }
    }

    return false;
}

const string MALAYSIAN_STATES[16] = {
    "Johor", "Kedah", "Kelantan", "Melaka", "Negeri Sembilan",
    "Pahang", "Penang", "Perak", "Perlis", "Sabah",
    "Sarawak", "Selangor", "Terengganu",
    "WP Kuala Lumpur", "WP Labuan", "WP Putrajaya"
};

bool isValidPostcode(const string& postcode) {
    if (postcode.length() != 5) return false;
    for (char c : postcode) {
        if (!isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

void promptMalaysiaAddress(string& outAddress) {
    cout << "\n  Address\n";

    string street;
    while (true) {
        street = readString("  Street Address / Unit : ");
        if (street.find('|') != string::npos) {
            cout << "  [!] Address cannot contain '|' character.\n";
            continue;
        }
        break;
    }

    string city;
    while (true) {
        city = readString("  City / Town           : ");
        if (city.find('|') != string::npos) {
            cout << "  [!] City cannot contain '|' character.\n";
            continue;
        }
        break;
    }

    string postcode;
    while (true) {
        postcode = readString("  Postcode (5 digits)   : ");
        if (!isValidPostcode(postcode)) {
            cout << "  [!] Malaysian postcode must be exactly 5 digits (e.g. 50450, 10150).\n";
            continue;
        }
        break;
    }

    cout << "\n  Select State / Federal Territory:\n";
    for (int i = 0; i < 16; i++) {
        cout << "   " << right << setw(2) << (i + 1) << ". " << left << setw(22) << MALAYSIAN_STATES[i];
        if ((i + 1) % 2 == 0) cout << "\n";
    }

    int stateChoice = readInt("  Select State (1-16): ", 1, 16);
    string state = MALAYSIAN_STATES[stateChoice - 1];

    outAddress = street + ", " + postcode + " " + city + ", " + state;
    cout << "  Address set to: " << outAddress << "\n";
}



void registerPatient() {

    cout
        << "\n--- Register New Patient ---\n";


    Patient patient{};


    patient.id =
        generateNextPatientId();

    while (true) {

        patient.name =
            readString(
                "  Full Name       : "
            );


        if (
            toUpper(patient.name)
            ==
            "X"
        ) {

            cout
                << "  Registration cancelled.\n";

            return;
        }


        if (
            isValidPatientName(
                patient.name
            )
        ) {

            break;
        }


        cout
            << "  [!] Name may contain letters, "
            << "spaces, hyphen (-) and apostrophe (').\n";
    }

    patient.age =
        readInt(
            "  Age             : ",
            0,
            130
        );


    while (true) {

        cout
            << "  Gender (M/F/-)  : ";

        string genderInput;

        getline(
            cin,
            genderInput
        );


        if (
            genderInput == "0" ||
            toUpper(genderInput) == "X"
        ) {

            cout
                << "  Registration cancelled.\n";

            return;
        }


        if (
            genderInput.size() == 1
        ) {

            patient.gender =
                toupper(
                    static_cast<unsigned char>(
                        genderInput[0]
                    )
                );
        }

        else {

            patient.gender = ' ';
        }


        if (
            patient.gender == 'M' ||
            patient.gender == 'F' ||
            patient.gender == '-'
        ) {

            break;
        }


        cout
            << "  [!] Enter M, F, or - (Prefer not to say). "
            << "Enter X to cancel.\n";
    }

    while (true) {

        patient.phone =
            readString(
                "  Phone Number    : "
            );


        if (
            toUpper(patient.phone)
            ==
            "X"
        ) {

            cout
                << "  Registration cancelled.\n";

            return;
        }


        if (
            !isValidPatientPhone(
                patient.phone
            )
        ) {

            cout
                << "  [!] Enter a valid phone "
                << "number containing 9-15 digits.\n";

            continue;
        }


        if (
            patientPhoneExists(
                patient.phone
            )
        ) {

            cout
                << "  [!] This phone number is already "
                << "registered to another patient.\n";

            continue;
        }


        break;
    }

    promptMalaysiaAddress(patient.address);
    
    const string validBloodTypes[] = {
        "A+",
        "A-",
        "B+",
        "B-",
        "AB+",
        "AB-",
        "O+",
        "O-"
    };


    while (true) {

        patient.bloodType =
            toUpper(
                readString(
                    "  Blood Type (A+, B-, O+...): "
                )
            );


        if (
            patient.bloodType
            ==
            "X"
        ) {

            cout
                << "  Registration cancelled.\n";

            return;
        }


        bool valid = false;


        for (
            const string& bloodType :
            validBloodTypes
        ) {

            if (
                patient.bloodType
                ==
                bloodType
            ) {

                valid = true;
                break;
            }
        }


        if (valid)
            break;


        cout
            << "  [!] Invalid blood type. "
            << "Use A+, A-, B+, B-, AB+, AB-, O+ or O-.\n";
    }

    patient.registrationDate =
        getCurrentDate();

    cout
        << "\n"
        << "  Please confirm the patient information:\n";

    displayPatientRecord(
        patient
    );


    cout
        << "  1. Confirm & Register\n"
        << "  0. Cancel Registration\n";


    int confirmation =
        readInt(
            "  Choice: ",
            0,
            1
        );


    if (confirmation == 0) {

        cout
            << "  Registration cancelled.\n";

        return;
    }

    patients.push_back(
        patient
    );

    savePatients();


    cout
        << "\n"
        << "  [OK] Patient registered successfully.\n"
        << "  Patient ID: "
        << patient.id
        << "\n";
}




void searchPatient() {

    while (true) {

        cout
            << "\n--- Search Patient ---\n"

            << "  1. By Patient ID\n"
            << "  2. By Name\n"
            << "  3. By Phone\n"
            << "  0. Back\n";


        int searchMode =
            readInt(
                "  Select: ",
                0,
                3
            );


        if (searchMode == 0)
            return;


        string searchTerm;


        if (searchMode == 1) {

            searchTerm =
                readString(
                    "  Patient ID (X to cancel): "
                );
        }

        else if (searchMode == 2) {

            searchTerm =
                readString(
                    "  Name (X to cancel): "
                );
        }

        else {

            searchTerm =
                readString(
                    "  Phone (X to cancel): "
                );
        }


        if (
            toUpper(searchTerm)
            ==
            "X"
        ) {

            continue;
        }


        bool found = false;


        for (
            const Patient& patient :
            patients
        ) {

            bool match = false;


            switch (searchMode) {

                case 1:

                    match =
                        toUpper(
                            patient.id
                        )
                        ==
                        toUpper(
                            searchTerm
                        );

                    break;


                case 2:

                    match =
                        toUpper(
                            patient.name
                        )
                        .find(
                            toUpper(
                                searchTerm
                            )
                        )
                        != string::npos;

                    break;


                case 3:

                    match =
                        normalizePatientPhone(
                            patient.phone
                        )
                        ==
                        normalizePatientPhone(
                            searchTerm
                        );

                    break;
            }


            if (match) {

                displayPatientRecord(
                    patient
                );

                found = true;
            }
        }


        if (!found) {

            cout
                << "  [!] No patient found.\n";
        }


        pause();
    }
}




void updatePatient() {

    cout
        << "\n--- Update Patient Record ---\n";


    string patientId =
        readString(
            "  Patient ID (0 to cancel): "
        );


    if (patientId == "0")
        return;


    int patientIndex = -1;


    if (
        !patientExists(
            patientId,
            patientIndex
        )
    ) {

        cout
            << "  [!] Patient not found.\n";

        return;
    }


    Patient& patient =
        patients[
            patientIndex
        ];


    displayPatientRecord(
        patient
    );


    cout
        << "  1. Full Name\n"
        << "  2. Phone\n"
        << "  3. Address\n"
        << "  4. Age\n"
        << "  5. Gender\n"
        << "  6. Blood Type\n"
        << "  0. Back\n";


    int updateOption =
        readInt(
            "  Select: ",
            0,
            6
        );


    if (updateOption == 0)
        return;


    switch (updateOption) {

        case 1: {

            while (true) {

                string newName =
                    readString(
                        "  New Full Name (X to cancel): "
                    );


                if (
                    toUpper(newName)
                    ==
                    "X"
                ) {

                    return;
                }


                if (
                    isValidPatientName(
                        newName
                    )
                ) {

                    patient.name =
                        newName;

                    break;
                }


                cout
                    << "  [!] Invalid name. "
                    << "Letters, spaces, hyphen "
                    << "and apostrophe only.\n";
            }

            break;
        }

        case 2: {

            while (true) {

                string newPhone =
                    readString(
                        "  New Phone (X to cancel): "
                    );


                if (
                    toUpper(newPhone)
                    ==
                    "X"
                ) {

                    return;
                }


                if (
                    !isValidPatientPhone(
                        newPhone
                    )
                ) {

                    cout
                        << "  [!] Enter a valid phone "
                        << "number containing 9-15 digits.\n";

                    continue;
                }


                if (
                    patientPhoneExists(
                        newPhone,
                        patientIndex
                    )
                ) {

                    cout
                        << "  [!] This phone number "
                        << "belongs to another patient.\n";

                    continue;
                }


                patient.phone =
                    newPhone;

                break;
            }

            break;
        }

        case 3: {

            promptMalaysiaAddress(patient.address);
            break;
        }

        case 4:

            patient.age =
                readInt(
                    "  New Age (0-130): ",
                    0,
                    130
                );

            break;


        case 5: {

            while (true) {

                cout
                    << "  New Gender "
                    << "(M/F/-, X to cancel): ";


                string genderInput;

                getline(
                    cin,
                    genderInput
                );


                if (
                    genderInput == "X" ||
                    genderInput == "x"
                ) {

                    return;
                }


                if (
                    genderInput.size() == 1
                ) {

                    char gender =
                        toupper(
                            static_cast<unsigned char>(
                                genderInput[0]
                            )
                        );


                    if (
                        gender == 'M' ||
                        gender == 'F' ||
                        gender == '-'
                    ) {

                        patient.gender =
                            gender;

                        break;
                    }
                }


                cout
                    << "  [!] Enter M, F, or - only.\n";
            }

            break;
        }

        case 6: {

            const string validBloodTypes[] = {
                "A+",
                "A-",
                "B+",
                "B-",
                "AB+",
                "AB-",
                "O+",
                "O-"
            };


            while (true) {

                string newBloodType =
                    toUpper(
                        readString(
                            "  New Blood Type "
                            "(X to cancel): "
                        )
                    );


                if (
                    newBloodType
                    ==
                    "X"
                ) {

                    return;
                }


                bool valid = false;


                for (
                    const string& bloodType :
                    validBloodTypes
                ) {

                    if (
                        newBloodType
                        ==
                        bloodType
                    ) {

                        valid = true;
                        break;
                    }
                }


                if (valid) {

                    patient.bloodType =
                        newBloodType;

                    break;
                }


                cout
                    << "  [!] Invalid blood type.\n";
            }

            break;
        }
    }


    savePatients();


    cout
        << "\n"
        << "  [OK] Patient record "
        << "updated successfully.\n";


    displayPatientRecord(
        patient
    );
}




void listAllPatients() {

    cout
        << "\n--- All Patients ---\n";


    if (patients.empty()) {

        cout
            << "  No patients registered.\n";

        return;
    }


    printLine();


    cout
        << left
        << setw(8)
        << "ID"

        << setw(25)
        << "Name"

        << setw(5)
        << "Age"

        << setw(20)
        << "Gender"

        << setw(20)
        << "Phone"

        << "Blood Type\n";


    printLine();


    for (
        const Patient& patient :
        patients
    ) {

        cout
            << left

            << setw(8)
            << patient.id

            << setw(25)
            << patient.name

            << setw(5)
            << patient.age

            << setw(20)
            << (
                patient.gender == 'M'
                    ? "Male"
                    : (patient.gender == 'F'
                        ? "Female"
                        : "Prefer not to say")
            )

            << setw(20)
            << patient.phone

            << patient.bloodType
            << "\n";
    }


    printLine();


    cout
        << "  Total: "
        << patients.size()
        << " patient(s)\n";
}




void viewPatientAppointmentHistory() {

    cout
        << "\n--- Patient Appointment History ---\n";


    string patientId =
        readString(
            "  Patient ID (0 to cancel): "
        );


    if (patientId == "0")
        return;


    int patientIndex = -1;


    if (
        !patientExists(
            patientId,
            patientIndex
        )
    ) {

        cout
            << "  [!] Patient not found.\n";

        return;
    }


    const Patient& patient =
        patients[
            patientIndex
        ];


    printLine('=');

    printCentered(
        "PATIENT APPOINTMENT HISTORY"
    );

    printLine('=');


    cout
        << left
        << setw(20)
        << "Patient ID"
        << ": "
        << patient.id
        << "\n"

        << setw(20)
        << "Patient Name"
        << ": "
        << patient.name
        << "\n";


    printLine('-');


    cout
        << left

        << setw(10)
        << "Appt ID"

        << setw(28)
        << "Doctor"

        << setw(16)
        << "Date"

        << setw(16)
        << "Time"

        << "Status\n";


    printLine('-');


    bool found = false;
    int appointmentCount = 0;


    for (
        const Appointment& appointment :
        appointments
    ) {

        if (
            toUpper(
                appointment.patientId
            )
            !=
            toUpper(
                patient.id
            )
        ) {

            continue;
        }


        found = true;
        appointmentCount++;


        string doctorName =
            "Unknown";


        int doctorIndex = -1;


        if (
            doctorExists(
                appointment.doctorId,
                doctorIndex
            )
        ) {

            doctorName =
                "Dr. "
                +
                doctors[
                    doctorIndex
                ].name;
        }


        cout
            << left

            << setw(10)
            << appointment.id

            << setw(28)
            << doctorName

            << setw(16)
            << appointment.date

            << setw(16)
            << appointment.timeSlot

            << appointment.status
            << "\n";
    }


    printLine('-');


    if (!found) {

        cout
            << "  No appointment history "
            << "found for this patient.\n";
    }

    else {

        cout
            << "  Total appointments: "
            << appointmentCount
            << "\n";
    }


    printLine('=');
}




void deletePatient() {

    cout
        << "\n--- Delete Patient ---\n";


    string patientId =
        readString(
            "  Patient ID (0 to cancel): "
        );


    if (patientId == "0")
        return;


    int patientIndex = -1;


    if (
        !patientExists(
            patientId,
            patientIndex
        )
    ) {

        cout
            << "  [!] Patient not found.\n";

        return;
    }


    displayPatientRecord(
        patients[
            patientIndex
        ]
    );


    if (
        patientHasLinkedRecords(
            patients[
                patientIndex
            ].id
        )
    ) {

        cout
            << "\n"
            << "  [!] Deletion blocked.\n"

            << "      This patient has linked "
            << "appointment or billing records.\n"

            << "      Historical records are protected "
            << "to prevent broken references.\n"

            << "      Please update the patient record "
            << "instead.\n";

        return;
    }


    cout
        << "\n"
        << "  1. Confirm Delete\n"
        << "  0. Cancel\n";


    int confirmation =
        readInt(
            "  Choice: ",
            0,
            1
        );


    if (
        confirmation == 0
    ) {

        cout
            << "  Deletion cancelled.\n";

        return;
    }


    patients.erase(
        patients.begin()
        +
        patientIndex
    );


    savePatients();


    cout
        << "  [OK] Patient deleted successfully.\n";
}




void patientMenu() {

    int menuChoice;


    do {

        clearScreen();


        cout
            << "\n";


        printLine('=');


        printCentered(
            "PATIENT MANAGEMENT"
        );


        printLine('=');


        cout
            << "  1. Register Patient\n"
            << "  2. Search Patient\n"
            << "  3. Update Patient Record\n"
            << "  4. List All Patients\n"
            << "  5. View Patient Appointment History\n"
            << "  6. Delete Patient\n"
            << "  0. Back\n";


        printLine();


        menuChoice =
            readInt(
                "  Choice: ",
                0,
                6
            );


        clearScreen();


        switch (menuChoice) {

            case 1:
                registerPatient();
                break;

            case 2:
                searchPatient();
                break;

            case 3:
                updatePatient();
                break;

            case 4:
                listAllPatients();
                break;

            case 5:
                viewPatientAppointmentHistory();
                break;

            case 6:
                deletePatient();
                break;
        }


        if (
            menuChoice != 0
        ) {

            pause();
        }


    } while (
        menuChoice != 0
    );
}