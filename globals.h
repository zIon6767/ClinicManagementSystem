#pragma once

// ── Standard Library Includes ─────────────────────────────────────────────────
#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <limits>
using namespace std;

// ── Schedule Grid Dimensions ──────────────────────────────────────────────────
const int    MAX_DAYS  = 7;   // Monday – Sunday
const int    MAX_SLOTS = 8;   // 08:00 – 17:00 (1-hour slots, lunch break excluded)

// ── Financial Constants ───────────────────────────────────────────────────────
const double TAX_RATE  = 0.06;  // 6% SST applied to all bills

// ── Clinic Information ────────────────────────────────────────────────────────
inline const string CLINIC_NAME  = "MedCare Clinic";
inline const string CLINIC_ADDR  = "123 Healthway, Penang";
inline const string CLINIC_PHONE = "+603-1234 5678";

// ── Schedule Lookup Tables ────────────────────────────────────────────────────
inline const string DAY_NAMES[MAX_DAYS] = {
    "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"
};
inline const string TIME_SLOTS[MAX_SLOTS] = {
    "08:00-09:00", "09:00-10:00", "10:00-11:00", "11:00-12:00",
    "13:00-14:00", "14:00-15:00", "15:00-16:00", "16:00-17:00"
};

// =============================================================================
// Data Structures
// =============================================================================

// ── Patient ───────────────────────────────────────────────────────────────────
// Stores all personal and medical information for a registered patient.
struct Patient {
    string id;                // Unique patient ID (e.g. P0001)
    string name;              // Full name
    string phone;             // Contact phone number
    string address;           // Full Malaysian address
    string bloodType;         // ABO blood group (e.g. A+, O-)
    string registrationDate;  // Date first registered (YYYY-MM-DD)
    int    age;               // Age in years
    char   gender;            // 'M' = Male, 'F' = Female, '-' = Prefer not to say
};

// ── Doctor ────────────────────────────────────────────────────────────────────
// Stores professional information for a clinic doctor.
struct Doctor {
    string id;              // Unique doctor ID (e.g. D0001)
    string name;            // Full name (without "Dr." prefix)
    string specialization;  // Medical specialization (e.g. Cardiology)
    string phone;           // Contact phone number
    double consultFee;      // Consultation fee in RM
    bool   available;       // true = accepting appointments, false = unavailable
};

// ── Appointment ───────────────────────────────────────────────────────────────
// Links a patient to a doctor for a specific date and time slot.
struct Appointment {
    string id;        // Unique appointment ID (e.g. A0001)
    string patientId; // References Patient.id
    string doctorId;  // References Doctor.id
    string date;      // Appointment date (YYYY-MM-DD)
    string timeSlot;  // One of the TIME_SLOTS values (e.g. "09:00-10:00")
    string status;    // "Pending" | "Completed" | "Cancelled"
    string notes;     // Optional doctor/patient notes
};

// ── Medicine ──────────────────────────────────────────────────────────────────
// Represents a medicine item in the clinic inventory.
struct Medicine {
    string code;       // Unique medicine code (e.g. PCM)
    string name;       // Medicine name (e.g. Paracetamol 500mg)
    string category;   // Category (e.g. Analgesic, Antibiotic)
    int    quantity;   // Current stock count
    double unitPrice;  // Price per unit in RM
};

// ── PrescriptionItem ──────────────────────────────────────────────────────────
// One line on a prescription — a single medicine with quantity and dosage.
struct PrescriptionItem {
    string medicineCode;  // References Medicine.code
    string dosage;        // Dosage instructions (e.g. "1 tablet twice daily")
    int    qty;           // Quantity prescribed
};

// ── Prescription ──────────────────────────────────────────────────────────────
// A full prescription issued for one completed appointment.
struct Prescription {
    string id;              // Unique prescription ID (e.g. RX0001)
    string appointmentId;   // References Appointment.id
    string doctorNotes;     // Additional notes from the prescribing doctor
    vector<PrescriptionItem> items;  // List of prescribed medicines
};

