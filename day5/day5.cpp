#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Range {
  long long start;
  long long end;
};

std::pair<std::vector<Range>, std::vector<long long>> read_input() {
  std::vector<Range> ranges;
  std::vector<long long> numbers;
  std::ifstream input_file("input.txt");
  std::string line;
  bool reading_ranges = true;

  while (std::getline(input_file, line)) {
    if (line.empty()) {
      reading_ranges = false;
      continue;
    }

    if (reading_ranges) {
      size_t hyphen_pos = line.find('-');
      if (hyphen_pos != std::string::npos) {
        Range r;
        r.start = std::stoll(line.substr(0, hyphen_pos));
        r.end = std::stoll(line.substr(hyphen_pos + 1));
        ranges.push_back(r);
      }
    } else {
      numbers.push_back(std::stoll(line));
    }
  }

  return {ranges, numbers};
}

std::vector<Range> merge_ranges(std::vector<Range> ranges) {
  if (ranges.empty()) return {};
    
  std::sort(ranges.begin(), ranges.end(), [](const Range& a, const Range& b) {
    return a.start < b.start;
  });
    
  std::vector<Range> merged;
  merged.push_back(ranges[0]);

  for (size_t i = 1; i < ranges.size(); ++i) {
    if (ranges[i].start <= merged.back().end + 1) { 
      merged.back().end = std::max(merged.back().end, ranges[i].end);
    } else {
      merged.push_back(ranges[i]);
    }
  }
  return merged;
}

long long solve1(std::vector<Range> ranges, const std::vector<long long>& numbers) {
  long long count = 0;

  for (const auto& number : numbers) {
    for (const auto& range : ranges) {
      if (number >= range.start && number <= range.end) {
        count++; 
        break; 
      }
    }
  }
  return count;
}

long long solve2(std::vector<Range> ranges, const std::vector<long long>& numbers) {
    long long total_ids = 0;

    for (const auto& r : ranges) {
        total_ids += (r.end - r.start + 1);
    }

    return total_ids;
}

int main() {
  auto [ranges, numbers] = read_input();

  std::vector<Range> merged_ranges = merge_ranges(ranges);

  cout << "Part 1: " << endl;
  cout << solve1(merged_ranges, numbers) << endl; 

  cout << "Part 2: " << endl;
  cout << solve2(merged_ranges, numbers) << endl;

  return 0;
}
