#include "cpu.hpp"

#include <iostream>
#include <sstream>

using namespace std;

cpu_t::cpu_t(memory_t &memory) : m_memory(memory) {
    PC() = constants::PC_START;
}

void cpu_t::set_inst_type() {
    if (prefixed) {
        if (opx() >= 0x4 && opx() <= 0x7 && (opy() & 0b111) != 0x6) {
            m_inst_type = inst_type_t::BIT;
            return;
        }
        if (opx() == 0x1 && opy() < 0x8 && opy() != 0x6) {
            m_inst_type = inst_type_t::RL;
            return;
        }
    } else {
        if (opx() == 0x0 && opy() == 0x0) {
            m_inst_type = inst_type_t::NOP;
            return;
        }
        if (((opx() == 0x2 || opx() == 0x3) &&
             (opy() == 0x0 || opy() == 0x8)) ||
            m_opcode == 0x18) {
            m_inst_type = inst_type_t::JR;
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && (opy() == 0x6 || opy() == 0xE) &&
            m_opcode != 0x36) {
            m_inst_type = inst_type_t::LD8;
            return;
        }
        if (opx() >= 0x4 && opx() <= 0x7 && (opx() != 0x7 || opy() >= 0x8) &&
            (opy() & 0b111) != 0x6) {
            m_inst_type = inst_type_t::LD_REG_REG;
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && (opy() == 0x5 || opy() == 0xD) &&
            m_opcode != 0x35) {
            m_inst_type = inst_type_t::DEC;
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && (opy() == 0x4 || opy() == 0xC) &&
            m_opcode != 0x34) {
            m_inst_type = inst_type_t::INC;
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && opy() == 0x3) {
            m_inst_type = inst_type_t::INC16;
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && opy() == 0xB) {
            m_inst_type = inst_type_t::DEC16;
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && opy() == 0x1) {
            m_inst_type = inst_type_t::LD16;
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && opy() == 0x2) {
            m_inst_type = inst_type_t::LDMEM16;
            return;
        }
        if (opx() >= 0xC && opx() <= 0xF && opy() == 0x1) {
            m_inst_type = inst_type_t::POP;
            return;
        }
        if (m_opcode == 0xC9) {
            m_inst_type = inst_type_t::RET;
            return;
        }
        if (opx() >= 0xC && opx() <= 0xF && opy() == 0x5) {
            m_inst_type = inst_type_t::PUSH;
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && opy() == 0xA) {
            m_inst_type = inst_type_t::LDFROMMEM;
            return;
        }
        if (m_opcode == 0xE2) {
            m_inst_type = inst_type_t::LD_INTO_C_OFFSET;
            return;
        }
        if (m_opcode == 0xF2) {
            m_inst_type = inst_type_t::LD_FROM_C_OFFSET;
            return;
        }
        if (m_opcode == 0xE0) {
            m_inst_type = inst_type_t::LD_INTO_IMM_OFFSET;
            return;
        }
        if (m_opcode == 0xF0) {
            m_inst_type = inst_type_t::LD_FROM_IMM_OFFSET;
            return;
        }
        if (m_opcode == 0xEA) {
            m_inst_type = inst_type_t::LD_INTO_IMM;
            return;
        }
        if (m_opcode == 0xFA) {
            m_inst_type = inst_type_t::LD_FROM_IMM;
            return;
        }
        /*if (m_opcode == 0x07) {
            m_inst_type = inst_type_t::RLCA;
            return;
        }*/
        if (m_opcode == 0x17) {
            m_inst_type = inst_type_t::RLA;
            return;
        }
        /* if (m_opcode == 0x0F) {
            m_inst_type = inst_type_t::RRCA;
            return;
        }
        if (m_opcode == 0x1F) {
            m_inst_type = inst_type_t::RRCA;
            return;
        }
        */
        if (opx() == 0x7 && opy() && opy() != 0x6) {
            m_inst_type = inst_type_t::LD_HL;
            return;
        }
        if (opx() == 0xA && opy() >= 0x8 && opy() != 0xE) {
            m_inst_type = inst_type_t::XOR;
            return;
        }
        if (m_opcode == 0xCD) {
            m_inst_type = inst_type_t::CALL;
            return;
        }
        if (m_opcode == 0xFE) {
            m_inst_type = inst_type_t::CP_IMM;
            return;
        }
    }

    stringstream oss;
    oss << hex << "pc: 0x" << m_fetch_pc << " unknown opcode: ";
    if (prefixed) {
        oss << "0xcb ";
    }
    oss << "0x" << static_cast<uint16_t>(m_opcode);
    cout << oss.str() << endl;
    // throw std::runtime_error(oss.str());
    m_failed = true;
}

void cpu_t::fetch() {
    m_fetch_pc = PC();

    m_opcode = m_memory.read(PC()++);
    prefixed = m_opcode == constants::PREFIX;
    if (prefixed) {
        m_opcode = m_memory.read(PC()++);
    }
    set_inst_type();
}

