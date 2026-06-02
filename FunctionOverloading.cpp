#include <iostream>

using namespace std;

int add(int a , int b, int c) {
    return a + b + c;
}

double add(double a, double b , double c) {
    return a + b + c;
}

void add(auto a, auto b, auto c) {
    cout  a + b + c << endl;
}

int main() {
    cout << "Sum of integers: " << add(1, 2, 3) << endl;
    cout << "Sum of doubles: " << add(1.5, 2.5, 3.5) << endl;
    cout << "Sum of mixed types: ";
    add(1, 2.5, 3); 

    return 0;
}
