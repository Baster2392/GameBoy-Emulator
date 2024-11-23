#include "CPU.h"

void CPU::executeOpcode(std::uint8_t opcode)
{
	uint8_t byte;
	switch (opcode)
	{
	case 0x00:
		NOP();
		break;
	case 0x10:
		STOP();
		break;
	case 0x20:
		JR_cc_e8(NZ_flag());
		break;
	case 0x30:
		JR_cc_e8(NC_flag());
		break;
	case 0x40: // LD B,B = NOP
		NOP();
		break;
	case 0x50:
		LD_r8_r8(&(this->D), &(this->B));
		break;
	case 0x60:
		LD_r8_r8(&(this->H), &(this->B));
		break;
	case 0x70:
		LD_HL_r8(this->B);
		break;
	case 0x80:
		ADD_A_r8(this->B);
		break;
	case 0x90:
		SUB_A_r8(this->B);
		break;
	case 0xA0:
		AND_A_r8(this->B);
		break;
	case 0xB0:
		OR_A_r8(this->B);
		break;
	case 0xC0:
		RET_cc(NZ_flag());
		break;
	case 0xD0:
		RET_cc(NC_flag());
		break;
	case 0xE0:
		LDH_a8_A();
		break;
	case 0xF0:
		LDH_A_a8();
		break;
	case 0x01:
		LD_r8r8_n16(&(this->B), &(this->C));
		break;
	case 0x11:
		LD_r8r8_n16(&(this->D), &(this->E));
		break;
	case 0x21:
		LD_r8r8_n16(&(this->H), &(this->L));
		break;
	case 0x31:
		LD_r16_n16(&(this->SP));
		break;
	case 0x41:
		LD_r8_r8(&(this->B), &(this->C));
		break;
	case 0x51:
		LD_r8_r8(&(this->D), &(this->C));
		break;
	case 0x61:
		LD_r8_r8(&(this->H), &(this->C));
		break;
	case 0x71:
		LD_HL_r8(this->C);
		break;
	case 0x81:
		ADD_A_r8(this->C);
		break;
	case 0x91:
		SUB_A_r8(this->C);
		break;
	case 0xA1:
		AND_A_r8(this->C);
		break;
	case 0xB1:
		OR_A_r8(this->C);
		break;
	case 0xC1:
		POP_r8r8(&(this->B), &(this->C));
		break;
	case 0xD1:
		POP_r8r8(&(this->D), &(this->E));
		break;
	case 0xE1:
		POP_r8r8(&(this->H), &(this->L));
		break;
	case 0xF1:
		POP_r8r8(&(this->A), &(this->F));
		break;
	case 0x02:
		LD_ar8r8_r8(&(this->B), &(this->C), this->A);
		break;
	case 0x12:
		LD_ar8r8_r8(&(this->D), &(this->E), this->A);
		break;
	case 0x22:
		LD_aHLI_A();
		break;
	case 0x32:
		LD_aHLD_A();
		break;
	case 0x42:
		LD_r8_r8(&(this->B), &(this->D));
		break;
	case 0x52:	// LD D,D
		NOP();
		break;
	case 0x62:
		LD_r8_r8(&(this->H), &(this->D));
		break;
	case 0x72:
		LD_ar8r8_r8(&(this->H), &(this->L), this->D);
		break;
	case 0x82:
		ADD_A_r8(this->D);
		break;
	case 0x92:
		SUB_A_r8(this->D);
		break;
	case 0xA2:
		AND_A_r8(this->D);
		break;
	case 0xB2:
		OR_A_r8(this->D);
		break;
	case 0xC2:
		JP_cc_a16(NZ_flag());
		break;
	case 0xD2:
		JP_cc_a16(NC_flag());
		break;
	case 0xE2:
		break;	// LD [C], A ???
	case 0xF2:
		break;	// LD A, [C] ???
	case 0x3:
		INC_r8r8(&(this->B), &(this->C));
		break;
	case 0x13:
		INC_r8r8(&(this->D), &(this->E));
		break;
	case 0x23:
		INC_r8r8(&(this->H), &(this->L));
		break;
	case 0x33:
		INC_r16(&(this->SP));
		break;
	case 0x43:
		LD_r8_r8(&(this->B), &(this->E));
		break;
	case 0x53:
		LD_r8_r8(&(this->D), &(this->E));
		break;
	case 0x63:
		LD_r8_r8(&(this->H), &(this->E));
		break;
	case 0x73:
		LD_ar8r8_r8(&(this->H), &(this->L), this->E);
		break;
	case 0x83:
		ADD_A_r8(this->E);
		break;
	case 0x93:
		SUB_A_r8(this->E);
		break;
	case 0xA3:
		AND_A_r8(this->E);
		break;
	case 0xB3:
		OR_A_r8(this->E);
		break;
	case 0xC3:
		JP_a16();
		break;
	case 0xF3:
		break;
	case 0x4:
		INC_r8(&(this->B));
		break;
	case 0x14:
		INC_r8(&(this->D));
		break;
	case 0x24:
		INC_r8(&(this->H));
		break;
	case 0x34:
		INC_aHL();
		break;
	case 0x44:
		LD_r8_r8(&(this->B), &(this->H));
		break;
	case 0x54:
		LD_r8_r8(&(this->D), &(this->H));
		break;
	case 0x64:
		LD_r8_r8(&(this->H), &(this->H));
		break;
	case 0x74:
		LD_ar8r8_r8(&(this->H), &(this->L), this->H);
		break;
	case 0x84:
		ADD_A_r8(this->H);
		break;
	case 0x94:
		SUB_A_r8(this->H);
		break;
	case 0xA4:
		AND_A_r8(this->H);
		break;
	case 0xB4:
		OR_A_r8(this->H);
		break;
	case 0xC4:
		CALL_cc_a16(NZ_flag());
		break;
	case 0xD4:
		CALL_cc_a16(NC_flag());
		break;
	case 0x5:
		DEC_r8(&(this->B));
		break;
	case 0x15:
		DEC_r8(&(this->D));
		break;
	case 0x25:
		DEC_r8(&(this->H));
		break;
	case 0x35:
		DEC_r8r8(&(this->H), &(this->L));
		break;
	case 0x45:
		LD_r8_r8(&(this->B), &(this->L));
		break;
	case 0x55:
		LD_r8_r8(&(this->D), &(this->L));
		break;
	case 0x65:
		LD_r8_r8(&(this->H), &(this->L));
		break;
	case 0x75:
		LD_ar8r8_r8(&(this->H), &(this->L), this->H);
		break;
	case 0x85:
		ADD_A_r8(this->L);
		break;
	case 0x95:
		SUB_A_r8(this->L);
		break;
	case 0xA5:
		AND_A_r8(this->L);
		break;
	case 0xB5:
		OR_A_r8(this->L);
		break;
	case 0xC5:
		PUSH_r8r8(&(this->B), &(this->C));
		break;
	case 0xD5:
		PUSH_r8r8(&(this->D), &(this->E));
		break;
	case 0xE5:
		PUSH_r8r8(&(this->H), &(this->L));
		break;
	case 0xF5:
		PUSH_r8r8(&(this->A), &(this->F));
		break;
	case 0x6:
		LD_r8_n8(&(this->B));
		break;
	case 0x16:
		LD_r8_n8(&(this->D));
		break;
	case 0x26:
		LD_r8_n8(&(this->H));
		break;
	case 0x36:
		LD_ar8r8_n8(&(this->H), &(this->L));
		break;
	case 0x46:
		LD_r8_ar8r8(&(this->B), this->H, this->L);
		break;
	case 0x56:
		LD_r8_ar8r8(&(this->D), this->H, this->L);
		break;
	case 0x66:
		LD_r8_ar8r8(&(this->H), this->H, this->L);
		break;
	case 0x76:
		HALT();
		break;
	case 0x86:
		ADD_A_ar8r8(&(this->H), &(this->L));
		break;
	case 0x96:
		SUB_A_ar8r8(&(this->H), &(this->L));
		break;
	case 0xA6:
		AND_A_ar8r8(&(this->H), &(this->L));
		break;
	case 0xB6:
		OR_A_ar8r8(&(this->H), &(this->L));
		break;
	case 0xC6:
		ADD_A_n8();
		break;
	case 0xD6:
		SUB_A_n8();
		break;
	case 0xE6:
		AND_A_n8();
		break;
	case 0xF6:
		OR_A_n8();
		break;
	case 0x7:
		RLC_r8(&(this->A));
		break;
	case 0x17:
		RL_r8(&(this->A));
		break;
	case 0x27:
		DAA();
		break;
	case 0x37:
		SCF();
		break;
	case 0x47:
		LD_r8_r8(&(this->B), &(this->A));
		break;
	case 0x57:
		LD_r8_r8(&(this->D), &(this->A));
		break;
	case 0x67:
		LD_r8_r8(&(this->H), &(this->A));
		break;
	case 0x77:
		LD_ar8r8_r8(&(this->H), &(this->L), this->A);
		break;
	case 0x87:
		ADD_A_r8(this->A);
		break;
	case 0x97:
		SUB_A_r8(this->A);
		break;
	case 0xA7:
		AND_A_r8(this->A);
		break;
	case 0xB7:
		OR_A_r8(this->A);
		break;
	case 0xC7:
		RST(0x00);
		break;
	case 0xD7:
		RST(0x10);
		break;
	case 0xE7:
		RST(0x20);
		break;
	case 0xF7:
		RST(0x30);
		break;
	case 0x8:
		LD_a16_r16(this->SP);
		break;
	case 0x18:
		JR_e8();
		break;
	case 0x28:
		JR_cc_e8(Z_flag());
		break;
	case 0x38:
		JR_cc_e8(C_flag());
		break;
	case 0x48:
		LD_r8_r8(&(this->C), &(this->B));
		break;
	case 0x58:
		LD_r8_r8(&(this->E), &(this->B));
		break;
	case 0x68:
		LD_r8_r8(&(this->L), &(this->B));
		break;
	case 0x78:
		LD_r8_r8(&(this->A), &(this->B));
		break;
	case 0x88:
		ADC_A_r8(this->B);
		break;
	case 0x98:
		SBC_A_r8(this->B);
		break;
	case 0xA8:
		XOR_A_r8(this->B);
		break;
	case 0xB8:
		CP_A_r8(this->B);
		break;
	case 0xC8:
		RET_cc(Z_flag());
		break;
	case 0xD8:
		RET_cc(C_flag());
		break;
	case 0xE8:
		ADD_SP_e8();
		break;
	case 0xF8:
		LD_HL_SPe8();
		break;
	case 0x9:
		ADD_HL_r8r8(&(this->B), &(this->C));
		break;
	case 0x19:
		ADD_HL_r8r8(&(this->D), &(this->E));
		break;
	case 0x29:
		ADD_HL_r8r8(&(this->H), &(this->L));
		break;
	case 0x39:
		ADD_HL_r16(&(this->SP));
		break;
	case 0x49:	// LD C, C
		NOP();
		break;
	case 0x59:
		LD_r8_r8(&(this->E), &(this->C));
		break;
	case 0x69:
		LD_r8_r8(&(this->L), &(this->C));
		break;
	case 0x79:
		LD_r8_r8(&(this->A), &(this->C));
		break;
	case 0x89:
		ADC_A_r8(this->C);
		break;
	case 0x99:
		SBC_A_r8(this->C);
		break;
	case 0xA9:
		XOR_A_r8(this->C);
		break;
	case 0xB9:
		CP_A_r8(this->C);
		break;
	case 0xC9:
		RETi();
		break;
	case 0xE9:
		JP_HL();
		break;
	case 0xF9:
		LD_SP_HL();
		break;
	case 0xA:
		LD_r8_ar8r8(&(this->A), this->B, this->C);
		break;
	case 0x1A:
		LD_r8_ar8r8(&(this->A), this->D, this->E);
		break;
	case 0x2A:
		LD_A_aHLI();
		break;
	case 0x3A:
		LD_A_aHLD();
		break;
	case 0x4A:
		LD_r8_r8(&(this->C), &(this->D));
		break;
	case 0x5A:
		LD_r8_r8(&(this->E), &(this->D));
		break;
	case 0x6A:
		LD_r8_r8(&(this->L), &(this->D));
		break;
	case 0x7A:
		LD_r8_r8(&(this->A), &(this->D));
		break;
	case 0x8A:
		ADC_A_r8(this->D);
		break;
	case 0x9A:
		SBC_A_r8(this->D);
		break;
	case 0xAA:
		XOR_A_r8(this->D);
		break;
	case 0xBA:
		CP_A_r8(this->D);
		break;
	case 0xCA:
		JP_cc_a16(this->Z_flag());
		break;
	case 0xDA:
		JP_cc_a16(this->C_flag());
		break;
	case 0xEA:
		LD_aa16_A();
		break;
	case 0xFA:
		LD_A_aa16();
		break;
	case 0xB:
		DEC_r8r8(&(this->B), &(this->C));
		break;
	case 0x1B:
		DEC_r8r8(&(this->D), &(this->E));
		break;
	case 0x2B:
		DEC_r8r8(&(this->H), &(this->L));
		break;
	case 0x3B:
		DEC_r16(&(this->SP));
		break;
	case 0x4B:
		LD_r8_r8(&(this->C), &(this->E));
		break;
	case 0x5B:
		LD_r8_r8(&(this->E), &(this->E));
		break;
	case 0x6B:
		LD_r8_r8(&(this->L), &(this->E));
		break;
	case 0x7B:
		LD_r8_r8(&(this->A), &(this->E));
		break;
	case 0x8B:
		ADC_A_r8(this->E);
		break;
	case 0x9B:
		SBC_A_r8(this->E);
		break;
	case 0xAB:
		XOR_A_r8(this->E);
		break;
	case 0xBB:
		CP_A_r8(this->E);
		break;
	case 0xCB:
		printf("Prefix!!!!!!!\n");
		break;
	case 0xFB:
		EI();
		break;
	case 0xC:
		INC_r8(&(this->C));
		break;
	case 0x1C:
		INC_r8(&(this->E));
		break;
	case 0x2C:
		INC_r8(&(this->L));
		break;
	case 0x3C:
		INC_r8(&(this->A));
		break;
	case 0x4C:
		LD_r8_r8(&(this->C), &(this->H));
		break;
	case 0x5C:
		LD_r8_r8(&(this->E), &(this->H));
		break;
	case 0x6C:
		LD_r8_r8(&(this->L), &(this->H));
		break;
	case 0x7C:
		LD_r8_r8(&(this->A), &(this->H));
		break;
	case 0x8C:
		ADC_A_r8(this->H);
		break;
	case 0x9C:
		SBC_A_r8(this->H);
		break;
	case 0xAC:
		XOR_A_r8(this->H);
		break;
	case 0xBC:
		CP_A_r8(this->H);
		break;
	case 0xCC:
		CALL_cc_a16(this->Z_flag());
		break;
	case 0xDC:
		CALL_cc_a16(this->C_flag());
		break;
	case 0xD:
		DEC_r8(&(this->C));
		break;
	case 0x1D:
		DEC_r8(&(this->E));
		break;
	case 0x2D:
		DEC_r8(&(this->L));
		break;
	case 0x3D:
		DEC_r8(&(this->A));
		break;
	case 0x4D:
		LD_r8_r8(&(this->C), &(this->L));
		break;
	case 0x5D:
		LD_r8_r8(&(this->E), &(this->L));
		break;
	case 0x6D:
		LD_r8_r8(&(this->L), &(this->L));
		break;
	case 0x7D:
		LD_r8_r8(&(this->A), &(this->L));
		break;
	case 0x8D:
		ADC_A_r8(this->L);
		break;
	case 0x9D:
		SBC_A_r8(this->L);
		break;
	case 0xAD:
		XOR_A_r8(this->L);
		break;
	case 0xBD:
		CP_A_r8(this->L);
		break;
	case 0xCD:
		CALL();
		break;
	case 0xE:
		LD_r8_n8(&(this->C));
		break;
	case 0x1E:
		LD_r8_n8(&(this->E));
		break;
	case 0x2E:
		LD_r8_n8(&(this->L));
		break;
	case 0x3E:
		LD_r8_n8(&(this->A));
		break;
	case 0x4E:
		LD_r8_ar8r8(&(this->C), this->H, this->L);
		break;
	case 0x5E:
		LD_r8_ar8r8(&(this->E), this->H, this->L);
		break;
	case 0x6E:
		LD_r8_ar8r8(&(this->L), this->H, this->L);
		break;
	case 0x7E:
		LD_r8_ar8r8(&(this->A), this->H, this->L);
		break;
	case 0x8E:
		ADC_A_ar8r8(this->H, this->L);
		break;
	case 0x9E:
		SBC_A_ar8r8(this->H, this->L);
		break;
	case 0xAE:
		XOR_A_ar8r8(&(this->H), &(this->L));
		break;
	case 0xBE:
		CP_A_ar8r8(this->H, this->L);
		break;
	case 0xCE:
		ADC_A_n8();
		break;
	case 0xDE:
		SBC_A_n8();
		break;
	case 0xEE:
		XOR_A_n8();
		break;
	case 0xFE:
		CP_A_n8();
		break;
	case 0xF:
		RRCA();
		break;
	case 0x1F:
		RRA();
		break;
	case 0x2F:
		CPL();
		break;
	case 0x3F:
		CCF();
		break;
	case 0x4F:
		LD_r8_r8(&(this->C), &(this->A));
		break;
	case 0x5F:
		LD_r8_r8(&(this->E), &(this->A));
		break;
	case 0x6F:
		LD_r8_r8(&(this->L), &(this->A));
		break;
	case 0x7F:
		LD_r8_r8(&(this->A), &(this->A));
		break;
	case 0x8F:
		ADC_A_r8(this->A);
		break;
	case 0x9F:
		SBC_A_r8(this->A);
		break;
	case 0xAF:
		XOR_A_r8(this->A);
		break;
	case 0xBF:
		CP_A_r8(this->A);
		break;
	case 0xCF:
		RST(0x8);
		break;
	case 0xDF:
		RST(0x18);
		break;
	case 0xEF:
		RST(0x28);
		break;
	case 0xFF:
		RST(0x38);
		break;
		
	default:
		printf("Not supported opcode.\n");
		break;
	}
}

