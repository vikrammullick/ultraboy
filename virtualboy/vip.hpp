#ifndef VIP_H
#define VIP_H

#include <vector>

consteval uint16_t bitmask(uint8_t len) {
    assert(len <= 15);
    return (1 << len) - 1;
}

class memory_block_t {
    std::vector<char> m_data;

  public:
    memory_block_t(size_t size);

    void write_b(uint32_t addr, uint8_t val);
    void write_h(uint32_t addr, uint16_t val);
    uint16_t read_h(uint32_t addr);
};

class frame_buffer_t : public memory_block_t {
  public:
    frame_buffer_t();
};

class character_table_t : public memory_block_t {
  public:
    character_table_t();
};

class background_maps_t : public memory_block_t {
  public:
    background_maps_t();
};

class world_attributes_t : public memory_block_t {
  public:
    world_attributes_t();
};

class column_table_t : public memory_block_t {
  public:
    column_table_t();
};

class object_attributes_t : public memory_block_t {
  public:
    object_attributes_t();
};

struct interrupt_register_t {
    bool m_TIMEERR = false;
    bool m_XPEND = false;
    bool m_SBHIT = false;
    bool m_FRAMESTART = false;
    bool m_GAMESTART = false;
    bool m_RFBEND = false;
    bool m_LFBEND = false;
    bool m_SCANERR = false;

    uint16_t read() {
        return (m_TIMEERR << 15) | (m_XPEND << 14) | (m_SBHIT << 13) |
               (m_FRAMESTART << 4) | (m_GAMESTART << 3) | (m_RFBEND << 2) |
               (m_LFBEND << 1) | (m_SCANERR << 0);
    }

    void write(uint16_t val) {
        m_TIMEERR = val & (1 << 15);
        m_XPEND = val & (1 << 14);
        m_SBHIT = val & (1 << 13);
        m_FRAMESTART = val & (1 << 4);
        m_GAMESTART = val & (1 << 3);
        m_RFBEND = val & (1 << 2);
        m_LFBEND = val & (1 << 1);
        m_SCANERR = val & (1 << 0);
    }

    void clear(uint16_t val) {
        m_TIMEERR = (val & (1 << 15)) ? false : m_TIMEERR;
        m_XPEND = (val & (1 << 14)) ? false : m_XPEND;
        m_SBHIT = (val & (1 << 13)) ? false : m_SBHIT;
        m_FRAMESTART = (val & (1 << 4)) ? false : m_FRAMESTART;
        m_GAMESTART = (val & (1 << 3)) ? false : m_GAMESTART;
        m_RFBEND = (val & (1 << 2)) ? false : m_RFBEND;
        m_LFBEND = (val & (1 << 1)) ? false : m_LFBEND;
        m_SCANERR = (val & (1 << 0)) ? false : m_SCANERR;
    }

    void dprst() {
        m_TIMEERR = false;
        m_FRAMESTART = false;
        m_GAMESTART = false;
        m_RFBEND = false;
        m_LFBEND = false;
        m_SCANERR = false;
    }

    void xprst() {
        m_TIMEERR = false;
        m_XPEND = false;
        m_SBHIT = false;
    }
};

struct vip_t;

struct display_control_register_t {
    vip_t &m_vip;
    bool m_LOCK;
    bool m_SYNCE = false;
    bool m_RE = false;
    bool m_FCLK;
    bool m_SCANRDY;
    bool m_R1BSY;
    bool m_L1BSY;
    bool m_R0BSY;
    bool m_L0BSY;
    bool m_DISP = false;

    display_control_register_t(vip_t &vip) : m_vip(vip) {}

    uint16_t read() {
        return (m_LOCK << 10) | (m_SYNCE << 9) | (m_RE << 8) | (m_FCLK << 7) |
               (m_SCANRDY << 6) | (m_R1BSY << 5) | (m_L1BSY << 4) |
               (m_R0BSY << 3) | (m_L0BSY << 2) | (m_DISP << 1);
    }

    void write(uint16_t val);
};

struct brightness_control_register_t {
    uint8_t m_duration;

    uint16_t read() { return m_duration; }

    void write(uint16_t val) { m_duration = val & 0xFF; }
};

