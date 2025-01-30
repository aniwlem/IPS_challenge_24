#include "../include/cell_range.h"
#include "../include/point.h"
#include <numeric>
#include <thread>
#include <vector>

void count_pairs_cell(const std::vector<Point> &reordered_points,
                      const std::vector<CellRange> &cell_ranges,
                      const IntPoint N_cells, const int cell_idx,
                      const float max_dist_sq, int &count) {

  // Count within cell first, must be handled separetely
  for (int i = cell_ranges[cell_idx].start; i < cell_ranges[cell_idx].end;
       i++) {
    for (int j = i + 1; j < cell_ranges[cell_idx].end; j++) {
      float dist_sq =
          get_distance_squared(reordered_points[i], reordered_points[j]);
      if (dist_sq < max_dist_sq) {
        count++;
      }
    }
  }

  // The offsets needed for each neighboring cell, no double counting.
  const std::vector<IntPoint> neighbor_offsets{
      // Faces
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1},
      // Edges
      {1, 1, 0},
      {1, 0, 1},
      {0, 1, 1},
      {1, -1, 0},
      {1, 0, -1},
      {0, 1, -1},
      // corners
      {1, 1, 1},
      {1, -1, 1},
      {1, 1, -1},
      {1, -1, -1},
  };

  int ix = cell_idx % N_cells.x;
  int iy = (cell_idx / N_cells.x) % N_cells.y;
  int iz = cell_idx / (N_cells.x * N_cells.y);

  for (int neigh_idx = 0; neigh_idx < static_cast<int>(neighbor_offsets.size());
       neigh_idx++) {

    int neigh_ix = ix + neighbor_offsets[neigh_idx].x;
    int neigh_iy = iy + neighbor_offsets[neigh_idx].y;
    int neigh_iz = iz + neighbor_offsets[neigh_idx].z;

    if ((neigh_ix >= 0 && neigh_ix < N_cells.x) &&
        (neigh_iy >= 0 && neigh_iy < N_cells.y) &&
        (neigh_iz >= 0 && neigh_iz < N_cells.z)) {
      int neigh_cell_idx =
          neigh_ix + N_cells.x * (neigh_iy + N_cells.y * neigh_iz);

      CellRange curr_neigh_range = cell_ranges[neigh_cell_idx];

      for (int i = cell_ranges[cell_idx].start; i < cell_ranges[cell_idx].end;
           i++) {
        for (int j = curr_neigh_range.start; j < curr_neigh_range.end; j++) {
          float dist_sq =
              get_distance_squared(reordered_points[i], reordered_points[j]);
          if (dist_sq < max_dist_sq) {
            count++;
          }
        }
      }
    }
  }
}

void count_pairs_block(const std::vector<Point> &reordered_points,
                       const std::vector<CellRange> &cell_ranges,
                       const IntPoint N_cells, const int cell_idx_start,
                       const int cell_idx_end, const float max_dist_sq,
                       int &count) {
  for (int i = cell_idx_start; i < cell_idx_end; i++) {
    count_pairs_cell(reordered_points, cell_ranges, N_cells, i, max_dist_sq,
                     count);
  }
}

int count_pairs(std::vector<Point> &reordered_points,
                std::vector<CellRange> &cell_ranges, IntPoint N_cells,
                const float max_dist_sq) {

  const std::size_t n_threads = std::thread::hardware_concurrency();

  std::vector<int> per_thread_counts(n_threads, 0);
  std::vector<std::thread> threads;

  size_t cells_per_thread = cell_ranges.size() / n_threads;

  for (size_t th = 0; th < n_threads; th++) {
    int start_cell_index = th * cells_per_thread;
    int end_cell_index = start_cell_index + cells_per_thread;

    // Take care of possibly left over cells
    if (th == n_threads - 1) {
      end_cell_index = cell_ranges.size();
    }

    threads.emplace_back(count_pairs_block, std::cref(reordered_points),
                         std::cref(cell_ranges), std::cref(N_cells),
                         start_cell_index, end_cell_index, max_dist_sq,
                         std::ref(per_thread_counts[th]));
  }

  for (auto &thread : threads) {
    thread.join();
  }

  int intercell_count =
      std::accumulate(per_thread_counts.begin(), per_thread_counts.end(), 0);

  return intercell_count;
}
