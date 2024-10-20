#include "GPU.h"

void GPU::executeOpcode(std::uint8_t opcode)
{
	uint8_t byte;
	switch (opcode)
	{
	case 0x0:
		NOP();
		break;
	case 0x1:
		STOP();
		break;
	case 0x2:
		JR_NZ_e8();
		break;
	case 0x3:
		JR_NC_e8();
		break;
	case 0x4: // LD B,B = NOP
		NOP();
		break;
	case 0x5:
		LD_r8_r8(&(this->D), &(this->B));
		break;
	case 0x6:
		LD_r8_r8(&(this->H), &(this->B));
		break;
	case 0x7:
		LD_HL_r8(this->B);
		break;
	case 0x8:
		ADD_A_r8(this->B);
		break;
	case 0x9:
		SUB_A_r8(this->B);
		break;
	case 0xA:
		AND_A_r8(this->B);
		break;
	case 0xB:
		OR_A_r8(this->B);
		break;
	case 0xC:
		RET_NZ();
		break;
	case 0xD:
		RET_NC();
		break;
	default:
		break;
	}
}

std::uint8_t GPU::readMemory(std::uint16_t addressToRead)
{
	// TODO: read memory
	return std::uint8_t();
}

std::uint8_t GPU::readInstruction(std::uint16_t addressToRead)
{
	// TODO: read intruction
	return std::uint8_t();
}

void GPU::writeMemory(std::uint16_t addressToWrite, std::uint8_t value)
{
	// TODO: write value to memory
}

void GPU::setFlag(char flag, bool value)
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

bool GPU::Z_flag()
{
	return this->F < 0b10000000;
}

bool GPU::N_flag()
{
	return (this->F << 1) < 0b10000000;
}

bool GPU::H_flag()
{
	return (this->F << 2) < 0b10000000;
}

bool GPU::C_flag()
{
	return (this->F << 3) < 0b10000000;
}

bool GPU::NZ_flag()
{
	return !Z_flag();
}

bool GPU::NN_flag()
{
	return !N_flag();
}

bool GPU::NH_flag()
{
	return !H_flag();
}

bool GPU::NC_flag()
{
	return !C_flag();
}

void GPU::NOP()
{
	this->cycles += 4;
}

void GPU::STOP()
{
	this->cycles += 0; // idk
}

void GPU::JR_NZ_e8()
{
	if (NZ_flag())
	{
		JR_e8();
	}
	else
	{
		this->cycles += 8;
	}
}

void GPU::JR_NC_e8()
{
	if (NC_flag())
	{
		JR_e8();
	}
	else
	{
		this->cycles += 8;
	}
}

void GPU::LD_r8_r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	*reg1 = *reg2;
	this->cycles += 4;
}

void GPU::LD_HL_r8(std::uint8_t regValue)
{
	uint16_t addressToWrite = uint16_t(this->H) << 8;
	addressToWrite += uint16_t(this->L);

	writeMemory(addressToWrite, regValue);
	this->cycles += 8;
}

void GPU::ADD_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', this->A + reg == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + (reg & 0x0F)) > 0x0F);
	setFlag('C', (uint16_t(this->A) + uint16_t(reg)) > 0xFF);

	this->A += reg;
	this->cycles += 4;
}

void GPU::SUB_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', this->A - reg == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) < (reg & 0x0F)));
	setFlag('C', this->A < reg);

	this->A -= reg;
	this->cycles += 4;
}

void GPU::AND_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', (this->A & reg) == 0);
	setFlag('N', false);
	setFlag('H', true);
	setFlag('C', false);

	this->A &= reg;
	this->cycles += 4;
}

void GPU::OR_A_r8(std::uint8_t reg)
{
	// set flags
	setFlag('Z', (this->A | reg) == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', false);

	this->A |= reg;
	this->cycles += 4;
}

void GPU::PUSH_r8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	this->SP -= 1;
	writeMemory(this->SP, *reg1);	// high
	this->SP -= 1;
	writeMemory(this->SP, *reg2);	// low

	this->cycles += 16;
}

void GPU::POP_r8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	*reg2 = readMemory(this->SP);	// low
	this->SP += 1;
	*reg1 = readMemory(this->SP);	// high
	this->SP += 1;

	this->cycles += 12;
}

void GPU::RET()
{
	uint16_t popedValue = uint16_t(readMemory(this->SP));	// low
	this->SP += 1;
	popedValue += uint16_t(readMemory(this->SP)) << 8;	// high
	this->SP += 1;

	this->cycles += 16;
}

void GPU::RET_NZ()
{
	if (NZ_flag())
	{
		RET();
		this->cycles += 4;
	}
	else
	{
		this->cycles += 8;
	}
}

void GPU::RET_NC()
{
	if (NC_flag())
	{
		RET();
		this->cycles += 4;
	}
	else
	{
		this->cycles += 8;
	}
}

void GPU::LDH_a8_A()
{
	uint8_t a8 = readInstruction(this->PC); // read unsigned offset
	this->PC++;

	writeMemory(0xFF00 + uint16_t(a8), this->A);
	this->cycles += 12;
}

void GPU::LDH_A_a8()
{
	uint8_t a8 = readInstruction(this->PC); // read unsigned offset
	this->PC++;

	this->A = readMemory(0xFF00 + uint16_t(a8));
	this->cycles += 12;
}

void GPU::JR_e8()
{
	uint8_t offset = readInstruction(this->PC); // read signed offset
	this->PC++;
	
	if (offset >= 0b10000000) // if sign bit is 1
	{
		this->PC += uint16_t(offset) + 0xFF00;
	}
	else
	{
		this->PC += uint16_t(offset);
	}

	this->cycles += 12;
}
