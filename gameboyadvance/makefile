CXX := g++

CXXFLAGS := -std=c++20 -Wall -O3 -Wswitch-enum

SRCS := run.cpp emulator.cpp


OBJS := $(SRCS:.cpp=.o)

$HEADERS := emulator.hpp

TARGET := ultraboy

all: $(TARGET)

run: all
	./ultraboy roms/wario.gba

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)