std::uint8_t CPU::readMemory(std::uint16_t addressToRead)
{
	uint8_t value;
	switch (addressToRead)
	{
	case 0xFF40:
		value = gpu.lcdc;
		break;
	case 0xFF42:
		value = gpu.scx;
		break;
	case 0xFF43:
		value = gpu.scy;
		break;
	case 0xFF44:
		value = gpu.line;
		break;
	default:
		value = mmu.read_memory(addressToRead);
		break;
	}

	printf("Odczytano wartosc: %x z adresu: %x\n", value, addressToRead);
	return value;
}

std::uint8_t CPU::readInstruction(std::uint16_t addressToRead)
{
	return mmu.read_memory(addressToRead);
}

void CPU::writeMemory(std::uint16_t addressToWrite, std::uint8_t value)
{
	switch (addressToWrite)
	{
	case 0xFF40:
		this->gpu.lcdc = value;
		break;
	case 0xFF42:
		this->gpu.scx = value;
		break;
	case 0xFF43:
		this->gpu.scx = value;
		break;
	case 0xFF47:
		this->gpu.bgp = value;
		break;
	default:
		mmu.write_memory(addressToWrite, value);
		break;
	}

	printf("Zapisano wartosc: %x pod adresem %x\n", value, addressToWrite);
}

