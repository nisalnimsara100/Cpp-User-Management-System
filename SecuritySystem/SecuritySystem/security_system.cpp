#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

#define _CRT_SECURE_NO_WARNINGS // Suppress localtime_s warnings

void displayStartupScreen() {
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "      Awesome Security App          \n";
    std::cout << "====================================\n";
    std::cout << "Security Management - Personnel - Reports\n";
    std::cout << "& much more!\n";
    std::cout << "====================================\n\n";
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

bool authenticateAdmin() {
    std::string username, password;
    std::cout << "Admin Login\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;
    std::cin.ignore();
    return (username == "admin" && password == "admin");
}

bool authenticateUser(const std::string& studentId, const std::string& password) {
    std::ifstream file("users.csv");
    if (!file.is_open()) return false;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, pwd;
        std::getline(ss, id, ',');
        std::getline(ss, pwd, ',');
        if (id == studentId && pwd == password) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

class User {
private:
    std::string studentId;
    std::string password;
    std::string name;
    std::string contact;
    std::string currentLocation;
    std::string lastLocation1;
    std::string lastLocation1Time;
    std::string lastLocation2;
    std::string lastLocation2Time;
    std::time_t workStartTime;
    std::time_t workEndTime;
    double totalSalary;

public:
    User(std::string id, std::string pwd, std::string n, std::string c)
        : studentId(id), password(pwd), name(n), contact(c), currentLocation(""),
        lastLocation1(""), lastLocation1Time(""), lastLocation2(""), lastLocation2Time(""),
        workStartTime(0), workEndTime(0), totalSalary(0.0) {}

    std::string getStudentId() const { return studentId; }
    std::string getPassword() const { return password; }
    std::string getName() const { return name; }
    std::string getContact() const { return contact; }
    std::string getCurrentLocation() const { return currentLocation; }
    std::string getLastLocation1() const { return lastLocation1; }
    std::string getLastLocation1Time() const { return lastLocation1Time; }
    std::string getLastLocation2() const { return lastLocation2; }
    std::string getLastLocation2Time() const { return lastLocation2Time; }
    std::time_t getWorkStartTime() const { return workStartTime; }
    std::time_t getWorkEndTime() const { return workEndTime; }
    double getTotalSalary() const { return totalSalary; }

    void setPassword(const std::string& pwd) { password = pwd; }
    void setName(const std::string& n) { name = n; }
    void setContact(const std::string& c) { contact = c; }
    void setCurrentLocation(const std::string& loc) {
        lastLocation2 = lastLocation1;
        lastLocation2Time = lastLocation1Time;
        lastLocation1 = currentLocation;

        std::time_t now = std::time(nullptr);
        std::tm tm;
        localtime_s(&tm, &now);
        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        lastLocation1Time = ss.str();
        currentLocation = loc;
    }
    void startWork() { workStartTime = std::time(nullptr); }
    void endWork() {
        workEndTime = std::time(nullptr);
        std::tm tm;
        localtime_s(&tm, &workEndTime);
        double minutes = difftime(workEndTime, workStartTime) / 60.0;
        totalSalary += minutes * 35.0; // Rs. 35 per minute for overtime/work time
    }
    void resetWorkTimes() { workStartTime = 0; workEndTime = 0; }
    void addSalary(double amount) { totalSalary += amount; }
    void setTotalSalary(double amount) { totalSalary = amount; }

    std::string toCSV() const {
        return studentId + "," + password + "," + name + "," + contact + "," +
            currentLocation + "," + lastLocation1 + "," + lastLocation1Time + "," +
            lastLocation2 + "," + lastLocation2Time + "," +
            std::to_string(totalSalary);
    }

    static User fromCSV(const std::string& line) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;
        while (std::getline(ss, item, ',')) {
            tokens.push_back(item);
        }
        User user(tokens[0], tokens[1], tokens[2], tokens[3]);
        user.currentLocation = tokens[4];
        user.lastLocation1 = tokens[5];
        user.lastLocation1Time = tokens[6];
        user.lastLocation2 = tokens[7];
        user.lastLocation2Time = tokens[8];
        user.totalSalary = std::stod(tokens[9]);
        return user;
    }
};

class DutyRoster {
private:
    int rosterId;
    std::string studentId;
    std::string shiftStart;
    std::string shiftEnd;
    std::string colleagues;

public:
    DutyRoster(int rId, std::string sId, std::string start, std::string end, std::string cols)
        : rosterId(rId), studentId(sId), shiftStart(start), shiftEnd(end), colleagues(cols) {}

    std::string getStudentId() const { return studentId; }
    std::string getShiftStart() const { return shiftStart; }
    std::string getShiftEnd() const { return shiftEnd; }
    std::string getColleagues() const { return colleagues; }

    std::string toCSV() const {
        return std::to_string(rosterId) + "," + studentId + "," +
            shiftStart + "," + shiftEnd + "," + colleagues;
    }

    static DutyRoster fromCSV(const std::string& line) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;
        while (std::getline(ss, item, ',')) {
            tokens.push_back(item);
        }
        return DutyRoster(std::stoi(tokens[0]), tokens[1], tokens[2], tokens[3], tokens[4]);
    }
};