// ── Bill ──────────────────────────────────────────────────────────────────────
// Financial record generated after a prescription is issued.
struct Bill {
    string billId;          // Unique bill ID (e.g. B0001)
    string appointmentId;   // References Appointment.id
    string patientId;       // References Patient.id
    string issueDate;       // Date bill was generated (YYYY-MM-DD)
    string paymentMethod;   // "Cash" | "Card" | "Online"
    double consultFee;      // Doctor's consultation fee
    double medicineCost;    // Total cost of all prescribed medicines
    double tax;             // SST amount (6% of subtotal)
    double totalAmount;     // consultFee + medicineCost + tax
    double amountPaid;      // Amount tendered by patient
    bool   isPaid;          // true = payment received
};

// =============================================================================
// Global In-Memory Data Stores
// =============================================================================
// All data is loaded from text files at startup (fileio.cpp → loadAllData())
// and persisted back on changes via individual save functions.
// =============================================================================

inline vector<Patient>      patients;       // All registered patients
inline vector<Doctor>       doctors;        // All registered doctors
inline vector<Appointment>  appointments;   // All appointments
inline vector<Medicine>     medicines;      // Medicine inventory
inline vector<Prescription> prescriptions;  // All issued prescriptions
inline vector<Bill>         bills;          // All generated bills
inline string               scheduleGrid[MAX_DAYS][MAX_SLOTS]; // Doctor schedule grid

// =============================================================================
// Forward Declarations — utils.cpp
// =============================================================================
// General-purpose utility functions used across all modules.

void   clearScreen();
string getCurrentDate();

void   printLine(char ch = '-', int width = 90);
void   printCentered(const string& text, int width = 90);
void   printBanner();

int    readInt(const string& prompt, int minVal, int maxVal);
double readDouble(const string& prompt, double minVal);
string readString(const string& prompt);
string toUpper(string s);

bool   isValidPhoneNumber(const string& phone);
bool   isValidName(const string& name);
bool   parseDate(const string& date, int& year, int& month, int& day);
bool   isValidDate(const string& date);
bool   isFutureOrTodayDate(const string& date);
string normalizeDate(const string& date);
void   pause();

bool   patientExists(const string& id, int& idx);
bool   doctorExists(const string& id, int& idx);
bool   appointmentExists(const string& id, int& idx);
bool   medicineExists(const string& code, int& idx);
string generateId(const string& prefix, int count);
void   clearScheduleGrid();

// =============================================================================
// Forward Declarations — fileio.cpp
// =============================================================================
// File persistence: load all data at startup, save individual entities on change.

void loadAllData();
void saveAllData();
void savePatients();
void saveDoctors();
void saveAppointments();
void saveMedicines();
void saveBills();
void savePrescriptions();

// =============================================================================
// Forward Declarations — patient.cpp
// =============================================================================

void displayPatientRecord(const Patient& p);
void registerPatient();
void searchPatient();
void updatePatient();
void listAllPatients();
void deletePatient();
void patientMenu();

// =============================================================================
// Forward Declarations — doctor.cpp
// =============================================================================

void displayDoctorRecord(const Doctor& d);
void addDoctor();
void searchDoctor();
void updateDoctor();
void listDoctors();
void updateDoctorAvailability();
void viewDoctorSchedule();
void viewDoctorAppointments();
void deleteDoctor();
void doctorMenu();

// =============================================================================
// Forward Declarations — appointment.cpp
// =============================================================================

void printAppointmentSlip(const Appointment& a);
bool isSlotBooked(const string& doctorId, const string& date, const string& slot);
void bookAppointment();
void cancelAppointment();
void rescheduleAppointment();
void completeAppointment();
void listAppointments();
void appointmentMenu();

// =============================================================================
// Forward Declarations — billing.cpp
// =============================================================================

double calcMedicineCost(const string& apptId);
void   generateBill(const string& apptId);
void   printInvoice(const Bill& b);
void   processPayment();
void   viewBill();
void   billingMenu();

// =============================================================================
// Forward Declarations — reports.cpp
// =============================================================================

void reportRevenueSummary();
void reportAppointmentStats();
void reportPatientStatistics();
void reportOutstandingBills();
void reportsMenu();

// =============================================================================
// Forward Declarations — prescription.cpp
// =============================================================================

void printPrescription(const Prescription& pr);
void createPrescription();
void viewPrescription();
void prescriptionMenu();

// =============================================================================
// Forward Declarations — inventory.cpp
// =============================================================================

void listMedicines();
void addMedicine();
void updateMedicineStock();
void lowStockAlert();
void inventoryMenu();
