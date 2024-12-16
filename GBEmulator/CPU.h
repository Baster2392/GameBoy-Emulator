#pragma once

#include <cstddef>
#include <cstdint>
#include <chrono>
#include <thread>
#include <functional>
#include <array>
#include <memory>
#include "MMU.h"
#include "GPU.h"
#include "Renderer.h"
#include "KeyboardHandler.h"

class CPU
{
public:
	// 8-bit registers
	std::uint8_t A, B, C, D, E, H, L;
	/*
		Flag register architecture:
		msb -> [Z, N, H, C, 0, 0, 0, 0] <- lsb
		Z - zero flag
		N - Subtract flag
		H - Half carry flag
		C - Carry flag
		0 - just zero value
	*/
	std::uint8_t F;
	// stack pointer, program counter
	std::uint16_t SP, PC;
	// Interrupt master enable flag
	std::uint8_t IME;
	// cycles counter
	int cycles;

	// Keyboard handler
	shared_ptr<KeyboardHandler> keyboardHandler = make_shared<KeyboardHandler>();
	// Memory Management Unit
	MMU mmu = MMU(keyboardHandler);
	// Graphic Processing Unit
	GPU gpu = GPU(&mmu);
	// Renderer
	Renderer renderer;

	// others
	bool debugMode;

	CPU(bool debugMode);

	// Opcode table
	static constexpr size_t OPCODE_COUNT = 256;
	std::array<std::function<void()>, OPCODE_COUNT> opcodeTable, opcodeTableBitOperations;
	void initializeOpcodeTable();

	// CPU actions
	void executeOpcode(std::uint8_t opcode);
	void handlePrefix();
	std::uint8_t readMemory(std::uint16_t addressToRead);
	std::uint8_t readInstruction(std::uint16_t addressToRead);
	void writeMemory(std::uint16_t addressToWrite, std::uint8_t value);
	/*
		flag values:
		'Z', 'N', 'H', 'C'
	*/
	void setFlag(char flag, bool value);

	void step();

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

	/*
		Add the value in r8 to A.
	*/
	void ADD_A_r8(std::uint8_t reg);

	/*
		Add the value in memory pointed by reg1reg2 to A.
	*/
	void ADD_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Add the immediate value n8 to A.
	*/
	void ADD_A_n8();

	/*
		Add the signed value e8 to SP.
	*/
	void ADD_SP_e8();

	/*
		Add the value in r8r8 to HL.
	*/
	void ADD_HL_r8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Add the value in r16 to HL.
	*/
	void ADD_HL_r16(std::uint16_t* reg);

	/*
		Subtract the value in r8 from A.
	*/
	void SUB_A_r8(std::uint8_t reg);

	/*
		Subtract the value in memory pointed by reg1reg2 to A.
	*/
	void SUB_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Subtract the immediate value n8 from A.
	*/
	void SUB_A_n8();

	/*
		Bitwise AND between the value in r8 and A.
	*/
	void AND_A_r8(std::uint8_t reg);

	/*
		Bitwise AND between the value in memory pointed by reg1reg2 and A.
	*/
	void AND_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Bitwise AND between the immediate value n8 and A.
	*/
	void AND_A_n8();

	/*
		Store into A the bitwise OR of the value in r8 and A.
	*/
	void OR_A_r8(std::uint8_t reg);

	/*
		Bitwise OR between the value in memory pointed by reg1reg2 and A.
	*/
	void OR_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Bitwise OR between the immediate value n8 and A.
	*/
	void OR_A_n8();

	/*
		Store into A the bitwise XOR of the value in r8 and A.
	*/
	void XOR_A_r8(std::uint8_t reg);

	/*
		Bitwise XOR between the value in memory pointed by reg1reg2 and A.
	*/
	void XOR_A_ar8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Bitwise XOR between the immediate value n8 and A.
	*/
	void XOR_A_n8();

	/*
		Add the value in r8 plus the carry flag to A.
	*/
	void ADC_A_r8(std::uint8_t reg);

	/*
		Add the value in memory pointed by ar8r8 plus the carry flag to A.
	*/
	void ADC_A_ar8r8(std::uint8_t reg1, std::uint8_t reg2);

	/*
		Add the value n8 plus the carry flag to A.
	*/
	void ADC_A_n8();

	/*
		Subtract the value in r8 minus the carry flag to A.
	*/
	void SBC_A_r8(std::uint8_t reg);

