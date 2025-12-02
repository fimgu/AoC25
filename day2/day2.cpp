#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

std::vector<std::vector<long long>> read_input() {
  vector<vector<long long>> result;
  ifstream input_file("test.txt");
  string line;
  long long a, b;
  char hyphen;
  char comma;

  while (input_file >> a >> hyphen >> b) {
    result.push_back({a, b});
    if (input_file.peek() == ',') {
      input_file >> comma;
    }
  }

  return result;
}

bool is_halves_matching(long long n) {
    int digits = (int)log10(n) + 1;
    if (digits % 2 != 0) return false;
    long long divisor = (long long)pow(10, digits / 2);
    return (n / divisor) == (n % divisor);
}

bool is_periodic(long long n) {
    std::string s = std::to_string(n);
    std::string doubled = s + s;
    std::string trimmed = doubled.substr(1, doubled.length() - 2);
    return trimmed.find(s) != std::string::npos;
}

long long solve1(vector<vector<long long>> input) {
  long long res = 0;
  for (auto &line : input) {
    for (long long i = line[0]; i <= line[1]; ++i) {
      if (is_halves_matching(i)) {
        res += i;
      }
    }
  }

  return res;
}

long long solve2(vector<vector<long long>> input) {
  long long res = 0;
  for (auto &line : input) {
    for (long long i = line[0]; i <= line[1]; ++i) {
      if (is_periodic(i)) {
        res += i;
      }
    }
  }

  return res;
}

int main() {
  std::vector<vector<long long>> input = read_input();

  cout << "Part 1: " << endl;
  cout << solve1(input) << endl;
  cout << "Part 2: " << endl;
  cout << solve2(input) << endl;

  return 0;
}
