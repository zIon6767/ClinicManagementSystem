#include "globals.h"

void printAppointmentSlip(const Appointment& appointment) {
    int patientIndex, doctorIndex;
    string patientName = "N/A", doctorName = "N/A";
    if (patientExists(appointment.patientId, patientIndex)) patientName = patients[patientIndex].name;
    if (doctorExists(appointment.doctorId,   doctorIndex)) doctorName = "Dr. " + doctors[doctorIndex].name;
    printLine('*');
    printCentered("** APPOINTMENT CONFIRMATION SLIP **");
    printLine('*');
    cout << left
         << setw(22) << "Appointment ID" << ": " << appointment.id       << "\n"
         << setw(22) << "Patient"        << ": " << patientName      << " (" << appointment.patientId << ")\n"
         << setw(22) << "Doctor"         << ": " << doctorName      << " (" << appointment.doctorId  << ")\n"
         << setw(22) << "Date"           << ": " << appointment.date     << "\n"
         << setw(22) << "Time"           << ": " << appointment.timeSlot << "\n"
         << setw(22) << "Status"         << ": " << appointment.status   << "\n";
    if (!appointment.notes.empty())
        cout << setw(22) << "Notes" << ": " << appointment.notes << "\n";
    printLine('*');
}

bool isSlotBooked(const string& doctorId, const string& date, const string& slot) {
    for (auto& appointment : appointments)
        if (appointment.doctorId == doctorId && appointment.date == date &&
            appointment.timeSlot == slot     && appointment.status != "Cancelled")
            return true;
    return false;
}

void bookAppointment() {
    cout << "\n--- Book Appointment ---\n";
    if (patients.empty()) { cout << "  [!] No patients registered.\n"; return; }
    if (doctors.empty())  { cout << "  [!] No doctors registered.\n";  return; }

    listAllPatients();
    cout << "  0. Back\n";
    string patientId = readString("  Patient ID: ");
    if (patientId == "0") return;
    int patientIndex;
    if (!patientExists(patientId, patientIndex)) { cout << "  [!] Patient not found.\n"; return; }

    listDoctors();
    cout << "  0. Back\n";
    string doctorId = readString("  Doctor ID: ");
    if (doctorId == "0") return;
    int doctorIndex;
    if (!doctorExists(doctorId, doctorIndex)) { cout << "  [!] Doctor not found.\n"; return; }
    if (!doctors[doctorIndex].available)   { cout << "  [!] Doctor unavailable.\n"; return; }

    string date;
    do {
        date = readString("  Date (YYYY-MM-DD): ");
        if (!isValidDate(date)) 
            cout << "  [!] Invalid date format.\n";
        else if (!isFutureOrTodayDate(date))
            cout << "  [!] Appointment date must be today or in the future.\n";
    } while (!isValidDate(date) || !isFutureOrTodayDate(date));
    date = normalizeDate(date);

    cout << "\n  Available Slots:\n";
    bool anyFree = false;
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (!isSlotBooked(doctorId, date, TIME_SLOTS[i])) {
            cout << "  " << (i + 1) << ". " << TIME_SLOTS[i] << "\n";
            anyFree = true;
        }
    }
    if (!anyFree) { cout << "  [!] All slots booked for this date.\n"; return; }

    int slotNum = readInt("  Slot number (1-8): ", 1, MAX_SLOTS);
    string slot = TIME_SLOTS[slotNum - 1];
    if (isSlotBooked(doctorId, date, slot)) { cout << "  [!] Slot taken.\n"; return; }

    string notes;
    cout << "  Notes (Enter to skip): ";
    getline(cin, notes);

    Appointment appointment;
    appointment.id        = generateId("A", (int)appointments.size());
    appointment.patientId = patientId;
    appointment.doctorId  = doctorId;
    appointment.date      = date;
    appointment.timeSlot  = slot;
    appointment.status    = "Pending";
    appointment.notes     = notes;
    appointments.push_back(appointment);
    saveAppointments();
    cout << "\n  [OK] Appointment booked!\n";
    printAppointmentSlip(appointment);
}

void cancelAppointment() {
    cout << "\n--- Cancel Appointment ---\n";
    cout << "  0. Back\n";
    string appointmentId = readString("  Appointment ID: ");
    if (appointmentId == "0") return;
    int appointmentIndex;
    if (!appointmentExists(appointmentId, appointmentIndex)) { cout << "  [!] Not found.\n"; return; }
    if (appointments[appointmentIndex].status == "Cancelled") { cout << "  [!] Already cancelled.\n"; return; }
    printAppointmentSlip(appointments[appointmentIndex]);
    cout << "  Confirm? (Y/N): ";
    char confirmation; cin >> confirmation; cin.ignore();
    if (toupper(confirmation) == 'Y') {
        appointments[appointmentIndex].status = "Cancelled";
        saveAppointments();
        cout << "  [OK] Cancelled.\n";
    }
}

