CXX := g++

CXXFLAGS := -std=c++20 -Wall -O3 -Wswitch-enum

SRCS := run.cpp emulator.cpp cpu.cpp memory_bus.cpp vip.cpp wram.cpp game_pak.cpp link.cpp

OBJS := $(SRCS:.cpp=.o)

$HEADERS := emulator.hpp cpu.hpp memory_bus.hpp vip.hpp wram.hpp game_pak.hpp link.hpp

TARGET := ultraboy

all: $(TARGET)

run: all
	./ultraboy roms/tennis.vb

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lSDL2

%.o: %.cpp $(HEADERS) 
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) $(OBJS)


