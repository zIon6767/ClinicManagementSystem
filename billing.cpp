#include "globals.h"

double calcMedicineCost(const string& appointmentId) {
    double total = 0.0;
    for (auto& prescription : prescriptions)
        if (prescription.appointmentId == appointmentId)
            for (auto& item : prescription.items) {
                int medicineIndex;
                if (medicineExists(item.medicineCode, medicineIndex))
                    total += medicines[medicineIndex].unitPrice * item.qty;
            }
    return total;
}

void generateBill(const string& appointmentId) {
    int appointmentIndex;
    if (!appointmentExists(appointmentId, appointmentIndex)) { cout << "  [!] Appointment not found.\n"; return; }
    if (appointments[appointmentIndex].status != "Completed") {
        cout << "  [!] Appointment must be Completed first.\n"; return;
    }
    for (auto& bill : bills)
        if (bill.appointmentId == appointmentId) { cout << "  [!] Bill exists: " << bill.billId << "\n"; return; }

    int doctorIndex; double consultFee = 0.0;
    if (doctorExists(appointments[appointmentIndex].doctorId, doctorIndex))
        consultFee = doctors[doctorIndex].consultFee;

    double medicineCost = calcMedicineCost(appointmentId);
    double subtotal     = consultFee + medicineCost;
    double tax          = subtotal * TAX_RATE;

    Bill bill;
    bill.billId        = generateId("B", (int)bills.size());
    bill.appointmentId = appointmentId;
    bill.patientId     = appointments[appointmentIndex].patientId;
    bill.issueDate     = getCurrentDate();
    bill.consultFee    = consultFee;
    bill.medicineCost  = medicineCost;
    bill.tax           = tax;
    bill.totalAmount   = subtotal + tax;
    bill.amountPaid    = 0.0;
    bill.paymentMethod = "";
    bill.isPaid        = false;
    bills.push_back(bill);
    saveBills();
    cout << "  [OK] Bill generated. ID: " << bill.billId << "\n";
}

static string centerText(const string& text, int width) {
    if ((int)text.size() >= width) return text.substr(0, width);
    int padLeft = (width - (int)text.size()) / 2;
    int padRight = width - padLeft - (int)text.size();
    return string(padLeft, ' ') + text + string(padRight, ' ');
}

static void printAsciiTop(int width) {
    cout << "+" << string(width - 2, '=') << "+\n";
}

static void printAsciiDivider(int width) {
    cout << "+" << string(width - 2, '-') << "+\n";
}

static void printAsciiLine(const string& text, int width) {
    cout << "|" << centerText(text, width - 2) << "|\n";
}

static void printAsciiBottom(int width) {
    cout << "+" << string(width - 2, '=') << "+\n";
}

