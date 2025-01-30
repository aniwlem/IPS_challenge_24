#ifndef CELL_RANGE_H
#define CELL_RANGE_H
// The data has seven significant digits, which is the expected accuracy for
// 4-byte floats, so using anything larger would be waste of memory

struct CellRange {
    int start {};
    int end {};
};


#endif
