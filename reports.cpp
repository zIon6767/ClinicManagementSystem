#include "globals.h"

struct ReportPeriod {
    string label;
    string startDate;
    string endDate;
};

static ReportPeriod chooseReportPeriod() {
    ReportPeriod period;
    string today = getCurrentDate();

    cout << "\n  Report timeframe:\n"
         << "  1. All time\n"
         << "  2. Today\n"
         << "  3. This month\n"
         << "  4. This year\n"
         << "  5. Custom date range\n";
    int choice = readInt("  Choice: ", 1, 5);

    if (choice == 1) {
        period.label = "All time";
    } else if (choice == 2) {
        period.label = "Today (" + today + ")";
        period.startDate = today;
        period.endDate = today;
    } else if (choice == 3) {
        period.label = "This month";
        period.startDate = today.substr(0, 8) + "01";
        period.endDate = today;
    } else if (choice == 4) {
        period.label = "This year";
        period.startDate = today.substr(0, 4) + "-01-01";
        period.endDate = today;
    } else {
        while (true) {
            period.startDate = readString("  Start date (YYYY-MM-DD): ");
            period.endDate = readString("  End date (YYYY-MM-DD): ");
            period.startDate = normalizeDate(period.startDate);
            period.endDate = normalizeDate(period.endDate);
            if (isValidDate(period.startDate) && isValidDate(period.endDate) &&
                period.startDate <= period.endDate) {
                break;
            }
            cout << "  [!] Enter valid dates with the start date first.\n";
        }
        period.label = period.startDate + " to " + period.endDate;
    }

    return period;
}

static bool isInReportPeriod(const string& date, const ReportPeriod& period) {
    return period.startDate.empty() ||
           (date >= period.startDate && date <= period.endDate);
}

static void printReportPeriod(const ReportPeriod& period) {
    printCentered("Period: " + period.label);
}

void reportRevenueSummary() {
    ReportPeriod period = chooseReportPeriod();
    printLine('=');
    printCentered("REVENUE SUMMARY REPORT");
    printCentered("Generated: " + getCurrentDate());
    printReportPeriod(period);
    printLine('=');
    double totalRevenue = 0.0, totalConsult = 0.0, totalMed = 0.0, totalOutstanding = 0.0;
    int totalBills = 0, paidCount = 0, unpaidCount = 0;
    for (auto& bill : bills) {
        if (!isInReportPeriod(bill.issueDate, period)) continue;
        totalBills++;
        if (bill.isPaid) {
            totalRevenue += bill.totalAmount;
            totalConsult += bill.consultFee;
            totalMed     += bill.medicineCost;
            paidCount++;
        } else {
            totalOutstanding += bill.totalAmount;
            unpaidCount++;
        }
    }
    cout << fixed << setprecision(2)
            << left << setw(35) << "Total Bills Issued" << ": " << totalBills << "\n"
         << setw(35) << "Bills Paid" << ": " << paidCount << "\n"
         << setw(35) << "Bills Outstanding" << ": " << unpaidCount << "\n";
    printLine('-');
    cout << setw(35) << "Revenue - Consultation" << ": RM " << totalConsult << "\n"
         << setw(35) << "Revenue - Medicine" << ": RM " << totalMed << "\n"
         << setw(35) << "TOTAL REVENUE COLLECTED" << ": RM " << totalRevenue << "\n"
         << setw(35) << "TOTAL OUTSTANDING" << ": RM " << totalOutstanding << "\n";
    printLine('=');
}

