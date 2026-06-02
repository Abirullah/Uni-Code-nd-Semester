#include <iostream>
#include <string>

using namespace std;

class student {
protected:
    int roll_no;
    string name;
    float GPA;
    string Program;
public:
    float CalculateGPA(int s1, int s2, int s3 , int s4, int s5) {
        float total = s1 + s2 + s3 + s4 + s5;
        GPA = total * 4.0 / 500.0;
        return GPA;
    }
    void set_data(int r, string n, float g , string p) {
        roll_no = r;
        name = n;
        GPA = g;
        Program = p;
    }
    void display() const {
        cout << "Roll No: " << roll_no << endl;
        cout << "Name: " << name << endl;
        cout << "GPA: " << GPA << endl;
    }

};

class DSAStudent : public student {
public:
    using student::CalculateGPA; 

    float CalculateGPA(int s1, int s2, int s3 , int s4, int s5, int L1, int L2, int L3) {
        float total = s1 + s2 + s3 + s4 + s5 + L1 + L2 + L3;
        GPA = total * 4.0 / 800.0;
        return GPA;
    }
};

int main() {
    student s1;
    s1.set_data(1, "Alice", 0.0, "Computer Science");
    s1.CalculateGPA(80, 90, 85, 70, 95);
    s1.display();

    DSAStudent s2;
    s2.set_data(2, "Bob", 0.0, "Data Structures and Algorithms");
    s2.CalculateGPA(80, 90, 85, 70, 95, 100, 90, 80);
    s2.display();

    return 0;
}
