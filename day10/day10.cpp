#include <fstream>
#include <iostream>
#include <sstream>
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
  return 0;
}

long long solve2(vector<string> input) {
  return 0;
}

int main() {
  std::vector<string> input = read_input();

  cout << "Part 1: " << endl;
  cout << solve1(input) << endl;
  cout << "Part 2: " << endl;
  cout << solve2(input) << endl;

  return 0;
}
