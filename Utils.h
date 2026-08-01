#pragma once
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>

// Reads an int, re-prompting on invalid (non-numeric) input.
inline int getValidInt(const std::string &prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a whole number.\n" << prompt;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return value;
}

// Reads a float, re-prompting on invalid (non-numeric) input.
inline float getValidFloat(const std::string &prompt) {
    float value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a number.\n" << prompt;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return value;
}

// Reads a full line of text after a prompt.
inline std::string getLine(const std::string &prompt) {
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    return value;
}

// Strips commas from free-text input so it can't corrupt CSV rows. Fixes bug #4.
inline std::string sanitizeForCsv(std::string s) {
    std::replace(s.begin(), s.end(), ',', ' ');
    return s;
}