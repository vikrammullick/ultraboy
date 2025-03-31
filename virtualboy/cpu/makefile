CXX := g++

CXXFLAGS := -std=c++20 -Wall -O3 -Wswitch-enum

SRCS := cpu_tb.cpp memory_bus.cpp cpu.cpp 

OBJS := $(SRCS:.cpp=.o)

$HEADERS := memory_bus.hpp cpu.hpp 

TARGET := cpu_tb

all: $(TARGET)

run: all
	./cpu_tb

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lSDL2

%.o: %.cpp $(HEADERS) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)


