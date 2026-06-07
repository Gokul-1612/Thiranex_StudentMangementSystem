/*
 * ============================================================
 *        STUDENT MANAGEMENT SYSTEM
 *        Console-based | File Handling | Menu-Driven
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <climits>

using namespace std;

// ─────────────────────────────────────────────
//  CONSTANTS
// ─────────────────────────────────────────────
const string DATA_FILE = "students.dat";
const string DELIMITER = "|";

// ─────────────────────────────────────────────
//  STUDENT STRUCTURE
// ─────────────────────────────────────────────
struct Student {
    int    id;
    string name;
    int    age;
    string course;
    double gpa;
    string email;
    string phone;
};

// ─────────────────────────────────────────────
//  UTILITY HELPERS
// ─────────────────────────────────────────────
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\n  Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void printLine(char ch = '-', int len = 60) {
    cout << string(len, ch) << "\n";
}

void printHeader(const string& title) {
    clearScreen();
    printLine('=');
    cout << setw(35 + title.size() / 2) << title << "\n";
    printLine('=');
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Safe integer input
int getInt(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Invalid input. Try again.\n";
    }
}

// Safe double input
double getDouble(const string& prompt, double minVal = 0.0, double maxVal = 4.0) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Invalid input. Try again.\n";
    }
}

string getString(const string& prompt) {
    string val;
    cout << prompt;
    getline(cin, val);
    return trim(val);
}

// ─────────────────────────────────────────────
//  FILE HANDLING
// ─────────────────────────────────────────────

// Serialize one student to a pipe-delimited line
string serialize(const Student& s) {
    ostringstream oss;
    oss << s.id    << DELIMITER
        << s.name  << DELIMITER
        << s.age   << DELIMITER
        << s.course<< DELIMITER
        << fixed << setprecision(2) << s.gpa << DELIMITER
        << s.email << DELIMITER
        << s.phone;
    return oss.str();
}

// Deserialize a pipe-delimited line to a Student
bool deserialize(const string& line, Student& s) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, '|'))
        tokens.push_back(token);
    if (tokens.size() != 7) return false;
    try {
        s.id     = stoi(tokens[0]);
        s.name   = tokens[1];
        s.age    = stoi(tokens[2]);
        s.course = tokens[3];
        s.gpa    = stod(tokens[4]);
        s.email  = tokens[5];
        s.phone  = tokens[6];
    } catch (...) { return false; }
    return true;
}

// Load all students from file
vector<Student> loadAll() {
    vector<Student> students;
    ifstream file(DATA_FILE);
    if (!file.is_open()) return students;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        Student s;
        if (deserialize(line, s))
            students.push_back(s);
    }
    return students;
}

// Save all students to file (overwrite)
void saveAll(const vector<Student>& students) {
    ofstream file(DATA_FILE, ios::trunc);
    for (const auto& s : students)
        file << serialize(s) << "\n";
}

// Get next available ID
int nextID(const vector<Student>& students) {
    int maxID = 1000;
    for (const auto& s : students)
        maxID = max(maxID, s.id);
    return maxID + 1;
}

// ─────────────────────────────────────────────
//  DISPLAY HELPERS
// ─────────────────────────────────────────────
void printTableHeader() {
    cout << "\n";
    cout << left
         << setw(6)  << "ID"
         << setw(22) << "Name"
         << setw(5)  << "Age"
         << setw(18) << "Course"
         << setw(7)  << "GPA"
         << setw(22) << "Email"
         << setw(14) << "Phone"
         << "\n";
    printLine('-', 94);
}

void printStudentRow(const Student& s) {
    cout << left
         << setw(6)  << s.id
         << setw(22) << s.name.substr(0, 20)
         << setw(5)  << s.age
         << setw(18) << s.course.substr(0, 16)
         << setw(7)  << fixed << setprecision(2) << s.gpa
         << setw(22) << s.email.substr(0, 20)
         << setw(14) << s.phone
         << "\n";
}

void printStudentDetail(const Student& s) {
    printLine('-', 50);
    cout << "  Student ID   : " << s.id     << "\n"
         << "  Full Name    : " << s.name   << "\n"
         << "  Age          : " << s.age    << "\n"
         << "  Course       : " << s.course << "\n"
         << "  GPA          : " << fixed << setprecision(2) << s.gpa << "\n"
         << "  Email        : " << s.email  << "\n"
         << "  Phone        : " << s.phone  << "\n";
    printLine('-', 50);
}

// ─────────────────────────────────────────────
//  MENU OPERATIONS
// ─────────────────────────────────────────────

// 1. Add Student
void addStudent() {
    printHeader("  ADD NEW STUDENT");
    vector<Student> students = loadAll();

    Student s;
    s.id     = nextID(students);
    cout << "  Auto-assigned ID: " << s.id << "\n\n";

    s.name   = getString("  Full Name    : ");
    if (s.name.empty()) { cout << "  [!] Name cannot be empty.\n"; pauseScreen(); return; }

    s.age    = getInt("  Age          : ", 5, 100);
    s.course = getString("  Course       : ");
    s.gpa    = getDouble("  GPA (0.0-4.0): ", 0.0, 4.0);
    s.email  = getString("  Email        : ");
    s.phone  = getString("  Phone        : ");

    students.push_back(s);
    saveAll(students);

    cout << "\n  [✓] Student added successfully! (ID: " << s.id << ")\n";
    pauseScreen();
}

// 2. Display All Students
void displayAll() {
    printHeader("  ALL STUDENTS");
    vector<Student> students = loadAll();

    if (students.empty()) {
        cout << "\n  No student records found.\n";
        pauseScreen();
        return;
    }

    printTableHeader();
    for (const auto& s : students)
        printStudentRow(s);

    cout << "\n  Total Records: " << students.size() << "\n";
    pauseScreen();
}

// 3. Search Student
void searchStudent() {
    printHeader("  SEARCH STUDENT");

    cout << "  Search by:\n"
         << "  [1] Student ID\n"
         << "  [2] Name\n";
    int choice = getInt("  Choice: ", 1, 2);

    vector<Student> students = loadAll();
    vector<Student> results;

    if (choice == 1) {
        int id = getInt("  Enter Student ID: ", 1);
        for (const auto& s : students)
            if (s.id == id) results.push_back(s);
    } else {
        string keyword = getString("  Enter Name (partial ok): ");
        transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
        for (const auto& s : students) {
            string nameLower = s.name;
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            if (nameLower.find(keyword) != string::npos)
                results.push_back(s);
        }
    }

    if (results.empty()) {
        cout << "\n  [!] No matching records found.\n";
    } else {
        cout << "\n  Found " << results.size() << " result(s):\n";
        for (const auto& s : results)
            printStudentDetail(s);
    }
    pauseScreen();
}

// 4. Update Student
void updateStudent() {
    printHeader("  UPDATE STUDENT");

    int id = getInt("  Enter Student ID to update: ", 1);
    vector<Student> students = loadAll();

    bool found = false;
    for (auto& s : students) {
        if (s.id != id) continue;
        found = true;

        cout << "\n  Current Record:\n";
        printStudentDetail(s);

        cout << "\n  Enter new values (leave blank to keep current):\n\n";

        string input;

        cout << "  Name [" << s.name << "]: ";
        getline(cin, input);
        if (!trim(input).empty()) s.name = trim(input);

        input = to_string(s.age);
        int newAge = getInt("  Age [" + input + "]: ", 5, 100);
        s.age = newAge;

        cout << "  Course [" << s.course << "]: ";
        getline(cin, input);
        if (!trim(input).empty()) s.course = trim(input);

        double newGPA = getDouble("  GPA [" + to_string(s.gpa) + "]: ", 0.0, 4.0);
        s.gpa = newGPA;

        cout << "  Email [" << s.email << "]: ";
        getline(cin, input);
        if (!trim(input).empty()) s.email = trim(input);

        cout << "  Phone [" << s.phone << "]: ";
        getline(cin, input);
        if (!trim(input).empty()) s.phone = trim(input);

        break;
    }

    if (!found) {
        cout << "\n  [!] Student ID " << id << " not found.\n";
    } else {
        saveAll(students);
        cout << "\n  [✓] Student record updated successfully!\n";
    }
    pauseScreen();
}

// 5. Delete Student
void deleteStudent() {
    printHeader("  DELETE STUDENT");

    int id = getInt("  Enter Student ID to delete: ", 1);
    vector<Student> students = loadAll();

    auto it = find_if(students.begin(), students.end(),
                      [id](const Student& s){ return s.id == id; });

    if (it == students.end()) {
        cout << "\n  [!] Student ID " << id << " not found.\n";
        pauseScreen();
        return;
    }

    cout << "\n  Record to delete:\n";
    printStudentDetail(*it);

    cout << "  Confirm deletion? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(confirm) == 'Y') {
        students.erase(it);
        saveAll(students);
        cout << "\n  [✓] Student deleted successfully.\n";
    } else {
        cout << "\n  [!] Deletion cancelled.\n";
    }
    pauseScreen();
}

// 6. Display Statistics
void showStatistics() {
    printHeader("  STATISTICS");
    vector<Student> students = loadAll();

    if (students.empty()) {
        cout << "\n  No records available for statistics.\n";
        pauseScreen();
        return;
    }

    double totalGPA = 0, minGPA = 4.0, maxGPA = 0.0;
    int    totalAge = 0;
    string topStudent, bottomStudent;

    for (const auto& s : students) {
        totalGPA += s.gpa;
        totalAge += s.age;
        if (s.gpa > maxGPA) { maxGPA = s.gpa; topStudent = s.name; }
        if (s.gpa < minGPA) { minGPA = s.gpa; bottomStudent = s.name; }
    }

    double avgGPA = totalGPA / students.size();
    double avgAge = (double)totalAge / students.size();

    // GPA distribution
    int gpaA=0, gpaB=0, gpaC=0, gpaD=0, gpaF=0;
    for (const auto& s : students) {
        if      (s.gpa >= 3.5) gpaA++;
        else if (s.gpa >= 3.0) gpaB++;
        else if (s.gpa >= 2.5) gpaC++;
        else if (s.gpa >= 2.0) gpaD++;
        else                   gpaF++;
    }

    cout << "\n";
    printLine('-', 50);
    cout << "  Total Students   : " << students.size()           << "\n"
         << "  Average GPA      : " << fixed << setprecision(2) << avgGPA << "\n"
         << "  Highest GPA      : " << maxGPA << " (" << topStudent << ")\n"
         << "  Lowest GPA       : " << minGPA << " (" << bottomStudent << ")\n"
         << "  Average Age      : " << setprecision(1) << avgAge << "\n";
    printLine('-', 50);
    cout << "  GPA Distribution :\n"
         << "    A (3.5-4.0) : " << gpaA << " students\n"
         << "    B (3.0-3.4) : " << gpaB << " students\n"
         << "    C (2.5-2.9) : " << gpaC << " students\n"
         << "    D (2.0-2.4) : " << gpaD << " students\n"
         << "    F (< 2.0)   : " << gpaF << " students\n";
    printLine('-', 50);
    pauseScreen();
}

// ─────────────────────────────────────────────
//  MAIN MENU
// ─────────────────────────────────────────────
void showMenu() {
    printHeader("  STUDENT MANAGEMENT SYSTEM");
    cout << "\n"
         << "    [1]  Add New Student\n"
         << "    [2]  Display All Students\n"
         << "    [3]  Search Student\n"
         << "    [4]  Update Student\n"
         << "    [5]  Delete Student\n"
         << "    [6]  View Statistics\n"
         << "    [0]  Exit\n"
         << "\n";
    printLine();
}

int main() {
    cout << "\n  Initializing Student Management System...\n";

    // Ensure data file exists
    ifstream check(DATA_FILE);
    if (!check.is_open()) {
        ofstream create(DATA_FILE);
        cout << "  Data file created: " << DATA_FILE << "\n";
    }

    int choice = -1;
    do {
        showMenu();
        choice = getInt("  Enter your choice: ", 0, 6);
        switch (choice) {
            case 1: addStudent();    break;
            case 2: displayAll();   break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: showStatistics(); break;
            case 0:
                cout << "\n  Thank you for using Student Management System. Goodbye!\n\n";
                break;
            default:
                cout << "\n  [!] Invalid option. Try again.\n";
                pauseScreen();
        }
    } while (choice != 0);

    return 0;
}
