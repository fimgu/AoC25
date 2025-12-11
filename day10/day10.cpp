#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Machine {
  int target_light_mask;
  int num_lights;
  vector<int> button_masks;

  int num_counters;
  int num_buttons;
  vector<long long> joltage_targets;
  vector<vector<double>> matrix;
};

Machine parse_machine(const string &line) {
  Machine m;

  size_t bracket_start = line.find('[');
  size_t bracket_end = line.find(']');
  string light_str =
      line.substr(bracket_start + 1, bracket_end - bracket_start - 1);

  m.target_light_mask = 0;
  m.num_lights = 0;
  for (char c : light_str) {
    if (c == '#')
      m.target_light_mask |= (1 << m.num_lights);
    m.num_lights++;
  }

  vector<vector<int>> temp_buttons;
  size_t pos = bracket_end + 1;
  while ((pos = line.find('(', pos)) != string::npos) {
    size_t end_paren = line.find(')', pos);
    string btn_str = line.substr(pos + 1, end_paren - pos - 1);

    int mask = 0;
    vector<int> indices;
    stringstream ss(btn_str);
    string segment;

    while (getline(ss, segment, ',')) {
      if (!segment.empty()) {
        int idx = stoi(segment);
        mask |= (1 << idx);
        indices.push_back(idx);
      }
    }
    m.button_masks.push_back(mask);
    temp_buttons.push_back(indices);
    pos = end_paren + 1;
  }
  m.num_buttons = temp_buttons.size();

  size_t brace_start = line.find('{');
  if (brace_start != string::npos) {
    size_t brace_end = line.find('}', brace_start);
    string targ_str = line.substr(brace_start + 1, brace_end - brace_start - 1);

    stringstream ss(targ_str);
    string segment;
    while (getline(ss, segment, ',')) {
      if (!segment.empty()) {
        m.joltage_targets.push_back(stoll(segment));
      }
    }
  }
  m.num_counters = m.joltage_targets.size();

  m.matrix.assign(m.num_counters, vector<double>(m.num_buttons, 0.0));
  for (int b = 0; b < m.num_buttons; ++b) {
    for (int c_idx : temp_buttons[b]) {
      m.matrix[c_idx][b] = 1.0;
    }
  }

  return m;
}

int solve1(const Machine &m) {
  int limit = 1 << m.num_lights;
  vector<int> dist(limit, -1);
  vector<int> q;
  q.reserve(limit);

  int head = 0;
  dist[0] = 0;
  q.push_back(0);

  while (head < q.size()) {
    int u = q[head++];
    if (u == m.target_light_mask)
      return dist[u];

    for (int btn_mask : m.button_masks) {
      int v = u ^ btn_mask;
      if (dist[v] == -1) {
        dist[v] = dist[u] + 1;
        q.push_back(v);
      }
    }
  }
  return -1;
}

long long min_p2_presses;
const double EPS = 1e-9;

void iterate_free_vars(int current_idx, const vector<int> &free_vars,
                       vector<long long> &solution,
                       const vector<vector<double>> &rref,
                       const vector<int> &pivot_col_to_row, const Machine &m) {

  if (current_idx == free_vars.size()) {
    long long current_total = 0;

    bool valid = true;

    for (int r = 0; r < rref.size(); ++r) {
      int p = -1;
      for (int c = 0; c < m.num_buttons; ++c) {
        if (abs(rref[r][c]) > EPS) {
          p = c;
          break;
        }
      }
      if (p == -1) {
        if (abs(rref[r].back()) > EPS) {
          valid = false;
          break;
        }
        continue;
      }

      double val = rref[r].back();

      for (int f : free_vars) {
        if (abs(rref[r][f]) > EPS) {
          val -= rref[r][f] * (double)solution[f];
        }
      }

      if (val < -EPS) {
        valid = false;
        break;
      }
      long long nearest = (long long)round(val);
      if (abs(val - nearest) > 1e-4) {
        valid = false;
        break;
      }

      solution[p] = nearest;
    }

    if (valid) {
      long long sum = 0;
      for (long long x : solution)
        sum += x;

      if (min_p2_presses == -1 || sum < min_p2_presses) {
        min_p2_presses = sum;
      }
    }
    return;
  }

  int var_idx = free_vars[current_idx];

  long long limit = -1;

  for (int r = 0; r < m.num_counters; ++r) {
    if (m.matrix[r][var_idx] > 0.5) {
      long long allowed = m.joltage_targets[r];
      if (limit == -1 || allowed < limit) {
        limit = allowed;
      }
    }
  }
  if (limit == -1)
    limit = 0;

  for (long long k = 0; k <= limit; ++k) {
    solution[var_idx] = k;

    iterate_free_vars(current_idx + 1, free_vars, solution, rref,
                      pivot_col_to_row, m);
  }
}

long long solve2(const Machine &m) {
  min_p2_presses = -1;
  int rows = m.num_counters;
  int cols = m.num_buttons;

  vector<vector<double>> sys = m.matrix;
  for (int r = 0; r < rows; ++r) {
    sys[r].push_back((double)m.joltage_targets[r]);
  }

  int pivot_row = 0;
  vector<int> pivot_cols;
  vector<int> free_cols;
  vector<int> col_type(cols, 0);

  for (int c = 0; c < cols && pivot_row < rows; ++c) {
    int sel = pivot_row;
    while (sel < rows && abs(sys[sel][c]) < EPS)
      sel++;

    if (sel == rows) {
      col_type[c] = 2;
      free_cols.push_back(c);
      continue;
    }

    swap(sys[sel], sys[pivot_row]);

    double div = sys[pivot_row][c];
    for (int j = c; j <= cols; ++j)
      sys[pivot_row][j] /= div;

    for (int r = 0; r < rows; ++r) {
      if (r != pivot_row) {
        double fact = sys[r][c];
        if (abs(fact) > EPS) {
          for (int j = c; j <= cols; ++j)
            sys[r][j] -= fact * sys[pivot_row][j];
        }
      }
    }

    col_type[c] = 1;
    pivot_cols.push_back(c);
    pivot_row++;
  }

  for (int c = 0; c < cols; ++c) {
    if (col_type[c] == 0)
      free_cols.push_back(c);
  }

  for (int r = pivot_row; r < rows; ++r) {
    if (abs(sys[r].back()) > EPS)
      return -1;
  }

  vector<long long> solution(cols, 0);

  iterate_free_vars(0, free_cols, solution, sys, {}, m);

  return (min_p2_presses == -1) ? 0 : min_p2_presses;
}

int main(int argc, char *argv[]) {
  string filename = "input.txt";
  if (argc > 1)
    filename = argv[1];

  ifstream infile(filename);
  if (!infile) {
    cerr << "Error opening file: " << filename << endl;
    return 1;
  }

  string line;
  long long total_p1 = 0;
  long long total_p2 = 0;

  while (getline(infile, line)) {
    if (line.empty())
      continue;
    Machine m = parse_machine(line);

    int p1 = solve1(m);
    if (p1 != -1)
      total_p1 += p1;

    long long p2 = solve2(m);
    total_p2 += p2;
  }

  cout << "Part 1 Total: " << total_p1 << endl;
  cout << "Part 2 Total: " << total_p2 << endl;

  return 0;
}