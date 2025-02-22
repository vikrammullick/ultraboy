CXX := g++

CXXFLAGS := -std=c++17 -Wall -O3 -Wswitch-enum

SRCS := run.cpp emulator.cpp cpu.cpp memory.cpp ppu.cpp sdl_helpers.cpp 

OBJS := $(SRCS:.cpp=.o)

$HEADERS := emulator.hpp cpu.hpp memory.hpp constants.hpp ppu.hpp sdl_helpers.hpp

TARGET := ultraboy

all: $(TARGET)

run: all
	./ultraboy roms/tetris.gb

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lSDL2

%.o: %.cpp $(HEADERS) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)


