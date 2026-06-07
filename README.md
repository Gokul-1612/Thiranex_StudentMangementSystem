# Student Management System

A console-based Student Management System built in **C++** that allows users to manage student records efficiently using file handling and a menu-driven interface.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Data Structure](#data-structure)
- [Prerequisites](#prerequisites)
- [Installation & Compilation](#installation--compilation)
- [How to Run](#how-to-run)
- [Usage Guide](#usage-guide)
- [File Storage Format](#file-storage-format)
- [Input Validation](#input-validation)
- [Sample Output](#sample-output)
- [Technical Details](#technical-details)
- [Limitations](#limitations)
- [Future Improvements](#future-improvements)
- [License](#license)

---

## Overview

The Student Management System is a command-line application designed to store, retrieve, update, and delete student records persistently. All data is saved to a local file (`students.dat`), ensuring records are retained between sessions. The system is built entirely using core C++ standard libraries — no external dependencies required.

---

## Features

| Feature | Description |
|---|---|
| ➕ Add Student | Register a new student with auto-assigned ID |
| 📋 Display All | View all records in a formatted table |
| 🔍 Search | Find students by ID or partial name match |
| ✏️ Update | Modify any field of an existing student record |
| 🗑️ Delete | Remove a student record with confirmation prompt |
| 📊 Statistics | View GPA averages, top/bottom students, and grade distribution |
| 💾 Persistent Storage | All data saved to `students.dat` across sessions |
| ✅ Input Validation | All inputs are type-checked and range-validated |

---

## Project Structure

```
StudentMS/
├── student_management.cpp   # Main source file (all logic)
├── students.dat             # Auto-created data file on first run
└── README.md                # This file
```

> **Note:** `students.dat` is created automatically when the program runs for the first time.

---

## Data Structure

Each student record holds the following fields:

| Field | Type | Constraints |
|---|---|---|
| `id` | Integer | Auto-assigned, starts at 1001 |
| `name` | String | Cannot be empty |
| `age` | Integer | 5 – 100 |
| `course` | String | Free text |
| `gpa` | Double | 0.0 – 4.0 |
| `email` | String | Free text |
| `phone` | String | Free text |

---

## Prerequisites

- A C++ compiler supporting **C++17** or later
  - **Linux / macOS:** GCC (`g++`) or Clang (`clang++`)
  - **Windows:** MinGW-w64, MSVC (Visual Studio), or Cygwin

---

## Installation & Compilation

### Linux / macOS

```bash
# Clone or download the source file, then:
g++ -std=c++17 -Wall -o sms student_management.cpp
```

### Windows (MinGW)

```cmd
g++ -std=c++17 -Wall -o sms.exe student_management.cpp
```

### Windows (MSVC — Developer Command Prompt)

```cmd
cl /std:c++17 /EHsc student_management.cpp /Fe:sms.exe
```

---

## How to Run

### Linux / macOS

```bash
./sms
```

### Windows

```cmd
sms.exe
```

The program will automatically create `students.dat` in the current working directory if it does not already exist.

---

## Usage Guide

When launched, the main menu is displayed:

```
============================================================
              STUDENT MANAGEMENT SYSTEM
============================================================

    [1]  Add New Student
    [2]  Display All Students
    [3]  Search Student
    [4]  Update Student
    [5]  Delete Student
    [6]  View Statistics
    [0]  Exit

------------------------------------------------------------
  Enter your choice:
```

### Option 1 — Add New Student

- The system auto-assigns the next available Student ID.
- You will be prompted to enter: Name, Age, Course, GPA, Email, and Phone.
- The record is immediately saved to `students.dat`.

### Option 2 — Display All Students

- Displays all student records in a formatted table.
- Shows total record count at the bottom.

### Option 3 — Search Student

- Search by **Student ID** (exact match) or **Name** (partial, case-insensitive).
- Displays full details for all matching records.

### Option 4 — Update Student

- Enter the Student ID to update.
- Current values are shown in brackets — press **Enter** to keep them unchanged.
- Only fields you type new values for will be updated.

### Option 5 — Delete Student

- Enter the Student ID to delete.
- The record is displayed for confirmation before deletion.
- Type `Y` to confirm or `N` to cancel.

### Option 6 — View Statistics

- Total student count
- Average, highest, and lowest GPA (with student names)
- Average age
- GPA grade distribution:
  - **A** — 3.5 to 4.0
  - **B** — 3.0 to 3.4
  - **C** — 2.5 to 2.9
  - **D** — 2.0 to 2.4
  - **F** — Below 2.0

### Option 0 — Exit

Exits the application. All data is already saved; no manual save step is needed.

---

## File Storage Format

Student records are stored in `students.dat` as plain text, one record per line, with fields separated by a pipe character (`|`):

```
1001|Alice Johnson|20|Computer Science|3.80|alice@uni.edu|9876543210
1002|Bob Smith|22|Mathematics|3.20|bob@uni.edu|9123456789
1003|Carol White|21|Physics|2.70|carol@uni.edu|9988776655
```

**Field order:** `id | name | age | course | gpa | email | phone`

> The file is human-readable and can be inspected with any text editor. However, manual edits must follow the exact format to avoid deserialization errors.

---

## Input Validation

The system enforces the following rules at input time:

- **Name** — Cannot be blank; will re-prompt if empty.
- **Age** — Must be an integer between 5 and 100.
- **GPA** — Must be a decimal number between 0.0 and 4.0.
- **Menu choice** — Must be a valid integer within the displayed range.
- **Non-numeric input** — Cleared and re-prompted automatically; no crash occurs.

---

## Sample Output

### Display All Students

```
ID    Name                 Age  Course            GPA    Email                Phone
----------------------------------------------------------------------------------------------
1001  Alice Johnson        20   Computer Science  3.80   alice@uni.edu        9876543210
1002  Bob Smith            22   Mathematics       3.20   bob@uni.edu          9123456789
1003  Carol White          21   Physics           2.70   carol@uni.edu        9988776655

  Total Records: 3
```

### Statistics

```
--------------------------------------------------
  Total Students   : 3
  Average GPA      : 3.23
  Highest GPA      : 3.80 (Alice Johnson)
  Lowest GPA       : 2.70 (Carol White)
  Average Age      : 21.0
--------------------------------------------------
  GPA Distribution :
    A (3.5-4.0) : 1 students
    B (3.0-3.4) : 1 students
    C (2.5-2.9) : 1 students
    D (2.0-2.4) : 0 students
    F (< 2.0)   : 0 students
--------------------------------------------------
```

---

## Technical Details

| Aspect | Implementation |
|---|---|
| Language Standard | C++17 |
| File I/O | `std::ifstream` / `std::ofstream` |
| Data Format | Pipe-delimited plain text |
| Data Storage | Load-all → modify → save-all strategy |
| Search | Linear scan; case-insensitive `std::transform` |
| ID Generation | `max existing ID + 1` (starts at 1001) |
| Screen Clear | `system("cls")` on Windows, `system("clear")` on Unix |
| Includes Used | `iostream`, `fstream`, `sstream`, `vector`, `string`, `iomanip`, `limits`, `algorithm`, `climits` |

---

## Limitations

- **No concurrent access** — The file is fully read into memory and rewritten on every operation. Not suitable for multi-user environments.
- **No sorting** — Records are displayed in insertion order.
- **No pagination** — All records are printed at once; large datasets may scroll off screen.
- **Pipe character in data** — If a student's name or field contains `|`, it will break deserialization. Input is not currently sanitized for this character.
- **No password protection** — The data file is plain text and accessible to anyone with file system access.

---

## Future Improvements

- [ ] Sort records by name, GPA, or ID
- [ ] Paginated display for large datasets
- [ ] Export records to CSV format
- [ ] Input sanitization to prevent delimiter conflicts
- [ ] Backup and restore functionality
- [ ] Colour-coded terminal output (using ANSI codes)
- [ ] Binary file format for faster I/O and tamper resistance
- [ ] Course-wise filtering and grouping

---

## License

This project is released for educational purposes. You are free to use, modify, and distribute it for learning and academic projects.

---

*Built with C++ · Console Application · File Handling · Menu-Driven Design*
