#include <iostream>
#include <random>
#include <cmath>
#include <string>
#include <cfloat>
#include <iomanip>
#include <math.h>
#include <numbers>

using namespace std;

void drill1(){
    cout << " Hello World";
}
void drill2(){
    static random_device rd;
    static mt19937_64 gen(rd());
    static uniform_int_distribution<int> dis(1, 100);

}
void drill3(){
    int size;
        
    cout<< "Enter a Number: "; 
    cin >> size;

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++) cout << " * ";
        cout << endl;
    }

}
void drill4(){
    int num1, num2, sum;

    cout << "Enter a number: " << endl;
    cin >> num1;
    cout << "Enter another number: ";
    cin >> num2;

    sum =  num1 * num2;
    cout <<"The sum of two int is: " << sum << endl;

}
void drill5(){
    cout << fixed << setprecision(10);
    cout << "float:  " << numbers::pi_v<float> << endl;
}

void drill6(){
        int size;

        cout <<"Enter a numer; ";
        cin >> size;

        for(int i = 0; i <= size; i++){
            for(int j =0; j < size + 2; j++) cout << " * ";
            cout << endl;
        }
}
void drill7(){
     double base;
     int exp;
     double result = 1;
        cout << "Enter base: ";
     cin >> base;
        cout << "Enter exponent: ";
        cin >> exp;

     for (int i = 0; i < exp; i++) {
        double prev = result;
        result *= base;
        cout << prev << " x " << base << " = " << result << endl;
    }

    cout << base << "^" << exp << " = " << result << endl;
}
void drill8(){
     string name;

     cout<< "Enter your name: " << endl;
     cin >> name;
     
     cout << "Hello There! " << name << endl;

}
void drill9(){
           float num1, num2, num3, sum;

        cout << "Enter Subject 1: ";
        cin >> num1;
        cout << "Enter Subject 2: ";
        cin >> num2;
        cout  << "Enter Subject 3: ";
        cin >> num3;

        sum = (num1 + num2 + num3)/3; 
        cout << "Your grade is " << sum << endl;
}
void drill10(){
     rand() % 2 == 0;
}
int main(){
    int choice;

        do{
            cout<<"[1] printWelcome";
            cout<<"[2] generateRandomNumber";
            cout<<"[3] printSqura";
            cout<<"[4] calculateSum";
            cout<<"[5] getPi";
            cout<<"[6] printRectangle";
            cout<<"[7] calculatePower";
            cout<<"[8] printGreeting";
            cout<<"[9] calculateAverage";
            cout<<"[10] getRandomBoolean";
            cin >> choice;

                switch (choice){
                case 1: drill1(); break;
                case 2: drill2(); break;
                case 3: drill3(); break;
                case 4: drill4(); break;
                case 5: drill5(); break;
                case 6: drill6(); break;
                case 7: drill7(); break;
                case 8: drill8(); break;
                case 9: drill9(); break;
                case 10: drill10(); break;
                default: cout <<"Invalid choice. Try again.";
                }
        } while (choice != 0);
        return 0;
}
