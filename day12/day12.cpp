#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Point {
  int r, c;
  bool operator<(const Point &other) const {
    if (r != other.r)
      return r < other.r;
    return c < other.c;
  }
};

typedef vector<Point> Shape;

struct Query {
  int width;
  int height;
  vector<int> present_counts;
};

void normalize(Shape &s) {
  if (s.empty())
    return;
  int min_r = 10000, min_c = 10000;
  for (auto &p : s) {
    if (p.r < min_r)
      min_r = p.r;
    if (p.c < min_c)
      min_c = p.c;
  }
  for (auto &p : s) {
    p.r -= min_r;
    p.c -= min_c;
  }
  sort(s.begin(), s.end());
}

Shape rotate(const Shape &s) {
  Shape new_s;
  for (auto &p : s)
    new_s.push_back({p.c, -p.r});
  normalize(new_s);
  return new_s;
}

Shape flip(const Shape &s) {
  Shape new_s;
  for (auto &p : s)
    new_s.push_back({p.r, -p.c});
  normalize(new_s);
  return new_s;
}

vector<Shape> generate_variations(Shape base) {
  normalize(base);
  vector<Shape> vars;
  Shape cur = base;
  for (int i = 0; i < 4; i++) {
    vars.push_back(cur);
    vars.push_back(flip(cur));
    cur = rotate(cur);
  }
  sort(vars.begin(), vars.end());
  vars.erase(unique(vars.begin(), vars.end(),
                    [](const Shape &a, const Shape &b) {
                      if (a.size() != b.size())
                        return false;
                      for (size_t i = 0; i < a.size(); i++) {
                        if (a[i].r != b[i].r || a[i].c != b[i].c)
                          return false;
                      }
                      return true;
                    }),
             vars.end());
  return vars;
}

vector<string> read_input() {
  vector<string> res;
  ifstream f("input.txt");
  string l;
  while (getline(f, l))
    res.push_back(l);
  return res;
}

void parse_data(const vector<string> &input, vector<Shape> &shapes,
                vector<Query> &queries) {
  size_t i = 0;
  while (i < input.size()) {
    string line = input[i];
    if (line.empty()) {
      i++;
      continue;
    }

    if (line.find('x') != string::npos) {
      for (char &c : line)
        if (c == ':' || c == 'x')
          c = ' ';
      stringstream ss(line);
      Query q;
      ss >> q.width >> q.height;
      int c;
      while (ss >> c)
        q.present_counts.push_back(c);
      queries.push_back(q);
      i++;
    } else if (line.find(':') != string::npos) {
      i++;
      Shape s;
      int r = 0;
      while (i < input.size() && !input[i].empty()) {
        for (int c = 0; c < input[i].size(); c++) {
          if (input[i][c] == '#')
            s.push_back({r, c});
        }
        r++;
        i++;
      }
      shapes.push_back(s);
    } else {
      i++;
    }
  }
}

bool fits(const vector<vector<bool>> &grid, const Shape &s, int r_offset,
          int c_offset, int W, int H) {
  for (const auto &p : s) {
    int nr = r_offset + p.r;
    int nc = c_offset + p.c;
    if (nr < 0 || nr >= H || nc < 0 || nc >= W)
      return false;
    if (grid[nr][nc])
      return false;
  }
  return true;
}

void toggle(vector<vector<bool>> &grid, const Shape &s, int r_offset,
            int c_offset, bool val) {
  for (const auto &p : s) {
    grid[r_offset + p.r][c_offset + p.c] = val;
  }
}

bool backtrack(int idx, const vector<int> &shape_indices,
               vector<vector<bool>> &grid, int W, int H,
               const vector<vector<Shape>> &all_variations) {

  if (idx >= shape_indices.size())
    return true;

  int shape_id = shape_indices[idx];
  const vector<Shape> &vars = all_variations[shape_id];

  for (int r = 0; r < H; r++) {
    for (int c = 0; c < W; c++) {
      for (const auto &v : vars) {
        if (fits(grid, v, r, c, W, H)) {
          toggle(grid, v, r, c, true);

          if (backtrack(idx + 1, shape_indices, grid, W, H, all_variations))
            return true;

          toggle(grid, v, r, c, false);
        }
      }
    }
  }
  return false;
}

int main() {
  vector<string> input = read_input();
  vector<Shape> shapes;
  vector<Query> queries;
  parse_data(input, shapes, queries);

  vector<vector<Shape>> all_variations;
  for (auto &s : shapes)
    all_variations.push_back(generate_variations(s));

  int valid_regions = 0;

  for (const auto &q : queries) {
    vector<int> pieces_to_fit;
    int total_piece_area = 0;

    for (int id = 0; id < q.present_counts.size(); id++) {
      int count = q.present_counts[id];
      for (int k = 0; k < count; k++) {
        pieces_to_fit.push_back(id);
        total_piece_area += shapes[id].size();
      }
    }

    sort(pieces_to_fit.begin(), pieces_to_fit.end(),
         [&](int a, int b) { return shapes[a].size() > shapes[b].size(); });

    int region_area = q.width * q.height;
    if (total_piece_area > region_area) {
      continue;
    }

    vector<vector<bool>> grid(q.height, vector<bool>(q.width, false));

    if (backtrack(0, pieces_to_fit, grid, q.width, q.height, all_variations)) {
      valid_regions++;
    }
  }

  cout << "Part 1 Solution: " << valid_regions << endl;

  return 0;
}