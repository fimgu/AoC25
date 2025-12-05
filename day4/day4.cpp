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

    if (!input_file.is_open()) {
        cerr << "Error: Could not open input.txt" << endl;
        return result;
    }

    while (getline(input_file, line)) {
        result.push_back(line);
    }
    return result;
}

const int dr[] = {-1, -1, -1,  0, 0,  1, 1, 1};
const int dc[] = {-1,  0,  1, -1, 1, -1, 0, 1};

int count_neighbors(const vector<string>& grid, int r, int c) {
    int rows = grid.size();
    int cols = grid[0].size();
    int count = 0;

    for (int i = 0; i < 8; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
            if (grid[nr][nc] == '@') {
                count++;
            }
        }
    }
    return count;
}

long long solve1(vector<string> input) {
    if (input.empty()) return 0;
    
    long long accessible_count = 0;
    int rows = input.size();
    int cols = input[0].size();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (input[r][c] == '@') {
                if (count_neighbors(input, r, c) < 4) {
                    accessible_count++;
                }
            }
        }
    }
    return accessible_count;
}

long long solve2(vector<string> input) {
    if (input.empty()) return 0;

    long long total_removed = 0;
    int rows = input.size();
    int cols = input[0].size();

    while (true) {
        vector<pair<int, int>> to_remove;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (input[r][c] == '@') {
                    if (count_neighbors(input, r, c) < 4) {
                        to_remove.push_back({r, c});
                    }
                }
            }
        }

        if (to_remove.empty()) {
            break;
        }
        total_removed += to_remove.size();
        for (auto p : to_remove) {
            input[p.first][p.second] = '.';
        }
    }

    return total_removed;
}

int main() {
    std::vector<string> input = read_input();

    cout << "Part 1: " << endl;
    cout << solve1(input) << endl;
    
    cout << "Part 2: " << endl;
    cout << solve2(input) << endl;

    return 0;
}