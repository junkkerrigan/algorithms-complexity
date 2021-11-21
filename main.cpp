#include <iostream>
#include <queue>
#include <vector>
#include <cctype>
#include <ctime>
#include <string>
#include <map>

using namespace std;

const size_t MAX_QUEUE_SIZE = 6;

void printArray(const vector<int>& array) {
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
    static void debug(const vector<int>& array) {
        if (ENABLE_DEBUG_LOGS) {
            printArray(array);
        }
    }
    static void debug(const string& value) {
        if (ENABLE_DEBUG_LOGS) {
            cout << value;
        }
    }
    static void debug(unsigned long long value) {
        if (ENABLE_DEBUG_LOGS) {
            cout << value;
        }
    }

    static void log(const vector<int>& array) {
        if (ENABLE_INFO_LOGS) {
            printArray(array);
        }
    }
    static void log(const string& value) {
        if (ENABLE_INFO_LOGS) {
            cout << value;
        }
    }
    static void log(unsigned long long value) {
        if (ENABLE_INFO_LOGS) {
            cout << value;
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
        logger.debug("\n\nq size after pushing ");
        logger.debug(tail);
        logger.debug(": ");
        log(q.size());

    }
    winners.push_back(q.top());
    log("\n\nq size after popping ");
    log(q.top());
    q.pop();
    log(": ");
    log(q.size());

    int cntr = 0;
    while (!array.empty()) {
        if (array.front() > winners.back()) {
            int tail = get_first_and_erase(array);
            q.push(tail);
            log("\n\nq size after pushing ");
            log(tail);
            log(": ");
            log(q.size());
        } else {
            losers.push_back(get_first_and_erase(array));
        }

        if (!q.empty()) {
            winners.push_back(q.top());
            log("\n\nq size after popping ");
            log(q.top());
            q.pop();
            log(": ");
            log(q.size());
        }

        log(cntr++);
    }
    while (!q.empty()) {
        log(q.empty());
        log("\n");
        log(q.size());
        log("\n");
        log("before all\n");
        winners.push_back(q.top());
        log("after pb all\n");
        q.pop();
    }

    log("\n\nwinners:\n");
    log(winners);
    log("\n\nlosers:\n");
    log(losers);

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

    const size_t array_size = get_random_int(15000, 20000);
    cout << "array size: " << array_size << "\n";
    vector<int> array;
    for (size_t i = 0; i < array_size; i++) {
        array.push_back(get_random_int(1, 10000000));
    }
    cout << "initial array:\n";
    printArray(array);
    cout << "\n";

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

    cout << "distinct array:\n";
    printArray(distinct_array);
    cout << "\n\n";

    auto sorted_array = tournament_sort(distinct_array);
    cout << "sorted array:\n";
    printArray(sorted_array);

    return 0;
}
