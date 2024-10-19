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
		LD_R_R(&(this->D), &(this->B));
		break;
	case 0x6:
		LD_R_R(&(this->H), &(this->B));
		break;
	case 0x7:
		
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
	if (this->F < 0b10000000)
	{
		JR_e8();
	}
}

void GPU::JR_NC_e8()
{
	uint8_t f = this->F;
	f <<= 3;
	if (f < 0b10000000)
	{
		JR_e8();
	}
}

void GPU::LD_R_R(std::uint8_t* reg1, std::uint8_t* reg2)
{
	*reg1 = *reg2;
	this->cycles += 4;
}

void GPU::LD_HL_R(std::uint8_t regValue)
{
	uint16_t addressToWrite = uint16_t(this->H) << 8;
	addressToWrite += uint16_t(this->L);

	writeMemory(addressToWrite, regValue);
	this->cycles += 8;
}

void GPU::ADD_A_R(std::uint8_t* reg1)
{
	uint8_t a = this->A;
	this->A += *reg1;

	// set flags
	setFlag('Z', this->A == 0);
	setFlag('N', false);
	
}

void GPU::JR_e8()
{
	uint8_t byte = readInstruction(this->PC); // read signed offset
	this->PC++;
	
	if (byte >= 0b10000000) // if sign bit is 1
	{
		this->PC += uint16_t(byte) + 0xFF00;
	}
	else
	{
		this->PC += uint16_t(byte);
	}

	this->cycles += 12;
}