class SalaryDetails {
private:
    int salaryId;
    std::string studentId;
    double basicSalary;
    double overtimeHours;
    double allowance;
    double totalSalary;

public:
    SalaryDetails(int sId, std::string s, double basic, double ot, double allow)
        : salaryId(sId), studentId(s), basicSalary(basic), overtimeHours(ot), allowance(allow) {
        totalSalary = basic + (ot * 35) + allow; // Rs. 35 per minute for overtime
    }

    std::string getStudentId() const { return studentId; }
    double getBasicSalary() const { return basicSalary; }
    double getOvertimeHours() const { return overtimeHours; }
    double getAllowance() const { return allowance; }
    double getTotalSalary() const { return totalSalary; }

    void setBasicSalary(double basic) { basicSalary = basic; updateTotal(); }
    void setOvertimeHours(double ot) { overtimeHours = ot; updateTotal(); }
    void setAllowance(double allow) { allowance = allow; updateTotal(); }

private:
    void updateTotal() {
        totalSalary = basicSalary + (overtimeHours * 35) + allowance;
    }

public:
    std::string toCSV() const {
        return std::to_string(salaryId) + "," + studentId + "," +
            std::to_string(basicSalary) + "," + std::to_string(overtimeHours) + "," +
            std::to_string(allowance) + "," + std::to_string(totalSalary);
    }

    static SalaryDetails fromCSV(const std::string& line) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;
        while (std::getline(ss, item, ',')) {
            tokens.push_back(item);
        }
        return SalaryDetails(std::stoi(tokens[0]), tokens[1],
            std::stod(tokens[2]), std::stod(tokens[3]), std::stod(tokens[4]));
    }
};

class SecuritySystem {
private:
    std::string usersFile = "users.csv";
    std::string rosterFile = "duty_roster.csv";
    std::string salaryFile = "salary_details.csv";
    std::string vacationFile = "vacation_records.csv";
    std::string reportsFile = "problem_reports.csv";
    int nextRosterId = 1;
    int nextSalaryId = 1;
    int nextVacationId = 1;
    int nextReportId = 1;

    void initializeFiles() {
        std::ofstream file(usersFile);
        file << "student_id,password,name,contact,current_location,last_location1,last_location1_time,last_location2,last_location2_time,total_salary\n";
        file.close();

        file.open(rosterFile);
        file << "roster_id,student_id,shift_start,shift_end,colleagues\n";
        file.close();

        file.open(salaryFile);
        file << "salary_id,student_id,basic_salary,overtime_hours,allowance,total_salary\n";
        file.close();

        file.open(vacationFile);
        file << "vacation_id,student_id,vacation_date\n";
        file.close();

        file.open(reportsFile);
        file << "report_id,student_id,problem\n";
        file.close();
    }