	/*
		Subtract the value in memory pointed by ar8r8 minus the carry flag to A.
	*/
	void SBC_A_ar8r8(std::uint8_t reg1, std::uint8_t reg2);

	/*
		Subtract the value n8 minuus the carry flag to A.
	*/
	void SBC_A_n8();

	/*
		Subtract the value in r8 but not store it in A register.
	*/
	void CP_A_r8(std::uint8_t reg);

	/*
		Subtract the value in memory pointed by ar8r8 but not store it in A register.
	*/
	void CP_A_ar8r8(std::uint8_t reg1, std::uint8_t reg2);

	/*
		Subtract the value n8 but not store it in A register.
	*/
	void CP_A_n8();

	/*
		Increment value in register r8 by 1.
	*/
	void INC_r8(std::uint8_t* reg);

	/*
		Increment the byte pointed by HL by 1.
	*/
	void INC_aHL();

	/*
		Increment value in register reg by 1.
	*/
	void INC_r16(std::uint16_t* reg);

	/*
		Increment value in registers reg1 and reg2 by 1.
	*/
	void INC_r8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Decrement value in register r8 by 1.
	*/
	void DEC_r8(std::uint8_t* reg);

	/*
		Decrement the byte pointed to by HL by 1.
	*/
	void DEC_aHL();

	/*
		Decrement value in register reg by 1.
	*/
	void DEC_r16(std::uint16_t* reg);

	/*
		Decrement value in registers reg1 and reg2 by 1.
	*/
	void DEC_r8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Push register r16 into the stack.
		reg1 - higher bits
		reg2 - lower bits
	*/
	void PUSH_r8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Push register r16 into the stack.
		reg1 - higher bits
		reg2 - lower bits
	*/
	void PUSH_r16(std::uint16_t reg);

	/*
		Pop register r16 from the stack.
		reg1 - higher bits
		reg2 - lower bits
	*/
	void POP_r8r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Pop register AF from the stack and sets .
	*/

	/*
		Return from subroutine. This is basically a POP PC (if such an instruction existed).
		See POP r16 for an explanation of how POP works.
	*/
	void RET();

	/*
		Return from subroutine if condition cc is met.
	*/
	void RET_cc(bool cc);

	/*
		Return from subroutine and enable interrupts. This is basically equivalent to executing EI then RET, meaning that IME is set right after this instruction.
	*/
	void RETi();

	/*** Load instructions ***/

	/*
		Load (copy) value in register on the right into register on the left.
	*/
	void LD_r8_r8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Store value in register r8 into the byte pointed to by register HL.
	*/
	void LD_HL_r8(std::uint8_t regValue);

	/*
		Store value in register A into the byte at address $FF00+a8.
	*/
	void LDH_a8_A();

	/*
		Store byte value at address $FF00+a8 into the A register.
	*/
	void LDH_A_a8();

	/*
		Load value n8 into register r8.
	*/
	void LD_r8_n8(std::uint8_t* reg);

