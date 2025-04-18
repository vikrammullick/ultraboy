#include "game_pad.hpp"
#include "memory_bus.hpp"

#include <iostream>

using namespace std;

void game_pad_t::write_misc_b(uint32_t addr, uint8_t val) {
    if (addr == misc_hardware_registers::SCR_ADDR) {
        // TODO: implement serial-level accuracy
        return;
    }

    cout << std::hex << addr << ": " << static_cast<uint16_t>(val) << endl;
    assert(false);
}

uint8_t game_pad_t::read_misc_b(uint32_t addr) {
    if (addr == misc_hardware_registers::SCR_ADDR) {
        // TODO: implement serial-level accuracy
        return 0;
    }

    cout << std::hex << addr << endl;
    assert(false);
}
