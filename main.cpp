#include <iostream>
#include <queue>
#include <vector>
#include <cctype>
#include <ctime>
#include <string>
#include <map>
#include <any>
#include <cxxabi.h>

using namespace std;

const size_t MAX_QUEUE_SIZE = 6;

void print_array(const vector<int>& array) {
    for (int i = 0; i < array.size(); i++) {
        cout << array[i];
        if (array.size() - i > 1) {
            cout << ' ';
        }
    }
}

const volatile bool ENABLE_DEBUG_LOGS = false;
const volatile bool ENABLE_INFO_LOGS = true;
class Logger {
public:
    static void debug(const std::any& value) {
        if (!ENABLE_DEBUG_LOGS) {
            return;
        }
        Logger::print(value);
    }
    static void debug(const std::any& value1, const std::any& value2) {
        if (!ENABLE_DEBUG_LOGS) {
            return;
        }
        Logger::print(value1);
        Logger::print(value2);
    }
    static void debug(const std::any& value1, const std::any& value2, const std::any& value3) {
        if (!ENABLE_DEBUG_LOGS) {
            return;
        }
        Logger::print(value1);
        Logger::print(value2);
        Logger::print(value3);
    }

    static void info(const std::any& value) {
        if (!ENABLE_INFO_LOGS) {
            return;
        }
        Logger::print(value);
    }
    static void info(const std::any& value1, const std::any& value2) {
        if (!ENABLE_INFO_LOGS) {
            return;
        }
        Logger::print(value1);
        Logger::print(value2);
    }
    static void info(const std::any& value1, const std::any& value2, const std::any& value3) {
        if (!ENABLE_INFO_LOGS) {
            return;
        }
        Logger::print(value1);
        Logger::print(value2);
        Logger::print(value3);
    }

private:
    static void print(std::any value) {
        string type_name = abi::__cxa_demangle(value.type().name(), nullptr, nullptr, nullptr);

        if (string("std::vector<int, std::allocator<int> >") == type_name) {
            print_array(std::any_cast<vector<int>>(value));
        } else if (string("int") == type_name) {
            cout << std::any_cast<int>(value);
        } else if (string("bool") == type_name) {
            cout << std::any_cast<bool>(value);
        } else if (string("unsigned long long") == type_name) {
            cout << std::any_cast<unsigned long long>(value);
        } else if (string("char const*") == type_name) {
            cout << std::any_cast<char const*>(value);
        } else {
            throw std::invalid_argument("Unsupported type: '" + type_name + "'. Please, add its support in Logger class.");
        }
    }
};

int get_first_and_erase(vector<int>& array) {
    int tail = array[0];
    array.erase(array.begin());

    return tail;
}

pair<bool, vector<int>> perform_tournament(const vector<int>& original_array) {
    vector<int> array(original_array);
    size_t array_size = array.size();
    priority_queue <int, vector<int>, greater<>> q;
    vector<int> winners;
    vector<int> losers;

    for (int i = 0; i < min(MAX_QUEUE_SIZE, array_size); i++) {
        int tail = get_first_and_erase(array);
        q.push(tail);
        Logger::debug("\n\nq size after pushing ");
        Logger::debug(tail);
        Logger::debug(": ");
        Logger::debug(q.size());

    }
    winners.push_back(q.top());
    Logger::debug("\n\nq size after popping ");
    Logger::debug(q.top());
    q.pop();
    Logger::debug(": ");
    Logger::debug(q.size());

    int cntr = 0;
    while (!array.empty()) {
        if (array.front() > winners.back()) {
            int tail = get_first_and_erase(array);
            q.push(tail);
            Logger::debug("\n\nq size after pushing ");
            Logger::debug(tail);
            Logger::debug(": ");
            Logger::debug(q.size());
        } else {
            losers.push_back(get_first_and_erase(array));
        }

        if (!q.empty()) {
            winners.push_back(q.top());
            Logger::debug("\n\nq size after popping ");
            Logger::debug(q.top());
            q.pop();
            Logger::debug(": ");
            Logger::debug(q.size());
        }

        Logger::debug(cntr++);
    }
    while (!q.empty()) {
        Logger::debug(q.empty());
        Logger::debug("\n");
        Logger::debug(q.size());
        Logger::debug("\n");
        Logger::debug("before all\n");
        winners.push_back(q.top());
        Logger::debug("after pb all\n");
        q.pop();
    }

    Logger::debug("\n\nwinners:\n");
    Logger::debug(winners);
    Logger::debug("\n\nlosers:\n");
    Logger::debug(losers);

    if (losers.empty()) {
        return make_pair(true, winners);
    } else {
        losers.insert(losers.end(), winners.begin(), winners.end());
        return make_pair(false, losers);
    }
}

vector<int> tournament_sort(const vector<int>& original_array) {
    vector<int> current_array(original_array);
    bool sorted = false;

    while(!sorted) {
        auto result = perform_tournament(current_array);
        sorted = result.first;
        current_array = result.second;
    }

    return current_array;
}

auto get_random_int = [](int min, int max) -> int {
    return rand() % (max - min + 1) + min;
};

int main() {
    srand(time(nullptr));

    const size_t array_size = get_random_int(15, 20);
    Logger::info("array size: ", array_size, "\n");
    vector<int> array;
    for (size_t i = 0; i < array_size; i++) {
        array.push_back(get_random_int(1, 10000000));
    }
    Logger::info("initial array:\n", array, "\n");

    map<int, int> item_counts;
    vector<int> distinct_array;
    for (auto item : array) {
        if (item_counts.find(item) == item_counts.end()) {
            item_counts[item] = 1;
            distinct_array.push_back(item);
        } else {
            item_counts[item]++;
        }
    }

    Logger::info("distinct array:\n", distinct_array, "\n");

    auto sorted_array = tournament_sort(distinct_array);
    Logger::info("\nsorted array:\n", sorted_array);

    return 0;
}