void CPU::setFlag(char flag, bool value)
{
	if (value == true)
	{
		switch (flag)
		{
		case 'Z':
			this->F |= 0b10000000;
			break;
		case 'N':
			this->F |= 0b01000000;
			break;
		case 'H':
			this->F |= 0b00100000;
			break;
		case 'C':
			this->F |= 0b00010000;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (flag)
		{
		case 'Z':
			this->F &= 0b01111111;
			break;
		case 'N':
			this->F &= 0b10111111;
			break;
		case 'H':
			this->F &= 0b11011111;
			break;
		case 'C':
			this->F &= 0b11101111;
			break;
		default:
			break;
		}
	}
}

void CPU::step()
{
	uint8_t opcode = readInstruction(this->PC);
	this->PC++;
	printf("Executing opcode %x...\n", opcode);
	executeOpcode(opcode);
	gpu.step(this->cycles / 4);
	renderer.render(gpu.framebuffer);

	this->cycles = 0;
	// using namespace std::chrono_literals;
	// std::this_thread::sleep_for(10ms);
}

bool CPU::Z_flag()
{
	return this->F >= 0b10000000;
}

bool CPU::N_flag()
{
	return (this->F << 1) >= 0b10000000;
}

bool CPU::H_flag()
{
	return (this->F << 2) >= 0b10000000;
}

bool CPU::C_flag()
{
	return (this->F << 3) >= 0b10000000;
}

bool CPU::NZ_flag()
{
	return !Z_flag();
}

bool CPU::NN_flag()
{
	return !N_flag();
}

bool CPU::NH_flag()
{
	return !H_flag();
}

bool CPU::NC_flag()
{
	return !C_flag();
}

void CPU::NOP()
{
	this->cycles += 4;
}

void CPU::SCF()
{
	// TODO
}

void CPU::STOP()
{
	this->cycles += 0; // idk
	this->PC++;
}

void CPU::RRCA()
{
	uint8_t lsb = this->A & 1;

	// set flags
	setFlag('Z', false);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', lsb);

	this->A >>= 1;
	this->A += lsb << 7;

	this->cycles += 4;
}

void CPU::RRA()
{
	uint8_t lsb = this->A & 1;
	uint8_t c = C_flag();

	// set flags
	setFlag('Z', false);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', lsb);

	this->A >>= 1;
	this->A += c << 7;

	this->cycles += 4;
}

void CPU::JR_cc_e8(bool cc)
{
	if (cc)
	{
		JR_e8();
	}
	else
	{
		this->PC++;
		this->cycles += 8;
	}
}

void CPU::JP_a16()
{
	uint16_t address = readInstruction(this->PC);
	this->PC++;
	address += readInstruction(this->PC) << 8;
	this->PC++;

	this->PC = address;
	this->cycles += 16;
}

void CPU::JP_cc_a16(bool cc)
{
	if (cc)
	{
		JP_a16();
	}
	else
	{
		this->PC += 2;
		this->cycles += 12;
	}
}

void CPU::JP_HL()
{
	uint16_t address = (this->H << 8) + this->L;
	this->PC = address;
	this->cycles += 4;
}

void CPU::CALL()
{
	PUSH_r16(this->PC + 3);	// push address of next instruction

	// read address from instruction set
	uint16_t address = readInstruction(this->PC);
	this->PC++;
	address += readInstruction(this->PC) << 8;
	this->PC++;

	// jump to address
	this->PC = address;

	this->cycles += 24;
}

void CPU::CALL_cc_a16(bool cc)
{
	if (cc)
	{
		CALL();
	}
	else
	{
		this->PC += 2;
		this->cycles += 12;
	}
}

void CPU::RST(std::uint16_t vectorAddress)
{
	PUSH_r16(this->PC + 1);
	this->PC = vectorAddress;
	this->cycles += 16;
}

void CPU::RLC_r8(std::uint8_t* reg)
{
	uint8_t value = *reg;
	setFlag('Z', false);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', value >= 0b10000000);
	value <<= 1;
	value += C_flag();
	*reg = value;

	this->cycles += 4;
}

void CPU::RL_r8(std::uint8_t* reg)
{
	uint8_t value = *reg;
	value <<= 1;
	value += C_flag();
	setFlag('Z', false);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', value >= 0b10000000);
	*reg = value;

	this->cycles += 4;
}

void CPU::CCF()
{
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', C_flag() ? 0 : 1);
	this->cycles += 4;
}

void CPU::CPL()
{
	this->A = ~(this->A);
	setFlag('N', true);
	setFlag('H', true);
}

void CPU::DAA()
{
	// TODO
}

void CPU::DI()
{
	// TODO
}

void CPU::EI()
{
	this->cycles += 4;
}

void CPU::HALT()
{
	// TODO
}

void CPU::LD_r8_r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	*reg1 = *reg2;
	this->cycles += 4;
}

