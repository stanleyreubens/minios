#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

void parseAndCalculate(const std::string& input);

void calculatorFunction();

void parseAndCalculate(const string& input) {
    stringstream ss(input);
    char operation;
    double num1, num2, result;

    // Parse the input in the format: number operation number
    if (ss >> num1 >> operation >> num2) {
        switch (operation) {
        case '+':
            result = num1 + num2;
            cout << "Result: " << result << endl;
            break;
        case '-':
            result = num1 - num2;
            cout << "Result: " << result << endl;
            break;
        case '*':
            result = num1 * num2;
            cout << "Result: " << result << endl;
            break;
        case '/':
            if (num2 != 0) {
                result = num1 / num2;
                cout << "Result: " << result << endl;
            } else {
                cout << "Error: Division by zero is not allowed." << endl;
            }
            break;
        case '%': // modulus
            if (num2 != 0) {
                int num1_int = (int)num1;
                int num2_int = (int)num2;
                result = num1_int % num2_int;
                if (result < 0) {
                    result += num2_int;
                }
                cout << "Remainder: " << result << endl;
            } else {
                cout << "Error: Division by zero is not allowed." << endl;
            }
            break;
        case '^': // exponential
            result = pow(num1, num2);
            cout << "Result: " << result << endl;
            break;
        default:
            cout << "Error: Invalid operation." << endl;
        }
    } else {
        cout << "Error: Invalid input format. Please use 'number operation number' format." << endl;
    }
}

void calculatorFunction() {
    string input;

    cout << "This is a Simple Calculator on MiniOS." << endl;
    cout << "Supports Addition(+), Subtraction(-), Multiplication(*), Division(/), Modulus(%), and Exponential(^)." << endl;
    cout << "Type 'exit' or 'quit' to close the calculator." << endl;
    cout << "\n";
    cout << "Enter calculation (e.g., '5 + 3', '8.5 * 2.1', or '3 ^ 5', ...):" << endl;

    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input == "exit" || input == "quit") {
            cout << "Exiting calculator." << endl;
            break;
        }

        parseAndCalculate(input);
    }
}
