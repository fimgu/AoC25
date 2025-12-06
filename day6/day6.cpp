#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

std::vector<string> read_input() {
    vector<string> result;
    ifstream input_file("input.txt");
    string line;

    while (getline(input_file, line)) {
        result.push_back(line);
    }

    return result;
}

bool is_column_empty(const vector<string>& grid, int col) {
    for (const auto& row : grid) {
        if (col < row.length() && row[col] != ' ') {
            return false;
        }
    }
    return true;
}

long long solve1(vector<string> input) {
    if (input.empty()) return 0;

    size_t max_width = 0;
    for (const auto& s : input) max_width = max(max_width, s.length());
    for (auto& s : input) s.resize(max_width, ' ');

    long long grand_total = 0;
    int col = 0;
    int height = input.size();

    while (col < max_width) {
        if (is_column_empty(input, col)) {
            col++;
            continue;
        }

        int start_col = col;
        int end_col = start_col + 1;
        while (end_col < max_width && !is_column_empty(input, end_col)) {
            end_col++;
        }

        vector<long long> numbers;
        char op = ' ';

        string op_segment = input[height - 1].substr(start_col, end_col - start_col);
        for (char c : op_segment) {
            if (c == '+' || c == '*') {
                op = c;
                break;
            }
        }

        for (int r = 0; r < height - 1; ++r) {
            string segment = input[r].substr(start_col, end_col - start_col);
            string num_str;
            for (char c : segment) {
                if (isdigit(c)) num_str += c;
            }
            if (!num_str.empty()) {
                numbers.push_back(stoll(num_str));
            }
        }

        if (!numbers.empty()) {
            long long current_result = numbers[0];
            for (size_t i = 1; i < numbers.size(); ++i) {
                if (op == '+') current_result += numbers[i];
                else if (op == '*') current_result *= numbers[i];
            }
            grand_total += current_result;
        }

        col = end_col;
    }

    return grand_total;
}

long long solve2(vector<string> input) {
    if (input.empty()) return 0;

    size_t max_width = 0;
    for (const auto& s : input) max_width = max(max_width, s.length());
    for (auto& s : input) s.resize(max_width, ' ');

    long long grand_total = 0;
    int col = 0;
    int height = input.size();

    while (col < max_width) {
        if (is_column_empty(input, col)) {
            col++;
            continue;
        }
        int start_col = col;
        int end_col = start_col + 1;
        while (end_col < max_width && !is_column_empty(input, end_col)) {
            end_col++;
        }

        vector<long long> numbers;
        char op = ' ';

        for(int c = start_col; c < end_col; c++) {
            char ch = input[height - 1][c];
            if (ch == '+' || ch == '*') {
                op = ch;
                break;
            }
        }

        for (int c = end_col - 1; c >= start_col; --c) {
            string num_str;
            for (int r = 0; r < height - 1; ++r) {
                if (isdigit(input[r][c])) {
                    num_str += input[r][c];
                }
            }
            
            if (!num_str.empty()) {
                numbers.push_back(stoll(num_str));
            }
        }

        if (!numbers.empty()) {
            long long current_result = numbers[0];
            for (size_t i = 1; i < numbers.size(); ++i) {
                if (op == '+') current_result += numbers[i];
                else if (op == '*') current_result *= numbers[i];
            }
            grand_total += current_result;
        }

        col = end_col;
    }

    return grand_total;
}

int main() {
    std::vector<string> input = read_input();

    cout << "Part 1: " << endl;
    cout << solve1(input) << endl;
    
    cout << "Part 2: " << endl;
    cout << solve2(input) << endl;

    return 0;
}