void rescheduleAppointment() {
    cout << "\n--- Reschedule Appointment ---\n";
    cout << "  0. Back\n";
    string appointmentId = readString("  Appointment ID: ");
    if (appointmentId == "0") return;
    int appointmentIndex;
    if (!appointmentExists(appointmentId, appointmentIndex)) { cout << "  [!] Not found.\n"; return; }
    if (appointments[appointmentIndex].status == "Cancelled") {
        cout << "  [!] Cannot reschedule a cancelled appointment.\n"; return;
    }
    printAppointmentSlip(appointments[appointmentIndex]);

    string newDate;
    do {
        newDate = readString("  New Date (YYYY-MM-DD): ");
        if (!isValidDate(newDate)) 
            cout << "  [!] Invalid date.\n";
        else if (!isFutureOrTodayDate(newDate))
            cout << "  [!] Appointment date must be today or in the future.\n";
    } while (!isValidDate(newDate) || !isFutureOrTodayDate(newDate));
    newDate = normalizeDate(newDate);

    cout << "  Available Slots on " << newDate << ":\n";
    for (int i = 0; i < MAX_SLOTS; i++)
        if (!isSlotBooked(appointments[appointmentIndex].doctorId, newDate, TIME_SLOTS[i]))
            cout << "  " << (i + 1) << ". " << TIME_SLOTS[i] << "\n";

    int slotNum    = readInt("  New slot (1-8): ", 1, MAX_SLOTS);
    string newSlot = TIME_SLOTS[slotNum - 1];
    if (isSlotBooked(appointments[appointmentIndex].doctorId, newDate, newSlot)) {
        cout << "  [!] Slot taken.\n"; return;
    }
    appointments[appointmentIndex].date     = newDate;
    appointments[appointmentIndex].timeSlot = newSlot;
    saveAppointments();
    cout << "  [OK] Rescheduled.\n";
    printAppointmentSlip(appointments[appointmentIndex]);
}

void completeAppointment() {
    cout << "\n--- Mark as Completed ---\n";
    cout << "  0. Back\n";
    string appointmentId = readString("  Appointment ID: ");
    if (appointmentId == "0") return;
    int appointmentIndex;
    if (!appointmentExists(appointmentId, appointmentIndex)) { cout << "  [!] Not found.\n"; return; }
    if (appointments[appointmentIndex].status != "Pending") {
        cout << "  [!] Only Pending appointments can be completed.\n"; return;
    }
    appointments[appointmentIndex].status = "Completed";
    saveAppointments();
    cout << "  [OK] Marked as Completed.\n";
}

void listAppointments() {
    cout << "\n--- All Appointments ---\n";
    if (appointments.empty()) { cout << "  No appointments.\n"; return; }

    size_t wId = 2, wPat = 7, wDoc = 6, wDate = 4, wSlot = 9, wStatus = 6;
    for (auto& a : appointments) {
        wId     = max(wId,     a.id.size());
        wPat    = max(wPat,    a.patientId.size());
        wDoc    = max(wDoc,    a.doctorId.size());
        wDate   = max(wDate,   a.date.size());
        wSlot   = max(wSlot,   a.timeSlot.size());
        wStatus = max(wStatus, a.status.size());
    }
    wId += 2; wPat += 2; wDoc += 2; wDate += 2; wSlot += 2;

    printLine();
    cout << left << setw(wId)     << "ID"
                 << setw(wPat)    << "Patient"
                 << setw(wDoc)    << "Doctor"
                 << setw(wDate)   << "Date"
                 << setw(wSlot)   << "Time Slot"
                 << "Status\n";
    printLine();
    for (auto& a : appointments)
        cout << left << setw(wId)   << a.id
                     << setw(wPat)  << a.patientId
                     << setw(wDoc)  << a.doctorId
                     << setw(wDate) << a.date
                     << setw(wSlot) << a.timeSlot
                     << a.status << "\n";
    printLine();
    cout << "  Total: " << appointments.size() << "\n";
}

void appointmentMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n";
        printLine('=');
        printCentered("APPOINTMENT MANAGEMENT");
        printLine('=');
        cout << "  1. Book Appointment\n"
             << "  2. Cancel Appointment\n"
             << "  3. Reschedule Appointment\n"
             << "  4. Mark as Completed\n"
             << "  5. List All Appointments\n"
             << "  0. Back\n";
        printLine();
        choice = readInt("  Choice: ", 0, 5);
        clearScreen();
        switch (choice) {
            case 1: bookAppointment();       break;
            case 2: cancelAppointment();     break;
            case 3: rescheduleAppointment(); break;
            case 4: completeAppointment();   break;
            case 5: listAppointments();      break;
        }
        if (choice != 0) pause();
    } while (choice != 0);
}
