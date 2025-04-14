#include "vip.hpp"

#include <iostream>

using namespace std;

constexpr uint32_t VIP_MEMORY_MASK = 0x0007FFFF;

memory_block_t::memory_block_t(size_t size) : m_data(size) {}

void memory_block_t::write_h(uint32_t addr, uint16_t val) {
    char *byte_ptr = m_data.data() + addr;
    uint16_t *half_word_ptr = reinterpret_cast<uint16_t *>(byte_ptr);
    *half_word_ptr = val;
}

uint16_t memory_block_t::read_h(uint32_t addr) {
    const char *byte_ptr = m_data.data() + addr;
    const uint16_t *half_word_ptr =
        reinterpret_cast<const uint16_t *>(byte_ptr);
    return *half_word_ptr;
}

void display_control_register_t::write(uint16_t val) {
    m_LOCK = val & (1 << 10);
    m_SYNCE = val & (1 << 9);
    m_RE = val & (1 << 8);
    m_DISP = val & (1 << 1);

    bool DPRST = val & (1 << 0);
    if (DPRST) {
        m_vip.m_INTPND.dprst();
        m_vip.m_INTENB.dprst();
    }
}

void drawing_control_register_t::write(uint16_t val) {
    m_SBCMP = (val >> 8) & 0b11111;
    m_XPEN = val & (1 << 1);

    bool XPRST = val & (1 << 0);
    if (XPRST) {
        m_XPEN = false;
        m_vip.m_INTPND.xprst();
        m_vip.m_INTENB.xprst();
    }
}

constexpr size_t FRAME_BUFFER_SIZE = 0x6000;
frame_buffer_t::frame_buffer_t() : memory_block_t(FRAME_BUFFER_SIZE) {}

constexpr size_t CHARACTER_TABLE_SIZE = 0x2000;
character_table_t::character_table_t() : memory_block_t(CHARACTER_TABLE_SIZE) {}

constexpr size_t BACKGROUND_MAPS_SIZE = 0x1D800;
background_maps_t::background_maps_t() : memory_block_t(BACKGROUND_MAPS_SIZE) {}

constexpr size_t WORLD_ATTRIBUTES_SIZE = 0x400;
world_attributes_t::world_attributes_t()
    : memory_block_t(WORLD_ATTRIBUTES_SIZE) {}

constexpr size_t COLUMN_TABLE_SIZE = 0x200;
column_table_t::column_table_t() : memory_block_t(COLUMN_TABLE_SIZE) {}

constexpr size_t FRAME_BUFFER_LEFT_0_START = 0x00000000;
constexpr size_t CHARACTER_TABLE_0_START = 0x00006000;
constexpr size_t FRAME_BUFFER_LEFT_1_START = 0x00008000;
constexpr size_t CHARACTER_TABLE_1_START = 0x0000E000;
constexpr size_t FRAME_BUFFER_RIGHT_0_START = 0x00010000;
constexpr size_t CHARACTER_TABLE_2_START = 0x00016000;
constexpr size_t FRAME_BUFFER_RIGHT_1_START = 0x00018000;
constexpr size_t CHARACTER_TABLE_3_START = 0x0001E000;
constexpr size_t BACKGROUND_MAPS_START = 0x00020000;
constexpr size_t WORLD_ATTRIBUTES_START = 0x0003D800;
constexpr size_t COLUMN_TABLE_LEFT_START = 0x0003DC00;
constexpr size_t COLUMN_TABLE_RIGHT_START = 0x0003DE00;

constexpr size_t CHARACTER_TABLE_MIRROR_0_START = 0x00078000;
constexpr size_t CHARACTER_TABLE_MIRROR_1_START = 0x0007A000;
constexpr size_t CHARACTER_TABLE_MIRROR_2_START = 0x0007C000;
constexpr size_t CHARACTER_TABLE_MIRROR_3_START = 0x0007E000;

namespace io_registers {
constexpr size_t INTPND_ADDR = 0x0005F800;
constexpr size_t INTENB_ADDR = 0x0005F802;
constexpr size_t INTCLR_ADDR = 0x0005F804;
constexpr size_t DPSTTS_ADDR = 0x0005F820;
constexpr size_t DPCTRL_ADDR = 0x0005F822;
constexpr size_t BRTA_ADDR = 0x0005F824;
constexpr size_t BRTB_ADDR = 0x0005F826;
constexpr size_t BRTC_ADDR = 0x0005F828;
constexpr size_t REST_ADDR = 0x0005F82A;
constexpr size_t FRMCYC_ADDR = 0x0005F82E;
constexpr size_t CTA_ADDR = 0x0005F830;
constexpr size_t XPSTTS_ADDR = 0x0005F840;
constexpr size_t XPCTRL_ADDR = 0x0005F842;
constexpr size_t VER_ADDR = 0x0005F844;
constexpr size_t SPT0_ADDR = 0x0005F848;
constexpr size_t SPT1_ADDR = 0x0005F84A;
constexpr size_t SPT2_ADDR = 0x0005F84C;
constexpr size_t SPT3_ADDR = 0x0005F84E;
constexpr size_t GPLT0_ADDR = 0x0005F860;
constexpr size_t GPLT1_ADDR = 0x0005F862;
constexpr size_t GPLT2_ADDR = 0x0005F864;
constexpr size_t GPLT3_ADDR = 0x0005F866;
constexpr size_t JPLT0_ADDR = 0x0005F868;
constexpr size_t JPLT1_ADDR = 0x0005F86A;
constexpr size_t JPLT2_ADDR = 0x0005F86C;
constexpr size_t JPLT3_ADDR = 0x0005F86E;
constexpr size_t BKCOL_ADDR = 0x0005F870;
} // namespace io_registers

