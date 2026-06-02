#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

// ==========================
// Base Class
// ==========================
class Person {
protected:
    int id;
    string name;
    int age;
    string gender;

public:
    Person() {}

    virtual void input() = 0;
    virtual void display() const = 0;

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }
};

// ==========================
// Patient Class
// ==========================
class Patient : public Person {
private:
    string disease;
    string phone;

public:
    void input() override {
        cout << "\nPatient ID: ";
        cin >> id;

        cin.ignore();

        cout << "Patient Name: ";
        getline(cin, name);

        cout << "Age: ";
        cin >> age;

        cin.ignore();

        cout << "Gender: ";
        getline(cin, gender);

        cout << "Disease: ";
        getline(cin, disease);

        cout << "Phone: ";
        getline(cin, phone);
    }

    void display() const override {
        cout << left
             << setw(8) << id
             << setw(20) << name
             << setw(8) << age
             << setw(10) << gender
             << setw(20) << disease
             << setw(15) << phone
             << endl;
    }

    string serialize() const {
        return to_string(id) + "," + name + "," +
               to_string(age) + "," + gender + "," +
               disease + "," + phone;
    }
};

// ==========================
// Doctor Class
// ==========================
class Doctor : public Person {
private:
    string specialization;
    double fee;

public:
    void input() override {
        cout << "\nDoctor ID: ";
        cin >> id;

        cin.ignore();

        cout << "Doctor Name: ";
        getline(cin, name);

        cout << "Age: ";
        cin >> age;

        cin.ignore();

        cout << "Gender: ";
        getline(cin, gender);

        cout << "Specialization: ";
        getline(cin, specialization);

        cout << "Consultation Fee: ";
        cin >> fee;
    }

    void display() const override {
        cout << left
             << setw(8) << id
             << setw(20) << name
             << setw(8) << age
             << setw(10) << gender
             << setw(20) << specialization
             << setw(10) << fee
             << endl;
    }

    double getFee() const {
        return fee;
    }

    string getSpecialization() const {
        return specialization;
    }
};

// ==========================
// Appointment Class
// ==========================
class Appointment {
private:
    int appointmentId;
    int patientId;
    int doctorId;
    string date;

public:
    void input() {
        cout << "\nAppointment ID: ";
        cin >> appointmentId;

        cout << "Patient ID: ";
        cin >> patientId;

        cout << "Doctor ID: ";
        cin >> doctorId;

        cin.ignore();

        cout << "Date (DD/MM/YYYY): ";
        getline(cin, date);
    }

    void display() const {
        cout << left
             << setw(15) << appointmentId
             << setw(15) << patientId
             << setw(15) << doctorId
             << setw(20) << date
             << endl;
    }
};

// ==========================
// Billing Class
// ==========================
class Bill {
private:
    int billId;
    int patientId;
    double amount;

public:
    void generate() {
        cout << "\nBill ID: ";
        cin >> billId;

        cout << "Patient ID: ";
        cin >> patientId;

        cout << "Amount: ";
        cin >> amount;
    }

    void display() const {
        cout << left
             << setw(10) << billId
             << setw(15) << patientId
             << setw(15) << amount
             << endl;
    }
};

// ==========================
// Hospital Management System
// ==========================
class HospitalManagementSystem {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;
    vector<Bill> bills;

public:

    // ======================
    // Patient CRUD
    // ======================
    void addPatient() {
        Patient p;
        p.input();
        patients.push_back(p);

        cout << "\nPatient Added Successfully!\n";
    }

    void viewPatients() {
        if (patients.empty()) {
            cout << "\nNo Patients Found.\n";
            return;
        }

        cout << "\n================ PATIENTS ================\n\n";

        cout << left
             << setw(8) << "ID"
             << setw(20) << "Name"
             << setw(8) << "Age"
             << setw(10) << "Gender"
             << setw(20) << "Disease"
             << setw(15) << "Phone"
             << endl;

        for (const auto &p : patients) {
            p.display();
        }
    }

    void searchPatient() {
        int id;
        cout << "\nEnter Patient ID: ";
        cin >> id;

        bool found = false;

        for (const auto &p : patients) {
            if (p.getId() == id) {
                cout << "\nPatient Found\n";
                p.display();
                found = true;
            }
        }

        if (!found)
            cout << "\nPatient Not Found.\n";
    }

