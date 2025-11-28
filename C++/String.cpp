#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Part 1 — Basic Concepts (Write your answers)
/*
1. What is the difference between string and char[]?
Your answer: string is a C++ class with built-in functions, dynamic sizing, and automatic memory management, 
while char[] is a C-style fixed-size array that requires manual memory management and null termination.

2. Which function gets the length of a string?
a. size()
b. length()
c. Both
Your answer: c

3. What function do you use to get user input with spaces?
Your answer: getline()

4. What does this do?
string s = "Hello";
s += " World";
Your answer: Concatenates " World" to "Hello", making s = "Hello World"
*/

// Part 2 — Short Exercises
void part2() {
    // 5. Write a line of code that declares a string named city:
    string city;
    
    // 6. Make a program snippet that prints the first character of a string word:
    string word = "Hello";
    cout << "First character: " << word[0] << endl;
    
    // 7. Given: string name = "Jonathan";
    string name = "Jonathan";
    cout << "substr(0, 3): " << name.substr(0, 3) << endl; // Output: Jon
}

// Part 3 — Coding Problems

// 8. Full Name Formatter
void fullNameFormatter() {
    string firstName, lastName;
    
    cout << "Enter first name: ";
    cin >> firstName;
    cout << "Enter last name: ";
    cin >> lastName;
    
    string fullName = firstName + " " + lastName;
    cout << "Your full name is: " << fullName << endl;
}

// 9. Word Reverser
void wordReverser() {
    string word;
    cout << "Enter a word: ";
    cin >> word;
    
    string reversed = "";
    for (int i = word.length() - 1; i >= 0; i--) {
        reversed += word[i];
    }
    
    cout << "Reversed word: " << reversed << endl;
}

// 10. Capitalize the First Letter
void capitalizeFirstLetter() {
    string sentence;
    cout << "Enter a sentence: ";
    cin.ignore(); // Clear input buffer
    getline(cin, sentence);
    
    bool capitalizeNext = true;
    for (int i = 0; i < sentence.length(); i++) {
        if (capitalizeNext && isalpha(sentence[i])) {
            sentence[i] = toupper(sentence[i]);
            capitalizeNext = false;
        }
        else if (isspace(sentence[i])) {
            capitalizeNext = true;
        }
    }
    
    cout << "Capitalized: " << sentence << endl;
}

// Part 4 — Applied String Functions

// 11. Password Strength Checker
void passwordChecker() {
    string password;
    cout << "Enter password: ";
    cin >> password;
    
    bool hasDigit = false;
    bool hasUpper = false;
    bool hasNoSpaces = true;
    bool hasMinLength = password.length() >= 8;
    
    for (char c : password) {
        if (isdigit(c)) hasDigit = true;
        if (isupper(c)) hasUpper = true;
        if (isspace(c)) hasNoSpaces = false;
    }
    
    if (hasMinLength && hasDigit && hasUpper && hasNoSpaces) {
        cout << "Strong Password!" << endl;
    } else {
        cout << "Weak Password!" << endl;
    }
}

// 12. Email Validator (Simple Version)
void emailValidator() {
    string email;
    cout << "Enter email: ";
    cin >> email;
    
    bool hasAt = email.find('@') != string::npos;
    bool hasDotCom = email.find(".com") != string::npos;
    
    if (hasAt && hasDotCom) {
        cout << "Valid email!" << endl;
    } else {
        cout << "Invalid email." << endl;
    }
}

// Part 5 — Challenge Problem
void wordFrequencyCounter() {
    string paragraph;
    cout << "Enter a paragraph: ";
    cin.ignore();
    getline(cin, paragraph);
    
    // Convert to lowercase
    for (int i = 0; i < paragraph.length(); i++) {
        paragraph[i] = tolower(paragraph[i]);
    }
    
    string target = "the";
    int count = 0;
    size_t pos = paragraph.find(target);
    
    while (pos != string::npos) {
        count++;
        pos = paragraph.find(target, pos + target.length());
    }
    
    cout << "'the' appears " << count << " times." << endl;
}

// Part 6 — Bonus Questions
/*
14. What does this return?
string s = "Computer";
cout << s.find("put");
Your answer: 3 (position where "put" starts)

15. Which is TRUE?
a. Strings automatically resize ✓
b. Strings require manual null termination
c. Strings cannot contain spaces
Your answer: a
*/

int main() {
    cout << "=== C++ STRING WORKSHEET ===" << endl;
    
    // Uncomment the functions you want to test:
    
    // Part 2
    // part2();
    
    // Part 3
    // fullNameFormatter();
    // wordReverser();
    // capitalizeFirstLetter();
    
    // Part 4
    // passwordChecker();
    // emailValidator();
    
    // Part 5
    // wordFrequencyCounter();
    
    return 0;
}
