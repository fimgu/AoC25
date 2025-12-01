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

int solve1(vector<string> input) {
  int sum = 0;
  int pos = 50; // start pos
  for(auto & n : input) {
    char dir = n[0];
    int amount = stoi(n.substr(1));
    int chg = 0;
    if (dir == 'L') {
      pos = (pos - amount) % 100;
      if (pos < 0) pos += 100;
    } else {
      pos = (pos + amount) % 100; 
    }
    if (pos == 0) sum++;
    // cout << "The dial is rotated " << n << " to point at " << pos << endl;
  }
  return sum;
}

int solve2(vector<string> input) {
  long long sum = 0;
  int pos = 50;

  for(auto & n : input) {
    char dir = n[0];
    int amount = stoi(n.substr(1));

    if (dir == 'R') {
      sum += (pos + amount) / 100;
      pos = (pos + amount) % 100;

    } else { // dir == 'L'
      int dist_to_zero = (pos == 0) ? 100 : pos;
      if (amount >= dist_to_zero) {
        sum++; 
        int remaining = amount - dist_to_zero;
        sum += remaining / 100; 
      }
      pos = (pos - amount) % 100;
      if (pos < 0) pos += 100;
    }
  }
  return (int)sum;
}

int main() {
  std::vector<string> input = read_input();

  cout << "Part 1: " << endl;
  printf("%d\n", solve1(input));
  cout << "Part 2: " << endl;
  printf("%d\n", solve2(input));

  return 0;
}