#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

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

long long solve1(vector<string> input) {
  int rows = input.size();
  if (rows == 0)
    return 0;
  int cols = input[0].size();

  long long total_splits = 0;
  std::set<int> current_beams;

  int start_y = 0;
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (input[r][c] == 'S') {
        current_beams.insert(c);
        start_y = r;
      }
    }
    if (!current_beams.empty())
      break;
  }

  for (int y = start_y + 1; y < rows; ++y) {
    std::set<int> next_beams;

    for (int x : current_beams) {
      char cell = input[y][x];

      if (cell == '.') {
        next_beams.insert(x);
      } else if (cell == '^') {
        total_splits++;

        if (x - 1 >= 0)
          next_beams.insert(x - 1);
        if (x + 1 < cols)
          next_beams.insert(x + 1);
      } else if (cell == 'S') {
        next_beams.insert(x);
      }
    }

    current_beams = next_beams;

    if (current_beams.empty())
      break;
  }

  return total_splits;
}

long long solve2(vector<string> input) {
  int rows = input.size();
  if (rows == 0)
    return 0;
  int cols = input[0].size();

  std::map<int, long long> current_counts;
  long long completed_timelines = 0;

  int start_y = 0;
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      if (input[r][c] == 'S') {
        current_counts[c] = 1;
        start_y = r;
      }
    }
    if (!current_counts.empty())
      break;
  }

  for (int y = start_y + 1; y < rows; ++y) {
    std::map<int, long long> next_counts;

    for (auto const &pair : current_counts) {
      int x = pair.first;
      long long count = pair.second;
      char cell = input[y][x];

      if (cell == '.') {
        next_counts[x] += count;
      } else if (cell == '^') {

        if (x - 1 >= 0) {
          next_counts[x - 1] += count;
        } else {
          completed_timelines += count;
        }

        if (x + 1 < cols) {
          next_counts[x + 1] += count;
        } else {
          completed_timelines += count;
        }
      } else if (cell == 'S') {

        next_counts[x] += count;
      }
    }

    current_counts = next_counts;
  }

  for (auto const &pair : current_counts) {
    long long count = pair.second;
    completed_timelines += count;
  }

  return completed_timelines;
}

int main() {
  std::vector<string> input = read_input();

  cout << "Part 1: " << endl;
  cout << solve1(input) << endl;
  cout << "Part 2: " << endl;
  cout << solve2(input) << endl;

  return 0;
}
