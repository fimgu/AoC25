#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Point {
  long long x, y, z;
};

struct Edge {
  int u, v;
  long long distSq;
};

struct DSU {
  vector<int> parent;
  vector<int> size;
  int num_components;

  DSU(int n) {
    parent.resize(n);
    iota(parent.begin(), parent.end(), 0);
    size.assign(n, 1);
    num_components = n;
  }

  int find(int i) {
    if (parent[i] == i)
      return i;
    return parent[i] = find(parent[i]);
  }

  bool unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j) {
      if (size[root_i] < size[root_j])
        swap(root_i, root_j);
      parent[root_j] = root_i;
      size[root_i] += size[root_j];
      num_components--;
      return true;
    }
    return false;
  }
};

long long distSq(const Point &a, const Point &b) {
  long long dx = a.x - b.x;
  long long dy = a.y - b.y;
  long long dz = a.z - b.z;
  return dx * dx + dy * dy + dz * dz;
}

vector<Point> parse_input(vector<string> &lines) {
  vector<Point> points;
  for (const string &line : lines) {
    stringstream ss(line);
    Point p;
    char comma;
    if (ss >> p.x >> comma >> p.y >> comma >> p.z) {
      points.push_back(p);
    }
  }
  return points;
}

long long solve(vector<string> lines, int connections_to_make) {
  vector<Point> points = parse_input(lines);
  int n = points.size();
  if (n == 0)
    return 0;

  vector<Edge> edges;
  edges.reserve(n * (n - 1) / 2);

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      edges.push_back({i, j, distSq(points[i], points[j])});
    }
  }

  sort(edges.begin(), edges.end(),
       [](const Edge &a, const Edge &b) { return a.distSq < b.distSq; });

  DSU dsu(n);
  int count = 0;
  for (const auto &edge : edges) {
    if (count >= connections_to_make)
      break;

    dsu.unite(edge.u, edge.v);
    count++;
  }

  vector<int> sizes;
  for (int i = 0; i < n; ++i) {
    if (dsu.parent[i] == i) {
      sizes.push_back(dsu.size[i]);
    }
  }

  sort(sizes.rbegin(), sizes.rend());

  if (sizes.size() < 3)
    return 0; // Should not happen given problem constraints

  return (long long)sizes[0] * sizes[1] * sizes[2];
}

long long solve2(vector<string> lines) {
  vector<Point> points = parse_input(lines);
  int n = points.size();
  if (n == 0)
    return 0;

  vector<Edge> edges;
  edges.reserve(n * (n - 1) / 2);

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      edges.push_back({i, j, distSq(points[i], points[j])});
    }
  }

  sort(edges.begin(), edges.end(),
       [](const Edge &a, const Edge &b) { return a.distSq < b.distSq; });

  DSU dsu(n);
  for (const auto &edge : edges) {
    if (dsu.unite(edge.u, edge.v)) {
      if (dsu.num_components == 1) {
        return points[edge.u].x * points[edge.v].x;
      }
    }
  }
  return 0;
}

std::vector<string> read_input(string filename) {
  vector<string> result;
  ifstream input_file(filename);
  string line;

  while (getline(input_file, line)) {
    if (!line.empty())
      result.push_back(line);
  }

  return result;
}

int main() {
  // Example test
  auto test_input = read_input("test.txt");
  cout << "Test Result Part 1: " << solve(test_input, 10) << endl;
  cout << "Test Result Part 2 (Target 25272): " << solve2(test_input) << endl;

  // Real input
  auto real_input = read_input("input.txt");
  cout << "Part 1 Result: " << solve(real_input, 1000) << endl;
  cout << "Part 2 Result: " << solve2(real_input) << endl;

  return 0;
}
