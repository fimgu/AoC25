#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Point {
  long long x, y;
};

std::vector<Point> parseInput(const std::string &filename) {
  std::vector<Point> points;
  std::ifstream file(filename);
  std::string line;
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return points;
  }
  while (std::getline(file, line)) {
    if (line.empty())
      continue;
    std::replace(line.begin(), line.end(), ',', ' ');
    std::stringstream ss(line);
    Point p;
    if (ss >> p.x >> p.y) {
      points.push_back(p);
    }
  }
  return points;
}

bool is_point_in_rect(const Point &p, long long min_x, long long max_x,
                      long long min_y, long long max_y) {
  return p.x > min_x && p.x < max_x && p.y > min_y && p.y < max_y;
}

bool is_inside_polygon(double x, double y, const std::vector<Point> &points) {
  int intersect_count = 0;
  size_t n = points.size();
  for (size_t i = 0; i < n; ++i) {
    Point p1 = points[i];
    Point p2 = points[(i + 1) % n];

    // Check if vertical edge crosses y
    // Using strict inequality for ray casting
    if ((p1.y > y) != (p2.y > y)) {
      // Calculate x-intersection
      double intersect_x =
          (p2.x - p1.x) * (y - p1.y) / (double)(p2.y - p1.y) + p1.x;
      if (x < intersect_x) {
        intersect_count++;
      }
    }
  }
  return (intersect_count % 2) == 1;
}

bool edges_intersect_rect(long long min_x, long long max_x, long long min_y,
                          long long max_y, const std::vector<Point> &points) {
  size_t n = points.size();
  for (size_t i = 0; i < n; ++i) {
    Point p1 = points[i];
    Point p2 = points[(i + 1) % n];

    if (p1.x == p2.x) { // Vertical edge
      long long x = p1.x;
      long long y_start = std::min(p1.y, p2.y);
      long long y_end = std::max(p1.y, p2.y);

      // Check if edge x is strictly inside rect x range
      if (x > min_x && x < max_x) {
        // Check if y range overlaps rect y range
        if (std::max(y_start, min_y) < std::min(y_end, max_y)) {
          return true;
        }
      }
    } else if (p1.y == p2.y) { // Horizontal edge
      long long y = p1.y;
      long long x_start = std::min(p1.x, p2.x);
      long long x_end = std::max(p1.x, p2.x);

      // Check if edge y is strictly inside rect y range
      if (y > min_y && y < max_y) {
        // Check if x range overlaps rect x range
        if (std::max(x_start, min_x) < std::min(x_end, max_x)) {
          return true;
        }
      }
    }
  }
  return false;
}

bool is_valid_rectangle(const std::vector<Point> &points, size_t i, size_t j) {
  long long min_x = std::min(points[i].x, points[j].x);
  long long max_x = std::max(points[i].x, points[j].x);
  long long min_y = std::min(points[i].y, points[j].y);
  long long max_y = std::max(points[i].y, points[j].y);

  // 1. Check if any polygon vertex is strictly inside the rectangle.
  for (const auto &p : points) {
    if (is_point_in_rect(p, min_x, max_x, min_y, max_y)) {
      return false;
    }
  }

  // 2. Check if any polygon edge intersects the interior of the rectangle.
  if (edges_intersect_rect(min_x, max_x, min_y, max_y, points)) {
    return false;
  }

  // 3. Check if a representative point strictly inside/on boundary is inside
  // polygon. Use offset to avoid boundary ambiguities
  double center_x = min_x + 0.5;
  double center_y = min_y + 0.5;

  // If rectangle is just a point or line, we need to handle carefully?
  // But since area > 0 test in main loop might rely on this, let's just run it.
  // However, if min_x == max_x, width is 1. The code is robust.

  return is_inside_polygon(center_x, center_y, points);
}

int main(int argc, char *argv[]) {
  std::string filename = "input.txt";
  if (argc > 1) {
    filename = argv[1];
  }

  std::vector<Point> points = parseInput(filename);
  if (points.empty()) {
    return 1;
  }

  long long max_area_p1 = 0;
  long long max_area_p2 = 0;

  // Brute force all pairs
  for (size_t i = 0; i < points.size(); ++i) {
    for (size_t j = i + 1; j < points.size(); ++j) {
      long long width = std::abs(points[i].x - points[j].x) + 1;
      long long height = std::abs(points[i].y - points[j].y) + 1;
      long long area = width * height;

      // Part 1
      if (area > max_area_p1) {
        max_area_p1 = area;
      }

      // Part 2
      if (is_valid_rectangle(points, i, j)) {
        if (area > max_area_p2) {
          max_area_p2 = area;
        }
      }
    }
  }

  std::cout << "Max area Part 1: " << max_area_p1 << std::endl;
  std::cout << "Max area Part 2: " << max_area_p2 << std::endl;

  return 0;
}
