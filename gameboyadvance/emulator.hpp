#ifndef EMULATOR_H
#define EMULATOR_H

#include <vector>

class emulator_t {
   public:
    emulator_t(const std::vector<char> &bios_bytes,
               const std::vector<char> &rom_bytes);

    void run();
};

#endif  // EMULATOR_H
