# Clinic Management System

A menu-driven **C++ Clinic Management System** developed as an academic group project. The application provides a console-based interface for managing clinic operations, including patient records, doctor schedules, appointments, billing, prescriptions, and medicine inventory.

## Features

The system is organized into seven main modules:

1. **Patient Management** – Manage patient records.
2. **Doctor & Schedule Management** – Manage doctor information and schedules.
3. **Appointment Booking** – Handle clinic appointment bookings.
4. **Billing & Payment Processing** – Manage billing and payment operations.
5. **Reports & Statistics** – Generate clinic reports and statistics.
6. **Prescription Management** – Manage patient prescriptions.
7. **Medicine Inventory** – Manage medicine inventory records.

## Technologies Used

- C++
- File Handling
- Text-file-based data storage
- Microsoft Visual Studio C++ Compiler (MSVC)

## Project Structure

| File | Description |
|---|---|
| `main.cpp` | Program entry point and main menu |
| `globals.h` | Shared declarations and definitions |
| `globals.cpp` | Global data and shared functionality |
| `patient.cpp` | Patient management module |
| `doctor.cpp` | Doctor management module |
| `appointment.cpp` | Appointment management module |
| `billing.cpp` | Billing and payment module |
| `prescription.cpp` | Prescription management module |
| `inventory.cpp` | Medicine inventory module |
| `reports.cpp` | Reports and statistics module |
| `fileio.cpp` | File loading and saving operations |
| `utils.cpp` | Utility functions |

The `.txt` files store application data for the corresponding modules.

## Getting Started

### Requirements

- Windows
- Visual Studio or Visual Studio Build Tools with the C++ workload
- Microsoft C++ compiler (`cl.exe`)

### Compilation

Open a **Developer Command Prompt for Visual Studio**, navigate to the project directory, and compile the application:

```bash
cl /EHsc /Fe:ClinicManagementSystem.exe main.cpp
```

The project includes its module `.cpp` files through `main.cpp`, so compile the entry-point file rather than compiling each module separately.

### Run

```bash
ClinicManagementSystem.exe
```

## Data Storage

The application uses text files to store and retrieve records between program sessions.

On the first run, the program initializes demonstration records when the patient and doctor data are empty. These records are fictional sample data for testing and demonstration purposes, not actual clinic or patient information.

The application saves its data when the user selects **Save & Exit**.

## Project Information

- **Project Type:** Academic Group Project
- **Programming Language:** C++
- **Application Type:** Console-Based Application
- **Repository:** [ClinicManagementSystem](https://github.com/zIon6767/ClinicManagementSystem)

---

*Developed for academic learning and practical application of C++ programming concepts.*
