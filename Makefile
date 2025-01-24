CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
SRC = main.cpp file_reader.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = closeCounter

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
