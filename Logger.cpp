#include <iostream>
#include <vector>
#include <string>
#include <any>
#include <cxxabi.h>

#include "Logger.h"

using namespace std;

void print_array(const vector<int>& array) {
    for (int i = 0; i < array.size(); i++) {
        cout << array[i];
        if (array.size() - i > 1) {
            cout << ' ';
        }
    }
}

const volatile bool ENABLE_DEBUG_LOGS = 0;
const volatile bool ENABLE_INFO_LOGS = 0;

void Logger::debug(const std::any& value) {
    if (!ENABLE_DEBUG_LOGS) {
        return;
    }
    Logger::print(value);
}
void Logger::debug(const std::any& value1, const std::any& value2) {
    if (!ENABLE_DEBUG_LOGS) {
        return;
    }
    Logger::print(value1);
    Logger::print(value2);
}
void Logger::debug(const std::any& value1, const std::any& value2, const std::any& value3) {
    if (!ENABLE_DEBUG_LOGS) {
        return;
    }
    Logger::print(value1);
    Logger::print(value2);
    Logger::print(value3);
}

void Logger::info(const std::any& value) {
    if (!ENABLE_INFO_LOGS) {
        return;
    }
    Logger::print(value);
}
void Logger::info(const std::any& value1, const std::any& value2) {
    if (!ENABLE_INFO_LOGS) {
        return;
    }
    Logger::print(value1);
    Logger::print(value2);
}
void Logger::info(const std::any& value1, const std::any& value2, const std::any& value3) {
    if (!ENABLE_INFO_LOGS) {
        return;
    }
    Logger::print(value1);
    Logger::print(value2);
    Logger::print(value3);
}

void Logger::print(std::any value) {
    string type_name = abi::__cxa_demangle(value.type().name(), nullptr, nullptr, nullptr);

    if (string("std::vector<int, std::allocator<int> >") == type_name) {
        print_array(std::any_cast<vector<int>>(value));
    } else if (string("std::__1::vector<int, std::__1::allocator<int> >") == type_name) {
        print_array(std::any_cast<vector<int>>(value));
    } else if (string("int") == type_name) {
        cout << std::any_cast<int>(value);
    } else if (string("bool") == type_name) {
        cout << std::any_cast<bool>(value);
    } else if (string("unsigned long") == type_name) {
        cout << std::any_cast<unsigned long>(value);
    } else if (string("unsigned long long") == type_name) {
        cout << std::any_cast<unsigned long long>(value);
    } else if (string("char const*") == type_name) {
        cout << std::any_cast<char const*>(value);
    } else {
        throw std::invalid_argument("Unsupported type: '" + type_name + "'. Please, add its support in Logger class.");
    }
}
