#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<string> read_input() {
  vector<string> result;
  ifstream input_file("input.txt");
  string line;

  while (getline(input_file, line)) {
    if (!line.empty()) {
      result.push_back(line);
    }
  }
  return result;
}

int get_max_joltage(const string& bank) {
  int n = bank.length();
  if (n < 2) return 0;

  for (char first_digit = '9'; first_digit >= '1'; --first_digit) {
    size_t first_idx = bank.find(first_digit);
    if (first_idx != string::npos && first_idx < n - 1) {
      char max_second_digit = '0';
      for (size_t i = first_idx + 1; i < n; ++i) {
        if (bank[i] > max_second_digit) {
          max_second_digit = bank[i];
        }
      }

      int val1 = first_digit - '0';
      int val2 = max_second_digit - '0';

      return (val1 * 10) + val2;
    }
  }
  return 0;
}

long long solve1(const vector<string>& input) {
  long long total_joltage = 0;

  for (const string& line : input) {
    total_joltage += get_max_joltage(line);
  }

  return total_joltage;
}

long long get_max_12_digits(const string& bank) {
  string result = "";
  int current_search_start = 0;
  int needed = 12;
  int n = bank.length();

  while (needed > 0) {
    char max_digit = '/';
    int max_digit_index = -1;
    int search_limit = n - needed;
    for (int i = current_search_start; i <= search_limit; ++i) {
      if (bank[i] > max_digit) {
        max_digit = bank[i];
        max_digit_index = i;
        if (max_digit == '9') break; 
      }
    }

    result += max_digit;
    current_search_start = max_digit_index + 1;
    needed--;
  }

  return stoll(result);
}

long long solve2(const vector<string>& input) {
    long long total_joltage = 0;

    for (const string& line : input) {
        // Only process lines that actually have enough chars (just in case)
        if (line.length() >= 12) {
             total_joltage += get_max_12_digits(line);
        }
    }

    return total_joltage;
}


int main() {
  vector<string> input = read_input();

  cout << "Part 1 Total Joltage: " << endl;
  cout << solve1(input) << endl;
  cout << "Part 2: " << endl;
  cout << solve2(input) << endl;

  return 0;
}