    // ======================
    // Doctor CRUD
    // ======================
    void addDoctor() {
        Doctor d;
        d.input();
        doctors.push_back(d);

        cout << "\nDoctor Added Successfully!\n";
    }

    void viewDoctors() {
        if (doctors.empty()) {
            cout << "\nNo Doctors Available.\n";
            return;
        }

        cout << "\n================ DOCTORS ================\n\n";

        cout << left
             << setw(8) << "ID"
             << setw(20) << "Name"
             << setw(8) << "Age"
             << setw(10) << "Gender"
             << setw(20) << "Specialization"
             << setw(10) << "Fee"
             << endl;

        for (const auto &d : doctors) {
            d.display();
        }
    }

    // ======================
    // Appointment CRUD
    // ======================
    void addAppointment() {
        Appointment a;
        a.input();
        appointments.push_back(a);

        cout << "\nAppointment Booked Successfully!\n";
    }

    void viewAppointments() {
        if (appointments.empty()) {
            cout << "\nNo Appointments.\n";
            return;
        }

        cout << "\n============== APPOINTMENTS ==============\n\n";

        cout << left
             << setw(15) << "App ID"
             << setw(15) << "Patient ID"
             << setw(15) << "Doctor ID"
             << setw(20) << "Date"
             << endl;

        for (const auto &a : appointments) {
            a.display();
        }
    }

    // ======================
    // Billing
    // ======================
    void generateBill() {
        Bill b;
        b.generate();

        bills.push_back(b);

        cout << "\nBill Generated Successfully!\n";
    }

    void viewBills() {
        if (bills.empty()) {
            cout << "\nNo Bills Found.\n";
            return;
        }

        cout << "\n================ BILLS ================\n\n";

        cout << left
             << setw(10) << "Bill ID"
             << setw(15) << "Patient ID"
             << setw(15) << "Amount"
             << endl;

        for (const auto &b : bills) {
            b.display();
        }
    }

    // ======================
    // Reports
    // ======================
    void reports() {
        cout << "\n========== REPORT ==========\n";

        cout << "Total Patients      : "
             << patients.size() << endl;

        cout << "Total Doctors       : "
             << doctors.size() << endl;

        cout << "Total Appointments  : "
             << appointments.size() << endl;

        cout << "Total Bills         : "
             << bills.size() << endl;
    }

    // ======================
    // File Handling
    // ======================
    void savePatients() {
        ofstream file("patients.txt");

        for (const auto &p : patients) {
            file << p.serialize() << endl;
        }

        file.close();

        cout << "\nPatient Data Saved.\n";
    }
};

// ==========================
// Main Function
// ==========================
int main() {

    HospitalManagementSystem hms;

    int choice;

    do {

        cout << "\n=====================================\n";
        cout << "      HOSPITAL MANAGEMENT SYSTEM\n";
        cout << "=====================================\n";

        cout << "1. Add Patient\n";
        cout << "2. View Patients\n";
        cout << "3. Search Patient\n";
        cout << "4. Add Doctor\n";
        cout << "5. View Doctors\n";
        cout << "6. Book Appointment\n";
        cout << "7. View Appointments\n";
        cout << "8. Generate Bill\n";
        cout << "9. View Bills\n";
        cout << "10. Reports\n";
        cout << "11. Save Patients File\n";
        cout << "0. Exit\n";

        cout << "\nEnter Choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            hms.addPatient();
            break;

        case 2:
            hms.viewPatients();
            break;

        case 3:
            hms.searchPatient();
            break;

        case 4:
            hms.addDoctor();
            break;

        case 5:
            hms.viewDoctors();
            break;

        case 6:
            hms.addAppointment();
            break;

        case 7:
            hms.viewAppointments();
            break;

        case 8:
            hms.generateBill();
            break;

        case 9:
            hms.viewBills();
            break;

        case 10:
            hms.reports();
            break;

        case 11:
            hms.savePatients();
            break;

        case 0:
            cout << "\nThank You!\n";
            break;

        default:
            cout << "\nInvalid Choice.\n";
        }

    } while (choice != 0);

    return 0;
}