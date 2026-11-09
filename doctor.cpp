#include "globals.h"
#include <cctype>


bool isValidDoctorName(const string& name) {

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

            if (
                !isalpha(
                    static_cast<unsigned char>(
                        name[i - 1]
                    )
                )
                ||
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

    if (
        name.front() == ' ' ||
        name.back() == ' '
    )
        return false;

    return true;
}



bool isValidSpecialization(
    const string& specialization
) {

    if (specialization.empty())
        return false;

    bool hasLetter = false;


    for (char ch : specialization) {

        unsigned char uch =
            static_cast<unsigned char>(ch);

        if (isalpha(uch)) {

            hasLetter = true;
        }

        else if (
            ch == ' ' ||
            ch == '-' ||
            ch == '/' ||
            ch == '&' ||
            ch == '.'
        ) {

            
        }

        else {

            return false;
        }
    }


    if (!hasLetter)
        return false;


    if (
        specialization.front() == ' ' ||
        specialization.back() == ' '
    ) {

        return false;
    }


    return true;
}

string normalizeDoctorPhone(
    const string& phone
) {

    string digits;


    for (char ch : phone) {

        if (
            isdigit(
                static_cast<unsigned char>(ch)
            )
        ) {

            digits += ch;
        }
    }


    return digits;
}


bool isValidDoctorPhone(
    const string& phone
) {

    if (phone.empty())
        return false;

    int digitCount = 0;


    for (
        size_t i = 0;
        i < phone.size();
        i++
    ) {

        char ch = phone[i];


        if (
            isdigit(
                static_cast<unsigned char>(ch)
            )
        ) {

            digitCount++;
        }

        else if (
            ch == '+' &&
            i == 0
        ) {

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


bool doctorPhoneExists(
    const string& phone,
    int ignoreIndex = -1
) {

    string normalized =
        normalizeDoctorPhone(phone);


    if (normalized.empty())
        return false;


    for (
        int i = 0;
        i < static_cast<int>(
                doctors.size()
            );
        i++
    ) {

        if (i == ignoreIndex)
            continue;


        if (
            normalizeDoctorPhone(
                doctors[i].phone
            )
            ==
            normalized
        ) {

            return true;
        }
    }


    return false;
}


string generateNextDoctorId() {

    int highestId = 0;


    for (
        const Doctor& doctor :
        doctors
    ) {

        if (doctor.id.size() <= 1)
            continue;


        if (
            toupper(
                static_cast<unsigned char>(
                    doctor.id[0]
                )
            ) != 'D'
        ) {

            continue;
        }


        try {

            int number =
                stoi(
                    doctor.id.substr(1)
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
        << 'D'
        << setw(4)
        << setfill('0')
        << (highestId + 1);


    return oss.str();
}


int getDayIndexFromDate(
    const string& date
) {

    int year;
    int month;
    int day;


    if (
        !parseDate(
            date,
            year,
            month,
            day
        )
    ) {

        return -1;
    }


    tm timeStruct = {};


    timeStruct.tm_year =
        year - 1900;

    timeStruct.tm_mon =
        month - 1;

    timeStruct.tm_mday =
        day;

    timeStruct.tm_hour = 12;


    mktime(
        &timeStruct
    );


    return (
        timeStruct.tm_wday + 6
    ) % 7;
}


string getMondayOfWeek(
    const string& date
) {

    int year;
    int month;
    int day;


    if (
        !parseDate(
            date,
            year,
            month,
            day
        )
    ) {

        return "";
    }


    tm timeStruct = {};


    timeStruct.tm_year =
        year - 1900;

    timeStruct.tm_mon =
        month - 1;

    timeStruct.tm_mday =
        day;

    timeStruct.tm_hour = 12;


    mktime(
        &timeStruct
    );


    int mondayOffset =
        (timeStruct.tm_wday + 6) % 7;


    timeStruct.tm_mday -=
        mondayOffset;


    mktime(
        &timeStruct
    );


    ostringstream oss;


    oss
        << put_time(
            &timeStruct,
            "%Y-%m-%d"
        );


    return oss.str();
}


string addDaysToDate(
    const string& date,
    int days
) {

    int year;
    int month;
    int day;


    if (
        !parseDate(
            date,
            year,
            month,
            day
        )
    ) {

        return "";
    }


    tm timeStruct = {};


    timeStruct.tm_year =
        year - 1900;

    timeStruct.tm_mon =
        month - 1;

    timeStruct.tm_mday =
        day;

    timeStruct.tm_hour = 12;


    mktime(
        &timeStruct
    );


    timeStruct.tm_mday +=
        days;


    mktime(
        &timeStruct
    );


    ostringstream oss;


    oss
        << put_time(
            &timeStruct,
            "%Y-%m-%d"
        );


    return oss.str();
}


int getTimeSlotIndex(
    const string& timeSlot
) {

    for (
        int i = 0;
        i < MAX_SLOTS;
        i++
    ) {

        if (
            TIME_SLOTS[i]
            ==
            timeSlot
        ) {

            return i;
        }
    }


    return -1;
}


bool doctorHasActiveAppointments(
    const string& doctorId
) {

    for (
        const Appointment& appointment :
        appointments
    ) {

        if (
            appointment.doctorId
            ==
            doctorId
            &&
            toUpper(
                appointment.status
            )
            !=
            "CANCELLED"
        ) {

            return true;
        }
    }


    return false;
}




void displayDoctorRecord(
    const Doctor& doctor
) {

    printLine();


    cout
        << left

        << setw(20)
        << "Doctor ID"
        << ": "
        << doctor.id
        << "\n"

        << setw(20)
        << "Name"
        << ": Dr. "
        << doctor.name
        << "\n"

        << setw(20)
        << "Specialization"
        << ": "
        << doctor.specialization
        << "\n"

        << setw(20)
        << "Phone"
        << ": "
        << doctor.phone
        << "\n"

        << setw(20)
        << "Consult Fee"
        << ": RM "
        << fixed
        << setprecision(2)
        << doctor.consultFee
        << "\n"

        << setw(20)
        << "Status"
        << ": "
        << (
            doctor.available
                ? "Available"
                : "Unavailable"
        )
        << "\n";


    printLine();
}




void addDoctor() {

    cout
        << "\n--- Add Doctor ---\n";


    Doctor doctor{};


    doctor.id =
        generateNextDoctorId();




    while (true) {

        doctor.name =
            readString(
                "  Full Name         : "
            );


        if (
            toUpper(
                doctor.name
            )
            ==
            "X"
        ) {

            cout
                << "  Registration cancelled.\n";

            return;
        }


        if (
            isValidDoctorName(
                doctor.name
            )
        ) {

            break;
        }


        cout
            << "  [!] Invalid name. "
            << "Use letters, spaces, hyphen "
            << "or apostrophe only.\n";
    }




    while (true) {

        doctor.specialization =
            readString(
                "  Specialization    : "
            );


        if (
            toUpper(
                doctor.specialization
            )
            ==
            "X"
        ) {

            cout
                << "  Registration cancelled.\n";

            return;
        }


        if (
            isValidSpecialization(
                doctor.specialization
            )
        ) {

            break;
        }


        cout
            << "  [!] Invalid specialization.\n";
    }




    while (true) {

        doctor.phone =
            readString(
                "  Phone             : "
            );


        if (
            toUpper(
                doctor.phone
            )
            ==
            "X"
        ) {

            cout
                << "  Registration cancelled.\n";

            return;
        }


        if (
            !isValidDoctorPhone(
                doctor.phone
            )
        ) {

            cout
                << "  [!] Enter a valid phone "
                << "number containing 9-15 digits.\n";

            continue;
        }


        if (
            doctorPhoneExists(
                doctor.phone
            )
        ) {

            cout
                << "  [!] This phone number is already "
                << "registered to another doctor.\n";

            continue;
        }


        break;
    }




    doctor.consultFee =
        readDouble(
            "  Consultation Fee RM: ",
            0.0
        );



    doctor.available =
        true;


    cout
        << "\n"
        << "  Please confirm doctor information:\n";


    displayDoctorRecord(
        doctor
    );


    cout
        << "  1. Confirm & Add\n"
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
            << "  Registration cancelled.\n";

        return;
    }


    doctors.push_back(
        doctor
    );


    saveDoctors();


    cout
        << "\n"
        << "  [OK] Doctor added successfully.\n"
        << "  Doctor ID: "
        << doctor.id
        << "\n";
}





void searchDoctor() {

    while (true) {

        cout
            << "\n--- Search Doctor ---\n"

            << "  1. By Doctor ID\n"
            << "  2. By Name\n"
            << "  3. By Specialization\n"
            << "  4. By Phone\n"
            << "  0. Back\n";


        int searchMode =
            readInt(
                "  Select: ",
                0,
                4
            );


        if (
            searchMode == 0
        ) {

            return;
        }


        string searchTerm;


        switch (searchMode) {

            case 1:

                searchTerm =
                    readString(
                        "  Doctor ID (X to cancel): "
                    );

                break;


            case 2:

                searchTerm =
                    readString(
                        "  Name (X to cancel): "
                    );

                break;


            case 3:

                searchTerm =
                    readString(
                        "  Specialization (X to cancel): "
                    );

                break;


            case 4:

                searchTerm =
                    readString(
                        "  Phone (X to cancel): "
                    );

                break;
        }


        if (
            toUpper(
                searchTerm
            )
            ==
            "X"
        ) {

            continue;
        }


        bool found = false;


        for (
            const Doctor& doctor :
            doctors
        ) {

            bool match = false;


            switch (searchMode) {

                case 1:

                    match =
                        toUpper(
                            doctor.id
                        )
                        ==
                        toUpper(
                            searchTerm
                        );

                    break;


                case 2:

                    match =
                        toUpper(
                            doctor.name
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
                        toUpper(
                            doctor.specialization
                        )
                        .find(
                            toUpper(
                                searchTerm
                            )
                        )
                        != string::npos;

                    break;


                case 4:

                    match =
                        normalizeDoctorPhone(
                            doctor.phone
                        )
                        ==
                        normalizeDoctorPhone(
                            searchTerm
                        );

                    break;
            }


            if (match) {

                displayDoctorRecord(
                    doctor
                );

                found = true;
            }
        }


        if (!found) {

            cout
                << "  [!] No doctor found.\n";
        }


        pause();
    }
}




void updateDoctor() {

    cout
        << "\n--- Update Doctor Record ---\n";


    string doctorId =
        readString(
            "  Doctor ID (0 to cancel): "
        );


    if (
        doctorId == "0"
    ) {

        return;
    }


    int doctorIndex = -1;


    if (
        !doctorExists(
            doctorId,
            doctorIndex
        )
    ) {

        cout
            << "  [!] Doctor not found.\n";

        return;
    }


    Doctor& doctor =
        doctors[
            doctorIndex
        ];


    displayDoctorRecord(
        doctor
    );


    cout
        << "  1. Full Name\n"
        << "  2. Specialization\n"
        << "  3. Phone\n"
        << "  4. Consultation Fee\n"
        << "  5. Availability\n"
        << "  0. Back\n";


    int option =
        readInt(
            "  Select: ",
            0,
            5
        );


    if (
        option == 0
    ) {

        return;
    }


    switch (option) {



        case 1: {

            while (true) {

                string newName =
                    readString(
                        "  New Full Name (X to cancel): "
                    );


                if (
                    toUpper(
                        newName
                    )
                    ==
                    "X"
                ) {

                    return;
                }


                if (
                    isValidDoctorName(
                        newName
                    )
                ) {

                    doctor.name =
                        newName;

                    break;
                }


                cout
                    << "  [!] Invalid name.\n";
            }

            break;
        }




        case 2: {

            while (true) {

                string newSpecialization =
                    readString(
                        "  New Specialization "
                        "(X to cancel): "
                    );


                if (
                    toUpper(
                        newSpecialization
                    )
                    ==
                    "X"
                ) {

                    return;
                }


                if (
                    isValidSpecialization(
                        newSpecialization
                    )
                ) {

                    doctor.specialization =
                        newSpecialization;

                    break;
                }


                cout
                    << "  [!] Invalid specialization.\n";
            }

            break;
        }




        case 3: {

            while (true) {

                string newPhone =
                    readString(
                        "  New Phone (X to cancel): "
                    );


                if (
                    toUpper(
                        newPhone
                    )
                    ==
                    "X"
                ) {

                    return;
                }


                if (
                    !isValidDoctorPhone(
                        newPhone
                    )
                ) {

                    cout
                        << "  [!] Enter a valid phone "
                        << "number containing 9-15 digits.\n";

                    continue;
                }


                if (
                    doctorPhoneExists(
                        newPhone,
                        doctorIndex
                    )
                ) {

                    cout
                        << "  [!] This phone number "
                        << "belongs to another doctor.\n";

                    continue;
                }


                doctor.phone =
                    newPhone;

                break;
            }

            break;
        }




        case 4: {

            doctor.consultFee =
                readDouble(
                    "  New Consultation Fee RM: ",
                    0.0
                );

            break;
        }




        case 5: {

            cout
                << "  Current status: "
                << (
                    doctor.available
                        ? "Available"
                        : "Unavailable"
                )
                << "\n";


            cout
                << "  1. Available\n"
                << "  2. Unavailable\n"
                << "  0. Cancel\n";


            int status =
                readInt(
                    "  Select: ",
                    0,
                    2
                );


            if (
                status == 0
            ) {

                return;
            }


            doctor.available =
                status == 1;


            break;
        }
    }


    saveDoctors();


    cout
        << "\n"
        << "  [OK] Doctor record updated successfully.\n";


    displayDoctorRecord(
        doctor
    );
}




void listDoctors() {

    cout
        << "\n--- All Doctors ---\n";


    if (
        doctors.empty()
    ) {

        cout
            << "  No doctors registered.\n";

        return;
    }


    printLine();


    cout
        << left

        << setw(8)
        << "ID"

        << setw(38)
        << "Name"

        << setw(32)
        << "Specialization"

        << "Status\n";


    printLine();


    for (
        const Doctor& doctor :
        doctors
    ) {

        cout
            << left

            << setw(8)
            << doctor.id

            << setw(38)
            << (
                "Dr. "
                + doctor.name
            )

            << setw(32)
            << doctor.specialization

            << (
                doctor.available
                    ? "Available"
                    : "Unavailable"
            )
            << "\n";
    }


    printLine();


    cout
        << "  Total: "
        << doctors.size()
        << " doctor(s)\n";
}




void updateDoctorAvailability() {

    cout
        << "\n--- Toggle Doctor Availability ---\n";


    if (
        doctors.empty()
    ) {

        cout
            << "  No doctors registered.\n";

        return;
    }


    listDoctors();


    string doctorId =
        readString(
            "  Doctor ID (0 to cancel): "
        );


    if (
        doctorId == "0"
    ) {

        return;
    }


    int doctorIndex = -1;


    if (
        !doctorExists(
            doctorId,
            doctorIndex
        )
    ) {

        cout
            << "  [!] Doctor not found.\n";

        return;
    }


    Doctor& doctor =
        doctors[
            doctorIndex
        ];


    cout
        << "\n"
        << "  Doctor: Dr. "
        << doctor.name
        << "\n"

        << "  Current Status: "
        << (
            doctor.available
                ? "Available"
                : "Unavailable"
        )
        << "\n";


    cout
        << "  1. Confirm Toggle\n"
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
            << "  Cancelled.\n";

        return;
    }


    doctor.available =
        !doctor.available;


    saveDoctors();


    cout
        << "  [OK] New status: "
        << (
            doctor.available
                ? "Available"
                : "Unavailable"
        )
        << "\n";
}




void viewDoctorSchedule() {

    cout
        << "\n--- Doctor Weekly Schedule ---\n";


    if (
        doctors.empty()
    ) {

        cout
            << "  No doctors registered.\n";

        return;
    }


    listDoctors();


    string doctorId =
        readString(
            "  Doctor ID (0 to cancel): "
        );


    if (
        doctorId == "0"
    ) {

        return;
    }


    int doctorIndex = -1;


    if (
        !doctorExists(
            doctorId,
            doctorIndex
        )
    ) {

        cout
            << "  [!] Doctor not found.\n";

        return;
    }


    string selectedDate =
        readString(
            "  Any date in desired week "
            "(YYYY-MM-DD, 0 to cancel): "
        );


    if (
        selectedDate == "0"
    ) {

        return;
    }


    if (
        !isValidDate(
            selectedDate
        )
    ) {

        cout
            << "  [!] Invalid date. "
            << "Use YYYY-MM-DD.\n";

        return;
    }


    string monday =
        getMondayOfWeek(
            selectedDate
        );


    if (
        monday.empty()
    ) {

        cout
            << "  [!] Unable to calculate week.\n";

        return;
    }


    clearScheduleGrid();

    string weekDates[MAX_DAYS];


    for (
        int day = 0;
        day < MAX_DAYS;
        day++
    ) {

        weekDates[day] =
            addDaysToDate(
                monday,
                day
            );
    }


    for (
        const Appointment& appointment :
        appointments
    ) {

        if (
            toUpper(
                appointment.doctorId
            )
            !=
            toUpper(
                doctorId
            )
        ) {

            continue;
        }


        if (
            toUpper(
                appointment.status
            )
            ==
            "CANCELLED"
        ) {

            continue;
        }


        int dayIndex = -1;


        for (
            int day = 0;
            day < MAX_DAYS;
            day++
        ) {

            if (
                appointment.date
                ==
                weekDates[day]
            ) {

                dayIndex =
                    day;

                break;
            }
        }


        if (
            dayIndex < 0
        ) {

            continue;
        }


        int slotIndex =
            getTimeSlotIndex(
                appointment.timeSlot
            );


        if (
            slotIndex < 0
        ) {

            continue;
        }


        scheduleGrid
            [dayIndex]
            [slotIndex]
            =
            appointment.id
            + " ("
            + appointment.patientId
            + ")";
    }


    printLine(
        '=',
        125
    );


    printCentered(
        "Weekly Schedule - Dr. "
        + doctors[
            doctorIndex
        ].name,
        125
    );


    cout
        << "  Week starting Monday: "
        << monday
        << "\n";


    printLine(
        '=',
        125
    );


    cout
        << left
        << setw(16)
        << "Time Slot";


    for (
        int day = 0;
        day < MAX_DAYS;
        day++
    ) {

        cout
            << setw(15)
            << (
                DAY_NAMES[day].substr(0, 3)
                + " "
                + weekDates[day].substr(5)
            );
    }


    cout
        << "\n";


    printLine(
        '-',
        125
    );


    for (
        int slot = 0;
        slot < MAX_SLOTS;
        slot++
    ) {

        cout
            << left
            << setw(16)
            << TIME_SLOTS[slot];


        for (
            int day = 0;
            day < MAX_DAYS;
            day++
        ) {

            string display =
                scheduleGrid[day][slot];


            if (
                display.empty()
            ) {

                display =
                    doctorIndex >= 0 &&
                    !doctors[
                        doctorIndex
                    ].available
                        ? "UNAVAILABLE"
                        : "FREE";
            }


            if (
                display.size() > 14
            ) {

                display =
                    display.substr(
                        0,
                        13
                    )
                    + ".";
            }


            cout
                << setw(15)
                << display;
        }


        cout
            << "\n";
    }


    printLine(
        '=',
        125
    );
}




void viewDoctorAppointments() {

    cout
        << "\n--- Doctor Appointment History ---\n";


    if (
        doctors.empty()
    ) {

        cout
            << "  No doctors registered.\n";

        return;
    }


    listDoctors();


    string doctorId =
        readString(
            "  Doctor ID (0 to cancel): "
        );


    if (
        doctorId == "0"
    ) {

        return;
    }


    int doctorIndex = -1;


    if (
        !doctorExists(
            doctorId,
            doctorIndex
        )
    ) {

        cout
            << "  [!] Doctor not found.\n";

        return;
    }


    cout
        << "\n"
        << "  Doctor: Dr. "
        << doctors[
            doctorIndex
        ].name
        << "\n";


    cout
        << "  1. All Appointments\n"
        << "  2. Upcoming / Active\n"
        << "  3. Completed\n"
        << "  4. Cancelled\n"
        << "  0. Back\n";


    int filter =
        readInt(
            "  Select: ",
            0,
            4
        );


    if (
        filter == 0
    ) {

        return;
    }


    printLine();


    cout
        << left

        << setw(10)
        << "Appt ID"

        << setw(14)
        << "Patient"

        << setw(14)
        << "Date"

        << setw(14)
        << "Time"

        << setw(16)
        << "Status"

        << "Notes\n";


    printLine();


    bool found = false;


    for (
        const Appointment& appointment :
        appointments
    ) {

        if (
            toUpper(
                appointment.doctorId
            )
            !=
            toUpper(
                doctorId
            )
        ) {

            continue;
        }


        string status =
            toUpper(
                appointment.status
            );


        bool include = false;


        switch (filter) {

            case 1:

                include = true;

                break;


            case 2:

                include =
                    status != "COMPLETED" &&
                    status != "CANCELLED";

                break;


            case 3:

                include =
                    status == "COMPLETED";

                break;


            case 4:

                include =
                    status == "CANCELLED";

                break;
        }


        if (!include)
            continue;


        found = true;


        string notes =
            appointment.notes;


        if (
            notes.size() > 22
        ) {

            notes =
                notes.substr(
                    0,
                    21
                )
                + ".";
        }


        cout
            << left

            << setw(10)
            << appointment.id

            << setw(14)
            << appointment.patientId

            << setw(14)
            << appointment.date

            << setw(14)
            << appointment.timeSlot

            << setw(16)
            << appointment.status

            << notes
            << "\n";
    }


    printLine();


    if (!found) {

        cout
            << "  No matching appointments found.\n";
    }
}




void deleteDoctor() {

    cout
        << "\n--- Delete Doctor ---\n";


    string doctorId =
        readString(
            "  Doctor ID (0 to cancel): "
        );


    if (
        doctorId == "0"
    ) {

        return;
    }


    int doctorIndex = -1;


    if (
        !doctorExists(
            doctorId,
            doctorIndex
        )
    ) {

        cout
            << "  [!] Doctor not found.\n";

        return;
    }


    displayDoctorRecord(
        doctors[
            doctorIndex
        ]
    );


    if (
        doctorHasActiveAppointments(
            doctorId
        )
    ) {

        cout
            << "\n"
            << "  [!] Deletion blocked.\n"

            << "      This doctor has active or "
            << "historical appointment records.\n"

            << "      Deleting the doctor would break "
            << "appointment references.\n"

            << "      Please cancel/reassign the "
            << "appointments first.\n";

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


    doctors.erase(
        doctors.begin()
        +
        doctorIndex
    );


    saveDoctors();


    cout
        << "  [OK] Doctor deleted successfully.\n";
}




void doctorMenu() {

    int choice;


    do {

        clearScreen();


        cout
            << "\n";


        printLine('=');


        printCentered(
            "DOCTOR & SCHEDULE MANAGEMENT"
        );


        printLine('=');


        cout
            << "  1. Add Doctor\n"
            << "  2. Search Doctor\n"
            << "  3. Update Doctor\n"
            << "  4. List Doctors\n"
            << "  5. Toggle Availability\n"
            << "  6. View Weekly Schedule\n"
            << "  7. View Doctor Appointments\n"
            << "  8. Delete Doctor\n"
            << "  0. Back\n";


        printLine();


        choice =
            readInt(
                "  Choice: ",
                0,
                8
            );


        clearScreen();


        switch (choice) {

            case 1:
                addDoctor();
                break;

            case 2:
                searchDoctor();
                break;

            case 3:
                updateDoctor();
                break;

            case 4:
                listDoctors();
                break;

            case 5:
                updateDoctorAvailability();
                break;

            case 6:
                viewDoctorSchedule();
                break;

            case 7:
                viewDoctorAppointments();
                break;

            case 8:
                deleteDoctor();
                break;
        }


        if (
            choice != 0
        ) {

            pause();
        }


    } while (
        choice != 0
    );
}