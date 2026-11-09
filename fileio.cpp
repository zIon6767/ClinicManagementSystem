#include "globals.h"

void savePatients() {
    ofstream f("patients.txt");
    for (auto& p : patients)
        f << p.id << "|" << p.name << "|" << p.age << "|" << p.gender
          << "|" << p.phone << "|" << p.address << "|"
          << p.bloodType << "|" << p.registrationDate << "\n";
}

static void loadPatients() {
    ifstream f("patients.txt");
    if (!f) return;
    string line;
    int lineNum = 0;
    while (getline(f, line)) {
        lineNum++;
        if (line.empty()) continue;
        try {
            istringstream ss(line);
            Patient patient; string ageStr;
            getline(ss, patient.id,               '|');
            getline(ss, patient.name,             '|');
            getline(ss, ageStr,                '|'); patient.age    = stoi(ageStr);
            getline(ss, ageStr,                '|'); patient.gender = ageStr[0];
            getline(ss, patient.phone,            '|');
            getline(ss, patient.address,          '|');
            getline(ss, patient.bloodType,        '|');
            getline(ss, patient.registrationDate, '|');
            patients.push_back(patient);
        } catch (const exception& e) {
            cerr << "  [WARNING] Skipped corrupted patient record at line " << lineNum << ": " << e.what() << "\n";
        }
    }
}

void saveDoctors() {
    ofstream f("doctors.txt");
    for (auto& doctor : doctors)
        f << doctor.id << "|" << doctor.name << "|" << doctor.specialization << "|"
          << doctor.phone << "|" << doctor.consultFee << "|" << doctor.available << "\n";
}

static void loadDoctors() {
    ifstream f("doctors.txt");
    if (!f) return;
    string line;
    int lineNum = 0;
    while (getline(f, line)) {
        lineNum++;
        if (line.empty()) continue;
        try {
            istringstream ss(line);
            Doctor doctor; string consultFeeStr;
            getline(ss, doctor.id,             '|');
            getline(ss, doctor.name,           '|');
            getline(ss, doctor.specialization, '|');
            getline(ss, doctor.phone,          '|');
            getline(ss, consultFeeStr, '|'); doctor.consultFee = stod(consultFeeStr);
            getline(ss, consultFeeStr, '|'); doctor.available  = (consultFeeStr == "1");
            doctors.push_back(doctor);
        } catch (const exception& e) {
            cerr << "  [WARNING] Skipped corrupted doctor record at line " << lineNum << ": " << e.what() << "\n";
        }
    }
}

void saveAppointments() {
    ofstream f("appointments.txt");
    for (auto& a : appointments)
        f << a.id << "|" << a.patientId << "|" << a.doctorId << "|"
          << a.date << "|" << a.timeSlot << "|" << a.status << "|"
          << a.notes << "\n";
}

static void loadAppointments() {
    ifstream f("appointments.txt");
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        Appointment a;
        getline(ss, a.id,        '|');
        getline(ss, a.patientId, '|');
        getline(ss, a.doctorId,  '|');
        getline(ss, a.date,      '|');
        getline(ss, a.timeSlot,  '|');
        getline(ss, a.status,    '|');
        getline(ss, a.notes,     '|');
        appointments.push_back(a);
    }
}

void saveMedicines() {
    ofstream f("medicines.txt");
    for (auto& medicine : medicines)
        f << medicine.code << "|" << medicine.name << "|" << medicine.quantity << "|"
          << medicine.unitPrice << "|" << medicine.category << "\n";
}

static void loadMedicines() {
    ifstream f("medicines.txt");
    if (!f) return;
    string line;
    int lineNum = 0;
    while (getline(f, line)) {
        lineNum++;
        if (line.empty()) continue;
        try {
            istringstream ss(line);
            Medicine medicine; string quantityStr;
            getline(ss, medicine.code,     '|');
            getline(ss, medicine.name,     '|');
            getline(ss, quantityStr,        '|'); medicine.quantity  = stoi(quantityStr);
            getline(ss, quantityStr,        '|'); medicine.unitPrice = stod(quantityStr);
            getline(ss, medicine.category, '|');
            medicines.push_back(medicine);
        } catch (const exception& e) {
            cerr << "  [WARNING] Skipped corrupted medicine record at line " << lineNum << ": " << e.what() << "\n";
        }
    }
}

