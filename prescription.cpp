#include "globals.h"

void printPrescription(const Prescription& pr) {
    int aIdx;
    string patName = "N/A", docName = "N/A";
    if (appointmentExists(pr.appointmentId, aIdx)) {
        int pIdx, dIdx;
        if (patientExists(appointments[aIdx].patientId, pIdx))
            patName = patients[pIdx].name;
        if (doctorExists(appointments[aIdx].doctorId, dIdx))
            docName = "Dr. " + doctors[dIdx].name;
    }
    printLine('=');
    printCentered("*** PRESCRIPTION ***");
    printCentered(CLINIC_NAME);
    printLine('=');
    cout << left
         << setw(22) << "Prescription ID" << ": " << pr.id            << "\n"
         << setw(22) << "Appointment"     << ": " << pr.appointmentId  << "\n"
         << setw(22) << "Patient"         << ": " << patName           << "\n"
         << setw(22) << "Prescribing Dr"  << ": " << docName           << "\n";
    printLine('-');
    cout << left << setw(5) << "No." << setw(12) << "Code"
                 << setw(25) << "Medicine" << setw(6) << "Qty" << "Dosage\n";
    printLine('-');
    for (int i = 0; i < (int)pr.items.size(); i++) {
        int mIdx; string mName = "Unknown";
        if (medicineExists(pr.items[i].medicineCode, mIdx))
            mName = medicines[mIdx].name;
        cout << left << setw(5)  << (i + 1)
                     << setw(12) << pr.items[i].medicineCode
                     << setw(25) << mName
                     << setw(6)  << pr.items[i].qty
                     << pr.items[i].dosage << "\n";
    }
    printLine('-');
    cout << "Doctor's Notes: " << pr.doctorNotes << "\n";
    printLine('=');
}

void createPrescription() {
    cout << "\n--- Create Prescription ---\n";
    cout << "  0. Back\n";
    string appointmentId = readString("  Appointment ID: ");
    if (appointmentId == "0") return;
    int appointmentIndex;
    if (!appointmentExists(appointmentId, appointmentIndex)) { cout << "  [!] Not found.\n"; return; }
    if (appointments[appointmentIndex].status != "Completed") {
        cout << "  [!] Appointment must be Completed first.\n"; return;
    }
    for (auto& prescription : prescriptions)
        if (prescription.appointmentId == appointmentId) { cout << "  [!] Prescription already exists.\n"; return; }
    if (medicines.empty()) { cout << "  [!] No medicines in inventory.\n"; return; }

    Prescription prescription;
    prescription.id            = generateId("RX", (int)prescriptions.size());
    prescription.appointmentId = appointmentId;

    char addMore = 'Y';
    while (toupper(addMore) == 'Y') {
        cout << "\n  Medicines:\n";
        cout << left << setw(10) << "Code" << setw(25) << "Name"
                     << setw(10) << "Stock" << "Price (RM)\n";
        printLine();
        for (auto& medicine : medicines)
            cout << left << setw(10) << medicine.code << setw(25) << medicine.name
                         << setw(10) << medicine.quantity
                         << fixed << setprecision(2) << medicine.unitPrice << "\n";
        string code = toUpper(readString("  Medicine Code: "));
        int medicineIndex;
        if (!medicineExists(code, medicineIndex)) {
            cout << "  [!] Not found.\n";
        } else if (medicines[medicineIndex].quantity <= 0) {
            cout << "  [!] Out of stock.\n";
        } else {
            PrescriptionItem item;
            item.medicineCode = code;
            item.qty    = readInt("  Quantity: ", 1, medicines[medicineIndex].quantity);
            item.dosage = readString("  Dosage: ");
            prescription.items.push_back(item);
        }
        cout << "  Add another? (Y/N): ";
        cin >> addMore; cin.ignore();
    }

    prescription.doctorNotes = readString("  Doctor's Notes: ");
    
    for (auto& item : prescription.items) {
        int medicineIndex;
        if (medicineExists(item.medicineCode, medicineIndex)) {
            medicines[medicineIndex].quantity -= item.qty;
        }
    }
    
    prescriptions.push_back(prescription);
    savePrescriptions();
    saveMedicines();
    cout << "\n  [OK] Prescription created.\n";
    printPrescription(prescription);
    cout << "\n  Generating bill...\n";
    generateBill(appointmentId);
}

void viewPrescription() {
    cout << "\n--- View Prescription ---\n";
    cout << "  0. Back\n";
    string prescriptionId = readString("  Prescription ID or Appointment ID: ");
    if (prescriptionId == "0") return;
    bool found = false;
    for (auto& prescription : prescriptions)
        if (prescription.id == prescriptionId || prescription.appointmentId == prescriptionId) { printPrescription(prescription); found = true; }
    if (!found) cout << "  [!] Not found.\n";
}

void prescriptionMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n";
        printLine('=');
        printCentered("PRESCRIPTION MANAGEMENT");
        printLine('=');
        cout << "  1. Create Prescription\n"
             << "  2. View Prescription\n"
             << "  0. Back\n";
        printLine();
        choice = readInt("  Choice: ", 0, 2);
        clearScreen();
        switch (choice) {
            case 1: createPrescription(); break;
            case 2: viewPrescription();   break;
        }
        if (choice != 0) pause();
    } while (choice != 0);
}
