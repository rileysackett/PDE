CXX = g++
CXXFLAGS = -O3 -Wall -std=c++17
TARGET = cn1d
SRCS = cn1d.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -lm -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) cn_solution.csv cn_errors.csv cn_solution_N*.csv

run: $(TARGET)
	./$(TARGET) 100 0.1