void CPU::LD_HL_r8(std::uint8_t regValue)
{
	uint16_t addressToWrite = uint16_t(this->H) << 8;
	addressToWrite += uint16_t(this->L);

	writeMemory(addressToWrite, regValue);
	this->cycles += 8;
}

void CPU::ADD_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', this->A + reg == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + (reg & 0x0F)) > 0x0F);
	setFlag('C', (uint16_t(this->A) + uint16_t(reg)) > 0xFF);

	this->A += reg;
	this->cycles += 4;
}

void CPU::ADD_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	uint16_t address = (uint16_t(*reg1) << 8) + uint16_t(*reg2);
	uint8_t value = readMemory(address);

	// set flags
	setFlag('Z', this->A + value == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + (value & 0x0F)) > 0x0F);
	setFlag('C', (uint16_t(this->A) + uint16_t(value)) > 0xFF);

	this->A += value;
	this->cycles += 8;
}

void CPU::ADD_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	setFlag('Z', this->A + value == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + (value & 0x0F)) > 0x0F);
	setFlag('C', (uint16_t(this->A) + uint16_t(value)) > 0xFF);

	this->A += value;
	this->cycles += 8;
}

void CPU::ADD_SP_e8()
{
	int8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	setFlag('Z', false);
	setFlag('N', false);
	setFlag('H', ((this->SP & 0x0F) + (value & 0x0F)) > 0x0F);
	setFlag('C', uint16_t(this->SP + value) > 0xFF);

	this->SP += value;
	this->cycles += 16;
}

