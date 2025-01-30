CXX = g++
CXXFLAGS = -O3 -march=znver4 -flto -fno-plt -fno-rtti -ffast-math -Wall -Wextra -std=c++17 -I./include
SRC = src/main.cpp src/point.cpp src/file_reader.cpp src/reorder_points.cpp src/count_pairs.cpp
OBJ = $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(SRC))
TARGET = count_pairs
BUILD_DIR = build

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

$(BUILD_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
