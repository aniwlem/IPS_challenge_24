#include "../include/cell_range.h"
#include "../include/point.h"
#include <cmath>
#include <vector>

/*
 * Find the largest and smallest x-, y- and z-values
 */
std::pair<Point, Point> find_grid_extremes(const std::vector<Point> &points) {

  Point min_point = points[0];
  Point max_point = points[0];

  for (std::size_t i = 1; i < points.size(); i++) {
    min_point.x = std::min(min_point.x, points[i].x);
    min_point.y = std::min(min_point.y, points[i].y);
    min_point.z = std::min(min_point.z, points[i].z);

    max_point.x = std::max(max_point.x, points[i].x);
    max_point.y = std::max(max_point.y, points[i].y);
    max_point.z = std::max(max_point.z, points[i].z);
  }

  return {min_point, max_point};
}

/*
 * Takes the largest and smallest x-, y- and z-values, and the grid length to
 * calculate the number of grid cells in each direction. Offsets the grid so the
 * smallest point is in the origin
 */
IntPoint find_grid_sizes(Point &min_point, Point &max_point,
                         const float grid_size) {
  // Need ceiling since the potential decimal part needs a cell as well
  int Nx = static_cast<int>(std::ceil((max_point.x - min_point.x) / grid_size));
  int Ny = static_cast<int>(std::ceil((max_point.y - min_point.y) / grid_size));
  int Nz = static_cast<int>(std::ceil((max_point.z - min_point.z) / grid_size));

  return {Nx, Ny, Nz};
}

/*
 * Computes the cell_index, i.e. index in a flattened version of the grid, of
 * _one_ point.
 */
int get_cell_index(Point point, const Point min_point, const IntPoint N_cells,
                   const float grid_size) {
  int i_x = static_cast<int>((point.x - min_point.x) / grid_size);
  int i_y = static_cast<int>((point.y - min_point.y) / grid_size);
  int i_z = static_cast<int>((point.z - min_point.z) / grid_size);

  // NOTE: It might generally be faster to use a Z-order/Morton-curve instead of
  // this linear index. It would make points that are spatially close closer to
  // each other in the vector, meaning it is more likely that neighbors have
  // been prefetched and cached.  For the large file with ~100k points, each
  // point needing 12B, almost the entire vector fits in L1. Since it only needs
  // to store 1/16th of it, caching is a non-issue.

  return i_x + N_cells.x * (i_y + N_cells.y * i_z);
}

/*
 * Goes through the entire vector of points, and finds each ones index in the
 * flattened grid. Finds the number of point in each cell as well.
 */
void get_cell_indices_and_counts(const std::vector<Point> &points,
                                 const Point min_point, const IntPoint N_cells,
                                 const float grid_size,
                                 std::vector<int> &grid_indices,
                                 std::vector<int> &cell_counts) {

  for (std::size_t i = 0; i < points.size(); i++) {
    int curr_grid_ind =
        get_cell_index(points[i], min_point, N_cells, grid_size);
    grid_indices[i] = curr_grid_ind;
    cell_counts[curr_grid_ind]++;
  }
}

/*
 * Finds the accumulated number of points before each cell in the flattened
 * grid.
 */
void get_prefix_sums(const std::vector<int> &cell_counts,
                     std::vector<int> &prefix_sums) {
  prefix_sums[0] = 0;
  for (std::size_t i = 1; i < prefix_sums.size(); i++) {
    prefix_sums[i] = prefix_sums[i - 1] + cell_counts[i - 1];
  }
}

/*
 * Takes a vector of points, and a grid size, and fills in reordered_points with
 * the points reordered so all points within a given cell are a contiguous block
 * in that vector. cell_ranges gives the start and end point (exclusive) of each
 * cell, according to the cells flattened index. Also fills in the number of
 * cells in each direction into N_cells Also fills in the number of cells in
 * each direction into N_cells.
 */
void reorder_points(const std::vector<Point> &points, const float grid_size,
                    std::vector<Point> &reordered_points,
                    std::vector<CellRange> &cell_ranges, IntPoint &N_cells) {

  auto [min_point, max_point] = find_grid_extremes(points);
  N_cells = find_grid_sizes(min_point, max_point, grid_size);

  int n_tot_cells = N_cells.x * N_cells.y * N_cells.z;
  int n_points = points.size();

  std::vector<int> grid_indices(n_points, 0);
  std::vector<int> cell_counts(n_tot_cells, 0);

  cell_ranges.resize(n_tot_cells);
  get_cell_indices_and_counts(points, min_point, N_cells, grid_size,
                              grid_indices, cell_counts);

  std::vector<int> prefix_sums(n_tot_cells);
  get_prefix_sums(cell_counts, prefix_sums);

  std::vector<int> next_in_cell_indices = prefix_sums;

  for (int i = 0; i < n_points; i++) {
    // If no points have been assigned to the cell we are about to insert into,
    // this will give us the first index for the region of this cell. Afterwards
    // we increment the index. This means the next point that will be assigned
    // this cell gets placed in the second slot in the cells region, increment
    // the index etc. Because we already know the number of points in each cell
    // we wont place any point outside of its cells region.
    int target_index = grid_indices[i];
    reordered_points[next_in_cell_indices[target_index]++] = points[i];
  }

  cell_ranges[0] = {0, cell_counts[0]};
  for (int i = 1; i < n_tot_cells; i++) {
    cell_ranges[i] = {prefix_sums[i - 1], prefix_sums[i]};
  }
}
