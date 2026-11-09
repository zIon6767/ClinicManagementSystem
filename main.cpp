#include "globals.h"
#include "appointment.cpp" // Shared structs, constants, and all function declarations
#include "billing.cpp"
#include "doctor.cpp"
#include "fileio.cpp"
#include "globals.cpp"
#include "inventory.cpp"
#include "patient.cpp"
#include "prescription.cpp"
#include "reports.cpp"
#include "utils.cpp"

// -----------------------------------------------------------------------------
// seedDemoData — Populates default doctors, patients, and medicines on first run.
// Only executes when both the patients and doctors vectors are empty (i.e., no
// existing data was loaded from file). Saves seeded data immediately to disk.
// -----------------------------------------------------------------------------
static void seedDemoData() {
    if (!patients.empty() || !doctors.empty()) return;

    doctors = {
        {"D0001", "Ahmad Faiz",    "General Practice", "+60 12-111 2222", 80.0,  true},
        {"D0002", "Priya Nair",    "Cardiology",       "+60 12-333 4444", 150.0, true},
        {"D0003", "Lee Chun Wai",  "Pediatrics",       "+60 12-555 6666", 100.0, true}
    };
    saveDoctors();

    patients = {
        {"P0001", "Ali Hassan",    "+60 11-123 4567", "Jalan Ampang, KL",  "O+", getCurrentDate(), 35, 'M'},
        {"P0002", "Siti Rahimah",  "+60 11-234 5678", "Petaling Jaya",     "A+", getCurrentDate(), 28, 'F'},
        {"P0003", "Tan Wei Ming",  "+60 11-345 6789", "Cheras, KL",        "B-", getCurrentDate(), 62, 'M'}
    };
    savePatients();

    medicines = {
        {"PCM", "Paracetamol 500mg",   "Analgesic",         200, 0.50},
        {"AMX", "Amoxicillin 250mg",   "Antibiotic",        150, 1.20},
        {"IBP", "Ibuprofen 400mg",     "Anti-inflammatory", 100, 0.80},
        {"ORS", "ORS Sachets",         "Electrolyte",        80, 1.50},
        {"VTB", "Vitamin B Complex",   "Supplement",        300, 0.30}
    };
    saveMedicines();

    cout << "  [INFO] Demo data: 3 doctors, 3 patients, 5 medicines.\n";
}

// -----------------------------------------------------------------------------
// showMainMenu — Renders the main menu banner and numbered module options.
// -----------------------------------------------------------------------------
static void showMainMenu() {
    cout << "\n";
    printBanner();
    printCentered("M A I N   M E N U");
    printLine('=');
    cout << "  1. Patient Management\n"
         << "  2. Doctor & Schedule Management\n"
         << "  3. Appointment Booking\n"
         << "  4. Billing & Payment Processing\n"
         << "  5. Reports & Statistics\n"
         << "  6. Prescription Management\n"
         << "  7. Medicine Inventory\n"
         << "  0. Save & Exit\n";
    printLine('=');
}

// -----------------------------------------------------------------------------
// main — Program entry point.
// Loads data, shows the main menu loop, and saves on exit.
// -----------------------------------------------------------------------------
int main() {
    cout << "\n  Loading data...\n";
    loadAllData();
    seedDemoData();

    int choice;
    do {
        clearScreen();
        showMainMenu();
        choice = readInt("  Choice: ", 0, 7);
        clearScreen();

        switch (choice) {
            case 1: patientMenu();      break;
            case 2: doctorMenu();       break;
            case 3: appointmentMenu();  break;
            case 4: billingMenu();      break;
            case 5: reportsMenu();      break;
            case 6: prescriptionMenu(); break;
            case 7: inventoryMenu();    break;
            case 0:
                saveAllData();
                cout << "\n  Data saved. Goodbye!\n\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