void printInvoice(const Bill& b) {
    string pName = "N/A";
    int pIdx;
    if (patientExists(b.patientId, pIdx)) pName = patients[pIdx].name;

    string docName = "N/A";
    int aIdx;
    if (appointmentExists(b.appointmentId, aIdx)) {
        int dIdx;
        if (doctorExists(appointments[aIdx].doctorId, dIdx))
            docName = "Dr. " + doctors[dIdx].name;
    }

    printLine('=');
    printCentered(CLINIC_NAME);
    printCentered("TAX INVOICE / RECEIPT");
    printLine('=');
    cout << left
         << setw(22) << "Bill ID"        << ": " << b.billId        << "\n"
         << setw(22) << "Date"           << ": " << b.issueDate     << "\n"
         << setw(22) << "Patient ID"     << ": " << b.patientId     << "\n"
         << setw(22) << "Patient Name"   << ": " << pName           << "\n"
         << setw(22) << "Doctor"         << ": " << docName         << "\n"
         << setw(22) << "Appointment ID" << ": " << b.appointmentId << "\n";

    vector<PrescriptionItem> medItems;
    for (const auto& pr : prescriptions) {
        if (pr.appointmentId == b.appointmentId) {
            medItems = pr.items;
            break;
        }
    }

    if (!medItems.empty()) {
        printLine('-');
        cout << "  MEDICINES PURCHASED:\n";
        printLine('-');
        cout << left
             << setw(6)  << "  No."
             << setw(12) << "Code"
             << setw(30) << "Medicine"
             << setw(8)  << "Qty"
             << setw(16) << "Unit Price(RM)"
             << "Total (RM)\n";
        printLine('-');
        for (size_t i = 0; i < medItems.size(); i++) {
            string mName = "Unknown";
            double uPrice = 0.0;
            int mIdx;
            if (medicineExists(medItems[i].medicineCode, mIdx)) {
                mName = medicines[mIdx].name;
                uPrice = medicines[mIdx].unitPrice;
            }
            double itemTotal = uPrice * medItems[i].qty;
            cout << left
                 << "  " << setw(4) << (i + 1)
                 << setw(12) << medItems[i].medicineCode
                 << setw(30) << mName
                 << setw(8)  << medItems[i].qty
                 << setw(16) << fixed << setprecision(2) << uPrice
                 << fixed << setprecision(2) << itemTotal << "\n";
        }
    }
    printLine('-');
    cout << left  << setw(38) << "Consultation Fee"
         << right << "RM " << fixed << setprecision(2) << setw(8) << b.consultFee << "\n"
         << left  << setw(38) << "Medicine Cost"
         << right << "RM " << setw(8) << b.medicineCost << "\n";
    printLine('-');
    cout << left  << setw(38) << "Subtotal"
         << right << "RM " << setw(8) << (b.consultFee + b.medicineCost) << "\n"
         << left  << setw(38) << "SST (6%)"
         << right << "RM " << setw(8) << b.tax << "\n";
    printLine('-');
    cout << left  << setw(38) << "TOTAL AMOUNT DUE"
         << right << "RM " << setw(8) << b.totalAmount << "\n";
    if (b.isPaid) {
        printLine('-');
        cout << left  << setw(38) << "Amount Paid"
             << right << "RM " << setw(8) << b.amountPaid << "\n";
        if (b.paymentMethod == "Cash")
            cout << left << setw(38) << "Change"
                 << right << "RM " << setw(8) << (b.amountPaid - b.totalAmount) << "\n";
        cout << left << setw(38) << "Payment Method" << right << setw(11) << b.paymentMethod << "\n";
        if (b.paymentMethod == "Online") {
            int boxWidth = 62;
            cout << "\n";
            printAsciiTop(boxWidth);
            printAsciiLine("ONLINE PAYMENT RECEIVED", boxWidth);
            printAsciiDivider(boxWidth);
            printAsciiLine("Thank you for paying online!", boxWidth);
            printAsciiLine("Transaction Reference: " + b.billId, boxWidth);
            printAsciiBottom(boxWidth);
        }
    }
    printLine('=');
    printCentered(b.isPaid ? "*** PAYMENT RECEIVED - THANK YOU ***" : "*** PAYMENT OUTSTANDING ***");
    printLine('=');
}

