#include "tournament_sort.h"

pair<vector<int>, vector<int>> perform_tournament(const vector<int>& array) {
    size_t array_size = array.size();

    size_t max_queue_size = array_size * round(log2(array_size));
    priority_queue <int, vector<int>, greater<>> q;

    vector<int> winners;
    vector<int> losers;

    int i;
    for (i = 0; i < min(max_queue_size, array_size); i++) {
        q.push(array[i]);
    }

    while (i < array_size) {
        if (q.size() == max_queue_size) {
            winners.push_back(q.top());
            q.pop();
        }

        int head = array[i];
        i++;
        if (head >= winners.back()) {
            q.push(head);
        } else {
            losers.push_back(head);
        }
    }
    while (!q.empty()) {
        winners.push_back(q.top());
        q.pop();
    }

    return make_pair(winners, losers);
}

vector<int> merge_sorted_arrays(const vector<vector<int>>& arrays) {
    size_t arrays_count = arrays.size();
    priority_queue<pair<int, size_t>, vector<pair<int, size_t>>, greater<>> q;
    vector<int> min_item_indexes(arrays_count, 0);
    vector<int> result;

    for (size_t i = 0; i < arrays_count; i++) {
        q.push({ arrays[i][0], i });
    }

    bool items_in_arrays_left = true;
    while (!q.empty()) {
        auto least = q.top();
        q.pop();
        int least_value = least.first;
        size_t current_array_index = least.second;

        result.push_back(least_value);
        if (!items_in_arrays_left) {
            continue;
        }

        if (min_item_indexes[current_array_index] < arrays[current_array_index].size() - 1) {
            int& min_item_index = min_item_indexes[current_array_index];
            min_item_index++;
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
                min_item_index++;
                q.push({
                   arrays[non_finished_array_index][min_item_index],
                   non_finished_array_index
               });
            }
        }
    }

    return result;
}

vector<int> tournament_sort(const vector<int>& original_array) {
    vector<int> current_array(original_array);
    vector<vector<int>> winners_array;

    while(!current_array.empty()) {
        auto result = perform_tournament(current_array);
        winners_array.push_back(result.first);
        current_array = std::move(result.second);
    }

    return merge_sorted_arrays(winners_array);
}