#pragma once

#include <cstddef>
#include <cstdint>

class GPU
{
	// 8-bit registers
	std::uint8_t A, B, C, D, E, H, L;
	/*
	* Flag register architecture:
	* msb -> [Z, N, H, C, 0, 0, 0, 0] <- lsb
	* Z - zero flag
	* N - Subtract flag
	* H - Half carry flag
	* C - Carry flag
	* 0 - just zero value
	*/
	std::uint8_t F;
	// stack pointer, program counter
	std::uint16_t SP, PC;
	// cycles counter
	int cycles;

public:
	void executeOpcode(std::uint8_t opcode);
	std::uint8_t readMemory(std::uint16_t addressToRead);
	std::uint8_t readInstruction(std::uint16_t addressToRead);
	void writeMemory(std::uint16_t addressToWrite, std::uint8_t value);
	/*
	* flag values:
	* 'Z', 'N', 'H', 'C'
	*/
	void setFlag(char flag, bool value);

	// CPU operations

	// No operation.
	void NOP();

	/*
		Enter CPU very low power mode.
		Also used to switch between double and normal speed CPU modes in GBC.
	*/
	void STOP();

	/*
		Relative Jump to address e8 if zero flag is set to 1.
		The address is encoded as a signed 8-bit offset from the address immediately following the JR instruction, so the target address e8 must be between -128 and 127 bytes away.
	*/
	void JR_NZ_e8();

	/*
		Relative Jump to address e8 if carry flag is set to 1.
		The address is encoded as a signed 8-bit offset from the address immediately following the JR instruction, so the target address e8 must be between -128 and 127 bytes away.
	*/
	void JR_NC_e8();

	/*
		Load (copy) value in register on the right into register on the left.
	*/
	void LD_R_R(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Store value in register r8 into the byte pointed to by register HL.
	*/
	void LD_HL_R(std::uint8_t regValue);

	/*
		Add the value in r8 to A.
	*/
	void ADD_A_R(std::uint8_t* reg1);

	/*
		Relative Jump to address e8.
		The address is encoded as a signed 8-bit offset from the address immediately following the JR instruction, so the target address e8 must be between -128 and 127 bytes away.
	*/
	void JR_e8();
};