void processPayment() {
    cout << "\n--- Process Payment ---\n";
    bool anyUnpaid = false;
    size_t wBill = 7, wAppt = 7, wPat = 7;
    for (auto& b : bills)
        if (!b.isPaid) {
            wBill = max(wBill, b.billId.size());
            wAppt = max(wAppt, b.appointmentId.size());
            wPat  = max(wPat,  b.patientId.size());
        }
    wBill += 2; wAppt += 2; wPat += 2;

    cout << left << setw(wBill) << "Bill ID"
                 << setw(wAppt) << "Appt ID"
                 << setw(wPat)  << "Patient"
                 << "Total (RM)\n";
    printLine();
    for (auto& bill : bills)
        if (!bill.isPaid) {
            cout << left << setw(wBill) << bill.billId
                         << setw(wAppt) << bill.appointmentId
                         << setw(wPat)  << bill.patientId
                         << fixed << setprecision(2) << bill.totalAmount << "\n";
            anyUnpaid = true;
        }
    if (!anyUnpaid) { cout << "  No outstanding bills.\n"; return; }

    cout << "  0. Back\n";
    string billId = readString("  Bill ID: ");
    if (billId == "0") return;
    Bill* billPointer = nullptr;
    for (auto& bill : bills)
        if (bill.billId == billId) { billPointer = &bill; break; }
    if (!billPointer)        { cout << "  [!] Not found.\n"; return; }
    if (billPointer->isPaid) { cout << "  [!] Already paid.\n"; return; }

    printInvoice(*billPointer);
    cout << "\n  1. Cash  2. Card  3. Online\n";
    int method = readInt("  Method: ", 1, 3);
    switch (method) {
        case 1: billPointer->paymentMethod = "Cash";   break;
        case 2: billPointer->paymentMethod = "Card";   break;
        case 3: billPointer->paymentMethod = "Online"; break;
    }
    if (method == 1) {
        double paid;
        do {
            paid = readDouble("  Amount Tendered RM: ", 0.0);
            if (paid < billPointer->totalAmount)
                cout << "  [!] Insufficient. Total: RM "
                     << fixed << setprecision(2) << billPointer->totalAmount << "\n";
        } while (paid < billPointer->totalAmount);
        billPointer->amountPaid = paid;
    } else {
        billPointer->amountPaid = billPointer->totalAmount;
    }
    billPointer->isPaid = true;
    saveBills();
    cout << "\n  [OK] Payment processed!\n";
    printInvoice(*billPointer);
}

void viewBill() {
    cout << "\n--- View Invoice ---\n";
    if (bills.empty()) {
        cout << "  [!] No bills available.\n";
        return;
    }
    cout << "\n  Existing Bills:\n";
    size_t wBill = 7, wAppt = 7, wPat = 7, wTotal = 5;
    for (auto& b : bills) {
        wBill = max(wBill, b.billId.size());
        wAppt = max(wAppt, b.appointmentId.size());
        wPat  = max(wPat,  b.patientId.size());
    }
    wBill += 2; wAppt += 2; wPat += 2; wTotal += 2;

    printLine();
    cout << left << setw(wBill)  << "Bill ID"
         << setw(wAppt)  << "Appt ID"
         << setw(wPat)   << "Patient"
         << setw(wTotal) << "Total"
         << "Status\n";
    printLine();
    for (auto& bill : bills) {
        cout << left << setw(wBill) << bill.billId
             << setw(wAppt) << bill.appointmentId
             << setw(wPat)  << bill.patientId
             << "RM " << fixed << setprecision(2) << setw(8) << bill.totalAmount
             << (bill.isPaid ? "Paid" : "Unpaid") << "\n";
    }
    printLine();
    cout << "  0. Back\n";
    string billId = readString("  Bill ID: ");
    if (billId == "0") return;
    for (auto& bill : bills)
        if (bill.billId == billId) { printInvoice(bill); return; }
    cout << "  [!] Not found.\n";
}

void billingMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n";
        printLine('=');
        printCentered("BILLING & PAYMENT");
        printLine('=');
        cout << "  1. Generate Bill\n"
             << "  2. Process Payment\n"
             << "  3. View Invoice\n"
             << "  0. Back\n";
        printLine();
        choice = readInt("  Choice: ", 0, 3);
        clearScreen();
        switch (choice) {
            case 1: {
                listAppointments();
                cout << "  0. Back\n";
                string apptId = readString("  Appointment ID: ");
                if (apptId != "0") generateBill(apptId);
                break;
            }
            case 2: processPayment(); break;
            case 3: viewBill();       break;
        }
        if (choice != 0) pause();
    } while (choice != 0);
}
