#include "file_reader.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::vector<Point> points = readPoints(argv[1]);

    for (const auto& point : points) {
        std::cout << "(" << point.x << ", " << point.y << ", " << point.z << ")\n";
    }

    return 0;
}
