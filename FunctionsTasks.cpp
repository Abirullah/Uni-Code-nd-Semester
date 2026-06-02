#include <iostream>

using namespace std;

void opertationUsed(string operation) {
    cout << "The operation used is: " << operation << endl;
}

int Add(int a, int b) {
    opertationUsed("Addition");
    return a + b;
}

int Subtract(int a, int b) {
    opertationUsed("Subtraction");
    return a - b;
}

int Multiply(int a, int b) {
    opertationUsed("Multiplication");
    return a * b;
}

int Divide(int a, int b) {
    opertationUsed("Division");
    return a / b;
}

int main() {
    int num1, num2;
    cout << "Enter two numbers: ";
    cin >> num1 >> num2;

    cout << "Addition: " << Add(num1, num2) << endl;
    cout << "Subtraction: " << Subtract(num1, num2) << endl;
    cout << "Multiplication: " << Multiply(num1, num2) << endl;
    cout << "Division: " << Divide(num1, num2) << endl;

    return 0;
}