#ifndef COUNT_PAIRS_H
#define COUNT_PAIRS_H

#include "cell_range.h"
#include "point.h"
#include <vector>

int count_pairs(std::vector<Point> &reordered_points,
                        std::vector<CellRange> &cell_ranges, IntPoint N_cells,
                        const float max_dist_sq);

#endif
