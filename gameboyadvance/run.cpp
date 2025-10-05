#include <fstream>
#include <iostream>

#include "emulator.hpp"

using namespace std;

bool read_bin(const char *path, vector<char> &bytes) {
    ifstream rom_ifstream(path, ios::binary);
    if (!rom_ifstream) {
        cerr << "file not found" << endl;
        return false;
    }

    bytes = vector<char>(istreambuf_iterator<char>(rom_ifstream),
                         istreambuf_iterator<char>());
    rom_ifstream.close();
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "unexpected cli args" << endl;
        return 1;
    }

    vector<char> bios_bytes;
    if (!read_bin("bios/gba_bios.bin", bios_bytes)) {
        return 1;
    }

    vector<char> rom_bytes;
    if (!read_bin(argv[1], rom_bytes)) {
        return 1;
    }

    emulator_t emulator(bios_bytes, rom_bytes);
    emulator.run();
}