    int getNextId(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        int maxId = 0;
        std::getline(file, line);
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string idStr;
            std::getline(ss, idStr, ',');
            int id = std::stoi(idStr);
            if (id > maxId) maxId = id;
        }
        file.close();
        return maxId + 1;
    }

    void saveUsers(const std::vector<User>& users) {
        std::ofstream file(usersFile);
        file << "student_id,password,name,contact,current_location,last_location1,last_location1_time,last_location2,last_location2_time,total_salary\n";
        for (const auto& user : users) {
            file << user.toCSV() << "\n";
        }
        file.close();
    }

public:
    SecuritySystem() {
        std::ifstream check(usersFile);
        if (!check.good()) {
            initializeFiles();
        }
        check.close();
        nextRosterId = getNextId(rosterFile);
        nextSalaryId = getNextId(salaryFile);
        nextVacationId = getNextId(vacationFile);
        nextReportId = getNextId(reportsFile);
    }

    void registerUser() {
        std::string studentId, password, name, contact;
        std::cout << "Register New User\n";
        std::cout << "Enter Student ID: ";
        std::cin >> studentId;
        std::cout << "Enter Password: ";
        std::cin >> password;
        std::cout << "Enter Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "Enter Contact: ";
        std::getline(std::cin, contact);

        std::vector<User> users;
        {
            std::ifstream file(usersFile);
            std::string line;
            std::getline(file, line);
            while (std::getline(file, line)) {
                users.push_back(User::fromCSV(line));
            }
        }

        for (const auto& user : users) {
            if (user.getStudentId() == studentId) {
                std::cout << "Student ID already exists!\n";
                return;
            }
        }

        users.emplace_back(studentId, password, name, contact);
        saveUsers(users);
        std::cout << "User registered successfully!\n";
    }

    void editUser() {
        std::string studentId;
        std::cout << "Enter Student ID to edit: ";
        std::cin >> studentId;
        std::cin.ignore();

        std::vector<User> users;
        {
            std::ifstream file(usersFile);
            std::string line;
            std::getline(file, line);
            while (std::getline(file, line)) {
                users.push_back(User::fromCSV(line));
            }
        }

        bool found = false;
        for (auto& user : users) {
            if (user.getStudentId() == studentId) {
                found = true;
                std::string password, name, contact;
                std::cout << "Enter new Password (leave blank to keep unchanged): ";
                std::getline(std::cin, password);
                std::cout << "Enter new Name (leave blank to keep unchanged): ";
                std::getline(std::cin, name);
                std::cout << "Enter new Contact (leave blank to keep unchanged): ";
                std::getline(std::cin, contact);

                if (!password.empty()) user.setPassword(password);
                if (!name.empty()) user.setName(name);
                if (!contact.empty()) user.setContact(contact);
                break;
            }
        }

        if (found) {
            saveUsers(users);
            std::cout << "User updated successfully!\n";
        }
        else {
            std::cout << "User not found!\n";
        }
    }

    void deleteUser() {
        std::string studentId;
        std::cout << "Enter Student ID to delete: ";
        std::cin >> studentId;
        std::cin.ignore();

        std::vector<User> users;
        {
            std::ifstream file(usersFile);
            std::string line;
            std::getline(file, line);
            while (std::getline(file, line)) {
                users.push_back(User::fromCSV(line));
            }
        }

        std::vector<User> updatedUsers;
        bool found = false;
        for (const auto& user : users) {
            if (user.getStudentId() == studentId) {
                found = true;
                continue;
            }
            updatedUsers.push_back(user);
        }

        if (found) {
            saveUsers(updatedUsers);
            std::cout << "User deleted successfully!\n";
        }
        else {
            std::cout << "User not found!\n";
        }
    }

    void checkUser() {
        std::string studentId;
        std::cout << "Enter Student ID to check: ";
        std::cin >> studentId;
        std::cin.ignore();

        std::ifstream file(usersFile);
        std::string line;
        std::getline(file, line);
        while (std::getline(file, line)) {
            User user = User::fromCSV(line);
            if (user.getStudentId() == studentId) {
                std::cout << "Student ID: " << user.getStudentId() << "\n";
                std::cout << "Name: " << user.getName() << "\n";
                std::cout << "Contact: " << user.getContact() << "\n";
                std::cout << "Current Location: " << user.getCurrentLocation() << "\n";
                std::cout << "Last Location 1: " << user.getLastLocation1() << " at " << user.getLastLocation1Time() << "\n";
                std::cout << "Last Location 2: " << user.getLastLocation2() << " at " << user.getLastLocation2Time() << "\n";
                std::cout << "Total Salary: Rs. " << user.getTotalSalary() << "\n";
                file.close();
                return;
            }
        }
        file.close();
        std::cout << "User not found!\n";
    }

    void addSalary() {
        std::string studentId;
        double basic, overtime, allowance;
        std::cout << "Enter Student ID: ";
        std::cin >> studentId;
        std::cout << "Enter Basic Salary: ";
        std::cin >> basic;
        std::cout << "Enter Overtime Hours: ";
        std::cin >> overtime;
        std::cout << "Enter Allowance: ";
        std::cin >> allowance;
        std::cin.ignore();

        std::ofstream file(salaryFile, std::ios::app);
        if (file.is_open()) {
            SalaryDetails salary(nextSalaryId++, studentId, basic, overtime, allowance);
            file << salary.toCSV() << "\n";
            file.close();

            std::vector<User> users;
            {
                std::ifstream userFile(usersFile);
                std::string line;
                std::getline(userFile, line);
                while (std::getline(userFile, line)) {
                    users.push_back(User::fromCSV(line));
                }
            }

            for (auto& user : users) {
                if (user.getStudentId() == studentId) {
                    user.addSalary(salary.getTotalSalary());
                    break;
                }
            }
            saveUsers(users);

            std::cout << "Salary added successfully!\n";
        }
        else {
            std::cerr << "Error: Could not open " << salaryFile << "\n";
        }
    }

    void viewSalaries() {
        std::ifstream salaryFileStream(salaryFile);
        std::ifstream userFileStream(usersFile);
        if (!salaryFileStream.is_open() || !userFileStream.is_open()) {
            std::cerr << "Error: Could not open files.\n";
            return;
        }

        std::vector<User> users;
        std::string line;
        std::getline(userFileStream, line);
        while (std::getline(userFileStream, line)) {
            users.push_back(User::fromCSV(line));
        }
        userFileStream.close();

        std::cout << "\nSalary Details:\n";
        std::getline(salaryFileStream, line);
        while (std::getline(salaryFileStream, line)) {
            SalaryDetails salary = SalaryDetails::fromCSV(line);
            for (const auto& user : users) {
                if (user.getStudentId() == salary.getStudentId()) {
                    std::cout << "Student ID: " << salary.getStudentId() << "\n";
                    std::cout << "Name: " << user.getName() << "\n";
                    std::cout << "Basic: Rs. " << salary.getBasicSalary() << "\n";
                    std::cout << "Overtime: " << salary.getOvertimeHours() << " hrs\n";
                    std::cout << "Allowance: Rs. " << salary.getAllowance() << "\n";
                    std::cout << "Total: Rs. " << salary.getTotalSalary() << "\n";
                    std::cout << "------------------------\n";
                }
            }
        }
        salaryFileStream.close();
    }

    void editSalary() {
        std::string studentId;
        std::cout << "Enter Student ID to edit salary: ";
        std::cin >> studentId;
        std::cin.ignore();

        std::vector<SalaryDetails> salaries;
        {
            std::ifstream file(salaryFile);
            std::string line;
            std::getline(file, line);
            while (std::getline(file, line)) {
                salaries.push_back(SalaryDetails::fromCSV(line));
            }
        }

        bool found = false;
        for (auto& salary : salaries) {
            if (salary.getStudentId() == studentId) {
                found = true;
                double basic, overtime, allowance;
                std::cout << "Enter new Basic Salary (current: " << salary.getBasicSalary() << "): ";
                std::cin >> basic;
                std::cout << "Enter new Overtime Hours (current: " << salary.getOvertimeHours() << "): ";
                std::cin >> overtime;
                std::cout << "Enter new Allowance (current: " << salary.getAllowance() << "): ";
                std::cin >> allowance;
                std::cin.ignore();

                salary.setBasicSalary(basic);
                salary.setOvertimeHours(overtime);
                salary.setAllowance(allowance);

                std::vector<User> users;
                {
                    std::ifstream userFile(usersFile);
                    std::string line;
                    std::getline(userFile, line);
                    while (std::getline(userFile, line)) {
                        users.push_back(User::fromCSV(line));
                    }
                }

                for (auto& user : users) {
                    if (user.getStudentId() == studentId) {
                        double oldTotal = user.getTotalSalary();
                        double salaryDiff = salary.getTotalSalary() - (oldTotal - user.getTotalSalary());
                        user.setTotalSalary(oldTotal + salaryDiff);
                        break;
                    }
                }
                saveUsers(users);
                break;
            }
        }

        if (found) {
            std::ofstream file(salaryFile);
            file << "salary_id,student_id,basic_salary,overtime_hours,allowance,total_salary\n";
            for (const auto& salary : salaries) {
                file << salary.toCSV() << "\n";
            }
            file.close();
            std::cout << "Salary updated successfully!\n";
        }
        else {
            std::cout << "Salary record not found!\n";
        }
    }

    void displayRealTimeStatus() {
        std::ifstream userFileStream(usersFile);
        std::ifstream rosterFileStream(rosterFile);
        if (!userFileStream.is_open() || !rosterFileStream.is_open()) {
            std::cerr << "Error: Could not open files.\n";
            return;
        }

        std::vector<User> users;
        std::string line;
        std::getline(userFileStream, line);
        while (std::getline(userFileStream, line)) {
            users.push_back(User::fromCSV(line));
        }
        userFileStream.close();

        std::vector<DutyRoster> rosters;
        std::getline(rosterFileStream, line);
        while (std::getline(rosterFileStream, line)) {
            rosters.push_back(DutyRoster::fromCSV(line));
        }
        rosterFileStream.close();

        std::time_t now = std::time(nullptr);
        std::tm tm;
        localtime_s(&tm, &now);
        std::cout << "\nReal-Time Status (as of " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "):\n";
        for (const auto& user : users) {
            bool onDuty = user.getWorkStartTime() != 0 && user.getWorkEndTime() == 0;
            std::string shiftInfo = "Not scheduled";
            for (const auto& roster : rosters) {
                if (roster.getStudentId() == user.getStudentId()) {
                    std::tm tmStart = {}, tmEnd = {};
                    std::stringstream ssStart(roster.getShiftStart()), ssEnd(roster.getShiftEnd());
                    ssStart >> std::get_time(&tmStart, "%Y-%m-%d %H:%M:%S");
                    ssEnd >> std::get_time(&tmEnd, "%Y-%m-%d %H:%M:%S");
                    std::time_t startTime = mktime(&tmStart);
                    std::time_t endTime = mktime(&tmEnd);
                    if (now >= startTime && now <= endTime) {
                        shiftInfo = "Shift: " + roster.getShiftStart() + " to " + roster.getShiftEnd();
                        break;
                    }
                }
            }
            std::cout << "Student ID: " << user.getStudentId() << "\n";
            std::cout << "Name: " << user.getName() << "\n";
            std::cout << "Current Location: " << user.getCurrentLocation() << "\n";
            std::cout << "Status: " << (onDuty ? "On Duty" : "Off Duty") << "\n";
            std::cout << shiftInfo << "\n";
            std::cout << "------------------------\n";
        }
    }

    void addDutyRoster(const std::string& studentId, const std::string& start, const std::string& end, const std::string& cols) {
        DutyRoster roster(nextRosterId++, studentId, start, end, cols);
        std::ofstream file(rosterFile, std::ios::app);
        if (file.is_open()) {
            file << roster.toCSV() << "\n";
            file.close();
            std::cout << "Duty roster added successfully!\n";
        }
        else {
            std::cerr << "Error: Could not open " << rosterFile << "\n";
        }
    }

    void addVacation(const std::string& studentId, const std::string& date) {
        std::ofstream file(vacationFile, std::ios::app);
        if (file.is_open()) {
            file << nextVacationId++ << "," << studentId << "," << date << "\n";
            file.close();
            std::cout << "Vacation record added successfully!\n";
        }
        else {
            std::cerr << "Error: Could not open " << vacationFile << "\n";
        }
    }

    void reportProblem(const std::string& studentId, const std::string& problem) {
        std::ofstream file(reportsFile, std::ios::app);
        if (file.is_open()) {
            file << nextReportId++ << "," << studentId << "," << problem << "\n";
            file.close();
            std::cout << "Problem reported successfully!\n";
        }
        else {
            std::cerr << "Error: Could not open " << reportsFile << "\n";
        }
    }

    void viewDutyRoster(const std::string& studentId) {
        std::ifstream file(rosterFile);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << rosterFile << "\n";
            return;
        }

        std::string line;
        std::cout << "\nDuty Roster for Student ID: " << studentId << "\n";
        std::getline(file, line);
        while (std::getline(file, line)) {
            DutyRoster roster = DutyRoster::fromCSV(line);
            if (roster.getStudentId() == studentId) {
                std::cout << "Shift: " << roster.getShiftStart() << " to " << roster.getShiftEnd() << "\n";
                std::cout << "Colleagues: " << roster.getColleagues() << "\n";
                std::cout << "------------------------\n";
            }
        }
        file.close();
    }

    std::string selectLocation() {
        int choice;
        std::cout << "Select Current Building:\n";
        std::cout << "1. Location A\n";
        std::cout << "2. Location B\n";
        std::cout << "3. Location C\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: return "Location A";
        case 2: return "Location B";
        case 3: return "Location C";
        default:
            std::cout << "Invalid choice! Defaulting to Location A.\n";
            return "Location A";
        }
    }

    void adminMenu() {
        int choice;
        while (true) {
            std::cout << "\nAdmin Menu\n";
            std::cout << "1. Register New User\n";
            std::cout << "2. Edit User Details\n";
            std::cout << "3. Delete User\n";
            std::cout << "4. Check User Details\n";
            std::cout << "5. Add Salary\n";
            std::cout << "6. View Salaries\n";
            std::cout << "7. Edit Salary\n";
            std::cout << "8. Add Duty Roster\n";
            std::cout << "9. View Real-Time Status\n";
            std::cout << "10. Logout\n";
            std::cout << "Enter choice: ";
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 10) break;

            switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                editUser();
                break;
            case 3:
                deleteUser();
                break;
            case 4:
                checkUser();
                break;
            case 5:
                addSalary();
                break;
            case 6:
                viewSalaries();
                break;
            case 7:
                editSalary();
                break;
            case 8: {
                std::string studentId, start, end, cols;
                std::cout << "Enter Student ID: ";
                std::cin >> studentId;
                std::cin.ignore();
                std::cout << "Enter Shift Start (YYYY-MM-DD HH:MM:SS): ";
                std::getline(std::cin, start);
                std::cout << "Enter Shift End (YYYY-MM-DD HH:MM:SS): ";
                std::getline(std::cin, end);
                std::cout << "Enter Colleagues: ";
                std::getline(std::cin, cols);
                addDutyRoster(studentId, start, end, cols);
                break;
            }
            case 9:
                displayRealTimeStatus();
                break;
            default:
                std::cout << "Invalid choice!\n";
            }
        }
    }

    void userMenu(const std::string& studentId) {
        std::vector<User> users;
        {
            std::ifstream file(usersFile);
            std::string line;
            std::getline(file, line);
            while (std::getline(file, line)) {
                users.push_back(User::fromCSV(line));
            }
        }

        User* currentUser = nullptr;
        for (auto& user : users) {
            if (user.getStudentId() == studentId) {
                currentUser = &user;
                break;
            }
        }

        if (!currentUser) {
            std::cout << "User not found!\n";
            return;
        }

        int choice;
        while (true) {
            std::cout << "\nUser Menu (Student ID: " << studentId << ")\n";
            std::cout << "Current Building: " << currentUser->getCurrentLocation() << "\n";
            std::cout << "1. Start Work\n";
            std::cout << "2. End Work\n";
            std::cout << "3. Change Location\n";
            std::cout << "4. Report a Problem\n";
            std::cout << "5. Add Vacation\n";
            std::cout << "6. View Duty Roster\n";
            std::cout << "7. View Real-Time Status\n";
            std::cout << "8. Logout\n";
            std::cout << "Enter choice: ";
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 8) break;

            switch (choice) {
            case 1:
                currentUser->startWork();
                {
                    std::time_t startTime = currentUser->getWorkStartTime();
                    std::tm tm;
                    localtime_s(&tm, &startTime);
                    std::cout << "Work started at: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";
                }
                break;
            case 2:
                if (currentUser->getWorkStartTime() == 0) {
                    std::cout << "You haven't started work yet!\n";
                }
                else {
                    currentUser->endWork();
                    {
                        std::time_t endTime = currentUser->getWorkEndTime();
                        std::tm tm;
                        localtime_s(&tm, &endTime);
                        std::cout << "Work ended at: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";
                    }
                    std::cout << "Total Salary Updated: Rs. " << currentUser->getTotalSalary() << "\n";
                    currentUser->resetWorkTimes();
                }
                break;
            case 3:
                currentUser->setCurrentLocation(selectLocation());
                std::cout << "Location updated to: " << currentUser->getCurrentLocation() << "\n";
                break;
            case 4: {
                std::string problem;
                std::cout << "Enter your problem: ";
                std::getline(std::cin, problem);
                reportProblem(studentId, problem);
                break;
            }
            case 5: {
                std::string date;
                std::cout << "Enter Vacation Date (YYYY-MM-DD): ";
                std::getline(std::cin, date);
                addVacation(studentId, date);
                break;
            }
            case 6:
                viewDutyRoster(studentId);
                break;
            case 7:
                displayRealTimeStatus();
                break;
            default:
                std::cout << "Invalid choice!\n";
            }
        }
        saveUsers(users);
    }
};