void CPU::ADD_HL_r8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{

}

void CPU::ADD_HL_r16(std::uint16_t* reg)
{
	uint16_t HL_value = (this->H << 8) + this->L;

	setFlag('N', false);
	setFlag('H', ((HL_value & 0x0FFF) + (HL_value & 0x0FFF)) > 0x0FFF);
	setFlag('C', uint16_t(this->SP + HL_value) > 0xFFFF);

	HL_value += *(reg);
	this->H = (HL_value >> 8);
	this->L = HL_value & 0xFF;
}

void CPU::SUB_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', this->A - reg == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < (reg & 0x0F)));
	setFlag('C', this->A < reg);

	this->A -= reg;
	this->cycles += 4;
}

void CPU::SUB_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	uint16_t address = (uint16_t(*reg1) << 8) + uint16_t(*reg2);
	uint8_t value = readMemory(address);

	// set flags
	setFlag('Z', this->A - value == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < (value & 0x0F)));
	setFlag('C', this->A < value);

	this->A -= value;
	this->cycles += 8;
}

void CPU::SUB_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	setFlag('Z', this->A - value == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < (value & 0x0F)));
	setFlag('C', this->A < value);

	this->A -= value;
	this->cycles += 8;
}

void CPU::AND_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', (this->A & reg) == 0);
	setFlag('N', false);
	setFlag('H', true);
	setFlag('C', false);

	this->A &= reg;
	this->cycles += 4;
}