	/*
		Load value n8 into memory pointed by reg1reg2.
	*/
	void LD_ar8r8_n8(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Load value from memory pointed by areg1areg2 to r8.
	*/
	void LD_r8_ar8r8(std::uint8_t* reg1, std::uint8_t areg1, std::uint8_t areg2);

	/*
		Store little-endian value n16 to two reg8 register.
		reg1 - higher bits
		reg2 - lower bits
	*/
	void LD_r8r8_n16(std::uint8_t* reg1, std::uint8_t* reg2);

	/*
		Store little-endian value n16 to reg16 register.
	*/
	void LD_r16_n16(std::uint16_t* reg);

	/*
		Store value in register A into the byte pointed by HL and increment HL afterwards.
	*/
	void LD_aHLI_A();

	/*
		Store value in register A into the byte pointed by HL and decrement HL afterwards.
	*/
	void LD_aHLD_A();

	/*
		Load value into register A from the byte pointed by HL and increment HL afterwards.
	*/
	void LD_A_aHLI();

	/*
		Load value into register A from the byte pointed by HL and decrement HL afterwards.
	*/
	void LD_A_aHLD();

	/*
		Store value in regValue into memory at address stored in areg1 and areg2.
		areg1 - higher bits
		areg2 - lower bits
	*/
	void LD_ar8r8_r8(std::uint8_t* areg1, std::uint8_t* areg2, std::uint8_t regValue);

	/*
		Store value in register A into the byte at address n16.
	*/
	void LD_aa16_A();

	/*
		Store value in the byte at address n16 register A into register A.
	*/
	void LD_A_aa16();

	/*
		Load value stored in register r16 to memory pointed by a16
	*/
	void LD_a16_r16(std::uint16_t reg);

	/*
		Add the signed value e8 to SP and store the result in HL.
	*/
	void LD_HL_SPe8();

	/*
		Load register HL into register SP.
	*/
	void LD_SP_HL();

	/*
		Store value in register A into the byte at address $FF00+C.
	*/
	void LDH_ar8_A(uint8_t* reg);

	/*
		Load value in register A from the byte at address $FF00+c.
	*/
	void LDH_A_ar8(uint8_t* reg);

	/*** Jump, call instructions ***/

	/*
		Relative Jump to address e8.
		The address is encoded as a signed 8-bit offset from the address immediately following the JR instruction, so the target address e8 must be between -128 and 127 bytes away.
	*/
	void JR_e8();

	/*
		Relative Jump to address e8 if condition cc is met.
		The address is encoded as a signed 8-bit offset from the address immediately following the JR instruction, so the target address e8 must be between -128 and 127 bytes away.
	*/
	void JR_cc_e8(bool cc_func);

	/*
		Jump to address a16.
	*/
	void JP_a16();

	/*
		Jump to address a16 if condition cc is met.
	*/
	void JP_cc_a16(bool cc);

	/*
		Jump to address in HL; effectively, load PC with value in register HL.
	*/
	void JP_HL();

	/*
		Call address a16. This pushes the address of the instruction after the CALL on the stack, such that RET can pop it later; then, it executes an implicit JP n16.
	*/
	void CALL();

	/*
		Call address a16 if condition cc is met.
	*/
	void CALL_cc_a16(bool cc);

	/*
		Call address vec. This is a shorter and faster equivalent to CALL for suitable values of vec.
	*/
	void RST(std::uint16_t vectorAddress);

	/*** Miscellaneous Instructions ***/

	/*
		Complement Carry Flag.
	*/
	void CCF();

	/*
		ComPLement accumulator (A = ~A).
	*/
	void CPL();

	/*
		Decimal Adjust Accumulator to get a correct BCD representation after an arithmetic instruction.
	*/
	void DAA();

	/*
		Disable Interrupts by clearing the IME flag.
	*/
	void DI();

	/*
		Enable Interrupts by setting the IME flag. The flag is only set after the instruction following EI.
	*/
	void EI();

	/*
		Enter CPU low-power consumption mode until an interrupt occurs. The exact behavior of this instruction depends on the state of the IME flag.
	*/
	void HALT();

	/*
		No operation.
	*/
	void NOP();

	/*
		Set Carry Flag.
	*/
	void SCF();

	/*
		Enter CPU very low power mode.
		Also used to switch between double and normal speed CPU modes in GBC.
	*/
	void STOP();

	/***** Bit Instructions *****/

	/*
		Rotate register A left and set carry flag.
	*/
	void RLCA();

	/*
		Rotate register A left, through the carry flag.
	*/
	void RLA();

	/*
		Rotate register A right and set carry flag.
	*/
	void RRCA();

	/*
		Rotate register A right with carry flag.
	*/
	void RRA();

	/*
	Rotate register r8 left.
*/
	void RLC_r8(std::uint8_t* reg);

	/*
		Rotate register C + r8 left.
	*/
	void RL_r8(std::uint8_t* reg);

	/*
		Rotate register r8 right, through the carry flag.
	*/
	void RR_r8(uint8_t* reg);

	/*
		Shift Right Logically register r8.
	*/
	void SRL_r8(uint8_t* reg);

	/*
		Shift Left Arithmetically register r8.
	*/
	void SLA_r8(uint8_t* reg);

	/*
		Check bit n in reg register
	*/
	void BIT_n_r8(uint8_t n, uint8_t* reg);

	/*
		Check bit n in byte at HL address
	*/
	void BIT_n_aHL(uint8_t n);

	/*
		Set bit n in reg register
	*/
	void SET_n_r8(uint8_t n, uint8_t* reg);

	/*
		Reset bit n in reg register
	*/
	void RES_n_r8(uint8_t n, uint8_t* reg);

	/*
		Reset bit n in byte at HL address
	*/
	void RES_n_aHL(uint8_t n);

	/*
		Swap the upper 4 bits in register r8 and the lower 4 ones.
	*/
	void SWAP_r8(uint8_t* reg);
};

