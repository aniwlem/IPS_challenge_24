#include "file_reader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<Point> readPoints(const std::string& filePath) {
    std::vector<Point> points;
    std::ifstream file(filePath);

    if (!file) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return points;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Point point;
        if (iss >> point.x >> point.y >> point.z) {
            points.push_back(point);
        } else {
            std::cerr << "Warning: Invalid line format - " << line << std::endl;
        }
    }

    return points;
}