void saveBills() {
    ofstream f("bills.txt");
    for (auto& bill : bills)
        f << bill.billId << "|" << bill.appointmentId << "|" << bill.patientId << "|"
          << bill.issueDate << "|" << bill.consultFee << "|" << bill.medicineCost << "|"
          << bill.tax << "|" << bill.totalAmount << "|" << bill.amountPaid << "|"
          << bill.paymentMethod << "|" << bill.isPaid << "\n";
}

static void loadBills() {
    ifstream f("bills.txt");
    if (!f) return;
    string line;
    int lineNum = 0;
    while (getline(f, line)) {
        lineNum++;
        if (line.empty()) continue;
        try {
            istringstream ss(line);
            Bill bill; string consultFeeStr;
            getline(ss, bill.billId,        '|');
            getline(ss, bill.appointmentId, '|');
            getline(ss, bill.patientId,     '|');
            getline(ss, bill.issueDate,     '|');
            getline(ss, consultFeeStr, '|'); bill.consultFee   = stod(consultFeeStr);
            getline(ss, consultFeeStr, '|'); bill.medicineCost = stod(consultFeeStr);
            getline(ss, consultFeeStr, '|'); bill.tax          = stod(consultFeeStr);
            getline(ss, consultFeeStr, '|'); bill.totalAmount  = stod(consultFeeStr);
            getline(ss, consultFeeStr, '|'); bill.amountPaid   = stod(consultFeeStr);
            getline(ss, bill.paymentMethod, '|');
            getline(ss, consultFeeStr, '|'); bill.isPaid = (consultFeeStr == "1");
            bills.push_back(bill);
        } catch (const exception& e) {
            cerr << "  [WARNING] Skipped corrupted bill record at line " << lineNum << ": " << e.what() << "\n";
        }
    }
}

void savePrescriptions() {
    ofstream f("prescriptions.txt");
    for (auto& prescription : prescriptions) {
        f << prescription.id << "|" << prescription.appointmentId << "|"
          << prescription.doctorNotes << "|" << prescription.items.size() << "\n";
        for (auto& item : prescription.items)
            f << item.medicineCode << "|" << item.qty << "|" << item.dosage << "\n";
    }
}

static void loadPrescriptions() {
    ifstream f("prescriptions.txt");
    if (!f) return;
    string line;
    int lineNum = 0;
    while (getline(f, line)) {
        lineNum++;
        if (line.empty()) continue;
        try {
            istringstream ss(line);
            Prescription prescription; string itemCountStr;
            getline(ss, prescription.id,            '|');
            getline(ss, prescription.appointmentId, '|');
            getline(ss, prescription.doctorNotes,   '|');
            getline(ss, itemCountStr,              '|');
            int count = stoi(itemCountStr);
            for (int i = 0; i < count; i++) {
                string itemLine; 
                if (!getline(f, itemLine)) {
                    cerr << "  [WARNING] Incomplete prescription at line " << lineNum << ": Expected " << count << " items, got " << i << ".\n";
                    throw runtime_error("Incomplete prescription items");
                }
                istringstream itemStream(itemLine);
                PrescriptionItem item; string quantityStr;
                getline(itemStream, item.medicineCode, '|');
                getline(itemStream, quantityStr, '|'); item.qty = stoi(quantityStr);
                getline(itemStream, item.dosage, '|');
                prescription.items.push_back(item);
            }
            prescriptions.push_back(prescription);
        } catch (const exception& e) {
            cerr << "  [WARNING] Skipped corrupted prescription record at line " << lineNum << ": " << e.what() << "\n";
        }
    }
}

void loadAllData() {
    loadPatients();
    loadDoctors();
    loadAppointments();
    loadMedicines();
    loadBills();
    loadPrescriptions();
}

void saveAllData() {
    savePatients();
    saveDoctors();
    saveAppointments();
    saveMedicines();
    saveBills();
    savePrescriptions();
}
