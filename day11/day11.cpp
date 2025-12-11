#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

std::unordered_map<string, vector<string>> read_input() {
  std::unordered_map<string, vector<string>> result;
  ifstream input_file("input.txt");
  string line;

  while (getline(input_file, line)) {
    string key = line.substr(0, line.find(":"));

    // Fix: Initialize the stringstream inside the loop with the substring
    std::stringstream ss(line.substr(line.find(":") + 1));

    string value;
    while (ss >> value) {
      result[key].push_back(value);
    }
  }

  return result;
}

long long count_paths(string current_node,
                      std::unordered_map<string, vector<string>> &input,
                      std::unordered_map<string, long long> &cache) {

  if (!(cache.find(current_node) == cache.end())) {
    return cache[current_node];
  }

  if (current_node == "out") {
    return 1;
  }

  long long total_paths = 0;

  for (const string &neighbor : input[current_node]) {
    total_paths += count_paths(neighbor, input, cache);
  }

  cache[current_node] = total_paths;
  return total_paths;
}

long long solve1(std::unordered_map<string, vector<string>> input) {
  std::unordered_map<string, long long> cache;

  return count_paths("you", input, cache);
}

// Add 'string target_node' to the arguments
long long count_paths2(string current_node, string target_node,
                       std::unordered_map<string, vector<string>> &input,
                       std::unordered_map<string, long long> &cache) {

  if (cache.find(current_node) != cache.end()) {
    return cache[current_node];
  }

  if (current_node == target_node) {
    return 1;
  }

  long long total_paths = 0;

  for (const string &neighbor : input[current_node]) {
    total_paths += count_paths2(neighbor, target_node, input, cache);
  }

  cache[current_node] = total_paths;
  return total_paths;
}

long long solve2(std::unordered_map<string, vector<string>> input) {
  std::unordered_map<string, long long> cache1, cache2, cache3;

  long long leg1 = count_paths2("svr", "dac", input, cache1);
  long long leg2 = count_paths2("dac", "fft", input, cache2);
  long long leg3 = count_paths2("fft", "out", input, cache3);

  long long path_A_count = leg1 * leg2 * leg3;

  // Possibility 2: svr -> fft -> dac -> out
  std::unordered_map<string, long long> cache4, cache5, cache6;

  long long leg4 = count_paths2("svr", "fft", input, cache4);
  long long leg5 = count_paths2("fft", "dac", input, cache5);
  long long leg6 = count_paths2("dac", "out", input, cache6);

  long long path_B_count = leg4 * leg5 * leg6;

  return path_A_count + path_B_count;
}

int main() {
  std::unordered_map<string, vector<string>> input = read_input();

  cout << "Part 1: " << endl;
  cout << solve1(input) << endl;
  cout << "Part 2: " << endl;
  cout << solve2(input) << endl;

  return 0;
}