#define ADD_RW_BOUNDS(addr, base, start)                                       \
    (addr >= start && addr < (start + base##_SIZE))

#define ADD_WRITE_H(buf, base)                                                 \
    if (ADD_RW_BOUNDS(addr, base, base##_START)) {                             \
        buf.write_h(addr - base##_START, val);                                 \
        return;                                                                \
    }

#define ADD_WRITE_H_SUFFIX(buf, base, suffix)                                  \
    if (ADD_RW_BOUNDS(addr, base, base##_##suffix##_START)) {                  \
        buf.write_h(addr - base##_##suffix##_START, val);                      \
        return;                                                                \
    }

#define ADD_REGISTER_OP(regaddr, reg, op)                                      \
    if (addr == io_registers::regaddr##_ADDR) {                                \
        m_##reg.op(val);                                                       \
        return;                                                                \
    }

#define ADD_REGISTER_OP_UNSUPPORTED(reg, op)                                   \
    if (addr == io_registers::reg##_ADDR) {                                    \
        std::cout << #op " not supported for " #reg "\n";                      \
        assert(false);                                                         \
    }

void vip_t::write_h(uint32_t addr, uint16_t val) {
    addr &= VIP_MEMORY_MASK;

    ADD_WRITE_H_SUFFIX(m_frame_buffer_left_0, FRAME_BUFFER, LEFT_0);
    ADD_WRITE_H_SUFFIX(m_character_table_0, CHARACTER_TABLE, 0);
    ADD_WRITE_H_SUFFIX(m_frame_buffer_left_1, FRAME_BUFFER, LEFT_1);
    ADD_WRITE_H_SUFFIX(m_character_table_1, CHARACTER_TABLE, 1);
    ADD_WRITE_H_SUFFIX(m_frame_buffer_right_0, FRAME_BUFFER, RIGHT_0);
    ADD_WRITE_H_SUFFIX(m_character_table_2, CHARACTER_TABLE, 2);
    ADD_WRITE_H_SUFFIX(m_frame_buffer_right_1, FRAME_BUFFER, RIGHT_1);
    ADD_WRITE_H_SUFFIX(m_character_table_3, CHARACTER_TABLE, 3);
    ADD_WRITE_H(m_background_maps, BACKGROUND_MAPS);
    ADD_WRITE_H(m_world_attributes, WORLD_ATTRIBUTES);
    ADD_WRITE_H_SUFFIX(m_column_table_left, COLUMN_TABLE, LEFT);
    ADD_WRITE_H_SUFFIX(m_column_table_right, COLUMN_TABLE, RIGHT);

    ADD_WRITE_H_SUFFIX(m_character_table_0, CHARACTER_TABLE, MIRROR_0);
    ADD_WRITE_H_SUFFIX(m_character_table_1, CHARACTER_TABLE, MIRROR_1);
    ADD_WRITE_H_SUFFIX(m_character_table_2, CHARACTER_TABLE, MIRROR_2);
    ADD_WRITE_H_SUFFIX(m_character_table_3, CHARACTER_TABLE, MIRROR_3);

    ADD_REGISTER_OP_UNSUPPORTED(INTPND, write);
    ADD_REGISTER_OP(INTENB, INTENB, write);
    ADD_REGISTER_OP(INTCLR, INTENB, clear);
    ADD_REGISTER_OP_UNSUPPORTED(DPSTTS, write);
    ADD_REGISTER_OP(DPCTRL, DPSTTS, write);
    ADD_REGISTER_OP(BRTA, BRTA, write);
    ADD_REGISTER_OP(BRTB, BRTB, write);
    ADD_REGISTER_OP(BRTC, BRTC, write);
    ADD_REGISTER_OP(REST, REST, write);
    ADD_REGISTER_OP(FRMCYC, FRMCYC, write);
    ADD_REGISTER_OP_UNSUPPORTED(CTA, write);
    ADD_REGISTER_OP_UNSUPPORTED(XPSTTS, write);
    ADD_REGISTER_OP(XPCTRL, XPSTTS, write);
    ADD_REGISTER_OP_UNSUPPORTED(VER, write);
    ADD_REGISTER_OP(SPT0, SPT0, write);
    ADD_REGISTER_OP(SPT1, SPT1, write);
    ADD_REGISTER_OP(SPT2, SPT2, write);
    ADD_REGISTER_OP(SPT3, SPT3, write);
    ADD_REGISTER_OP(GPLT0, GPLT0, write);
    ADD_REGISTER_OP(GPLT1, GPLT1, write);
    ADD_REGISTER_OP(GPLT2, GPLT2, write);
    ADD_REGISTER_OP(GPLT3, GPLT3, write);
    ADD_REGISTER_OP(JPLT0, JPLT0, write);
    ADD_REGISTER_OP(JPLT1, JPLT1, write);
    ADD_REGISTER_OP(JPLT2, JPLT2, write);
    ADD_REGISTER_OP(JPLT3, JPLT3, write);
    ADD_REGISTER_OP(BKCOL, BKCOL, write);

    cout << std::hex << addr << ": " << val << endl;
    assert(false);
}

#define ADD_READ_H(buf, base)                                                  \
    if (ADD_RW_BOUNDS(addr, base, base##_START)) {                             \
        return buf.read_h(addr - base##_START);                                \
    }

#define ADD_READ_H_SUFFIX(buf, base, suffix)                                   \
    if (ADD_RW_BOUNDS(addr, base, base##_##suffix##_START)) {                  \
        return buf.read_h(addr - base##_##suffix##_START);                     \
    }

#define ADD_REGISTER_READ(reg)                                                 \
    if (addr == io_registers::reg##_ADDR) {                                    \
        return m_##reg.read();                                                 \
    }

uint16_t vip_t::read_h(uint32_t addr) {
    addr &= VIP_MEMORY_MASK;

    ADD_READ_H_SUFFIX(m_frame_buffer_left_0, FRAME_BUFFER, LEFT_0);
    ADD_READ_H_SUFFIX(m_character_table_0, CHARACTER_TABLE, 0);
    ADD_READ_H_SUFFIX(m_frame_buffer_left_1, FRAME_BUFFER, LEFT_1);
    ADD_READ_H_SUFFIX(m_character_table_1, CHARACTER_TABLE, 1);
    ADD_READ_H_SUFFIX(m_frame_buffer_right_0, FRAME_BUFFER, RIGHT_0);
    ADD_READ_H_SUFFIX(m_character_table_2, CHARACTER_TABLE, 2);
    ADD_READ_H_SUFFIX(m_frame_buffer_right_1, FRAME_BUFFER, RIGHT_1);
    ADD_READ_H_SUFFIX(m_character_table_3, CHARACTER_TABLE, 3);
    ADD_READ_H(m_background_maps, BACKGROUND_MAPS);
    ADD_READ_H(m_world_attributes, WORLD_ATTRIBUTES);
    ADD_READ_H_SUFFIX(m_column_table_left, COLUMN_TABLE, LEFT);
    ADD_READ_H_SUFFIX(m_column_table_right, COLUMN_TABLE, RIGHT);

    ADD_READ_H_SUFFIX(m_character_table_0, CHARACTER_TABLE, MIRROR_0);
    ADD_READ_H_SUFFIX(m_character_table_1, CHARACTER_TABLE, MIRROR_1);
    ADD_READ_H_SUFFIX(m_character_table_2, CHARACTER_TABLE, MIRROR_2);
    ADD_READ_H_SUFFIX(m_character_table_3, CHARACTER_TABLE, MIRROR_3);

    ADD_REGISTER_READ(INTPND);
    ADD_REGISTER_READ(INTENB);
    ADD_REGISTER_OP_UNSUPPORTED(INTCLR, read);
    ADD_REGISTER_READ(DPSTTS);
    ADD_REGISTER_OP_UNSUPPORTED(DPCTRL, read);
    ADD_REGISTER_READ(BRTA);
    ADD_REGISTER_READ(BRTB);
    ADD_REGISTER_READ(BRTC);
    ADD_REGISTER_READ(REST);
    ADD_REGISTER_READ(FRMCYC);
    ADD_REGISTER_READ(CTA);
    ADD_REGISTER_READ(XPSTTS);
    ADD_REGISTER_OP_UNSUPPORTED(XPCTRL, read);
    ADD_REGISTER_READ(VER);
    ADD_REGISTER_READ(SPT0);
    ADD_REGISTER_READ(SPT1);
    ADD_REGISTER_READ(SPT2);
    ADD_REGISTER_READ(SPT3);
    ADD_REGISTER_READ(GPLT0);
    ADD_REGISTER_READ(GPLT1);
    ADD_REGISTER_READ(GPLT2);
    ADD_REGISTER_READ(GPLT3);
    ADD_REGISTER_READ(JPLT0);
    ADD_REGISTER_READ(JPLT1);
    ADD_REGISTER_READ(JPLT2);
    ADD_REGISTER_READ(JPLT3);
    ADD_REGISTER_READ(BKCOL);

    cout << std::hex << addr << endl;
    assert(false);
}
