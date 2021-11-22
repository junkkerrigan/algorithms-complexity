#include <iostream>
#include <queue>
#include <vector>
#include <ctime>
#include <map>
#include <any>

#include "Logger.h"

using namespace std;

const size_t MAX_QUEUE_SIZE = 500;

int get_first_and_erase(vector<int>& array) {
    int tail = array[0];
    array.erase(array.begin());

    return tail;
}

pair<vector<int>, vector<int>> perform_tournament(const vector<int>& original_array) {
    vector<int> array(original_array);
    size_t array_size = array.size();
    priority_queue <int, vector<int>, greater<>> q;
    vector<int> winners;
    vector<int> losers;

    for (int i = 0; i < min(MAX_QUEUE_SIZE, array_size); i++) {
        int tail = get_first_and_erase(array);
        q.push(tail);
//        Logger::debug("\n\nq size after pushing ");
//        Logger::debug(tail);
//        Logger::debug(": ");
//        Logger::debug(q.size());

    }
    winners.push_back(q.top());
//    Logger::debug("\n\nq size after popping ");
//    Logger::debug(q.top());
    q.pop();
//    Logger::debug(": ");
//    Logger::debug(q.size());

//    int cntr = 0;
    while (!array.empty()) {
        if (array.front() > winners.back()) {
            int tail = get_first_and_erase(array);
            q.push(tail);
//            Logger::debug("\n\nq size after pushing ");
//            Logger::debug(tail);
//            Logger::debug(": ");
//            Logger::debug(q.size());
        } else {
            losers.push_back(get_first_and_erase(array));
        }

        if (!q.empty()) {
            winners.push_back(q.top());
//            Logger::debug("\n\nq size after popping ");
//            Logger::debug(q.top());
            q.pop();
//            Logger::debug(": ");
//            Logger::debug(q.size());
        }

//        Logger::debug(cntr++);
    }
    while (!q.empty()) {
//        Logger::debug(q.empty());
//        Logger::debug("\n");
//        Logger::debug(q.size());
//        Logger::debug("\n");
//        Logger::debug("before all\n");
        winners.push_back(q.top());
//        Logger::debug("after pb all\n");
        q.pop();
    }

//    Logger::debug("\n\nwinners:\n");
//    Logger::debug(winners);
//    Logger::debug("\n\nlosers:\n");
//    Logger::debug(losers);
    return make_pair(winners, losers);
}

vector<int> merge_sorted_arrays(const vector<vector<int>>& arrays) {
    size_t arrays_count = arrays.size();
    vector<int> min_item_indexes(arrays_count, 0);
    vector<int> result;

    priority_queue<pair<int, size_t>, vector<pair<int, size_t>>, greater<>> q;
    for (size_t i = 0; i < arrays_count; i++) {
//        cout << "[0] pushing to queue {" << arrays[i][0] << ", " << i << "}\n";
        q.push({ arrays[i][0], i });
    }

    bool items_in_arrays_left = true;
    while (!q.empty()) {
        auto least = q.top();
        q.pop();
        int least_value = least.first;
        size_t current_array_index = least.second;

//        cout << "\npushing to result array " << least_value << " from array #" << current_array_index << "\n";
        result.push_back(least_value);
        if (!items_in_arrays_left) {
            continue;
        }

        if (min_item_indexes[current_array_index] < arrays[current_array_index].size() - 1) {
            int& min_item_index = min_item_indexes[current_array_index];
//            cout << "[0] min_item_index " << min_item_index << "\n";
            min_item_index++;
//            cout << "[1] pushing to queue {" << arrays[current_array_index][min_item_index] << ", " << current_array_index << "}\n";
            q.push({
                arrays[current_array_index][min_item_index],
                current_array_index
            });
        } else {
            size_t non_finished_array_index = -1;
            for (size_t i = 0; i < arrays_count; i++) {
                if (min_item_indexes[i] < arrays[i].size() - 1) {
                    non_finished_array_index = i;
                    break;
                }
            }

            if (non_finished_array_index == -1) {
                items_in_arrays_left = false;
            } else {
                int& min_item_index = min_item_indexes[non_finished_array_index];
//                cout << "[1] min_item_index " << min_item_index << "\n";
                min_item_index++;
//                cout << "[2] pushing to queue {" << arrays[non_finished_array_index][min_item_index] << ", " << non_finished_array_index << "}\n";
                q.push({
                   arrays[non_finished_array_index][min_item_index],
                   non_finished_array_index
               });
            }
        }
    }

    return result;
}

vector<int> tournament_sort_without_merge(const vector<int>& original_array) {
    vector<int> current_array(original_array);

    bool sorted = false;
    while(!sorted) {
//        Logger::info("\nperforming tournament\n", true);
        auto result = perform_tournament(current_array);

        if (result.second.empty()) {
            current_array = result.first;
            sorted = true;
        } else {
            current_array = result.second;
            current_array.insert(current_array.end(), result.first.begin(), result.first.end());
        }
    }

    return current_array;
}

vector<int> tournament_sort_with_merge(const vector<int>& original_array) {
    vector<int> current_array(original_array);
    vector<vector<int>> winners_array;

    while(!current_array.empty()) {
//        Logger::info("\nperforming tournament\n", true);
        auto result = perform_tournament(current_array);
        winners_array.push_back(result.first);
        current_array = result.second;
    }

    return merge_sorted_arrays(winners_array);
}

vector<int> tournament_sort(const vector<int>& original_array, bool sort_with_merge = false) {
    if (sort_with_merge) {
        return tournament_sort_with_merge(original_array);
    } else {
        return tournament_sort_without_merge(original_array);
    }
}

auto get_random_int = [](int min, int max) -> int {
    return rand() % (max - min + 1) + min;
};

bool is_sorted(const vector<int>& array) {
    if (array.size() <= 1) {
        return true;
    }

    for (int i = 0; i < array.size() - 1; i++) {
        if (array[i] > array[i + 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    /* test case */
//    Logger::info(merge_sorted_arrays({
//        {1, 2, 10, 11},
//        {3, 4, 8, 9, 10, 11, 12},
//        {5, 6, 11},
//        {1, 5, 6, 11, 111},
//    }));
    srand(time(nullptr));

    const size_t array_size = get_random_int(110000, 110001);
    cout << "array size: " << array_size << "\n";
    vector<int> array(array_size);
    for (size_t i = 0; i < array_size; i++) {
//        array[i] = ((array_size - i) * i) % (array_size / 2);
        array[i] = i % 2 ? array_size - i : i;
    }
    Logger::info("initial array:\n", array);

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

    Logger::info("\ndistinct array:\n", distinct_array, "\n");

    auto sorted_distinct_array = tournament_sort(distinct_array, true);
    vector<int> final_sorted_array(array_size);

    int i = 0;
    for (auto item : sorted_distinct_array) {
        for (int count = 0; count < item_counts[item]; count++) {
            final_sorted_array[i] = item;
            i++;
        }
    }
    Logger::info("\n\nsorted array:\n", final_sorted_array);

    cout << "\n\ncheckin if sorted\n";
    if (is_sorted(final_sorted_array)) {
        cout << "sorted";
    } else {
        cout << "Fck, it is _not_ sorted, again!";
    }

    return 0;
}
