#ifndef REORDER_POINTS_H
#define REORDER_POINTS_H

#include "cell_range.h"
#include "point.h"
#include <vector>

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
                    std::vector<CellRange> &cell_ranges, IntPoint &N_cells);
#endif
