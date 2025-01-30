#ifndef POINT_H
#define POINT_H
// The data has seven significant digits, which is the expected accuracy for
// 4-byte floats, so using anything larger would be waste of memory

struct Point {
  float x, y, z;
};

struct IntPoint {
  int x, y, z;
};

float get_distance_squared(const Point p1, const Point p2);
#endif