void reportAppointmentStats() {
    ReportPeriod period = chooseReportPeriod();
    printLine('=');
    printCentered("APPOINTMENT STATISTICS");
    printReportPeriod(period);
    printLine('=');
    int totalAppointments = 0, pending = 0, completed = 0, cancelled = 0;
    for (auto& appointment : appointments) {
        if (!isInReportPeriod(appointment.date, period)) continue;
        totalAppointments++;
        if (appointment.status == "Pending"){
            pending++;
        }
        else if (appointment.status == "Completed") {
            completed++;
        }
        else if (appointment.status == "Cancelled"){ 
            cancelled++;
        }
    }
    cout << left << setw(25) << "Total" << ": " << totalAppointments << "\n"
                 << setw(25) << "Pending" << ": " << pending << "\n"
                 << setw(25) << "Completed" << ": " << completed << "\n"
                 << setw(25) << "Cancelled" << ": " << cancelled << "\n";
    printLine('-');
    cout << "  Appointments per Doctor:\n";
    printLine();
    cout << left << setw(10) << "ID" << setw(28) << "Doctor" << "Count\n";
    printLine();
    for (auto& doctor : doctors) {
        int appointmentCount = 0;
        for (auto& appointment : appointments)
            if (isInReportPeriod(appointment.date, period) &&
                appointment.doctorId == doctor.id) appointmentCount++;
        cout << left << setw(10) << doctor.id << setw(28) << ("Dr. " + doctor.name) << appointmentCount << "\n";
    }
    printLine('=');
}

void reportPatientStatistics() {
    ReportPeriod period = chooseReportPeriod();
    printLine('=');
    printCentered("PATIENT DEMOGRAPHICS");
    printReportPeriod(period);
    printLine('=');
    int male = 0, female = 0, preferNotToSay = 0;
    int ageGroup[4] = {0, 0, 0, 0};
    for (auto& patient : patients) {
        if (!isInReportPeriod(patient.registrationDate, period)) continue;
        if (patient.gender == 'M') male++;
        else if (patient.gender == 'F') female++;
        else preferNotToSay++;
        if (patient.age < 18) ageGroup[0]++;
        else if (patient.age < 41) ageGroup[1]++;
        else if (patient.age < 61) ageGroup[2]++;
        else                       ageGroup[3]++;
    }
    int totalPatients = male + female + preferNotToSay;
    cout << left << setw(30) << "Total Patients" << ": " << totalPatients  << "\n"
                 << setw(30) << "Male" << ": " << male << "\n"
                 << setw(30) << "Female" << ": " << female << "\n"
                 << setw(30) << "Prefer not to say" << ": " << preferNotToSay << "\n";
    printLine('-');
    cout << setw(30) << "Children  (0-17)" << ": " << ageGroup[0] << "\n"
         << setw(30) << "Young Adults (18-40)" << ": " << ageGroup[1] << "\n"
         << setw(30) << "Middle-aged  (41-60)" << ": " << ageGroup[2] << "\n"
         << setw(30) << "Elderly (61+)" << ": " << ageGroup[3] << "\n";
    printLine('=');
}

void reportOutstandingBills() {
    ReportPeriod period = chooseReportPeriod();
    printReportPeriod(period);
    cout << "\n--- Outstanding Bills ---\n";
    printLine();
    cout << left << setw(10) << "Bill ID" << setw(14) << "Appt ID"
                 << setw(12) << "Patient" << "Amount Due (RM)\n";
    printLine();
    double totalDue = 0.0; bool any = false;
    for (auto& bill : bills)
        if (isInReportPeriod(bill.issueDate, period) && !bill.isPaid) {
            cout << left << setw(10) << bill.billId
                         << setw(14) << bill.appointmentId
                         << setw(12) << bill.patientId
                         << fixed << setprecision(2) << bill.totalAmount << "\n";
            totalDue += bill.totalAmount;
            any = true;
        }
    if (!any) { cout << "  No outstanding bills.\n"; }
    else {
        printLine();
        cout << left << setw(36) << "Total Outstanding" << ": RM "
             << fixed << setprecision(2) << totalDue << "\n";
    }
    printLine();
}

void reportsMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n";
        printLine('=');
        printCentered("REPORTS & STATISTICS");
        printLine('=');
        cout << "  1. Revenue Summary\n"
             << "  2. Appointment Statistics\n"
             << "  3. Patient Demographics\n"
             << "  4. Outstanding Bills\n"
             << "  0. Back\n";
        printLine();
        choice = readInt("  Choice: ", 0, 4);
        clearScreen();
        switch (choice) {
            case 1: reportRevenueSummary();    break;
            case 2: reportAppointmentStats();  break;
            case 3: reportPatientStatistics(); break;
            case 4: reportOutstandingBills();  break;
        }
        if (choice != 0) {
             pause();
        }
    } while (choice != 0);
}
