#include "globals.h"

void listMedicines() {
    cout << "\n--- Medicine Inventory ---\n";
    if (medicines.empty()) { cout << "  No medicines.\n"; return; }

    // Compute dynamic column widths
    size_t wCode = 4, wName = 4, wStock = 5, wPrice = 10, wCat = 8;
    for (auto& m : medicines) {
        wCode  = max(wCode,  m.code.size());
        wName  = max(wName,  m.name.size());
        wCat   = max(wCat,   m.category.size());
    }
    wCode += 2; wName += 2; wStock += 2; wPrice += 2;

    printLine();
    cout << left << setw(wCode)  << "Code"
                 << setw(wName)  << "Name"
                 << setw(wStock) << "Stock"
                 << setw(wPrice) << "Price (RM)"
                 << "Category\n";
    printLine();
    for (auto& m : medicines)
        cout << left << setw(wCode)  << m.code
                     << setw(wName)  << m.name
                     << setw(wStock) << m.quantity
                     << setw(wPrice) << fixed << setprecision(2) << m.unitPrice
                     << m.category << "\n";
    printLine();
    cout << "  Total: " << medicines.size() << "\n";
}

void addMedicine() {
    cout << "\n--- Add Medicine ---\n";
    cout << "  0. Back\n";
    Medicine medicine;
    medicine.code = toUpper(readString("  Code     : "));
    if (medicine.code == "0") return;
    int dummy;
    if (medicineExists(medicine.code, dummy)) { cout << "  [!] Code already exists.\n"; return; }
    medicine.name      = readString("  Name     : ");
    medicine.quantity  = readInt("  Quantity : ", 0, 99999);
    medicine.unitPrice = readDouble("  Price RM : ", 0.01);
    medicine.category  = readString("  Category : ");
    medicines.push_back(medicine);
    saveMedicines();
    cout << "  [OK] Medicine added.\n";
}

void updateMedicineStock() {
    cout << "\n--- Restock Medicine ---\n";
    listMedicines();
    cout << "  0. Back\n";
    string medicineCode = readString("  Medicine Code: ");
    if (medicineCode == "0") return;
    medicineCode = toUpper(medicineCode);
    int medicineIndex;
    if (!medicineExists(medicineCode, medicineIndex)) { cout << "  [!] Not found.\n"; return; }
    int addQuantity = readInt("  Add Quantity: ", 1, 99999);
    medicines[medicineIndex].quantity += addQuantity;
    saveMedicines();
    cout << "  [OK] New Stock: " << medicines[medicineIndex].quantity << "\n";
}

void lowStockAlert() {
    cout << "\n--- Low Stock Alert (< 10 units) ---\n";
    bool any = false;

    // Compute dynamic column widths
    size_t wCode = 4, wName = 4;
    for (auto& m : medicines)
        if (m.quantity < 10) {
            wCode = max(wCode, m.code.size());
            wName = max(wName, m.name.size());
        }
    wCode += 2; wName += 2;

    printLine();
    for (auto& m : medicines)
        if (m.quantity < 10) {
            cout << left << setw(wCode) << m.code
                         << setw(wName) << m.name
                         << "Stock: " << m.quantity << "\n";
            any = true;
        }
    if (!any) cout << "  All medicines have adequate stock.\n";
    else printLine();
}

void inventoryMenu() {
    int choice;
    do {
        clearScreen();
        cout << "\n";
        printLine('=');
        printCentered("MEDICINE INVENTORY");
        printLine('=');
        cout << "  1. Add Medicine\n"
             << "  2. List Medicines\n"
             << "  3. Restock\n"
             << "  4. Low Stock Alert\n"
             << "  0. Back\n";
        printLine();
        choice = readInt("  Choice: ", 0, 4);
        clearScreen();
        switch (choice) {
            case 1: addMedicine();         break;
            case 2: listMedicines();       break;
            case 3: updateMedicineStock(); break;
            case 4: lowStockAlert();       break;
        }
        if (choice != 0) pause();
    } while (choice != 0);
}
