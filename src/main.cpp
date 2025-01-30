#include "../include/cell_range.h"
#include "../include/count_pairs.h"
#include "../include/file_reader.h"
#include "../include/point.h"
#include "../include/reorder_points.h"
#include <chrono>
#include <cmath>
#include <iostream>

int main(int argc, char *argv[]) {
  auto start = std::chrono::high_resolution_clock::now();

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
    return 1;
  }

  // First step:
  // Assign each particle to a grid cell, and reorder the particles so all
  // particles within a cell are contiguous. The cells side length is
  // delta, where delta is the maximum allowed distance (0.005m).
  // This means all pairs of particles that are within the max distance of each
  // other must be in the same cell or in neighboring cells.

  const float max_dist = 0.05;
  const float max_dist_sq = max_dist * max_dist;
  const float grid_size = max_dist;

  std::vector<Point> points = read_points(argv[1]);

  std::cout << "Total number of points: " << points.size() << "\n";

  std::vector<Point> reordered_points(points.size());
  std::vector<CellRange> cell_ranges;
  IntPoint N_cells{};

  reorder_points(points, grid_size, reordered_points, cell_ranges, N_cells);

  // Second step:
  // iterate through all cells and check all possible pairs. This is
  // parellelized on the CPU, and should be easy to parallelize on the GPU.

  int pair_count =
      count_pairs(reordered_points, cell_ranges, N_cells, max_dist_sq);

  std::cout << "Total number of pairs within " << max_dist
            << "m: " << pair_count << "\n";

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "time: " << elapsed.count() << "s\n";
  return 0;
}