int main() {
    displayStartupScreen();

    int roleChoice;
    while (true) {
        std::cout << "\nSelect Role\n";
        std::cout << "1. Admin\n";
        std::cout << "2. User\n";
        std::cout << "Enter choice: ";
        std::cin >> roleChoice;
        std::cin.ignore();

        SecuritySystem system;
        if (roleChoice == 1) {
            if (authenticateAdmin()) {
                system.adminMenu();
            }
            else {
                std::cout << "Invalid admin credentials!\n";
            }
        }
        else if (roleChoice == 2) {
            std::string studentId, password;
            std::cout << "User Login\n";
            std::cout << "Enter Student ID: ";
            std::cin >> studentId;
            std::cout << "Enter Password: ";
            std::cin >> password;
            std::cin.ignore();

            if (authenticateUser(studentId, password)) {
                std::vector<User> users;
                {
                    std::ifstream file("users.csv");
                    std::string line;
                    std::getline(file, line);
                    while (std::getline(file, line)) {
                        users.push_back(User::fromCSV(line));
                    }
                }

                User* currentUser = nullptr;
                for (auto& user : users) {
                    if (user.getStudentId() == studentId) {
                        currentUser = &user;
                        break;
                    }
                }

                if (currentUser) {
                    std::string initialLocation = system.selectLocation();
                    currentUser->setCurrentLocation(initialLocation);
                    std::cout << "Current Building set to: " << initialLocation << "\n";
                    std::cout << "Press Enter to proceed to User Menu...";
                    std::cin.get();
                    system.userMenu(studentId);
                }
                else {
                    std::cout << "User data not found!\n";
                }
            }
            else {
                std::cout << "Invalid user credentials!\n";
            }
        }
        else {
            std::cout << "Invalid choice!\n";
        }
    }
    return 0;
}