#include <iostream>
#include <string>

using namespace std;

class Learner {
protected:
    int roll_no;
    string name;
    string program;
    float gpa;

    float to_gpa(float percent) {
        if (percent >= 85.0) return 4.0;
        if (percent >= 80.0) return 3.7;
        if (percent >= 75.0) return 3.3;
        if (percent >= 70.0) return 3.0;
        if (percent >= 65.0) return 2.7;
        if (percent >= 60.0) return 2.3;
        if (percent >= 55.0) return 2.0;
        return 1.0;
    }

public:
    void set_data(int r, const string &n, const string &p) {
        roll_no = r;
        name = n;
        program = p;
        gpa = 0.0;
    }

    float CalculateGPA(int s1, int s2, int s3, int s4) {
        float avg = (s1 + s2 + s3 + s4) / 4.0;
        gpa = to_gpa(avg);
        return gpa;
    }

    void display() const {
        cout << "Roll No: " << roll_no << endl;
        cout << "Name: " << name << endl;
        cout << "Program: " << program << endl;
        cout << "GPA: " << gpa << endl;
    }
};

class CSProjectStudent : public Learner {
public:
    using Learner::CalculateGPA;

    float CalculateGPA(int s1, int s2, int s3, int s4, int lab1, int lab2, int project) {
        float theory_avg = (s1 + s2 + s3 + s4) / 4.0;
        float lab_avg = (lab1 + lab2) / 2.0;
        float overall = (theory_avg * 0.60) + (lab_avg * 0.25) + (project * 0.15);
        gpa = to_gpa(overall);
        return gpa;
    }
};

int main() {
    Learner l1;
    l1.set_data(11, "Hira", "Software Engineering");
    l1.CalculateGPA(78, 82, 75, 88);
    l1.display();

    CSProjectStudent l2;
    l2.set_data(12, "Umar", "Computer Science");
    l2.CalculateGPA(78, 82, 75, 88, 92, 85, 80);
    l2.display();

    return 0;
}
