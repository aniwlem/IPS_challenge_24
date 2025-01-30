#include "../include/point.h"


float get_distance_squared(const Point p1, const Point p2) {
  float dx = (p1.x - p2.x);
  float dy = (p1.y - p2.y);
  float dz = (p1.z - p2.z);

  return dx * dx + dy * dy + dz * dz;
}