void CPU::AND_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	uint16_t address = (uint16_t(*reg1) << 8) + uint16_t(*reg2);
	uint8_t value = readMemory(address);

	// set flags
	setFlag('Z', (this->A & value) == 0);
	setFlag('N', false);
	setFlag('H', true);
	setFlag('C', false);

	this->A &= value;
	this->cycles += 8;
}

void CPU::AND_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	setFlag('Z', (this->A & value) == 0);
	setFlag('N', false);
	setFlag('H', true);
	setFlag('C', false);

	this->A &= value;
	this->cycles += 8;
}

void CPU::OR_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', (this->A | reg) == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', false);

	this->A |= reg;
	this->cycles += 4;
}

void CPU::OR_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	uint16_t address = (uint16_t(*reg1) << 8) + uint16_t(*reg2);
	uint8_t value = readMemory(address);

	// set flags
	setFlag('Z', (this->A | value) == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', false);

	this->A |= value;
	this->cycles += 4;
}

void CPU::OR_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	setFlag('Z', (this->A | value) == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', false);

	this->A |= value;
	this->cycles += 8;
}

void CPU::XOR_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', (this->A ^ reg) == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', false);

	this->A ^= reg;
	this->cycles += 4;
}

void CPU::XOR_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	uint16_t address = (uint16_t(*reg1) << 8) + uint16_t(*reg2);
	uint8_t value = readMemory(address);

	// set flags
	setFlag('Z', (this->A ^ value) == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', false);

	this->A ^= value;
	this->cycles += 4;
}

void CPU::XOR_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	setFlag('Z', (this->A ^ value) == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', false);

	this->A ^= value;
	this->cycles += 8;
}

void CPU::ADC_A_r8(std::uint8_t reg)
{
	// set flags
	bool cf = C_flag();
	setFlag('Z', this->A + reg + cf == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + ((reg + cf) & 0x0F)) > 0x0F);
	setFlag('C', (uint16_t(this->A) + uint16_t(reg)) + cf > 0xFF);

	this->A += reg + cf;
	this->cycles += 4;
}

void CPU::ADC_A_ar8r8(std::uint8_t reg1, std::uint8_t reg2)
{
	uint16_t address = (uint16_t(reg1) << 8) + uint16_t(reg2);
	uint8_t value = readMemory(address);

	// set flags
	bool cf = C_flag();
	setFlag('Z', this->A + value + cf == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + ((value + cf) & 0x0F)) > 0x0F);
	setFlag('C', (uint16_t(this->A) + uint16_t(value)) + cf > 0xFF);

	this->A += value + cf;
	this->cycles += 8;
}

void CPU::ADC_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	bool cf = C_flag();
	setFlag('Z', this->A + value + cf == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + ((value + cf) & 0x0F)) > 0x0F);
	setFlag('C', (uint16_t(this->A) + uint16_t(value)) + cf > 0xFF);

	this->A += value + cf;
	this->cycles += 8;
}

