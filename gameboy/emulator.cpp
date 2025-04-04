#include "emulator.hpp"
#include "sdl_helpers.hpp"

using namespace std;

emulator_t::emulator_t(const vector<char> &boot_bytes,
                       const vector<char> &rom_bytes)
    : m_memory(boot_bytes, rom_bytes, m_ppu), m_cpu(m_memory) {
    sdl_init_windows();
}

emulator_t::~emulator_t() { sdl_destroy_windows(); }

uint64_t now() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

void emulator_t::run() {
    uint64_t last_ts = now();
    while (g_running) {
        uint64_t current_ts = now();
        if ((current_ts - last_ts) < constants::NANOS_PER_TICK) {
            continue;
        }
        last_ts = current_ts;

        m_cpu.tick();
        m_ppu.tick();

        if (m_ppu.check_vblank_interrupt()) {
            m_cpu.request_vblank_interrupt();
        }

        sdl_poll_keyboard();
    }
}
