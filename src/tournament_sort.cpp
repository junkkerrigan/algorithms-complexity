#include <queue>
#include <vector>
#include <any>
#include <cmath>

#include "tournament_sort.h"
#include "Logger.h"

using namespace std;

pair<vector<int>, vector<int>> perform_tournament(const vector<int>& original_array, size_t max_queue_size) {
    vector<int> array(original_array);
    size_t array_size = array.size();
    priority_queue <int, vector<int>, greater<>> q;
    vector<int> winners;
    vector<int> losers;

    int i;
    for (i = 0; i < min(max_queue_size, array_size); i++) {
        q.push(array[i]);
//        Logger::debug("\n\nq size after pushing ");
//        Logger::debug(tail);
//        Logger::debug(": ");
//        Logger::debug(q.size());
    }

    while (i < array_size) {
        if (q.size() == max_queue_size) {
            winners.push_back(q.top());
//            Logger::debug("\n\nq size after popping ");
//            Logger::debug(q.top());
            q.pop();
//            Logger::debug(": ");
//            Logger::debug(q.size());
        }

        int head = array[i];
        i++;
        if (head >= winners.back()) {
            q.push(head);
//            Logger::debug("\n\nq size after pushing ");
//            Logger::debug(head);
//            Logger::debug(": ");
//            Logger::debug(q.size());
        } else {
            losers.push_back(head);
        }
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
    for (size_t j = 0; j < arrays.size(); j++) {
        Logger::info(j, ": ");
        Logger::info(arrays[j], '\n');
    }
    size_t arrays_count = arrays.size();
    priority_queue<pair<int, size_t>, vector<pair<int, size_t>>, greater<>> q;
    vector<int> min_item_indexes(arrays_count, 0);
    vector<int> result;

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

    Logger::info("result: ", result, '\n');

    return result;
}

vector<int> tournament_sort_without_merge(const vector<int>& original_array, size_t max_queue_size) {
    vector<int> current_array(original_array);

    while (true) {
//        Logger::info("\nperforming tournament\n", true);
        auto result = perform_tournament(current_array, max_queue_size);

        if (result.second.empty()) {
            return result.first;
        } else {
            current_array = result.second;
            current_array.insert(current_array.end(), result.first.begin(), result.first.end());
        }
    }
}

vector<int> tournament_sort_with_merge(const vector<int>& original_array, size_t max_queue_size) {
    vector<int> current_array(original_array);
    vector<vector<int>> winners_array;

    while(!current_array.empty()) {
//        Logger::info("\nperforming tournament\n", true);
        auto result = perform_tournament(current_array, max_queue_size);
        winners_array.push_back(result.first);
        current_array = result.second;
    }

    return merge_sorted_arrays(winners_array);
}

vector<int> tournament_sort(const vector<int>& original_array, bool sort_with_merge = false) {
    size_t array_size = original_array.size();
    size_t max_queue_size = array_size * round(log2(array_size));

    if (sort_with_merge) {
        return tournament_sort_with_merge(original_array, max_queue_size);
    } else {
        return tournament_sort_without_merge(original_array, max_queue_size);
    }
}