void cpu_t::decode() {
    switch (m_inst_type) {
    case inst_type_t::NOP:
    case inst_type_t::LD_REG_REG:
    case inst_type_t::LDMEM16:
    case inst_type_t::LDFROMMEM:
    case inst_type_t::LD_INTO_C_OFFSET:
    case inst_type_t::LD_FROM_C_OFFSET:
    // case inst_type_t::RLCA:
    case inst_type_t::RLA:
    // case inst_type_t::RRCA:
    // case inst_type_t::RRA:
    case inst_type_t::LD_HL:
    case inst_type_t::XOR:
    case inst_type_t::RL:
    case inst_type_t::BIT:
    case inst_type_t::INC:
    case inst_type_t::DEC:
    case inst_type_t::INC16:
    case inst_type_t::DEC16:
    case inst_type_t::PUSH:
    case inst_type_t::POP:
    case inst_type_t::RET:
        break;
    case inst_type_t::LD8:
    case inst_type_t::JR:
    case inst_type_t::LD_INTO_IMM_OFFSET:
    case inst_type_t::LD_FROM_IMM_OFFSET:
    case inst_type_t::CP_IMM:
        m_lo = m_memory.read(PC()++);
        break;
    case inst_type_t::LD16:
    case inst_type_t::CALL:
    case inst_type_t::LD_INTO_IMM:
    case inst_type_t::LD_FROM_IMM:
        m_lo = m_memory.read(PC()++);
        m_hi = m_memory.read(PC()++);
        break;
    default:
        throw std::runtime_error("unknown instruction type (decode)");
    }
}

uint8_t &split_reg(uint16_t &reg, bool upper) {
    uint8_t *bytes = (uint8_t *)&reg;
    return bytes[upper ? 1 : 0];
}

void cpu_t::execute() {
    bool c_flag;

    switch (m_inst_type) {
    case inst_type_t::NOP:
        break;
    case inst_type_t::JR:
        if (!flag_cond()) {
            return;
        }
        PC() += (int8_t)(m_lo);
        break;
    case inst_type_t::LD_REG_REG:
        r8x() = r8y();
        break;
    case inst_type_t::LDMEM16:
        if (opx() == 0x2) {
            m_memory.write(HL()++, A());
        } else if (opx() == 0x3) {
            m_memory.write(HL()--, A());
        } else {
            m_memory.write(r16x(), A());
        }
        break;
    case inst_type_t::LDFROMMEM:
        if (opx() == 0x2) {
            A() = m_memory.read(HL()++);
        } else if (opx() == 0x3) {
            A() = m_memory.read(HL()--);
        } else {
            A() = m_memory.read(r16x());
        }
        break;
    case inst_type_t::LD_INTO_IMM_OFFSET:
        m_memory.write(0xFF00 + m_lo, A());
        break;
    case inst_type_t::LD_FROM_IMM_OFFSET:
        A() = m_memory.read(0xFF00 + m_lo);
        break;
    case inst_type_t::LD_INTO_IMM:
        m_memory.write(n16(), A());
        break;
    case inst_type_t::LD_FROM_IMM:
        A() = m_memory.read(n16());
        break;
    case inst_type_t::LD_INTO_C_OFFSET:
        m_memory.write(0xFF00 + C(), A());
        break;
    case inst_type_t::LD_FROM_C_OFFSET:
        A() = m_memory.read(0xFF00 + C());
        break;
    /*case inst_type_t::RLCA:
        break;
        */
    case inst_type_t::RLA:
        c_flag = c();
        setC(A() & (1 << 7));
        A() <<= 1;
        A() |= (c_flag ? 1 : 0);
        setZ(0);
        setN(0);
        setH(0);
        break;
    /*case inst_type_t::RRCA:
        break;
    case inst_type_t::RRA:
        break;
    */
    case inst_type_t::LD_HL:
        m_memory.write(HL(), r8y());
        break;
    case inst_type_t::LD8:
        r8x() = m_lo;
        break;
    case inst_type_t::INC:
        r8x()++;
        setZ(!r8x());
        setN(0);
        setH(!(r8x() & 0xF));
        break;
    case inst_type_t::DEC:
        r8x()--;
        setZ(!r8x());
        setN(1);
        setH(r8x() & 0xF);
        break;
    case inst_type_t::INC16:
        r16x()++;
        break;
    case inst_type_t::DEC16:
        r16x()--;
        break;
    case inst_type_t::LD16:
        r16x() = n16();
        break;
    case inst_type_t::PUSH:
        m_memory.write(--SP(), split_reg(r16x(), true));
        m_memory.write(--SP(), split_reg(r16x(), false));
        break;
    case inst_type_t::POP:
        split_reg(r16x(), false) = m_memory.read(SP()++);
        split_reg(r16x(), true) = m_memory.read(SP()++);
        break;
    case inst_type_t::RET:
        split_reg(PC(), false) = m_memory.read(SP()++);
        split_reg(PC(), true) = m_memory.read(SP()++);
        break;
    case inst_type_t::XOR:
        A() ^= r8y();
        setZ(A() == 0);
        setN(0);
        setH(0);
        setC(0);
        break;
    case inst_type_t::RL:
        c_flag = c();
        setC(r8y() & (1 << 7));
        r8y() <<= 1;
        r8y() |= (c_flag ? 1 : 0);
        setZ(r8y() == 0);
        setN(0);
        setH(0);
        break;
    case inst_type_t::BIT:
        setZ(!(r8y() & (1 << ((opx() - 0x4) * 2 + opy() / 8))));
        setH(0);
        setC(1);
        break;
    case inst_type_t::CALL:
        m_memory.write(--SP(), split_reg(PC(), true));
        m_memory.write(--SP(), split_reg(PC(), false));
        PC() = n16();
        break;
    case inst_type_t::CP_IMM:
        setH((m_lo & 0xF) > (A() & 0xF));
        setC(m_lo > A());
        A() -= m_lo;
        setZ(A() == 0);
        setN(1);
        break;
    default:
        throw std::runtime_error("unknown instruction type (execute)");
    }
}

void cpu_t::tick() {
    if (m_failed) {
        return;
    }
    fetch();
    decode();
    execute();
}