struct rest_control_register_t {
    uint8_t m_duration;

    uint16_t read() { return m_duration; }

    void write(uint16_t val) { m_duration = val & 0xFF; }
};

struct game_frame_control_register_t {
    uint8_t m_FRMCYC;

    uint16_t read() { return m_FRMCYC & bitmask(4); }

    void write(uint16_t val) { m_FRMCYC = val & bitmask(4); }
};

struct column_table_address_register_t {
    uint8_t m_CTA_L;
    uint8_t m_CTA_R;

    uint16_t read() { return (m_CTA_R << 8) + m_CTA_L; }
};

struct drawing_control_register_t {
    vip_t &m_vip;
    bool m_SBOUT;
    uint8_t m_SBCOUNT;
    uint8_t m_SBCMP;
    bool m_OVERTIME;
    bool m_F1BSY;
    bool m_F0BSY;
    bool m_XPEN = false;

    drawing_control_register_t(vip_t &vip) : m_vip(vip) {}

    uint16_t read() {
        return (m_SBOUT << 15) | ((m_SBCOUNT & bitmask(5)) << 8) |
               (m_OVERTIME << 4) | (m_F1BSY << 3) | (m_F0BSY << 2) |
               (m_XPEN << 1);
    }

    void write(uint16_t val);
};

struct version_register_t {
    static constexpr uint16_t VER = 2;

    uint16_t read() { return VER; }
};

struct obj_control_register_t {
    uint16_t m_OBJ_end_number;

    uint16_t read() { return m_OBJ_end_number & bitmask(10); }

    void write(uint16_t val) { m_OBJ_end_number = val & bitmask(10); }
};

struct palette_control_register_t {
    uint8_t m_c1;
    uint8_t m_c2;
    uint8_t m_c3;

    uint16_t read() { return (m_c3 << 6) | (m_c2 << 4) | (m_c1 << 2); }

    void write(uint16_t val) {
        m_c1 = (val >> 2) & bitmask(2);
        m_c2 = (val >> 4) & bitmask(2);
        m_c3 = (val >> 6) & bitmask(2);
    }
};

struct bg_palette_control_register_t {
    uint8_t m_value;

    uint16_t read() { return m_value; }

    void write(uint16_t val) { m_value = val & bitmask(2); }
};

struct vip_t {
    frame_buffer_t m_frame_buffer_left_0;
    character_table_t m_character_table_0;
    frame_buffer_t m_frame_buffer_left_1;
    character_table_t m_character_table_1;
    frame_buffer_t m_frame_buffer_right_0;
    character_table_t m_character_table_2;
    frame_buffer_t m_frame_buffer_right_1;
    character_table_t m_character_table_3;
    background_maps_t m_background_maps;
    world_attributes_t m_world_attributes;
    column_table_t m_column_table_left;
    column_table_t m_column_table_right;
    object_attributes_t m_object_attributes;

    interrupt_register_t m_INTPND;
    interrupt_register_t m_INTENB;
    display_control_register_t m_DPSTTS;
    brightness_control_register_t m_BRTA;
    brightness_control_register_t m_BRTB;
    brightness_control_register_t m_BRTC;
    rest_control_register_t m_REST;
    game_frame_control_register_t m_FRMCYC;
    column_table_address_register_t m_CTA;
    drawing_control_register_t m_XPSTTS;
    version_register_t m_VER;
    obj_control_register_t m_SPT0;
    obj_control_register_t m_SPT1;
    obj_control_register_t m_SPT2;
    obj_control_register_t m_SPT3;
    palette_control_register_t m_GPLT0;
    palette_control_register_t m_GPLT1;
    palette_control_register_t m_GPLT2;
    palette_control_register_t m_GPLT3;
    palette_control_register_t m_JPLT0;
    palette_control_register_t m_JPLT1;
    palette_control_register_t m_JPLT2;
    palette_control_register_t m_JPLT3;
    bg_palette_control_register_t m_BKCOL;

    vip_t() : m_DPSTTS(*this), m_XPSTTS(*this) {}

    void tick();

    void write_b(uint32_t addr, uint8_t val);
    void write_h(uint32_t addr, uint16_t val);
    uint16_t read_h(uint32_t addr);
};

#endif // VIP_H
