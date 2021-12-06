#include "limited_RAM.h"

const string internal_input_file_name = "input-internal.txt";
const string winners_file_name = "winners.txt";
const string losers_file_name = "losers.txt";
const string losers_temp_file_name = "losers-temp.txt";

void open_file(ifstream& file, const string& file_name, const string& file_label) {
    file.open(file_name);
    Logger::debug("opened ", file_name, "\n\n");
    if (!file.is_open()) {
        stringstream ss;
        ss << "Failed to open " << file_label << "for reading";
        throw std::runtime_error(ss.str());
    }
}
void open_file(ofstream& file, const string& file_name, const string& file_label, bool clear_content = true) {
    if (clear_content) {
        file.open(file_name, ios::out | ios::trunc);
    } else {
        file.open(file_name);
    }

    if (!file.is_open()) {
        stringstream ss;
        ss << "Failed to open " << file_label << "for writing";
        throw std::runtime_error(ss.str());
    }
}

template<typename T = int>
T read_next_number(ifstream& file) {
    string number_string = "";
    char symbol;

    while (file >> symbol) {
        Logger::debug("read symbol ", symbol, "\n\n");
        if (symbol == ' ' || symbol == '\n') {
            break;
        }

        number_string += symbol;
    }

    T number;
    try {
        number = stoi(number_string);
    } catch (const std::exception&) {
        throw std::invalid_argument("Next string segment is not a number");
    }

    Logger::debug("read number ", number, "\n\n");

    return number;
}

void append_file_content(ofstream& append_to, ifstream& append_from) {
    char symbol;
    while (append_from >> symbol) {
        Logger::debug("in append_file_content: read ", symbol, "\n\n");
        append_to << symbol;
    }
}

void maybe_clear_and_push_back(int item, vector<int>& array, int max_array_size, ofstream& file) {
    if (array.size() == max_array_size) {
        for (auto i : array) {
            file << i << ' ';
        }
        array.clear();
    }

    array.push_back(item);
}

int perform_tournament(ifstream& input_file, size_t items_count_in_RAM, ofstream& winners_file, ofstream& losers_file) {
    size_t array_size = read_next_number<size_t>(input_file);

    size_t max_queue_size = array_size * round(log2(array_size));
    max_queue_size = min(max_queue_size, items_count_in_RAM);
    priority_queue <int, vector<int>, greater<>> q;

    vector<int> winners;
    int losers_count = 0;

    ofstream losers_temp_file_write;
    open_file(losers_temp_file_write, losers_temp_file_name, "the losers temp file");

    int i;
    for (i = 0; i < max_queue_size; i++) {
        q.push(read_next_number(input_file));
    }

    while (i < array_size) {
        if (q.size() == max_queue_size) {
            maybe_clear_and_push_back(q.top(), winners, items_count_in_RAM, winners_file);
            q.pop();
        }

        int head = read_next_number(input_file);
        i++;
        if (head >= winners.back()) {
            q.push(head);
        } else {
            losers_temp_file_write << head;
            if (i < array_size - 1) {
                losers_temp_file_write << ' ';
            }
            losers_count++;
        }
    }
    input_file.close();

    while (!q.empty()) {
        winners_file << q.top();
        winners_file << (q.size() == 1 ? '\n' : ' ');

        q.pop();
    }
    winners_file.close();

    losers_file << losers_count << '\n';

    losers_temp_file_write.close();
    ifstream losers_temp_file_read(losers_temp_file_name);
    open_file(losers_temp_file_read, losers_temp_file_name, "the losers temp file");

    append_file_content(losers_file, losers_temp_file_read);

    losers_file.close();
    losers_temp_file_read.close();

    return losers_count;
}

void tournament_sort_with_limited_RAM(const string& input_file_name, const string& output_file_name, size_t items_count_in_RAM) {
    ifstream original_input_file;
    open_file(original_input_file, input_file_name, "the original input file");
    ofstream internal_input_file_write;
    open_file(internal_input_file_write, internal_input_file_name, "the internal input file");

    append_file_content(internal_input_file_write, original_input_file);
    original_input_file.close();
    internal_input_file_write.close();

    ifstream internal_input_file_read;
    ifstream losers_file_read;
    ofstream losers_file_write;

    ofstream winners_file;
    open_file(winners_file, winners_file_name, "the winners file");

    int losers_count;
    while (true) {
        Logger::debug("beginning of a loop\n\n");
        open_file(internal_input_file_read, internal_input_file_name, "the internal input file");
        open_file(losers_file_write, losers_file_name, "the losers file");

        Logger::debug("running sorting\n\n");
        losers_count = perform_tournament(internal_input_file_read, items_count_in_RAM, winners_file, losers_file_write);
        Logger::debug("losers count", losers_count, "\n\n");
        if (!losers_count) {
            break;
        }

        internal_input_file_read.close();
        open_file(internal_input_file_write, internal_input_file_name, "the internal input file");
        open_file(losers_file_read, losers_file_name, "the losers file");

        append_file_content(internal_input_file_write, losers_file_read);

        internal_input_file_write.close();
        losers_file_read.close();
    }

//    return merge_sorted_arrays(winners_array);
}