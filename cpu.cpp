#include "cpu.hpp"

#include <iostream>
#include <sstream>

using namespace std;

cpu_t::cpu_t(memory_t &memory) : m_memory(memory) {
    PC() = constants::PC_START;
}

uint8_t cpu_t::read(uint16_t addr) {
    if (addr == constants::IF) {
        return m_IF;
    }

    if (addr == constants::IE) {
        return m_IE;
    }

    if (addr == constants::TMA) {
        return m_TMA;
    }

    if (addr == constants::TAC) {
        return m_TAC;
    }

    return m_memory.read(addr);
}

void cpu_t::write(uint16_t addr, uint8_t val) {
    if (addr == constants::IF) {
        m_IF = val;
        return;
    }

    if (addr == constants::IE) {
        m_IE = val;
        return;
    }

    if (addr == constants::TMA) {
        m_TMA = val;
        return;
    }

    if (addr == constants::TAC) {
        m_TAC = val;
        return;
    }

    m_memory.write(addr, val);
}

void cpu_t::set_inst_type() {
    if (prefixed) {
        if (opx() >= 0x4 && opx() <= 0x7) {
            if ((opy() & 0b111) == 0x6) {
                m_inst_type = inst_type_t::BIT_HL;
            } else {
                m_inst_type = inst_type_t::BIT;
            }
            return;
        }
        if (opx() >= 0x8 && opx() <= 0xB) {
            if ((opy() & 0b111) == 0x6) {
                m_inst_type = inst_type_t::RES_HL;
            } else {
                m_inst_type = inst_type_t::RES;
            }
            return;
        }
        if (opx() >= 0xC && opx() <= 0xF) {
            if ((opy() & 0b111) == 0x6) {
                m_inst_type = inst_type_t::SET_HL;
            } else {
                m_inst_type = inst_type_t::SET;
            }
            return;
        }
        if (opx() == 0x0 && opy() < 0x8) {
            if (opy() == 0x6) {
                m_inst_type = inst_type_t::RLC_HL;
            } else {
                m_inst_type = inst_type_t::RLC;
            }
            return;
        }
        if (opx() == 0x0 && opy() >= 0x8) {
            if (opy() == 0xE) {
                m_inst_type = inst_type_t::RRC_HL;
            } else {
                m_inst_type = inst_type_t::RRC;
            }
            return;
        }
        if (opx() == 0x1 && opy() < 0x8) {
            if (opy() == 0x6) {
                m_inst_type = inst_type_t::RL_HL;
            } else {
                m_inst_type = inst_type_t::RL;
            }
            return;
        }
        if (opx() == 0x1 && opy() >= 0x8) {
            if (opy() == 0xE) {
                m_inst_type = inst_type_t::RR_HL;
            } else {
                m_inst_type = inst_type_t::RR;
            }
            return;
        }
        if (opx() == 0x2 && opy() < 0x8) {
            if (opy() == 0x6) {
                m_inst_type = inst_type_t::SLA_HL;
            } else {
                m_inst_type = inst_type_t::SLA;
            }
            return;
        }
        if (opx() == 0x2 && opy() >= 0x8) {
            if (opy() == 0xE) {
                m_inst_type = inst_type_t::SRA_HL;
            } else {
                m_inst_type = inst_type_t::SRA;
            }
            return;
        }
        if (opx() == 0x3 && opy() < 0x8) {
            if (opy() == 0x6) {
                m_inst_type = inst_type_t::SWAP_HL;
            } else {
                m_inst_type = inst_type_t::SWAP;
            }
            return;
        }
        if (opx() == 0x3 && opy() >= 0x8) {
            if (opy() == 0xE) {
                m_inst_type = inst_type_t::SRL_HL;
            } else {
                m_inst_type = inst_type_t::SRL;
            }
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
        if (((opx() == 0xC || opx() == 0xD) &&
             (opy() == 0x2 || opy() == 0xA)) ||
            m_opcode == 0xC3) {
            m_inst_type = inst_type_t::JP;
            return;
        }
        if (m_opcode == 0x08) {
            m_inst_type = inst_type_t::LD_SP_MEM;
            return;
        }
        if (m_opcode == 0xE8) {
            m_inst_type = inst_type_t::ADD_SP_IMM;
            return;
        }
        if (m_opcode == 0xE9) {
            m_inst_type = inst_type_t::JP_HL;
            return;
        }
        if (m_opcode == 0xF8) {
            m_inst_type = inst_type_t::LD_SP_SUM_IMM_HL;
            return;
        }
        if (m_opcode == 0xF9) {
            m_inst_type = inst_type_t::LD_HL_SP;
            return;
        }
        if (m_opcode == 0x27) {
            m_inst_type = inst_type_t::DAA;
            return;
        }
        if (m_opcode == 0x37) {
            m_inst_type = inst_type_t::SCF;
            return;
        }
        if (m_opcode == 0x3F) {
            m_inst_type = inst_type_t::CCF;
            return;
        }
        if (m_opcode == 0x10) {
            m_inst_type = inst_type_t::STOP;
            return;
        }
        if (m_opcode == 0x76) {
            m_inst_type = inst_type_t::HALT;
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && (opy() == 0x6 || opy() == 0xE)) {
            if (m_opcode == 0x36) {
                m_inst_type = inst_type_t::LD8_HL;
            } else {
                m_inst_type = inst_type_t::LD8;
            }
            return;
        }
        if (opx() >= 0x4 && opx() <= 0x7 && (opx() != 0x7 || opy() >= 0x8)) {
            if ((opy() & 0b111) != 0x6) {
                m_inst_type = inst_type_t::LD_REG_REG;
            } else {
                m_inst_type = inst_type_t::LD_REG_FROM_HL;
            }
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && (opy() == 0x4 || opy() == 0xC)) {
            if (m_opcode != 0x34) {
                m_inst_type = inst_type_t::INC;
            } else {
                m_inst_type = inst_type_t::INC_HL;
            }
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && opy() == 0x3) {
            m_inst_type = inst_type_t::INC16;
            return;
        }

        if (opx() >= 0x0 && opx() <= 0x3 && (opy() == 0x5 || opy() == 0xD)) {
            if (m_opcode != 0x35) {
                m_inst_type = inst_type_t::DEC;
            } else {
                m_inst_type = inst_type_t::DEC_HL;
            }
            return;
        }
        if (opx() >= 0x0 && opx() <= 0x3 && opy() == 0xB) {
            m_inst_type = inst_type_t::DEC16;
            return;
        }

        if (opx() >= 0x0 && opx() <= 0x3 && opy() == 0x9) {
            m_inst_type = inst_type_t::ADD16;
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
        if (((opx() == 0xC || opx() == 0xD) &&
             (opy() == 0x0 || opy() == 0x8)) ||
            m_opcode == 0xC9) {
            m_inst_type = inst_type_t::RET;
            return;
        }
        if (m_opcode == 0xD9) {
            m_inst_type = inst_type_t::RETI;
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
        if (m_opcode == 0x07) {
            m_inst_type = inst_type_t::RLCA;
            return;
        }
        if (m_opcode == 0x0F) {
            m_inst_type = inst_type_t::RRCA;
            return;
        }
        if (m_opcode == 0x17) {
            m_inst_type = inst_type_t::RLA;
            return;
        }
        if (m_opcode == 0x1F) {
            m_inst_type = inst_type_t::RRA;
            return;
        }
        if (opx() == 0x7 && opy() < 0x8 && opy() != 0x6) {
            m_inst_type = inst_type_t::LD_HL;
            return;
        }
        if (((opx() == 0xC || opx() == 0xD) &&
             (opy() == 0x4 || opy() == 0xC)) ||
            m_opcode == 0xCD) {
            m_inst_type = inst_type_t::CALL;
            return;
        }
        if ((opx() >= 0xC && opx() <= 0xF) && (opy() == 0x7 || opy() == 0xF)) {
            m_inst_type = inst_type_t::RST;
            return;
        }
        if (m_opcode == 0xF3) {
            m_inst_type = inst_type_t::DI;
            return;
        }
        if (m_opcode == 0xFB) {
            m_inst_type = inst_type_t::EI;
            return;
        }
        if (m_opcode == 0x2F) {
            m_inst_type = inst_type_t::CPL;
            return;
        }

        if (opx() == 0x8 && opy() < 0x8) {
            if (opy() == 0x6) {
                m_inst_type = inst_type_t::ADD_HL;
            } else {
                m_inst_type = inst_type_t::ADD;
            }
            return;
        }
        if (opx() == 0x8 && opy() >= 0x8) {
            if (opy() == 0xE) {
                m_inst_type = inst_type_t::ADC_HL;
            } else {
                m_inst_type = inst_type_t::ADC;
            }
            return;
        }
        if (opx() == 0x9 && opy() < 0x8) {
            if (opy() == 0x6) {
                m_inst_type = inst_type_t::SUB_HL;
            } else {
                m_inst_type = inst_type_t::SUB;
            }
            return;
        }
        if (opx() == 0x9 && opy() >= 0x8) {
            if (opy() == 0xE) {
                m_inst_type = inst_type_t::SBC_HL;
            } else {
                m_inst_type = inst_type_t::SBC;
            }
            return;
        }
        if (opx() == 0xA && opy() < 0x8) {
            if (opy() == 0x6) {
                m_inst_type = inst_type_t::AND_HL;
            } else {
                m_inst_type = inst_type_t::AND;
            }
            return;
        }
        if (opx() == 0xA && opy() >= 0x8) {
            if (opy() == 0xE) {
                m_inst_type = inst_type_t::XOR_HL;
            } else {
                m_inst_type = inst_type_t::XOR;
            }
            return;
        }
        if (opx() == 0xB && opy() < 0x8) {
            if (opy() == 0x6) {
                m_inst_type = inst_type_t::OR_HL;
            } else {
                m_inst_type = inst_type_t::OR;
            }
            return;
        }
        if (opx() == 0xB && opy() >= 0x8) {
            if (opy() == 0xE) {
                m_inst_type = inst_type_t::CP_HL;
            } else {
                m_inst_type = inst_type_t::CP;
            }
            return;
        }
        if (m_opcode == 0xC6) {
            m_inst_type = inst_type_t::ADD_IMM;
            return;
        }
        if (m_opcode == 0xCE) {
            m_inst_type = inst_type_t::ADC_IMM;
            return;
        }
        if (m_opcode == 0xD6) {
            m_inst_type = inst_type_t::SUB_IMM;
            return;
        }
        if (m_opcode == 0xDE) {
            m_inst_type = inst_type_t::SBC_IMM;
            return;
        }
        if (m_opcode == 0xE6) {
            m_inst_type = inst_type_t::AND_IMM;
            return;
        }
        if (m_opcode == 0xEE) {
            m_inst_type = inst_type_t::XOR_IMM;
            return;
        }
        if (m_opcode == 0xF6) {
            m_inst_type = inst_type_t::OR_IMM;
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

    m_opcode = read(PC()++);
    prefixed = m_opcode == constants::PREFIX;
    if (prefixed) {
        m_opcode = read(PC()++);
    }

    // cout << hex << "pc: 0x" << m_fetch_pc << " opcode: 0x" << hex
    //<< int(m_opcode) << endl;
    set_inst_type();
}

void cpu_t::decode() {
    switch (m_inst_type) {
    case inst_type_t::NOP:
    case inst_type_t::LD_REG_REG:
    case inst_type_t::LD_REG_FROM_HL:
    case inst_type_t::LDMEM16:
    case inst_type_t::LDFROMMEM:
    case inst_type_t::LD_INTO_C_OFFSET:
    case inst_type_t::LD_FROM_C_OFFSET:
    case inst_type_t::LD_HL:
    case inst_type_t::ADD16:
    case inst_type_t::PUSH:
    case inst_type_t::POP:
    case inst_type_t::RET:
    case inst_type_t::RETI:
    case inst_type_t::DI:
    case inst_type_t::EI:
    case inst_type_t::CPL:
    case inst_type_t::RST:
    case inst_type_t::JP_HL:

    case inst_type_t::BIT:
    case inst_type_t::BIT_HL:
    case inst_type_t::RES:
    case inst_type_t::RES_HL:
    case inst_type_t::SET:
    case inst_type_t::SET_HL:

    case inst_type_t::INC:
    case inst_type_t::INC_HL:
    case inst_type_t::INC16:
    case inst_type_t::DEC:
    case inst_type_t::DEC_HL:
    case inst_type_t::DEC16:

    case inst_type_t::RLC:
    case inst_type_t::RRC:
    case inst_type_t::RL:
    case inst_type_t::RR:
    case inst_type_t::SLA:
    case inst_type_t::SRA:
    case inst_type_t::SWAP:
    case inst_type_t::SRL:
    case inst_type_t::RLC_HL:
    case inst_type_t::RRC_HL:
    case inst_type_t::RL_HL:
    case inst_type_t::RR_HL:
    case inst_type_t::SLA_HL:
    case inst_type_t::SRA_HL:
    case inst_type_t::SWAP_HL:
    case inst_type_t::SRL_HL:
    case inst_type_t::RLCA:
    case inst_type_t::RRCA:
    case inst_type_t::RLA:
    case inst_type_t::RRA:

    case inst_type_t::ADD:
    case inst_type_t::ADC:
    case inst_type_t::SUB:
    case inst_type_t::SBC:
    case inst_type_t::AND:
    case inst_type_t::XOR:
    case inst_type_t::OR:
    case inst_type_t::CP:
    case inst_type_t::ADD_HL:
    case inst_type_t::ADC_HL:
    case inst_type_t::SUB_HL:
    case inst_type_t::SBC_HL:
    case inst_type_t::AND_HL:
    case inst_type_t::XOR_HL:
    case inst_type_t::OR_HL:
    case inst_type_t::CP_HL:

    case inst_type_t::LD_HL_SP:

    case inst_type_t::DAA:
    case inst_type_t::SCF:
    case inst_type_t::CCF:

    case inst_type_t::STOP:
    case inst_type_t::HALT:
        break;
    case inst_type_t::LD8:
    case inst_type_t::LD8_HL:
    case inst_type_t::JR:
    case inst_type_t::LD_INTO_IMM_OFFSET:
    case inst_type_t::LD_FROM_IMM_OFFSET:

    case inst_type_t::ADD_IMM:
    case inst_type_t::ADC_IMM:
    case inst_type_t::SUB_IMM:
    case inst_type_t::SBC_IMM:
    case inst_type_t::AND_IMM:
    case inst_type_t::XOR_IMM:
    case inst_type_t::OR_IMM:
    case inst_type_t::CP_IMM:

    case inst_type_t::ADD_SP_IMM:
    case inst_type_t::LD_SP_SUM_IMM_HL:
        m_lo = read(PC()++);
        break;
    case inst_type_t::LD16:
    case inst_type_t::CALL:
    case inst_type_t::LD_INTO_IMM:
    case inst_type_t::LD_FROM_IMM:
    case inst_type_t::JP:
    case inst_type_t::LD_SP_MEM:
        m_lo = read(PC()++);
        m_hi = read(PC()++);
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
    int8_t signed_op = m_lo;

    switch (m_inst_type) {
    case inst_type_t::NOP:
        break;
    case inst_type_t::JP:
        if (!flag_cond()) {
            return;
        }
        PC() = n16();
        break;
    case inst_type_t::JP_HL:
        PC() = HL();
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
    case inst_type_t::LD_REG_FROM_HL:
        r8x() = read(HL());
        break;
    case inst_type_t::LDMEM16:
        if (opx() == 0x2) {
            write(HL()++, A());
        } else if (opx() == 0x3) {
            write(HL()--, A());
        } else {
            write(r16x(), A());
        }
        break;
    case inst_type_t::LDFROMMEM:
        if (opx() == 0x2) {
            A() = read(HL()++);
        } else if (opx() == 0x3) {
            A() = read(HL()--);
        } else {
            A() = read(r16x());
        }
        break;
    case inst_type_t::LD_INTO_IMM_OFFSET:
        write(0xFF00 + m_lo, A());
        break;
    case inst_type_t::LD_FROM_IMM_OFFSET:
        A() = read(0xFF00 + m_lo);
        break;
    case inst_type_t::LD_INTO_IMM:
        write(n16(), A());
        break;
    case inst_type_t::LD_FROM_IMM:
        A() = read(n16());
        break;
    case inst_type_t::LD_INTO_C_OFFSET:
        write(0xFF00 + C(), A());
        break;
    case inst_type_t::LD_FROM_C_OFFSET:
        A() = read(0xFF00 + C());
        break;
    case inst_type_t::LD_HL:
        write(HL(), r8y());
        break;
    case inst_type_t::LD8:
        r8x() = m_lo;
        break;
    case inst_type_t::LD8_HL:
        write(HL(), m_lo);
        break;
    case inst_type_t::ADD16:
        setH(((HL() & 0x0FFF) + (r16x() & 0x0FFF)) > 0x0FFF);
        setN(0);
        setC((static_cast<uint32_t>(HL()) + static_cast<uint32_t>(r16x())) >
             0xFFFF);
        HL() += r16x();
        break;
    case inst_type_t::LD16:
        r16x() = n16();
        break;
    case inst_type_t::PUSH:
        write(--SP(), split_reg(r16x(), true));
        write(--SP(), split_reg(r16x(), false));
        break;
    case inst_type_t::POP:
        split_reg(r16x(), false) = read(SP()++);
        F() = F() & 0b11110000;
        split_reg(r16x(), true) = read(SP()++);
        break;
    case inst_type_t::RET:
        if (!flag_cond()) {
            return;
        }
        split_reg(PC(), false) = read(SP()++);
        split_reg(PC(), true) = read(SP()++);
        break;
    case inst_type_t::RETI:
        m_IME = true;
        split_reg(PC(), false) = read(SP()++);
        split_reg(PC(), true) = read(SP()++);
        break;

    case inst_type_t::INC:
        r8x() = alu_inc(r8x());
        break;
    case inst_type_t::INC_HL:
        write(HL(), alu_inc(read(HL())));
        break;
    case inst_type_t::INC16:
        r16x()++;
        break;
    case inst_type_t::DEC:
        r8x() = alu_dec(r8x());
        break;
    case inst_type_t::DEC_HL:
        write(HL(), alu_dec(read(HL())));
        break;
    case inst_type_t::DEC16:
        r16x()--;
        break;

    case inst_type_t::RLC:
        r8y() = alu_rlc(r8y());
        break;
    case inst_type_t::RRC:
        r8y() = alu_rrc(r8y());
        break;
    case inst_type_t::RL:
        r8y() = alu_rl(r8y());
        break;
    case inst_type_t::RR:
        r8y() = alu_rr(r8y());
        break;
    case inst_type_t::SLA:
        r8y() = alu_sla(r8y());
        break;
    case inst_type_t::SRA:
        r8y() = alu_sra(r8y());
        break;
    case inst_type_t::SWAP:
        r8y() = alu_swap(r8y());
        break;
    case inst_type_t::SRL:
        r8y() = alu_srl(r8y());
        break;
    case inst_type_t::RLC_HL:
        write(HL(), alu_rlc(read(HL())));
        break;
    case inst_type_t::RRC_HL:
        write(HL(), alu_rrc(read(HL())));
        break;
    case inst_type_t::RL_HL:
        write(HL(), alu_rl(read(HL())));
        break;
    case inst_type_t::RR_HL:
        write(HL(), alu_rr(read(HL())));
        break;
    case inst_type_t::SLA_HL:
        write(HL(), alu_sla(read(HL())));
        break;
    case inst_type_t::SRA_HL:
        write(HL(), alu_sra(read(HL())));
        break;
    case inst_type_t::SWAP_HL:
        write(HL(), alu_swap(read(HL())));
        break;
    case inst_type_t::SRL_HL:
        write(HL(), alu_srl(read(HL())));
        break;

    case inst_type_t::RLCA:
        A() = alu_rlc(A());
        setZ(0);
        break;
    case inst_type_t::RRCA:
        A() = alu_rrc(A());
        setZ(0);
        break;
    case inst_type_t::RLA:
        A() = alu_rl(A());
        setZ(0);
        break;
    case inst_type_t::RRA:
        A() = alu_rr(A());
        setZ(0);
        break;
    case inst_type_t::CALL:
        if (!flag_cond()) {
            return;
        }
        call(n16());
        break;
    case inst_type_t::RST:
        call((opx() - 0xC) << 4 | (opy() - 0x7));
        break;
    case inst_type_t::DI:
        m_IME = false;
        break;
    case inst_type_t::EI:
        m_pending_IME = true;
        break;
    case inst_type_t::CPL:
        A() = ~A();
        setN(1);
        setH(1);
        break;

    case inst_type_t::LD_SP_MEM:
        write(n16(), SP() & 0xFF);
        write(n16() + 1, SP() >> 8);
        break;
    case inst_type_t::LD_HL_SP:
        SP() = HL();
        break;
    case inst_type_t::ADD_SP_IMM:
        setH(((SP() & 0xF) + (signed_op & 0xF)) > 0xF);
        setC(((SP() & 0xFF) + (signed_op & 0xFF)) > 0xFF);
        setN(0);
        setZ(0);
        SP() += signed_op;
        break;
    case inst_type_t::LD_SP_SUM_IMM_HL:
        setH(((SP() & 0xF) + (signed_op & 0xF)) > 0xF);
        setC(((SP() & 0xFF) + (signed_op & 0xFF)) > 0xFF);
        setN(0);
        setZ(0);
        HL() = SP() + signed_op;
        break;

    case inst_type_t::DAA:
        if (!n()) {
            if (c() || A() > 0x99) {
                A() += 0x60;
                setC(1);
            }
            if (h() || (A() & 0x0F) > 0x09) {
                A() += 0x6;
            }
        } else {
            if (c()) {
                A() -= 0x60;
            }
            if (h()) {
                A() -= 0x6;
            }
        }
        setZ(A() == 0);
        setH(0);
        break;
    case inst_type_t::SCF:
        setN(0);
        setH(0);
        setC(1);
        break;
    case inst_type_t::CCF:
        setN(0);
        setH(0);
        setC(!c());
        break;

    case inst_type_t::STOP:
        // TODO
        assert(false);
        break;
    case inst_type_t::HALT:
        // TODO
        assert(false);
        break;

    case inst_type_t::BIT:
        alu_bit(r8y());
        break;
    case inst_type_t::BIT_HL:
        alu_bit(read(HL()));
        break;
    case inst_type_t::RES:
        r8y() = alu_res(r8y());
        break;
    case inst_type_t::RES_HL:
        write(HL(), alu_res(read(HL())));
        break;
    case inst_type_t::SET:
        r8y() = alu_set(r8y());
        break;
    case inst_type_t::SET_HL:
        write(HL(), alu_set(read(HL())));
        break;

    case inst_type_t::ADD:
        alu_add(r8y());
        break;
    case inst_type_t::ADC:
        alu_adc(r8y());
        break;
    case inst_type_t::SUB:
        alu_sub(r8y());
        break;
    case inst_type_t::SBC:
        alu_sbc(r8y());
        break;
    case inst_type_t::AND:
        alu_and(r8y());
        break;
    case inst_type_t::XOR:
        alu_xor(r8y());
        break;
    case inst_type_t::OR:
        alu_or(r8y());
        break;
    case inst_type_t::CP:
        alu_cp(r8y());
        break;
    case inst_type_t::ADD_HL:
        alu_add(read(HL()));
        break;
    case inst_type_t::ADC_HL:
        alu_adc(read(HL()));
        break;
    case inst_type_t::SUB_HL:
        alu_sub(read(HL()));
        break;
    case inst_type_t::SBC_HL:
        alu_sbc(read(HL()));
        break;
    case inst_type_t::AND_HL:
        alu_and(read(HL()));
        break;
    case inst_type_t::XOR_HL:
        alu_xor(read(HL()));
        break;
    case inst_type_t::OR_HL:
        alu_or(read(HL()));
        break;
    case inst_type_t::CP_HL:
        alu_cp(read(HL()));
        break;
    case inst_type_t::ADD_IMM:
        alu_add(m_lo);
        break;
    case inst_type_t::ADC_IMM:
        alu_adc(m_lo);
        break;
    case inst_type_t::SUB_IMM:
        alu_sub(m_lo);
        break;
    case inst_type_t::SBC_IMM:
        alu_sbc(m_lo);
        break;
    case inst_type_t::AND_IMM:
        alu_and(m_lo);
        break;
    case inst_type_t::XOR_IMM:
        alu_xor(m_lo);
        break;
    case inst_type_t::OR_IMM:
        alu_or(m_lo);
        break;
    case inst_type_t::CP_IMM:
        alu_cp(m_lo);
        break;
    default:
        throw std::runtime_error("unknown instruction type (execute)");
    }
}

void cpu_t::call(uint16_t v) {
    write(--SP(), split_reg(PC(), true));
    write(--SP(), split_reg(PC(), false));
    PC() = v;
}

void cpu_t::alu_add(uint8_t operand) {
    setH(((A() & 0xF) + (operand & 0xF)) > 0xF);
    setN(0);
    setC((static_cast<uint16_t>(A()) + static_cast<uint16_t>(operand)) > 0xFF);
    A() += operand;
    setZ(A() == 0);
}
void cpu_t::alu_adc(uint8_t operand) {
    uint8_t carry = c() ? 1 : 0;
    uint8_t lower_nibble_sum = carry + (A() & 0xF) + (operand & 0xF);
    uint16_t total_sum =
        static_cast<uint16_t>(A()) + static_cast<uint16_t>(operand) + carry;
    A() = total_sum & 0xFF;
    setH(lower_nibble_sum > 0xF);
    setN(0);
    setC(total_sum > 0xFF);
    setZ(A() == 0);
}
void cpu_t::alu_sub(uint8_t operand) {
    setH((operand & 0xF) > (A() & 0xF));
    setC(operand > A());
    A() -= operand;
    setZ(A() == 0);
    setN(1);
}
void cpu_t::alu_sbc(uint8_t operand) {
    uint8_t carry = c() ? 1 : 0;
    setH(((operand & 0xF) + carry) > (A() & 0xF));
    setC((operand + carry) > A());
    A() -= operand;
    A() -= carry;
    setZ(A() == 0);
    setN(1);
}
void cpu_t::alu_and(uint8_t operand) {
    A() &= operand;
    setZ(A() == 0);
    setN(0);
    setH(1);
    setC(0);
}
void cpu_t::alu_xor(uint8_t operand) {
    A() ^= operand;
    setZ(A() == 0);
    setN(0);
    setH(0);
    setC(0);
}
void cpu_t::alu_or(uint8_t operand) {
    A() |= operand;
    setZ(A() == 0);
    setN(0);
    setH(0);
    setC(0);
}
void cpu_t::alu_cp(uint8_t operand) {
    setH((operand & 0xF) > (A() & 0xF));
    setC(operand > A());
    uint8_t temp_a = A() - operand;
    setZ(temp_a == 0);
    setN(1);
}

uint8_t cpu_t::alu_inc(uint8_t operand) {
    operand++;
    setZ(!operand);
    setN(0);
    setH(!(operand & 0xF));
    return operand;
}
uint8_t cpu_t::alu_dec(uint8_t operand) {
    setH((operand & 0xF) == 0);
    operand--;
    setZ(!operand);
    setN(1);
    return operand;
}

// TODO: de-dup the below
uint8_t cpu_t::alu_rlc(uint8_t operand) {
    setC(operand & (1 << 7));
    operand <<= 1;
    operand |= (c() ? 1 : 0);
    setZ(operand == 0);
    setN(0);
    setH(0);
    return operand;
}
uint8_t cpu_t::alu_rrc(uint8_t operand) {
    setC(operand & (1 << 0));
    operand >>= 1;
    operand |= ((c() ? 1 : 0) << 7);
    setZ(operand == 0);
    setN(0);
    setH(0);
    return operand;
}
uint8_t cpu_t::alu_rl(uint8_t operand) {
    bool c_flag = c();
    setC(operand & (1 << 7));
    operand <<= 1;
    operand |= (c_flag ? 1 : 0);
    setZ(operand == 0);
    setN(0);
    setH(0);
    return operand;
}
uint8_t cpu_t::alu_rr(uint8_t operand) {
    bool c_flag = c();
    setC(operand & (1 << 0));
    operand >>= 1;
    operand |= ((c_flag ? 1 : 0) << 7);
    setZ(operand == 0);
    setN(0);
    setH(0);
    return operand;
}
uint8_t cpu_t::alu_sla(uint8_t operand) {
    setC(operand & (1 << 7));
    operand <<= 1;
    setZ(operand == 0);
    setN(0);
    setH(0);
    return operand;
}
uint8_t cpu_t::alu_sra(uint8_t operand) {
    bool bit7 = operand & (1 << 7);
    setC(operand & (1 << 0));
    operand >>= 1;
    operand |= ((bit7 ? 1 : 0) << 7);
    setZ(operand == 0);
    setN(0);
    setH(0);
    return operand;
}
uint8_t cpu_t::alu_swap(uint8_t operand) {
    operand = (operand >> 4) | (operand << 4);
    setZ(operand == 0);
    setN(0);
    setH(0);
    setC(0);
    return operand;
}
uint8_t cpu_t::alu_srl(uint8_t operand) {
    setC(operand & (1 << 0));
    operand >>= 1;
    setZ(operand == 0);
    setN(0);
    setH(0);
    return operand;
}

void cpu_t::alu_bit(uint8_t operand) {
    setZ(!(operand & (1 << ((opx() - 0x4) * 2 + opy() / 8))));
    setN(0);
    setH(1);
}
uint8_t cpu_t::alu_res(uint8_t operand) {
    uint8_t bit = (opx() - 0x8) * 2 + opy() / 8;
    operand &= ~(1 << bit);
    return operand;
}
uint8_t cpu_t::alu_set(uint8_t operand) {
    uint8_t bit = (opx() - 0xC) * 2 + opy() / 8;
    operand |= (1 << bit);
    return operand;
}

void cpu_t::tick() {
    if (m_pending_IME) {
        m_IME = true;
        m_pending_IME = false;
    }

    if (m_failed) {
        return;
    }
    fetch();
    if (m_failed) {
        return;
    }
    decode();
    execute();

    // TODO: service interrupt requests
}