void CPU::SBC_A_r8(std::uint8_t reg)
{
	// set flags
	bool cf = C_flag();
	setFlag('Z', this->A - reg - cf == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < ((reg - cf) & 0x0F)));
	setFlag('C', this->A < reg);

	this->A -= reg + cf;
	this->cycles += 4;
}

void CPU::SBC_A_ar8r8(std::uint8_t reg1, std::uint8_t reg2)
{
	uint16_t address = (uint16_t(reg1) << 8) + uint16_t(reg2);
	uint8_t value = readMemory(address);

	// set flags
	bool cf = C_flag();
	setFlag('Z', this->A - value - cf == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < ((value - cf) & 0x0F)));
	setFlag('C', this->A < value);

	this->A -= value + cf;
	this->cycles += 8;
}

void CPU::SBC_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	bool cf = C_flag();
	setFlag('Z', this->A - value - cf == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < ((value - cf) & 0x0F)));
	setFlag('C', this->A < value);

	this->A -= value + cf;
	this->cycles += 8;
}

void CPU::CP_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', this->A - reg == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < (reg & 0x0F)));
	setFlag('C', this->A < reg);

	this->cycles += 4;
}

void CPU::CP_A_ar8r8(std::uint8_t reg1, std::uint8_t reg2)
{
	uint16_t address = (uint16_t(reg1) << 8) + uint16_t(reg2);
	uint8_t value = readMemory(address);

	// set flags
	setFlag('Z', this->A - value == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < (value & 0x0F)));
	setFlag('C', this->A < value);

	this->cycles += 8;
}

void CPU::CP_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	setFlag('Z', this->A - value == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < (value & 0x0F)));
	setFlag('C', this->A < value);

	this->cycles += 8;
}

void CPU::INC_r8(std::uint8_t* reg)
{
	// set flags
	setFlag('Z', (*reg) == 0xFF);
	setFlag('N', false);
	setFlag('H', ((*reg) & 0x0F) == 0b1111);

	(*reg)++;
	this->cycles += 4;
}

void CPU::INC_aHL()
{
	uint16_t address = (uint16_t(this->H) << 8) + uint16_t(this->L);
	uint8_t value = readMemory(address);

	// set flags
	setFlag('Z', value == 0xFF);
	setFlag('N', false);
	setFlag('H', (value & 0x0F) == 0b1111);

	value++;
	writeMemory(address, value);
	this->cycles += 12;
}

void CPU::INC_r16(std::uint16_t* reg)
{
	(*reg)++;
	this->cycles += 8;
}

void CPU::INC_r8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	uint16_t number = (uint16_t(*reg1) << 8) + uint16_t(*reg2);
	number++;
	*reg1 = uint8_t(number >> 8);
	*reg2 = uint8_t(number & 0xFF);

	this->cycles += 8;
}

void CPU::DEC_r8(std::uint8_t* reg)
{
	// set flags
	setFlag('Z', (*reg) - 1 == 0);
	setFlag('N', true);
	setFlag('H', ((*reg) & 0x0F) == 0);

	(*reg)--;
	this->cycles += 4;
}

void CPU::DEC_aHL()
{
	uint16_t address = (uint16_t(this->H) << 8) + uint16_t(this->L);
	uint8_t value = readMemory(address);

	// set flags
	setFlag('Z', (value) == 1);
	setFlag('N', true);
	setFlag('H', ((value) & 0x0F) == 0);

	value--;
	writeMemory(address, value);
	this->cycles += 12;
}

void CPU::DEC_r16(std::uint16_t* reg)
{
	(*reg)--;
	this->cycles += 8;
}

void CPU::DEC_r8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	uint16_t number = (uint16_t(*reg1) << 8) + uint16_t(*reg2);
	number--;
	*reg1 = uint8_t(number >> 8);
	*reg2 = uint8_t(number & 0xFF);

	this->cycles += 8;
}

void CPU::PUSH_r8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	this->SP -= 1;
	writeMemory(this->SP, *reg1);	// high
	this->SP -= 1;
	writeMemory(this->SP, *reg2);	// low

	this->cycles += 16;
}

void CPU::PUSH_r16(std::uint16_t reg)
{
	uint8_t high = uint8_t(reg >> 8);
	uint8_t low = uint8_t(reg & 0xFF);

	this->SP -= 1;
	writeMemory(this->SP, high);
	this->SP -= 1;
	writeMemory(this->SP, low);

	this->cycles += 16;
}

void CPU::POP_r8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	*reg2 = readMemory(this->SP);	// low
	this->SP += 1;
	*reg1 = readMemory(this->SP);	// high
	this->SP += 1;

	this->cycles += 12;
}

void CPU::RET()
{
	uint16_t popedValue = uint16_t(readMemory(this->SP));	// low
	this->SP += 1;
	popedValue += uint16_t(readMemory(this->SP)) << 8;	// high
	this->SP += 1;
	
	this->PC = popedValue - 1;
	this->cycles += 16;
}

void CPU::RET_cc(bool cc)
{
	if (cc)
	{
		RET();
		this->cycles += 4;
	}
	else
	{
		this->cycles += 8;
	}
}

void CPU::RETi()
{
	EI();
	RET();
	// EI() + RET() = 20, RETi takes 16
	this->cycles -= 4;
}

