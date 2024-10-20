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
		flag values:
		'Z', 'N', 'H', 'C'
	*/
	void setFlag(char flag, bool value);

	// check flags
	bool Z_flag();
	bool N_flag();
	bool H_flag();
	bool C_flag();
	bool NZ_flag();
	bool NN_flag();
	bool NH_flag();
	bool NC_flag();

	/***** CPU operations *****/

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
	void LD_r8_r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Store value in register r8 into the byte pointed to by register HL.
	*/
	void LD_HL_r8(std::uint8_t regValue);

	/*
		Add the value in r8 to A.
	*/
	void ADD_A_r8(std::uint8_t reg);

	/*
		Subtract the value in r8 from A.
	*/
	void SUB_A_r8(std::uint8_t reg);

	/*
		Bitwise AND between the value in r8 and A.
	*/
	void AND_A_r8(std::uint8_t reg);

	/*
		Store into A the bitwise OR of the value in r8 and A.
	*/
	void OR_A_r8(std::uint8_t reg);

	/*
		Push register r16 into the stack.
		reg1 - higher bits
		reg2 - lower bits
	*/
	void PUSH_r8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Pop register r16 from the stack.
		reg1 - higher bits
		reg2 - lower bits
	*/
	void POP_r8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Return from subroutine. This is basically a POP PC (if such an instruction existed).
		See POP r16 for an explanation of how POP works.
	*/
	void RET();

	/*
		Return from subroutine if condition NZ is met.
	*/
	void RET_NZ();

	/*
		Return from subroutine if condition NC is met.
	*/
	void RET_NC();

	/*
		Store value in register A into the byte at address $FF00+a8.
	*/
	void LDH_a8_A();

	/*
		Store byte value at address $FF00+a8 into the A register.
	*/
	void LDH_A_a8();

	/*
		Relative Jump to address e8.
		The address is encoded as a signed 8-bit offset from the address immediately following the JR instruction, so the target address e8 must be between -128 and 127 bytes away.
	*/
	void JR_e8();
};

