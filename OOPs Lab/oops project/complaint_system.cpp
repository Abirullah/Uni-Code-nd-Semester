#include <bits/stdc++.h>
using namespace std;

const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string WHITE = "\033[37m";
const string BRED = "\033[91m";
const string BGREEN = "\033[92m";
const string BYELLOW = "\033[93m";
const string BBLUE = "\033[94m";
const string BMAGENTA = "\033[95m";
const string BCYAN = "\033[96m";
const string BWHITE = "\033[97m";
const string BOLD = "\033[1m";
const string DIM = "\033[2m";
const string BG_BLUE = "\033[44m";
const string BG_CYAN = "\033[46m";

void clearScreen() {
    cout << "\033[2J\033[H";
}

const string STUDENT_FILE = "students.json";
const string ADMIN_FILE = "admins.json";
const string COMPLAINT_FILE = "complaints.json";
const string TEACHERS_FILE = "teachers.json";
const string ADMISSIONS_FILE = "admissions.json";
const string EXAMINATIONS_FILE = "examinations.json";
const string DRIVERS_FILE = "drivers.json";

void printLine(char ch = '-', int width = 60) {
    cout << CYAN << string(width, ch) << RESET << "\n";
}

void printBanner(const string& title) {
    clearScreen();
    const int width = 60;
    string safeTitle = title.substr(0, max(0, width - 4));
    int padLeft = max(0, (width - 2 - (int)safeTitle.size()) / 2);
    int padRight = max(0, width - 2 - (int)safeTitle.size() - padLeft);
    string border(width, '=');

    cout << BCYAN << border << RESET << "\n";
    cout << BCYAN << "|" << RESET
         << string(padLeft, ' ') << BOLD << BWHITE << safeTitle << RESET
         << string(padRight, ' ') << BCYAN << "|" << RESET << "\n";
    cout << BCYAN << border << RESET << "\n\n";
}

void printHeader(const string& text) {
    cout << "\n" << BOLD << BYELLOW << "  >> " << text << RESET << "\n";
    printLine('-', 50);
}

void success(const string& msg) {
    cout << "\n  " << BGREEN << "✔  " << msg << RESET << "\n";
}

void error(const string& msg) {
    cout << "\n  " << BRED << "✘  " << msg << RESET << "\n";
}

void info(const string& msg) {
    cout << "  " << BCYAN << "ℹ  " << msg << RESET << "\n";
}

void menuOption(int num, const string& label, const string& color = BWHITE) {
    cout << "   " << BCYAN << "[" << num << "]" << RESET
         << "  " << color << label << RESET << "\n";
}

string ask(const string& prompt) {
    cout << "  " << BYELLOW << prompt << RESET << " ";
    string input;
    getline(cin, input);
    return input;
}

void waitEnter() {
    cout << "\n  " << DIM << "Press Enter to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string now() {
    time_t t = time(nullptr);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&t));
    return string(buf);
}

string trim(const string& text) {
    size_t start = 0;
    while (start < text.size() && isspace(static_cast<unsigned char>(text[start]))) {
        start++;
    }

    size_t end = text.size();
    while (end > start && isspace(static_cast<unsigned char>(text[end - 1]))) {
        end--;
    }

    return text.substr(start, end - start);
}

string escapeJson(const string& value) {
    string out;
    for (char ch : value) {
        switch (ch) {
            case '\\': out += "\\\\"; break;
            case '"':  out += "\\\""; break;
            case '\n': out += "\\n"; break;
            case '\r': break;
            case '\t': out += "\\t"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            default: out += ch; break;
        }
    }
    return out;
}

string jsonString(const string& value) {
    return "\"" + escapeJson(value) + "\"";
}