void CPU::LDH_a8_A()
{
	uint8_t a8 = readInstruction(this->PC); // read unsigned offset
	this->PC++;

	writeMemory(0xFF00 + uint16_t(a8), this->A);
	this->cycles += 12;
}

void CPU::LDH_A_a8()
{
	uint8_t a8 = readInstruction(this->PC); // read unsigned offset
	this->PC++;

	this->A = readMemory(0xFF00 + uint16_t(a8));
	this->cycles += 12;
}

void CPU::LD_r8_n8(std::uint8_t* reg)
{
	// read value from instruction set
	uint8_t value = readInstruction(this->PC);
	this->PC++;
	*reg = value;
	this->cycles += 8;
}

void CPU::LD_ar8r8_n8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	// read value from instruction set
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// read address
	uint16_t address = (uint16_t(*reg1) << 8) + uint16_t(*reg2);
	writeMemory(address, value);

	this->cycles += 12;
}

void CPU::LD_r8_ar8r8(std::uint8_t* reg1, std::uint8_t areg1, std::uint8_t areg2)
{
	// read address
	uint16_t address = (uint16_t(areg1) << 8) + uint16_t(areg2);
	*reg1 = readMemory(address);

	this->cycles += 8;
}

void CPU::LD_r8r8_n16(std::uint8_t* reg1, std::uint8_t* reg2)
{
	*reg2 = readInstruction(this->PC);	// low
	this->PC += 1;
	*reg1 = readInstruction(this->PC);	// high
	this->PC += 1;

	this->cycles += 12;
}

void CPU::LD_r16_n16(std::uint16_t* reg)
{
	*reg = uint16_t(readInstruction(this->PC));	// low
	this->PC += 1;
	*reg += uint16_t(readInstruction(this->PC)) << 8;	// high
	this->PC += 1;

	this->cycles += 12;
}

void CPU::LD_aHLI_A()
{
	uint16_t address = (uint16_t(this->H) << 8) + uint16_t(this->L);
	writeMemory(address, this->A);
	INC_r8r8(&(this->H), &(this->L));	// cycles updated in this instruction
}

void CPU::LD_aHLD_A()
{
	uint16_t address = (uint16_t(this->H) << 8) + uint16_t(this->L);
	writeMemory(address, this->A);
	DEC_r8r8(&(this->H), &(this->L));	// cycles updated in this instruction
}

void CPU::LD_A_aHLI()
{
	uint16_t address = (uint16_t(this->H) << 8) + uint16_t(this->L);
	this->A = readMemory(address);
	INC_r8r8(&(this->H), &(this->L));	// cycles updated in this instruction
}

void CPU::LD_A_aHLD()
{
	uint16_t address = (uint16_t(this->H) << 8) + uint16_t(this->L);
	this->A = readMemory(address);
	DEC_r8r8(&(this->H), &(this->L));	// cycles updated in this instruction
}

void CPU::LD_ar8r8_r8(std::uint8_t* areg1, std::uint8_t* areg2, std::uint8_t regValue)
{
	uint16_t address = *areg2;
	address += (*areg1) << 8;
	writeMemory(address, regValue);
	this->cycles += 8;
}

void CPU::LD_aa16_A()
{
	// read address from instruction set
	uint16_t address = readInstruction(this->PC);
	this->PC++;
	address += uint16_t(readInstruction(this->PC)) << 8;
	this->PC++;

	writeMemory(address, this->A);
	this->cycles += 16;
}

void CPU::LD_A_aa16()
{
	// read address from instruction set
	uint16_t address = readInstruction(this->PC);
	this->PC++;
	address += uint16_t(readInstruction(this->PC)) << 8;
	this->PC++;

	this->A = readMemory(address);
	this->cycles += 16;
}

void CPU::LD_a16_r16(uint16_t reg)
{
	// read address from instruction set
	uint16_t address = readInstruction(this->PC);
	this->PC++;
	address += uint16_t(readInstruction(this->PC)) << 8;
	this->PC++;

	writeMemory(address, uint8_t(reg >> 8));	// high
	address--;
	writeMemory(address, uint8_t(reg & 0xFF));	// low

	this->cycles += 20;
}

void CPU::LD_HL_SPe8()
{
	// read address from instruction set
	int8_t e8 = readInstruction(this->PC);
	this->PC++;

	// set flags
	setFlag('Z', this->A + e8 == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + (e8 & 0x0F)) > 0x0F);
	setFlag('C', (uint16_t(this->A) + int16_t(e8)) > 0xFF);

	uint16_t value = this->SP + e8;
	uint8_t low = (value & 0xFF);
	uint8_t high = value >> 8;

	this->L = low;
	this->H = high;

	this->cycles += 12;
}

void CPU::LD_SP_HL()
{
	this->SP = (this->H << 8) + this->L;
	this->cycles += 8;
}

void CPU::JR_e8()
{
	int8_t offset = readInstruction(this->PC); // read signed offset
	this->PC++;
	
	this->PC += offset;
	printf("Jump at address %x to address: %x\n", this->PC - offset - 1, this->PC);

	this->cycles += 12;
}