string readFile(const string& file) {
    ifstream in(file);
    if (!in) return "";
    return string((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
}

void writeFile(const string& file, const string& text) {
    ofstream out(file, ios::trunc);
    out << text;
}

bool parseIntText(const string& text, int& value) {
    try {
        string cleaned = trim(text);
        size_t pos = 0;
        value = stoi(cleaned, &pos);
        return pos == cleaned.size();
    } catch (...) {
        return false;
    }
}

vector<string> extractJsonObjects(const string& text) {
    vector<string> objects;
    size_t startArray = text.find('[');
    size_t endArray = text.rfind(']');

    if (startArray == string::npos || endArray == string::npos || endArray <= startArray) {
        return objects;
    }

    bool inString = false;
    bool escape = false;
    int depth = 0;
    size_t objectStart = string::npos;

    for (size_t i = startArray; i <= endArray; ++i) {
        char ch = text[i];

        if (inString) {
            if (escape) {
                escape = false;
            } else if (ch == '\\') {
                escape = true;
            } else if (ch == '"') {
                inString = false;
            }
            continue;
        }

        if (ch == '"') {
            inString = true;
        } else if (ch == '{') {
            if (depth == 0) objectStart = i;
            depth++;
        } else if (ch == '}') {
            if (depth > 0) depth--;
            if (depth == 0 && objectStart != string::npos) {
                objects.push_back(text.substr(objectStart, i - objectStart + 1));
                objectStart = string::npos;
            }
        }
    }

    return objects;
}

string readJsonValue(const string& object, const string& key) {
    size_t pos = object.find("\"" + key + "\"");
    if (pos == string::npos) return "";

    pos = object.find(':', pos);
    if (pos == string::npos) return "";

    pos++;
    while (pos < object.size() && isspace(static_cast<unsigned char>(object[pos]))) {
        pos++;
    }

    if (pos >= object.size()) return "";

    if (object[pos] == '"') {
        pos++;
        string value;
        bool escape = false;

        for (; pos < object.size(); ++pos) {
            char ch = object[pos];

            if (escape) {
                switch (ch) {
                    case '"': value += '"'; break;
                    case '\\': value += '\\'; break;
                    case 'n': value += '\n'; break;
                    case 'r': value += '\r'; break;
                    case 't': value += '\t'; break;
                    case 'b': value += '\b'; break;
                    case 'f': value += '\f'; break;
                    default: value += ch; break;
                }
                escape = false;
            } else if (ch == '\\') {
                escape = true;
            } else if (ch == '"') {
                break;
            } else {
                value += ch;
            }
        }

        return value;
    }

    size_t end = pos;
    while (end < object.size() && object[end] != ',' && object[end] != '}') {
        end++;
    }
    return trim(object.substr(pos, end - pos));
}

int readJsonInt(const string& object, const string& key, int fallback = 0) {
    string value = readJsonValue(object, key);
    if (value.empty()) return fallback;

    try {
        return stoi(value);
    } catch (...) {
        return fallback;
    }
}

class Person {
public:
    int id = 0;
    string name;
    string email;
    string password;

    virtual ~Person() = default;
    

    string toJsonCommon() const {
        ostringstream out;
        out << "  {\n";
        out << "    \"id\": " << id << ",\n";
        out << "    \"name\": " << jsonString(name) << ",\n";
        out << "    \"email\": " << jsonString(email) << ",\n";
        out << "    \"password\": " << jsonString(password) << "\n";
        out << "  }";
        return out.str();
    }

    bool authenticate(const string& loginEmail, const string& loginPassword) const {
        return email == loginEmail && password == loginPassword;
    }

    template <typename T>
    static bool login(const string& loginEmail, const string& loginPassword, const vector<T>& list, T& out) {
        for (const auto& item : list) {
            if (item.authenticate(loginEmail, loginPassword)) {
                out = item;
                return true;
            }
        }
        return false;
    }
};

template <typename T, typename Loader>
vector<T> loadJsonList(const string& file, Loader loader) {
    vector<T> items;
    string text = readFile(file);

    for (const string& object : extractJsonObjects(text)) {
        items.push_back(loader(object));
    }

    return items;
}

template <typename T>
void saveJsonList(const string& file, const vector<T>& items) {
    if (items.empty()) {
        writeFile(file, "[]\n");
        return;
    }

    ostringstream out;
    out << "[\n";
    for (size_t i = 0; i < items.size(); ++i) {
        out << items[i].toJson();
        if (i + 1 < items.size()) out << ",";
        out << "\n";
    }
    out << "]\n";
    writeFile(file, out.str());
}

class Target {
public:
    int id = 0;
    string name;
    string type;
    string extra;

    string toJson() const {
        ostringstream out;
        out << "  {\n";
        out << "    \"id\": " << id << ",\n";
        out << "    \"name\": " << jsonString(name) << ",\n";
        out << "    \"type\": " << jsonString(type) << ",\n";
        out << "    \"extra\": " << jsonString(extra) << "\n";
        out << "  }";
        return out.str();
    }

    static Target fromJson(const string& object) {
        Target t;
        t.id = readJsonInt(object, "id");
        t.name = readJsonValue(object, "name");
        t.type = readJsonValue(object, "type");
        t.extra = readJsonValue(object, "extra");
        return t;
    }
};

vector<Target> loadTargets(const string& file, const string& type) {
    vector<Target> all = loadJsonList<Target>(file, Target::fromJson);
    vector<Target> filtered;

    for (const auto& target : all) {
        if (target.type == type) {
            filtered.push_back(target);
        }
    }

    return filtered;
}

void saveTargets(const string& file, const vector<Target>& targets) {
    saveJsonList(file, targets);
}

class Complaint {
public:
    int id = 0;
    int studentId = 0;
    string studentName;
    string title;
    string description;
    string category;
    int targetId = 0;
    string targetName;
    string targetType;
    string targetExtra;
    string status;
    string response;
    int rating = 0;
    string createdAt;

    string toJson() const {
        ostringstream out;
        out << "  {\n";
        out << "    \"id\": " << id << ",\n";
        out << "    \"studentId\": " << studentId << ",\n";
        out << "    \"studentName\": " << jsonString(studentName) << ",\n";
        out << "    \"title\": " << jsonString(title) << ",\n";
        out << "    \"description\": " << jsonString(description) << ",\n";
        out << "    \"category\": " << jsonString(category) << ",\n";
        out << "    \"targetId\": " << targetId << ",\n";
        out << "    \"targetName\": " << jsonString(targetName) << ",\n";
        out << "    \"targetType\": " << jsonString(targetType) << ",\n";
        out << "    \"targetExtra\": " << jsonString(targetExtra) << ",\n";
        out << "    \"status\": " << jsonString(status) << ",\n";
        out << "    \"response\": " << jsonString(response) << ",\n";
        out << "    \"rating\": " << rating << ",\n";
        out << "    \"createdAt\": " << jsonString(createdAt) << "\n";
        out << "  }";
        return out.str();
    }

    static Complaint fromJson(const string& object) {
        Complaint c;
        c.id = readJsonInt(object, "id");
        c.studentId = readJsonInt(object, "studentId");
        c.studentName = readJsonValue(object, "studentName");
        c.title = readJsonValue(object, "title");
        c.description = readJsonValue(object, "description");
        c.category = readJsonValue(object, "category");
        c.targetId = readJsonInt(object, "targetId");
        c.targetName = readJsonValue(object, "targetName");
        c.targetType = readJsonValue(object, "targetType");
        c.targetExtra = readJsonValue(object, "targetExtra");
        c.status = readJsonValue(object, "status");
        c.response = readJsonValue(object, "response");
        c.rating = readJsonInt(object, "rating");
        c.createdAt = readJsonValue(object, "createdAt");
        return c;
    }

    static vector<Complaint> loadAll() {
        return loadJsonList<Complaint>(COMPLAINT_FILE, Complaint::fromJson);
    }

    static void saveAll(const vector<Complaint>& list) {
        saveJsonList(COMPLAINT_FILE, list);
    }

    void save() const {
        vector<Complaint> list = loadAll();
        list.push_back(*this);
        saveAll(list);
    }

    static int nextId() {
        int maxId = 0;
        for (const auto& c : loadAll()) {
            maxId = max(maxId, c.id);
        }
        return maxId + 1;
    }

    static string statusColor(const string& s) {
        if (s == "Resolved") return BGREEN;
        if (s == "In Progress") return BYELLOW;
        return BRED;
    }

    void display() const {
        string sc = statusColor(status);

        printLine('-', 56);
        cout << "  " << BOLD << BCYAN << "Complaint #" << id << RESET
             << "   " << sc << "[" << status << "]" << RESET << "\n";
        printLine('-', 56);

        cout << "  " << BWHITE << "Title      : " << RESET << title << "\n";
        cout << "  " << BWHITE << "Category   : " << RESET << BYELLOW << category << RESET << "\n";
        cout << "  " << BWHITE << "Target     : " << RESET << targetName
             << "  (" << targetType << " - " << targetExtra << ")\n";
        cout << "  " << BWHITE << "Student    : " << RESET << studentName
             << "  (ID: " << studentId << ")\n";
        cout << "  " << BWHITE << "Submitted  : " << RESET << DIM << createdAt << RESET << "\n\n";

        cout << "  " << BWHITE << "Description:\n" << RESET;
        cout << "  " << DIM << description << RESET << "\n\n";

        cout << "  " << BWHITE << "Response   :\n" << RESET;
        if (response.empty()) {
            cout << "  " << DIM << "(No response yet)" << RESET << "\n";
        } else {
            cout << "  " << GREEN << response << RESET << "\n";
        }

        if (rating > 0) {
            cout << "\n  " << BWHITE << "Rating     : " << RESET;
            for (int i = 0; i < rating; ++i) cout << BYELLOW << "★" << RESET;
            for (int i = rating; i < 5; ++i) cout << DIM << "☆" << RESET;
            cout << "  " << DIM << "(" << rating << "/5)" << RESET << "\n";
        }
    }
};

class Student : public Person {
public:
    string toJson() const {
        return toJsonCommon();
    }

    static Student fromJson(const string& object) {
        Student s;
        s.id = readJsonInt(object, "id");
        s.name = readJsonValue(object, "name");
        s.email = readJsonValue(object, "email");
        s.password = readJsonValue(object, "password");
        return s;
    }

    static vector<Student> loadAll() {
        return loadJsonList<Student>(STUDENT_FILE, Student::fromJson);
    }

    void save() const {
        vector<Student> list = loadAll();
        list.push_back(*this);
        saveJsonList(STUDENT_FILE, list);
    }

    static bool login(const string& email, const string& password, Student& out) {
        return Person::login(email, password, loadAll(), out);
    }

    bool pickTarget(Complaint& c, const string& file, const string& type, const string& label) {
        vector<Target> list = loadTargets(file, type);
        if (list.empty()) {
            error("No " + label + " available in the system.");
            return false;
        }

        printHeader("Available " + label);
        for (const auto& item : list) {
            cout << "   " << BCYAN << "[" << item.id << "]" << RESET
                 << "  " << BWHITE << item.name << RESET
                 << "  " << DIM << "(" << item.extra << ")" << RESET << "\n";
        }
        cout << "\n";

        int tid = 0;
        string input = ask("Enter " + label + " ID:");
        if (!parseIntText(input, tid)) {
            error("Please enter a valid numeric ID.");
            return false;
        }

        for (const auto& item : list) {
            if (item.id == tid) {
                c.targetId = item.id;
                c.targetName = item.name;
                c.targetType = item.type;
                c.targetExtra = item.extra;
                return true;
            }
        }

        error("Invalid ID. Please try again.");
        return false;
    }

    void submitComplaint() {
        printBanner("Submit a Complaint");

        Complaint c;
        c.id = Complaint::nextId();
        c.studentId = id;
        c.studentName = name;

        printHeader("Select Category");
        menuOption(1, "Teachers", BBLUE);
        menuOption(2, "Admissions", BMAGENTA);
        menuOption(3, "Examinations", BYELLOW);
        menuOption(4, "Drivers", BGREEN);
        menuOption(5, "Others", BWHITE);
        cout << "\n";

        string choice = ask("Your choice:");

        if (choice == "1") {
            c.category = "Teachers";
            if (!pickTarget(c, TEACHERS_FILE, "Teacher", "Teacher")) return;
        } else if (choice == "2") {
            c.category = "Admissions";
            if (!pickTarget(c, ADMISSIONS_FILE, "Admission", "Admissions")) return;
        } else if (choice == "3") {
            c.category = "Examinations";
            if (!pickTarget(c, EXAMINATIONS_FILE, "Examination", "Exam Staff")) return;
        } else if (choice == "4") {
            c.category = "Drivers";
            if (!pickTarget(c, DRIVERS_FILE, "Driver", "Driver")) return;
        } else if (choice == "5") {
            c.category = "Others";
            c.targetId = 0;
            c.targetName = "N/A";
            c.targetType = "Other";
            c.targetExtra = "N/A";
        } else {
            error("Invalid choice.");
            return;
        }

        printHeader("Complaint Details");
        c.title = ask("Complaint title:");
        c.description = ask("Description:");
        c.status = "Pending";
        c.response = "";
        c.rating = 0;
        c.createdAt = now();

        c.save();
        success("Complaint submitted! Your complaint ID is: #" + to_string(c.id));
        waitEnter();
    }

    void viewMyComplaints() {
        printBanner("My Complaints");

        vector<Complaint> all = Complaint::loadAll();
        vector<Complaint> mine;
        for (const auto& item : all) {
            if (item.studentId == id) {
                mine.push_back(item);
            }
        }

        if (mine.empty()) {
            info("You have not submitted any complaints yet.");
            waitEnter();
            return;
        }

        for (const auto& item : mine) {
            item.display();
            cout << "\n";
        }

        cout << "\n";
        string rate = ask("Rate a resolved complaint? (y/n):");
        if (rate == "y" || rate == "Y") {
            int compId = 0;
            string cid = ask("Enter complaint ID to rate:");
            if (!parseIntText(cid, compId)) {
                error("Please enter a valid complaint ID.");
                waitEnter();
                return;
            }

            bool complaintFound = false;
            bool ratingSaved = false;

            for (auto& item : all) {
                if (item.id != compId || item.studentId != id) {
                    continue;
                }

                complaintFound = true;

                if (item.status != "Resolved") {
                    error("Only resolved complaints can be rated.");
                    break;
                }

                if (item.rating > 0) {
                    error("You already rated this complaint.");
                    break;
                }

                int stars = 0;
                string r = ask("Give a rating (1-5):");
                if (!parseIntText(r, stars) || stars < 1 || stars > 5) {
                    error("Rating must be 1 to 5.");
                    break;
                }

                item.rating = stars;
                ratingSaved = true;
                break;
            }

            if (!complaintFound) {
                error("Complaint ID not found.");
            } else if (ratingSaved) {
                Complaint::saveAll(all);
                success("Thank you! Your rating has been saved.");
            }
        }

        waitEnter();
    }
};

class Admin : public Person {
public:
    string toJson() const {
        return toJsonCommon();
    }

    static Admin fromJson(const string& object) {
        Admin a;
        a.id = readJsonInt(object, "id");
        a.name = readJsonValue(object, "name");
        a.email = readJsonValue(object, "email");
        a.password = readJsonValue(object, "password");
        return a;
    }

    static vector<Admin> loadAll() {
        return loadJsonList<Admin>(ADMIN_FILE, Admin::fromJson);
    }

    void save() const {
        vector<Admin> list = loadAll();
        list.push_back(*this);
        saveJsonList(ADMIN_FILE, list);
    }

    static bool login(const string& email, const string& password, Admin& out) {
        return Person::login(email, password, loadAll(), out);
    }

    static void ensureDefault() {
        if (loadAll().empty()) {
            Admin a;
            a.id = 1;
            a.name = "Administrator";
            a.email = "admin@domain.com";
            a.password = "admin123";
            a.save();
        }
    }

    void viewAll() const {
        printBanner("All Complaints");
        vector<Complaint> all = Complaint::loadAll();

        if (all.empty()) {
            info("No complaints have been submitted yet.");
            waitEnter();
            return;
        }

        int pending = 0, inprog = 0, resolved = 0;
        for (const auto& item : all) {
            if (item.status == "Pending") {
                pending++;
            } else if (item.status == "In Progress") {
                inprog++;
            } else {
                resolved++;
            }
        }

        cout << "  Total: " << BWHITE << all.size() << RESET
             << "   " << BRED << "Pending: " << pending << RESET
             << "   " << BYELLOW << "In Progress: " << inprog << RESET
             << "   " << BGREEN << "Resolved: " << resolved << RESET << "\n\n";

        for (const auto& item : all) {
            item.display();
            cout << "\n";
        }

        waitEnter();
    }

    void filterComplaints() const {
        printBanner("Filter Complaints");
        printHeader("Filter By");
        menuOption(1, "Status", BYELLOW);
        menuOption(2, "Category", BMAGENTA);
        menuOption(3, "Show All", BWHITE);
        cout << "\n";

        string opt = ask("Your choice:");
        vector<Complaint> all = Complaint::loadAll();
        bool any = false;

        if (opt == "1") {
            printHeader("Choose Status");
            menuOption(1, "Pending", BRED);
            menuOption(2, "In Progress", BYELLOW);
            menuOption(3, "Resolved", BGREEN);
            cout << "\n";

            string s = ask("Your choice:");
            string status;
            if (s == "1") {
                status = "Pending";
            } else if (s == "2") {
                status = "In Progress";
            } else {
                status = "Resolved";
            }

            for (const auto& item : all) {
                if (item.status == status) {
                    item.display();
                    cout << "\n";
                    any = true;
                }
            }
        } else if (opt == "2") {
            printHeader("Choose Category");
            menuOption(1, "Teachers", BBLUE);
            menuOption(2, "Admissions", BMAGENTA);
            menuOption(3, "Examinations", BYELLOW);
            menuOption(4, "Drivers", BGREEN);
            menuOption(5, "Others", BWHITE);
            cout << "\n";

            string s = ask("Your choice:");
            string cat;
            if (s == "1") {
                cat = "Teachers";
            } else if (s == "2") {
                cat = "Admissions";
            } else if (s == "3") {
                cat = "Examinations";
            } else if (s == "4") {
                cat = "Drivers";
            } else {
                cat = "Others";
            }

            for (const auto& item : all) {
                if (item.category == cat) {
                    item.display();
                    cout << "\n";
                    any = true;
                }
            }
        } else {
            for (const auto& item : all) {
                item.display();
                cout << "\n";
                any = true;
            }
        }

        if (!any) info("No complaints match your filter.");
        waitEnter();
    }

    void updateComplaint() const {
        printBanner("Update Complaint");
        vector<Complaint> all = Complaint::loadAll();

        if (all.empty()) {
            error("No complaints available.");
            waitEnter();
            return;
        }

        int cid = 0;
        string rawId = ask("Enter complaint ID to update:");
        if (!parseIntText(rawId, cid)) {
            error("Please enter a valid complaint ID.");
            waitEnter();
            return;
        }

        bool found = false;

        for (auto& item : all) {
            if (item.id != cid) continue;

            found = true;
            cout << "\n";
            item.display();
            cout << "\n";

            printHeader("Update Status");
            menuOption(1, "Pending", BRED);
            menuOption(2, "In Progress", BYELLOW);
            menuOption(3, "Resolved", BGREEN);
            cout << "\n";

            string s = ask("New status:");
            if (s == "1") {
                item.status = "Pending";
            } else if (s == "2") {
                item.status = "In Progress";
            } else if (s == "3") {
                item.status = "Resolved";
            } else {
                error("Invalid status choice.");
                waitEnter();
                return;
            }

            item.response = ask("Response message:");
            break;
        }

        if (!found) {
            error("Complaint #" + rawId + " not found.");
            waitEnter();
            return;
        }

        Complaint::saveAll(all);
        success("Complaint updated successfully!");
        waitEnter();
    }
};

void initData() {
    if (Student::loadAll().empty()) {
        Student s1; s1.id = 1; s1.name = "Alice Johnson";   s1.email = "alice@student.com";   s1.password = "pass123"; s1.save();
        Student s2; s2.id = 2; s2.name = "Bob Smith";       s2.email = "bob@student.com";     s2.password = "pass123"; s2.save();
        Student s3; s3.id = 3; s3.name = "Charlie Brown";   s3.email = "charlie@student.com"; s3.password = "pass123"; s3.save();
    }

    if (loadTargets(TEACHERS_FILE, "Teacher").empty()) {
        saveTargets(TEACHERS_FILE, {
            {1, "Dr. Smith", "Teacher", "Mathematics"},
            {2, "Prof. Johnson", "Teacher", "Physics"},
            {3, "Ms. Davis", "Teacher", "Chemistry"}
        });
    }

    if (loadTargets(ADMISSIONS_FILE, "Admission").empty()) {
        saveTargets(ADMISSIONS_FILE, {
            {1, "Mr. Wilson", "Admission", "Officer"},
            {2, "Mrs. Taylor", "Admission", "Clerk"}
        });
    }

    if (loadTargets(EXAMINATIONS_FILE, "Examination").empty()) {
        saveTargets(EXAMINATIONS_FILE, {
            {1, "Dr. Lee", "Examination", "Controller"}
        });
    }

    if (loadTargets(DRIVERS_FILE, "Driver").empty()) {
        saveTargets(DRIVERS_FILE, {
            {1, "John Driver", "Driver", "Bus Driver"},
            {2, "Mike Driver", "Driver", "Van Driver"}
        });
    }
}



void studentMenu(Student& student) {
    while (true) {
        printBanner("Student Portal  |  " + student.name);

        cout << "  " << DIM << "Logged in as: " << student.email << RESET << "\n\n";

        menuOption(1, "Submit a new complaint", BGREEN);
        menuOption(2, "View my complaints", BBLUE);
        menuOption(3, "Logout", BRED);
        cout << "\n";

        string choice = ask("Your choice:");

        if (choice == "1") {
            student.submitComplaint();
        } else if (choice == "2") {
            student.viewMyComplaints();
        } else if (choice == "3") {
            success("Logged out. Goodbye!");
            break;
        } else {
            error("Invalid option. Please enter 1, 2 or 3.");
        }
    }
}

void adminMenu(Admin& admin) {
    while (true) {
        printBanner("Admin Panel  |  " + admin.name);

        cout << "  " << DIM << "Logged in as: " << admin.email << RESET << "\n\n";

        menuOption(1, "View all complaints", BBLUE);
        menuOption(2, "Filter complaints", BYELLOW);
        menuOption(3, "Update complaint status/response", BGREEN);
        menuOption(4, "Logout", BRED);
        cout << "\n";

        string choice = ask("Your choice:");

        if (choice == "1") {
            admin.viewAll();
        } else if (choice == "2") {
            admin.filterComplaints();
        } else if (choice == "3") {
            admin.updateComplaint();
        } else if (choice == "4") {
            success("Logged out. Goodbye!");
            break;
        } else {
            error("Invalid option. Please enter 1, 2, 3 or 4.");
        }
    }
}

int main() {
    Admin::ensureDefault();
    initData();

    while (true) {
        printBanner("Student Complaint Management System");

        cout << "  " << DIM << "Welcome! Please choose how to login.\n" << RESET << "\n";

        menuOption(1, "Student Login", BBLUE);
        menuOption(2, "Admin Login", BMAGENTA);
        menuOption(3, "Exit", BRED);
        cout << "\n";

        string choice = ask("Your choice:");

        if (choice == "1") {
            printBanner("Student Login");
            string email, password;
            cout << "Email: ";
            cin >> email;
            cout << "Password: ";
            cin >> password;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Student student;
            if (Student::login(email, password, student)) {
                success("Welcome back, " + student.name + "!");
                waitEnter();
                studentMenu(student);
            } else {
                error("Incorrect email or password. Please try again.");
                waitEnter();
            }
        } else if (choice == "2") {
            printBanner("Admin Login");
            string email, password;
            cout << "Admin Email: ";
            cin >> email;
            cout << "Admin Password: ";
            cin >> password;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Admin admin;
            if (Admin::login(email, password, admin)) {
                success("Welcome, " + admin.name + "!");
                waitEnter();
                adminMenu(admin);
            } else {
                error("Incorrect admin credentials. Please try again.");
                waitEnter();
            }
        } else if (choice == "3") {
            printBanner("Goodbye!");
            cout << "  " << BCYAN << "Thank you for using the Complaint Management System." << RESET << "\n\n";
            break;
        } else {
            error("Invalid choice. Enter 1, 2 or 3.");
            waitEnter();
        }
    }

    return 0;
}
