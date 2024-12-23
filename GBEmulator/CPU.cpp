#include "CPU.h"

CPU::CPU(bool debugMode)
{
	this->debugMode = debugMode;
	this->initializeOpcodeTable();
}

void CPU::initializeOpcodeTable() {
	opcodeTable.fill([this]() { printf("Unknown opcode: %x\n", readInstruction(this->PC - 1)); });
	opcodeTableBitOperations.fill([this]() {
		printf("Unknown opcode with prefix: %x\n", readInstruction(this->PC - 1));
		});

	opcodeTable[0x00] = [this]() { NOP(); };
	opcodeTable[0x10] = [this]() { STOP(); };
	opcodeTable[0x20] = [this]() { JR_cc_e8(NZ_flag()); };
	opcodeTable[0x30] = [this]() { JR_cc_e8(NC_flag()); };
	opcodeTable[0x40] = [this]() { NOP(); };
	opcodeTable[0x50] = [this]() { LD_r8_r8(&(this->D), &(this->B)); };
	opcodeTable[0x60] = [this]() { LD_r8_r8(&(this->H), &(this->B)); };
	opcodeTable[0x70] = [this]() { LD_HL_r8(this->B); };
	opcodeTable[0x80] = [this]() { ADD_A_r8(this->B); };
	opcodeTable[0x90] = [this]() { SUB_A_r8(this->B); };
	opcodeTable[0xA0] = [this]() { AND_A_r8(this->B); };
	opcodeTable[0xB0] = [this]() { OR_A_r8(this->B); };
	opcodeTable[0xC0] = [this]() { RET_cc(NZ_flag()); };
	opcodeTable[0xD0] = [this]() { RET_cc(NC_flag()); };
	opcodeTable[0xE0] = [this]() { LDH_a8_A(); };
	opcodeTable[0xF0] = [this]() { LDH_A_a8(); };
	opcodeTable[0x01] = [this]() { LD_r8r8_n16(&(this->B), &(this->C)); };
	opcodeTable[0x11] = [this]() { LD_r8r8_n16(&(this->D), &(this->E)); };
	opcodeTable[0x21] = [this]() { LD_r8r8_n16(&(this->H), &(this->L)); };
	opcodeTable[0x31] = [this]() { LD_r16_n16(&(this->SP)); };
	opcodeTable[0x41] = [this]() { LD_r8_r8(&(this->B), &(this->C)); };
	opcodeTable[0x51] = [this]() { LD_r8_r8(&(this->D), &(this->C)); };
	opcodeTable[0x61] = [this]() { LD_r8_r8(&(this->H), &(this->C)); };
	opcodeTable[0x71] = [this]() { LD_HL_r8(this->C); };
	opcodeTable[0x81] = [this]() { ADD_A_r8(this->C); };
	opcodeTable[0x91] = [this]() { SUB_A_r8(this->C); };
	opcodeTable[0xA1] = [this]() { AND_A_r8(this->C); };
	opcodeTable[0xB1] = [this]() { OR_A_r8(this->C); };
	opcodeTable[0xC1] = [this]() { POP_r8r8(&(this->B), &(this->C)); };
	opcodeTable[0xD1] = [this]() { POP_r8r8(&(this->D), &(this->E)); };
	opcodeTable[0xE1] = [this]() { POP_r8r8(&(this->H), &(this->L)); };
	opcodeTable[0xF1] = [this]() { POP_r8r8(&(this->A), &(this->F)); };
	opcodeTable[0x02] = [this]() { LD_ar8r8_r8(&(this->B), &(this->C), this->A); };
	opcodeTable[0x12] = [this]() { LD_ar8r8_r8(&(this->D), &(this->E), this->A); };
	opcodeTable[0x22] = [this]() { LD_aHLI_A(); };
	opcodeTable[0x32] = [this]() { LD_aHLD_A(); };
	opcodeTable[0x42] = [this]() { LD_r8_r8(&(this->B), &(this->D)); };
	opcodeTable[0x52] = [this]() { NOP(); };
	opcodeTable[0x62] = [this]() { LD_r8_r8(&(this->H), &(this->D)); };
	opcodeTable[0x72] = [this]() { LD_ar8r8_r8(&(this->H), &(this->L), this->D); };
	opcodeTable[0x82] = [this]() { ADD_A_r8(this->D); };
	opcodeTable[0x92] = [this]() { SUB_A_r8(this->D); };
	opcodeTable[0xA2] = [this]() { AND_A_r8(this->D); };
	opcodeTable[0xB2] = [this]() { OR_A_r8(this->D); };
	opcodeTable[0xC2] = [this]() { JP_cc_a16(NZ_flag()); };
	opcodeTable[0xD2] = [this]() { JP_cc_a16(NC_flag()); };
	opcodeTable[0xE2] = [this]() { LDH_ar8_A(&(this->C)); };
	opcodeTable[0xF2] = [this]() { LDH_A_ar8(&(this->C)); };
	opcodeTable[0x03] = [this]() { INC_r8r8(&(this->B), &(this->C)); };
	opcodeTable[0x13] = [this]() { INC_r8r8(&(this->D), &(this->E)); };
	opcodeTable[0x23] = [this]() { INC_r8r8(&(this->H), &(this->L)); };
	opcodeTable[0x33] = [this]() { INC_r16(&(this->SP)); };
	opcodeTable[0x43] = [this]() { LD_r8_r8(&(this->B), &(this->E)); };
	opcodeTable[0x53] = [this]() { LD_r8_r8(&(this->D), &(this->E)); };
	opcodeTable[0x63] = [this]() { LD_r8_r8(&(this->H), &(this->E)); };
	opcodeTable[0x73] = [this]() { LD_ar8r8_r8(&(this->H), &(this->L), this->E); };
	opcodeTable[0x83] = [this]() { ADD_A_r8(this->E); };
	opcodeTable[0x93] = [this]() { SUB_A_r8(this->E); };
	opcodeTable[0xA3] = [this]() { AND_A_r8(this->E); };
	opcodeTable[0xB3] = [this]() { OR_A_r8(this->E); };
	opcodeTable[0xC3] = [this]() { JP_a16(); };
	opcodeTable[0xF3] = [this]() { DI(); };
	opcodeTable[0x04] = [this]() { INC_r8(&(this->B)); };
	opcodeTable[0x14] = [this]() { INC_r8(&(this->D)); };
	opcodeTable[0x24] = [this]() { INC_r8(&(this->H)); };
	opcodeTable[0x34] = [this]() { INC_aHL(); };
	opcodeTable[0x44] = [this]() { LD_r8_r8(&(this->B), &(this->H)); };
	opcodeTable[0x54] = [this]() { LD_r8_r8(&(this->D), &(this->H)); };
	opcodeTable[0x64] = [this]() { LD_r8_r8(&(this->H), &(this->H)); };
	opcodeTable[0x74] = [this]() { LD_ar8r8_r8(&(this->H), &(this->L), this->H); };
	opcodeTable[0x84] = [this]() { ADD_A_r8(this->H); };
	opcodeTable[0x94] = [this]() { SUB_A_r8(this->H); };
	opcodeTable[0xA4] = [this]() { AND_A_r8(this->H); };
	opcodeTable[0xB4] = [this]() { OR_A_r8(this->H); };
	opcodeTable[0xC4] = [this]() { CALL_cc_a16(NZ_flag()); };
	opcodeTable[0xD4] = [this]() { CALL_cc_a16(NC_flag()); };
	opcodeTable[0x05] = [this]() { DEC_r8(&(this->B)); };
	opcodeTable[0x15] = [this]() { DEC_r8(&(this->D)); };
	opcodeTable[0x25] = [this]() { DEC_r8(&(this->H)); };
	opcodeTable[0x35] = [this]() { DEC_aHL(); };
	opcodeTable[0x45] = [this]() { LD_r8_r8(&(this->B), &(this->L)); };
	opcodeTable[0x55] = [this]() { LD_r8_r8(&(this->D), &(this->L)); };
	opcodeTable[0x65] = [this]() { LD_r8_r8(&(this->H), &(this->L)); };
	opcodeTable[0x75] = [this]() { LD_ar8r8_r8(&(this->H), &(this->L), this->L); };
	opcodeTable[0x85] = [this]() { ADD_A_r8(this->L); };
	opcodeTable[0x95] = [this]() { SUB_A_r8(this->L); };
	opcodeTable[0xA5] = [this]() { AND_A_r8(this->L); };
	opcodeTable[0xB5] = [this]() { OR_A_r8(this->L); };
	opcodeTable[0xC5] = [this]() { PUSH_r8r8(&(this->B), &(this->C)); };
	opcodeTable[0xD5] = [this]() { PUSH_r8r8(&(this->D), &(this->E)); };
	opcodeTable[0xE5] = [this]() { PUSH_r8r8(&(this->H), &(this->L)); };
	opcodeTable[0xF5] = [this]() { PUSH_r8r8(&(this->A), &(this->F)); };
	opcodeTable[0x06] = [this]() { LD_r8_n8(&(this->B)); };
	opcodeTable[0x16] = [this]() { LD_r8_n8(&(this->D)); };
	opcodeTable[0x26] = [this]() { LD_r8_n8(&(this->H)); };
	opcodeTable[0x36] = [this]() { LD_ar8r8_n8(&(this->H), &(this->L)); };
	opcodeTable[0x46] = [this]() { LD_r8_ar8r8(&(this->B), this->H, this->L); };
	opcodeTable[0x56] = [this]() { LD_r8_ar8r8(&(this->D), this->H, this->L); };
	opcodeTable[0x66] = [this]() { LD_r8_ar8r8(&(this->H), this->H, this->L); };
	opcodeTable[0x76] = [this]() { HALT(); };
	opcodeTable[0x86] = [this]() { ADD_A_ar8r8(&(this->H), &(this->L)); };
	opcodeTable[0x96] = [this]() { SUB_A_ar8r8(&(this->H), &(this->L)); };
	opcodeTable[0xA6] = [this]() { AND_A_ar8r8(&(this->H), &(this->L)); };
	opcodeTable[0xB6] = [this]() { OR_A_ar8r8(&(this->H), &(this->L)); };
	opcodeTable[0xC6] = [this]() { ADD_A_n8(); };
	opcodeTable[0xD6] = [this]() { SUB_A_n8(); };
	opcodeTable[0xE6] = [this]() { AND_A_n8(); };
	opcodeTable[0xF6] = [this]() { OR_A_n8(); };
	opcodeTable[0x07] = [this]() { RLCA(); };
	opcodeTable[0x17] = [this]() { RLA(); };
	opcodeTable[0x27] = [this]() { DAA(); };
	opcodeTable[0x37] = [this]() { SCF(); };
	opcodeTable[0x47] = [this]() { LD_r8_r8(&(this->B), &(this->A)); };
	opcodeTable[0x57] = [this]() { LD_r8_r8(&(this->D), &(this->A)); };
	opcodeTable[0x67] = [this]() { LD_r8_r8(&(this->H), &(this->A)); };
	opcodeTable[0x77] = [this]() { LD_ar8r8_r8(&(this->H), &(this->L), this->A); };
	opcodeTable[0x87] = [this]() { ADD_A_r8(this->A); };
	opcodeTable[0x97] = [this]() { SUB_A_r8(this->A); };
	opcodeTable[0xA7] = [this]() { AND_A_r8(this->A); };
	opcodeTable[0xB7] = [this]() { OR_A_r8(this->A); };
	opcodeTable[0xC7] = [this]() { RST(0x00); };
	opcodeTable[0xD7] = [this]() { RST(0x10); };
	opcodeTable[0xE7] = [this]() { RST(0x20); };
	opcodeTable[0xF7] = [this]() { RST(0x30); };
	opcodeTable[0x08] = [this]() { LD_a16_r16(this->SP); };
	opcodeTable[0x18] = [this]() { JR_e8(); };
	opcodeTable[0x28] = [this]() { JR_cc_e8(Z_flag()); };
	opcodeTable[0x38] = [this]() { JR_cc_e8(C_flag()); };
	opcodeTable[0x48] = [this]() { LD_r8_r8(&(this->C), &(this->B)); };
	opcodeTable[0x58] = [this]() { LD_r8_r8(&(this->E), &(this->B)); };
	opcodeTable[0x68] = [this]() { LD_r8_r8(&(this->L), &(this->B)); };
	opcodeTable[0x78] = [this]() { LD_r8_r8(&(this->A), &(this->B)); };
	opcodeTable[0x88] = [this]() { ADC_A_r8(this->B); };
	opcodeTable[0x98] = [this]() { SBC_A_r8(this->B); };
	opcodeTable[0xA8] = [this]() { XOR_A_r8(this->B); };
	opcodeTable[0xB8] = [this]() { CP_A_r8(this->B); };
	opcodeTable[0xC8] = [this]() { RET_cc(Z_flag()); };
	opcodeTable[0xD8] = [this]() { RET_cc(C_flag()); };
	opcodeTable[0xE8] = [this]() { ADD_SP_e8(); };
	opcodeTable[0xF8] = [this]() { LD_HL_SPe8(); };
	opcodeTable[0x09] = [this]() { ADD_HL_r8r8(&(this->B), &(this->C)); };
	opcodeTable[0x19] = [this]() { ADD_HL_r8r8(&(this->D), &(this->E)); };
	opcodeTable[0x29] = [this]() { ADD_HL_r8r8(&(this->H), &(this->L)); };
	opcodeTable[0x39] = [this]() { ADD_HL_r16(&(this->SP)); };
	opcodeTable[0x49] = [this]() { NOP(); }; // LD C, C
	opcodeTable[0x59] = [this]() { LD_r8_r8(&(this->E), &(this->C)); };
	opcodeTable[0x69] = [this]() { LD_r8_r8(&(this->L), &(this->C)); };
	opcodeTable[0x79] = [this]() { LD_r8_r8(&(this->A), &(this->C)); };
	opcodeTable[0x89] = [this]() { ADC_A_r8(this->C); };
	opcodeTable[0x99] = [this]() { SBC_A_r8(this->C); };
	opcodeTable[0xA9] = [this]() { XOR_A_r8(this->C); };
	opcodeTable[0xB9] = [this]() { CP_A_r8(this->C); };
	opcodeTable[0xC9] = [this]() { RET(); };
	opcodeTable[0xD9] = [this]() { RETi(); };
	opcodeTable[0xE9] = [this]() { JP_HL(); };
	opcodeTable[0xF9] = [this]() { LD_SP_HL(); };
	opcodeTable[0x0A] = [this]() { LD_r8_ar8r8(&(this->A), this->B, this->C); };
	opcodeTable[0x1A] = [this]() { LD_r8_ar8r8(&(this->A), this->D, this->E); };
	opcodeTable[0x2A] = [this]() { LD_A_aHLI(); };
	opcodeTable[0x3A] = [this]() { LD_A_aHLD(); };
	opcodeTable[0x4A] = [this]() { LD_r8_r8(&(this->C), &(this->D)); };
	opcodeTable[0x5A] = [this]() { LD_r8_r8(&(this->E), &(this->D)); };
	opcodeTable[0x6A] = [this]() { LD_r8_r8(&(this->L), &(this->D)); };
	opcodeTable[0x7A] = [this]() { LD_r8_r8(&(this->A), &(this->D)); };
	opcodeTable[0x8A] = [this]() { ADC_A_r8(this->D); };
	opcodeTable[0x9A] = [this]() { SBC_A_r8(this->D); };
	opcodeTable[0xAA] = [this]() { XOR_A_r8(this->D); };
	opcodeTable[0xBA] = [this]() { CP_A_r8(this->D); };
	opcodeTable[0xCA] = [this]() { JP_cc_a16(this->Z_flag()); };
	opcodeTable[0xDA] = [this]() { JP_cc_a16(this->C_flag()); };
	opcodeTable[0xEA] = [this]() { LD_aa16_A(); };
	opcodeTable[0xFA] = [this]() { LD_A_aa16(); };
	opcodeTable[0x0B] = [this]() { DEC_r8r8(&(this->B), &(this->C)); };
	opcodeTable[0x1B] = [this]() { DEC_r8r8(&(this->D), &(this->E)); };
	opcodeTable[0x2B] = [this]() { DEC_r8r8(&(this->H), &(this->L)); };
	opcodeTable[0x3B] = [this]() { DEC_r16(&(this->SP)); };
	opcodeTable[0x4B] = [this]() { LD_r8_r8(&(this->C), &(this->E)); };
	opcodeTable[0x5B] = [this]() { LD_r8_r8(&(this->E), &(this->E)); };
	opcodeTable[0x6B] = [this]() { LD_r8_r8(&(this->L), &(this->E)); };
	opcodeTable[0x7B] = [this]() { LD_r8_r8(&(this->A), &(this->E)); };
	opcodeTable[0x8B] = [this]() { ADC_A_r8(this->E); };
	opcodeTable[0x9B] = [this]() { SBC_A_r8(this->E); };
	opcodeTable[0xAB] = [this]() { XOR_A_r8(this->E); };
	opcodeTable[0xBB] = [this]() { CP_A_r8(this->E); };
	opcodeTable[0xCB] = [this]() { handlePrefix(); };
	opcodeTable[0xFB] = [this]() { EI(); };
	opcodeTable[0x0C] = [this]() { INC_r8(&(this->C)); };
	opcodeTable[0x1C] = [this]() { INC_r8(&(this->E)); };
	opcodeTable[0x2C] = [this]() { INC_r8(&(this->L)); };
	opcodeTable[0x3C] = [this]() { INC_r8(&(this->A)); };
	opcodeTable[0x4C] = [this]() { LD_r8_r8(&(this->C), &(this->H)); };
	opcodeTable[0x5C] = [this]() { LD_r8_r8(&(this->E), &(this->H)); };
	opcodeTable[0x6C] = [this]() { LD_r8_r8(&(this->L), &(this->H)); };
	opcodeTable[0x7C] = [this]() { LD_r8_r8(&(this->A), &(this->H)); };
	opcodeTable[0x8C] = [this]() { ADC_A_r8(this->H); };
	opcodeTable[0x9C] = [this]() { SBC_A_r8(this->H); };
	opcodeTable[0xAC] = [this]() { XOR_A_r8(this->H); };
	opcodeTable[0xBC] = [this]() { CP_A_r8(this->H); };
	opcodeTable[0xCC] = [this]() { CALL_cc_a16(this->Z_flag()); };
	opcodeTable[0xDC] = [this]() { CALL_cc_a16(this->C_flag()); };
	opcodeTable[0x0D] = [this]() { DEC_r8(&(this->C)); };
	opcodeTable[0x1D] = [this]() { DEC_r8(&(this->E)); };
	opcodeTable[0x2D] = [this]() { DEC_r8(&(this->L)); };
	opcodeTable[0x3D] = [this]() { DEC_r8(&(this->A)); };
	opcodeTable[0x4D] = [this]() { LD_r8_r8(&(this->C), &(this->L)); };
	opcodeTable[0x5D] = [this]() { LD_r8_r8(&(this->E), &(this->L)); };
	opcodeTable[0x6D] = [this]() { LD_r8_r8(&(this->L), &(this->L)); };
	opcodeTable[0x7D] = [this]() { LD_r8_r8(&(this->A), &(this->L)); };
	opcodeTable[0x8D] = [this]() { ADC_A_r8(this->L); };
	opcodeTable[0x9D] = [this]() { SBC_A_r8(this->L); };
	opcodeTable[0xAD] = [this]() { XOR_A_r8(this->L); };
	opcodeTable[0xBD] = [this]() { CP_A_r8(this->L); };
	opcodeTable[0xCD] = [this]() { CALL(); };
	opcodeTable[0x0E] = [this]() { LD_r8_n8(&(this->C)); };
	opcodeTable[0x1E] = [this]() { LD_r8_n8(&(this->E)); };
	opcodeTable[0x2E] = [this]() { LD_r8_n8(&(this->L)); };
	opcodeTable[0x3E] = [this]() { LD_r8_n8(&(this->A)); };
	opcodeTable[0x4E] = [this]() { LD_r8_ar8r8(&(this->C), this->H, this->L); };
	opcodeTable[0x5E] = [this]() { LD_r8_ar8r8(&(this->E), this->H, this->L); };
	opcodeTable[0x6E] = [this]() { LD_r8_ar8r8(&(this->L), this->H, this->L); };
	opcodeTable[0x7E] = [this]() { LD_r8_ar8r8(&(this->A), this->H, this->L); };
	opcodeTable[0x8E] = [this]() { ADC_A_ar8r8(this->H, this->L); };
	opcodeTable[0x9E] = [this]() { SBC_A_ar8r8(this->H, this->L); };
	opcodeTable[0xAE] = [this]() { XOR_A_ar8r8(&(this->H), &(this->L)); };
	opcodeTable[0xBE] = [this]() { CP_A_ar8r8(this->H, this->L); };
	opcodeTable[0xCE] = [this]() { ADC_A_n8(); };
	opcodeTable[0xDE] = [this]() { SBC_A_n8(); };
	opcodeTable[0xEE] = [this]() { XOR_A_n8(); };
	opcodeTable[0xFE] = [this]() { CP_A_n8(); };
	opcodeTable[0x0F] = [this]() { RRCA(); };
	opcodeTable[0x1F] = [this]() { RRA(); };
	opcodeTable[0x2F] = [this]() { CPL(); };
	opcodeTable[0x3F] = [this]() { CCF(); };
	opcodeTable[0x4F] = [this]() { LD_r8_r8(&(this->C), &(this->A)); };
	opcodeTable[0x5F] = [this]() { LD_r8_r8(&(this->E), &(this->A)); };
	opcodeTable[0x6F] = [this]() { LD_r8_r8(&(this->L), &(this->A)); };
	opcodeTable[0x7F] = [this]() { LD_r8_r8(&(this->A), &(this->A)); };
	opcodeTable[0x8F] = [this]() { ADC_A_r8(this->A); };
	opcodeTable[0x9F] = [this]() { SBC_A_r8(this->A); };
	opcodeTable[0xAF] = [this]() { XOR_A_r8(this->A); };
	opcodeTable[0xBF] = [this]() { CP_A_r8(this->A); };
	opcodeTable[0xCF] = [this]() { RST(0x8); };
	opcodeTable[0xDF] = [this]() { RST(0x18); };
	opcodeTable[0xEF] = [this]() { RST(0x28); };
	opcodeTable[0xFF] = [this]() { RST(0x38); };

	opcodeTableBitOperations[0x0] = [this]() { RLC_r8(&(this->B)); };
	opcodeTableBitOperations[0x1] = [this]() { RLC_r8(&(this->C)); };
	opcodeTableBitOperations[0x2] = [this]() { RLC_r8(&(this->D)); };
	opcodeTableBitOperations[0x3] = [this]() { RLC_r8(&(this->E)); };
	opcodeTableBitOperations[0x4] = [this]() { RLC_r8(&(this->H)); };
	opcodeTableBitOperations[0x5] = [this]() { RLC_r8(&(this->L)); };
	opcodeTableBitOperations[0x7] = [this]() { RLC_r8(&(this->A)); };
	opcodeTableBitOperations[0x10] = [this]() { RL_r8(&(this->B)); };
	opcodeTableBitOperations[0x11] = [this]() { RL_r8(&(this->C)); };
	opcodeTableBitOperations[0x12] = [this]() { RL_r8(&(this->D)); };
	opcodeTableBitOperations[0x13] = [this]() { RL_r8(&(this->E)); };
	opcodeTableBitOperations[0x14] = [this]() { RL_r8(&(this->H)); };
	opcodeTableBitOperations[0x15] = [this]() { RL_r8(&(this->L)); };
	opcodeTableBitOperations[0x17] = [this]() { RL_r8(&(this->A)); };
	opcodeTableBitOperations[0x18] = [this]() { RR_r8(&(this->B)); };
	opcodeTableBitOperations[0x19] = [this]() { RR_r8(&(this->C)); };
	opcodeTableBitOperations[0x1A] = [this]() { RR_r8(&(this->D)); };
	opcodeTableBitOperations[0x1B] = [this]() { RR_r8(&(this->E)); };
	opcodeTableBitOperations[0x1C] = [this]() { RR_r8(&(this->H)); };
	opcodeTableBitOperations[0x1D] = [this]() { RR_r8(&(this->L)); };
	opcodeTableBitOperations[0x1F] = [this]() { RR_r8(&(this->A)); };
	opcodeTableBitOperations[0x20] = [this]() { SLA_r8(&(this->B)); };
	opcodeTableBitOperations[0x21] = [this]() { SLA_r8(&(this->C)); };
	opcodeTableBitOperations[0x22] = [this]() { SLA_r8(&(this->D)); };
	opcodeTableBitOperations[0x23] = [this]() { SLA_r8(&(this->E)); };
	opcodeTableBitOperations[0x24] = [this]() { SLA_r8(&(this->H)); };
	opcodeTableBitOperations[0x25] = [this]() { SLA_r8(&(this->L)); };
	opcodeTableBitOperations[0x27] = [this]() { SLA_r8(&(this->A)); };
	opcodeTableBitOperations[0x28] = [this]() { SRA_r8(&(this->B)); };
	opcodeTableBitOperations[0x29] = [this]() { SRA_r8(&(this->C)); };
	opcodeTableBitOperations[0x2A] = [this]() { SRA_r8(&(this->D)); };
	opcodeTableBitOperations[0x2B] = [this]() { SRA_r8(&(this->E)); };
	opcodeTableBitOperations[0x2C] = [this]() { SRA_r8(&(this->H)); };
	opcodeTableBitOperations[0x2D] = [this]() { SRA_r8(&(this->L)); };
	opcodeTableBitOperations[0x2F] = [this]() { SRA_r8(&(this->A)); };
	opcodeTableBitOperations[0x30] = [this]() { SWAP_r8(&(this->B)); };
	opcodeTableBitOperations[0x31] = [this]() { SWAP_r8(&(this->C)); };
	opcodeTableBitOperations[0x32] = [this]() { SWAP_r8(&(this->D)); };
	opcodeTableBitOperations[0x33] = [this]() { SWAP_r8(&(this->E)); };
	opcodeTableBitOperations[0x34] = [this]() { SWAP_r8(&(this->H)); };
	opcodeTableBitOperations[0x35] = [this]() { SWAP_r8(&(this->L)); };
	opcodeTableBitOperations[0x37] = [this]() { SWAP_r8(&(this->A)); };
	opcodeTableBitOperations[0x38] = [this]() { SRL_r8(&(this->B)); };
	opcodeTableBitOperations[0x39] = [this]() { SRL_r8(&(this->C)); };
	opcodeTableBitOperations[0x3A] = [this]() { SRL_r8(&(this->D)); };
	opcodeTableBitOperations[0x3B] = [this]() { SRL_r8(&(this->E)); };
	opcodeTableBitOperations[0x3C] = [this]() { SRL_r8(&(this->H)); };
	opcodeTableBitOperations[0x3D] = [this]() { SRL_r8(&(this->L)); };
	opcodeTableBitOperations[0x3F] = [this]() { SRL_r8(&(this->A)); };
	opcodeTableBitOperations[0x40] = [this]() { BIT_n_r8(0, &(this->B)); };
	opcodeTableBitOperations[0x41] = [this]() { BIT_n_r8(0, &(this->C)); };
	opcodeTableBitOperations[0x42] = [this]() { BIT_n_r8(0, &(this->D)); };
	opcodeTableBitOperations[0x43] = [this]() { BIT_n_r8(0, &(this->E)); };
	opcodeTableBitOperations[0x44] = [this]() { BIT_n_r8(0, &(this->H)); };
	opcodeTableBitOperations[0x45] = [this]() { BIT_n_r8(0, &(this->L)); };
	opcodeTableBitOperations[0x47] = [this]() { BIT_n_r8(0, &(this->A)); };
	opcodeTableBitOperations[0x48] = [this]() { BIT_n_r8(1, &(this->B)); };
	opcodeTableBitOperations[0x49] = [this]() { BIT_n_r8(1, &(this->C)); };
	opcodeTableBitOperations[0x4A] = [this]() { BIT_n_r8(1, &(this->D)); };
	opcodeTableBitOperations[0x4B] = [this]() { BIT_n_r8(1, &(this->E)); };
	opcodeTableBitOperations[0x4C] = [this]() { BIT_n_r8(1, &(this->H)); };
	opcodeTableBitOperations[0x4D] = [this]() { BIT_n_r8(1, &(this->L)); };
	opcodeTableBitOperations[0x4F] = [this]() { BIT_n_r8(1, &(this->A)); };
	opcodeTableBitOperations[0x50] = [this]() { BIT_n_r8(2, &(this->B)); };
	opcodeTableBitOperations[0x51] = [this]() { BIT_n_r8(2, &(this->C)); };
	opcodeTableBitOperations[0x52] = [this]() { BIT_n_r8(2, &(this->D)); };
	opcodeTableBitOperations[0x53] = [this]() { BIT_n_r8(2, &(this->E)); };
	opcodeTableBitOperations[0x54] = [this]() { BIT_n_r8(2, &(this->H)); };
	opcodeTableBitOperations[0x55] = [this]() { BIT_n_r8(2, &(this->L)); };
	opcodeTableBitOperations[0x57] = [this]() { BIT_n_r8(2, &(this->A)); };
	opcodeTableBitOperations[0x58] = [this]() { BIT_n_r8(3, &(this->B)); };
	opcodeTableBitOperations[0x59] = [this]() { BIT_n_r8(3, &(this->C)); };
	opcodeTableBitOperations[0x5A] = [this]() { BIT_n_r8(3, &(this->D)); };
	opcodeTableBitOperations[0x5B] = [this]() { BIT_n_r8(3, &(this->E)); };
	opcodeTableBitOperations[0x5C] = [this]() { BIT_n_r8(3, &(this->H)); };
	opcodeTableBitOperations[0x5D] = [this]() { BIT_n_r8(3, &(this->L)); };
	opcodeTableBitOperations[0x5F] = [this]() { BIT_n_r8(3, &(this->A)); };
	opcodeTableBitOperations[0x60] = [this]() { BIT_n_r8(4, &(this->B)); };
	opcodeTableBitOperations[0x61] = [this]() { BIT_n_r8(4, &(this->C)); };
	opcodeTableBitOperations[0x62] = [this]() { BIT_n_r8(4, &(this->D)); };
	opcodeTableBitOperations[0x63] = [this]() { BIT_n_r8(4, &(this->E)); };
	opcodeTableBitOperations[0x64] = [this]() { BIT_n_r8(4, &(this->H)); };
	opcodeTableBitOperations[0x65] = [this]() { BIT_n_r8(4, &(this->L)); };
	opcodeTableBitOperations[0x67] = [this]() { BIT_n_r8(4, &(this->A)); };
	opcodeTableBitOperations[0x68] = [this]() { BIT_n_r8(5, &(this->B)); };
	opcodeTableBitOperations[0x69] = [this]() { BIT_n_r8(5, &(this->C)); };
	opcodeTableBitOperations[0x6A] = [this]() { BIT_n_r8(5, &(this->D)); };
	opcodeTableBitOperations[0x6B] = [this]() { BIT_n_r8(5, &(this->E)); };
	opcodeTableBitOperations[0x6C] = [this]() { BIT_n_r8(5, &(this->H)); };
	opcodeTableBitOperations[0x6D] = [this]() { BIT_n_r8(5, &(this->L)); };
	opcodeTableBitOperations[0x6F] = [this]() { BIT_n_r8(5, &(this->A)); };
	opcodeTableBitOperations[0x70] = [this]() { BIT_n_r8(6, &(this->B)); };
	opcodeTableBitOperations[0x71] = [this]() { BIT_n_r8(6, &(this->C)); };
	opcodeTableBitOperations[0x72] = [this]() { BIT_n_r8(6, &(this->D)); };
	opcodeTableBitOperations[0x73] = [this]() { BIT_n_r8(6, &(this->E)); };
	opcodeTableBitOperations[0x74] = [this]() { BIT_n_r8(6, &(this->H)); };
	opcodeTableBitOperations[0x75] = [this]() { BIT_n_r8(6, &(this->L)); };
	opcodeTableBitOperations[0x77] = [this]() { BIT_n_r8(6, &(this->A)); };
	opcodeTableBitOperations[0x78] = [this]() { BIT_n_r8(7, &(this->B)); };
	opcodeTableBitOperations[0x79] = [this]() { BIT_n_r8(7, &(this->C)); };
	opcodeTableBitOperations[0x7A] = [this]() { BIT_n_r8(7, &(this->D)); };
	opcodeTableBitOperations[0x7B] = [this]() { BIT_n_r8(7, &(this->E)); };
	opcodeTableBitOperations[0x7C] = [this]() { BIT_n_r8(7, &(this->H)); };
	opcodeTableBitOperations[0x7D] = [this]() { BIT_n_r8(7, &(this->L)); };
	opcodeTableBitOperations[0x7F] = [this]() { BIT_n_r8(7, &(this->A)); };
	opcodeTableBitOperations[0x80] = [this]() { RES_n_r8(0, &(this->B)); };
	opcodeTableBitOperations[0x81] = [this]() { RES_n_r8(0, &(this->C)); };
	opcodeTableBitOperations[0x82] = [this]() { RES_n_r8(0, &(this->D)); };
	opcodeTableBitOperations[0x83] = [this]() { RES_n_r8(0, &(this->E)); };
	opcodeTableBitOperations[0x84] = [this]() { RES_n_r8(0, &(this->H)); };
	opcodeTableBitOperations[0x85] = [this]() { RES_n_r8(0, &(this->L)); };
	opcodeTableBitOperations[0x86] = [this]() { RES_n_aHL(0); };
	opcodeTableBitOperations[0x87] = [this]() { RES_n_r8(0, &(this->A)); };
	opcodeTableBitOperations[0x88] = [this]() { RES_n_r8(1, &(this->B)); };
	opcodeTableBitOperations[0x89] = [this]() { RES_n_r8(1, &(this->C)); };
	opcodeTableBitOperations[0x8A] = [this]() { RES_n_r8(1, &(this->D)); };
	opcodeTableBitOperations[0x8B] = [this]() { RES_n_r8(1, &(this->E)); };
	opcodeTableBitOperations[0x8C] = [this]() { RES_n_r8(1, &(this->H)); };
	opcodeTableBitOperations[0x8D] = [this]() { RES_n_r8(1, &(this->L)); };
	opcodeTableBitOperations[0x8E] = [this]() { RES_n_aHL(1); };
	opcodeTableBitOperations[0x8F] = [this]() { RES_n_r8(1, &(this->A)); };
	opcodeTableBitOperations[0x90] = [this]() { RES_n_r8(2, &(this->B)); };
	opcodeTableBitOperations[0x91] = [this]() { RES_n_r8(2, &(this->C)); };
	opcodeTableBitOperations[0x92] = [this]() { RES_n_r8(2, &(this->D)); };
	opcodeTableBitOperations[0x93] = [this]() { RES_n_r8(2, &(this->E)); };
	opcodeTableBitOperations[0x94] = [this]() { RES_n_r8(2, &(this->H)); };
	opcodeTableBitOperations[0x95] = [this]() { RES_n_r8(2, &(this->L)); };
	opcodeTableBitOperations[0x96] = [this]() { RES_n_aHL(2); };
	opcodeTableBitOperations[0x97] = [this]() { RES_n_r8(2, &(this->A)); };
	opcodeTableBitOperations[0x98] = [this]() { RES_n_r8(3, &(this->B)); };
	opcodeTableBitOperations[0x99] = [this]() { RES_n_r8(3, &(this->C)); };
	opcodeTableBitOperations[0x9A] = [this]() { RES_n_r8(3, &(this->D)); };
	opcodeTableBitOperations[0x9B] = [this]() { RES_n_r8(3, &(this->E)); };
	opcodeTableBitOperations[0x9C] = [this]() { RES_n_r8(3, &(this->H)); };
	opcodeTableBitOperations[0x9D] = [this]() { RES_n_r8(3, &(this->L)); };
	opcodeTableBitOperations[0x9E] = [this]() { RES_n_aHL(3); };
	opcodeTableBitOperations[0x9F] = [this]() { RES_n_r8(3, &(this->A)); };
	opcodeTableBitOperations[0xA0] = [this]() { RES_n_r8(4, &(this->B)); };
	opcodeTableBitOperations[0xA1] = [this]() { RES_n_r8(4, &(this->C)); };
	opcodeTableBitOperations[0xA2] = [this]() { RES_n_r8(4, &(this->D)); };
	opcodeTableBitOperations[0xA3] = [this]() { RES_n_r8(4, &(this->E)); };
	opcodeTableBitOperations[0xA4] = [this]() { RES_n_r8(4, &(this->H)); };
	opcodeTableBitOperations[0xA5] = [this]() { RES_n_r8(4, &(this->L)); };
	opcodeTableBitOperations[0xA6] = [this]() { RES_n_aHL(4); };
	opcodeTableBitOperations[0xA7] = [this]() { RES_n_r8(4, &(this->A)); };
	opcodeTableBitOperations[0xA8] = [this]() { RES_n_r8(5, &(this->B)); };
	opcodeTableBitOperations[0xA9] = [this]() { RES_n_r8(5, &(this->C)); };
	opcodeTableBitOperations[0xAA] = [this]() { RES_n_r8(5, &(this->D)); };
	opcodeTableBitOperations[0xAB] = [this]() { RES_n_r8(5, &(this->E)); };
	opcodeTableBitOperations[0xAC] = [this]() { RES_n_r8(5, &(this->H)); };
	opcodeTableBitOperations[0xAD] = [this]() { RES_n_r8(5, &(this->L)); };
	opcodeTableBitOperations[0xAE] = [this]() { RES_n_aHL(5); };
	opcodeTableBitOperations[0xAF] = [this]() { RES_n_r8(5, &(this->A)); };
	opcodeTableBitOperations[0xB0] = [this]() { RES_n_r8(6, &(this->B)); };
	opcodeTableBitOperations[0xB1] = [this]() { RES_n_r8(6, &(this->C)); };
	opcodeTableBitOperations[0xB2] = [this]() { RES_n_r8(6, &(this->D)); };
	opcodeTableBitOperations[0xB3] = [this]() { RES_n_r8(6, &(this->E)); };
	opcodeTableBitOperations[0xB4] = [this]() { RES_n_r8(6, &(this->H)); };
	opcodeTableBitOperations[0xB5] = [this]() { RES_n_r8(6, &(this->L)); };
	opcodeTableBitOperations[0xB6] = [this]() { RES_n_aHL(6); };
	opcodeTableBitOperations[0xB7] = [this]() { RES_n_r8(6, &(this->A)); };
	opcodeTableBitOperations[0xB8] = [this]() { RES_n_r8(7, &(this->B)); };
	opcodeTableBitOperations[0xB9] = [this]() { RES_n_r8(7, &(this->C)); };
	opcodeTableBitOperations[0xBA] = [this]() { RES_n_r8(7, &(this->D)); };
	opcodeTableBitOperations[0xBB] = [this]() { RES_n_r8(7, &(this->E)); };
	opcodeTableBitOperations[0xBC] = [this]() { RES_n_r8(7, &(this->H)); };
	opcodeTableBitOperations[0xBD] = [this]() { RES_n_r8(7, &(this->L)); };
	opcodeTableBitOperations[0xBE] = [this]() { RES_n_aHL(7); };
	opcodeTableBitOperations[0xBF] = [this]() { RES_n_r8(7, &(this->A)); };
	opcodeTableBitOperations[0xC0] = [this]() { SET_n_r8(0, &(this->B)); };
	opcodeTableBitOperations[0xC1] = [this]() { SET_n_r8(0, &(this->C)); };
	opcodeTableBitOperations[0xC2] = [this]() { SET_n_r8(0, &(this->D)); };
	opcodeTableBitOperations[0xC3] = [this]() { SET_n_r8(0, &(this->E)); };
	opcodeTableBitOperations[0xC4] = [this]() { SET_n_r8(0, &(this->H)); };
	opcodeTableBitOperations[0xC5] = [this]() { SET_n_r8(0, &(this->L)); };
	opcodeTableBitOperations[0xC7] = [this]() { SET_n_r8(0, &(this->A)); };
	opcodeTableBitOperations[0xC8] = [this]() { SET_n_r8(1, &(this->B)); };
	opcodeTableBitOperations[0xC9] = [this]() { SET_n_r8(1, &(this->C)); };
	opcodeTableBitOperations[0xCA] = [this]() { SET_n_r8(1, &(this->D)); };
	opcodeTableBitOperations[0xCB] = [this]() { SET_n_r8(1, &(this->E)); };
	opcodeTableBitOperations[0xCC] = [this]() { SET_n_r8(1, &(this->H)); };
	opcodeTableBitOperations[0xCD] = [this]() { SET_n_r8(1, &(this->L)); };
	opcodeTableBitOperations[0xCF] = [this]() { SET_n_r8(1, &(this->A)); };
	opcodeTableBitOperations[0xD0] = [this]() { SET_n_r8(2, &(this->B)); };
	opcodeTableBitOperations[0xD1] = [this]() { SET_n_r8(2, &(this->C)); };
	opcodeTableBitOperations[0xD2] = [this]() { SET_n_r8(2, &(this->D)); };
	opcodeTableBitOperations[0xD3] = [this]() { SET_n_r8(2, &(this->E)); };
	opcodeTableBitOperations[0xD4] = [this]() { SET_n_r8(2, &(this->H)); };
	opcodeTableBitOperations[0xD5] = [this]() { SET_n_r8(2, &(this->L)); };
	opcodeTableBitOperations[0xD7] = [this]() { SET_n_r8(2, &(this->A)); };
	opcodeTableBitOperations[0xD8] = [this]() { SET_n_r8(3, &(this->B)); };
	opcodeTableBitOperations[0xD9] = [this]() { SET_n_r8(3, &(this->C)); };
	opcodeTableBitOperations[0xDA] = [this]() { SET_n_r8(3, &(this->D)); };
	opcodeTableBitOperations[0xDB] = [this]() { SET_n_r8(3, &(this->E)); };
	opcodeTableBitOperations[0xDC] = [this]() { SET_n_r8(3, &(this->H)); };
	opcodeTableBitOperations[0xDD] = [this]() { SET_n_r8(3, &(this->L)); };
	opcodeTableBitOperations[0xDF] = [this]() { SET_n_r8(3, &(this->A)); };
	opcodeTableBitOperations[0xE0] = [this]() { SET_n_r8(4, &(this->B)); };
	opcodeTableBitOperations[0xE1] = [this]() { SET_n_r8(4, &(this->C)); };
	opcodeTableBitOperations[0xE2] = [this]() { SET_n_r8(4, &(this->D)); };
	opcodeTableBitOperations[0xE3] = [this]() { SET_n_r8(4, &(this->E)); };
	opcodeTableBitOperations[0xE4] = [this]() { SET_n_r8(4, &(this->H)); };
	opcodeTableBitOperations[0xE5] = [this]() { SET_n_r8(4, &(this->L)); };
	opcodeTableBitOperations[0xE7] = [this]() { SET_n_r8(4, &(this->A)); };
	opcodeTableBitOperations[0xE8] = [this]() { SET_n_r8(5, &(this->B)); };
	opcodeTableBitOperations[0xE9] = [this]() { SET_n_r8(5, &(this->C)); };
	opcodeTableBitOperations[0xEA] = [this]() { SET_n_r8(5, &(this->D)); };
	opcodeTableBitOperations[0xEB] = [this]() { SET_n_r8(5, &(this->E)); };
	opcodeTableBitOperations[0xEC] = [this]() { SET_n_r8(5, &(this->H)); };
	opcodeTableBitOperations[0xED] = [this]() { SET_n_r8(5, &(this->L)); };
	opcodeTableBitOperations[0xEF] = [this]() { SET_n_r8(5, &(this->A)); };
	opcodeTableBitOperations[0xF0] = [this]() { SET_n_r8(6, &(this->B)); };
	opcodeTableBitOperations[0xF1] = [this]() { SET_n_r8(6, &(this->C)); };
	opcodeTableBitOperations[0xF2] = [this]() { SET_n_r8(6, &(this->D)); };
	opcodeTableBitOperations[0xF3] = [this]() { SET_n_r8(6, &(this->E)); };
	opcodeTableBitOperations[0xF4] = [this]() { SET_n_r8(6, &(this->H)); };
	opcodeTableBitOperations[0xF5] = [this]() { SET_n_r8(6, &(this->L)); };
	opcodeTableBitOperations[0xF7] = [this]() { SET_n_r8(6, &(this->A)); };
	opcodeTableBitOperations[0xF8] = [this]() { SET_n_r8(7, &(this->B)); };
	opcodeTableBitOperations[0xF9] = [this]() { SET_n_r8(7, &(this->C)); };
	opcodeTableBitOperations[0xFA] = [this]() { SET_n_r8(7, &(this->D)); };
	opcodeTableBitOperations[0xFB] = [this]() { SET_n_r8(7, &(this->E)); };
	opcodeTableBitOperations[0xFC] = [this]() { SET_n_r8(7, &(this->H)); };
	opcodeTableBitOperations[0xFD] = [this]() { SET_n_r8(7, &(this->L)); };
	opcodeTableBitOperations[0xFF] = [this]() { SET_n_r8(7, &(this->A)); };

	opcodeTableBitOperations[0x46] = [this]() { BIT_n_aHL(0); };
	opcodeTableBitOperations[0x56] = [this]() { BIT_n_aHL(2); };
	opcodeTableBitOperations[0x66] = [this]() { BIT_n_aHL(4); };
	opcodeTableBitOperations[0x76] = [this]() { BIT_n_aHL(6); };
	opcodeTableBitOperations[0x4E] = [this]() { BIT_n_aHL(1); };
	opcodeTableBitOperations[0x5E] = [this]() { BIT_n_aHL(3); };
	opcodeTableBitOperations[0x6E] = [this]() { BIT_n_aHL(5); };
	opcodeTableBitOperations[0x7E] = [this]() { BIT_n_aHL(7); };

	opcodeTableBitOperations[0xC6] = [this]() { SET_n_aHL(0); };
	opcodeTableBitOperations[0xD6] = [this]() { SET_n_aHL(2); };
	opcodeTableBitOperations[0xE6] = [this]() { SET_n_aHL(4); };
	opcodeTableBitOperations[0xF6] = [this]() { SET_n_aHL(6); };
	opcodeTableBitOperations[0xCE] = [this]() { SET_n_aHL(1); };
	opcodeTableBitOperations[0xDE] = [this]() { SET_n_aHL(3); };
	opcodeTableBitOperations[0xEE] = [this]() { SET_n_aHL(5); };
	opcodeTableBitOperations[0xFE] = [this]() { SET_n_aHL(7); };


}


void CPU::executeOpcode(std::uint8_t opcode)
{
	opcodeTable[opcode]();
}

void CPU::handlePrefix()
{
	uint8_t opcode = readInstruction(this->PC++);
	opcodeTableBitOperations[opcode]();
}

std::uint8_t CPU::readMemory(std::uint16_t addressToRead)
{
	uint8_t value;
	switch (addressToRead)
	{
	case 0xFF40:
		value = gpu.lcdc;
		break;
	case 0xFF41:
		value = gpu.stat;
		break;
	case 0xFF42:
		value = gpu.scy;
		break;
	case 0xFF43:
		value = gpu.scx;
		break;
	case 0xFF44:
		value = gpu.line;
		break;
	case 0xFF45:
		value = gpu.lyc;
		break;
	case 0xFF4A:
		value = gpu.wy;
		break;
	case 0xFF4B:
		value = gpu.wx;
		break;
	default:
		value = mmu.read_memory(addressToRead);
		break;
	}

	if (this->debugMode) printf("Odczytano wartosc: %x z adresu: %x\n", value, addressToRead);
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
	case 0xFF41:
		this->gpu.stat = value;
		break;
	case 0xFF42:
		this->gpu.scy = value;
		break;
	case 0xFF43:
		this->gpu.scx = value;
		break;
	case 0xFF45:
		this->gpu.lyc = value;
		break;
	case 0xFF47:
		this->gpu.bgp = value;
		break;
	case 0xFF48:
		this->gpu.obp0 = value;
		break;
	case 0xFF49:
		this->gpu.obp1 = value;
		break;
	case 0xFF4A:
		this->gpu.wy = value;
		break;
	case 0xFF4B:
		this->gpu.wx = value;
		break;
	default:
		mmu.write_memory(addressToWrite, value);
		break;
	}

	if (this->debugMode) printf("Zapisano wartosc: %x pod adresem %x\n", value, addressToWrite);
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
	if (this->debugMode) printf("%x: Executing opcode %x...\n", this->PC - 1, opcode);
	executeOpcode(opcode);
	gpu.step(this->cycles);

	if (this->IME)	// if interrupts are enabled
	{
		uint8_t interrupt_enable_flags = mmu.read_memory(0xFFFF);
		uint8_t interrupt_flags = mmu.read_memory(0xFF0F);
		uint8_t enabled = interrupt_enable_flags & interrupt_flags;

		if (enabled & 0x01)	// v-blank interrupt
		{
			mmu.write_memory(0xFF0F, interrupt_flags - 0x01);
			this->IME = 0;
			RST(0x40);
		} else if (interrupt_enable_flags & 0x10 && this->keyboardHandler.interrupt_happened)	// high-to-low key interrupt
		{
			this->keyboardHandler.interrupt_happened = false;
			this->IME = 0;
			RST(0x60);
		}
		else if (enabled & 0x02)	// stat interrupt
		{
			mmu.write_memory(0xFF0F, interrupt_flags - 0x02);
			this->IME = 0;
			RST(0x48);
		}
	}

	if (gpu.ready_to_render)
	{
		renderer.render(gpu.framebuffer);
		gpu.ready_to_render = false;
	}

	this->cycles = 0;
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(125ns);
}

bool CPU::Z_flag()
{
	return uint8_t(this->F) >= 0b10000000;
}

bool CPU::N_flag()
{
	return uint8_t(this->F << 1) >= 0b10000000;
}

bool CPU::H_flag()
{
	return uint8_t(this->F << 2) >= 0b10000000;
}

bool CPU::C_flag()
{
	return uint8_t(this->F << 3) >= 0b10000000;
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
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', true);

	this->cycles += 4;
}

void CPU::STOP()
{
	this->cycles += 0; // idk
	this->PC++;
}

void CPU::RLCA()
{
	uint8_t msb = (this->A & 0x80) >> 7;

	// set flags
	setFlag('Z', false);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', msb);

	this->A <<= 1;
	this->A += msb;

	this->cycles += 4;
}

void CPU::RLA()
{
	uint8_t msb = (this->A & 0x80) >> 7;
	bool cf = C_flag();

	setFlag('Z', false);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', msb);

	this->A <<= 1;
	this->A += cf;

	this->cycles += 4;
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

void CPU::RR_r8(uint8_t* reg)
{
	bool cf = *reg & 0x1;
	*reg >>= 1;
	if (this->C_flag())
	{
		*reg += 0b10000000;
	}

	// set flags
	setFlag('Z', *reg == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', cf);

	this->cycles += 8;
}

void CPU::SRL_r8(uint8_t* reg)
{
	bool cf = *reg & 0x1;
	*reg >>= 1;

	// set flags
	setFlag('Z', *reg == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', cf);

	this->cycles += 8;
}

void CPU::SLA_r8(uint8_t* reg)
{
	bool cf = *reg & 0x80;
	*reg <<= 1;
	
	// set flags
	setFlag('Z', *reg == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', cf);

	this->cycles += 8;
}

void CPU::SRA_r8(uint8_t* reg)
{
	bool cf = *reg & 0x1;
	*reg >>= 1;

	// set flags
	setFlag('Z', *reg == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', cf);

	this->cycles += 8;
}

void CPU::BIT_n_r8(uint8_t n, uint8_t* reg)
{
	uint8_t bit = 1 << n;
	bool result = (*reg & bit) > 0;
	
	// set flags
	setFlag('Z', !result);
	setFlag('N', false);
	setFlag('H', true);

	this->cycles += 8;
}

void CPU::BIT_n_aHL(uint8_t n)
{
	uint16_t address = (this->H << 8) + this->L;
	uint8_t value = readMemory(address);
	uint8_t bit = 1 << n;
	bool result = (value & bit) > 0;

	// set flags
	setFlag('Z', !result);
	setFlag('N', false);
	setFlag('H', true);

	this->cycles += 12;
}

void CPU::SET_n_r8(uint8_t n, uint8_t* reg)
{
	uint8_t bit = 1 << n;
	*reg |= bit;
	this->cycles += 8;
}

void CPU::SET_n_aHL(uint8_t n)
{
	uint8_t bit = 1 << n;
	uint16_t address = (this->H << 8) + this->L;
	uint8_t value = readMemory(address);
	value |= bit;
	writeMemory(address, value);
	this->cycles += 16;
}

void CPU::RES_n_r8(uint8_t n, uint8_t* reg)
{
	uint8_t bit = 0b11111111 ^ (1 << n);
	*reg &= bit;
	this->cycles += 8;
}

void CPU::RES_n_aHL(uint8_t n)
{
	uint16_t address = (this->H << 8) + this->L;
	uint8_t value = readMemory(address);
	uint8_t bit = 0b11111111 ^ (1 << n);

	value &= bit;
	writeMemory(address, value);
	this->cycles += 12;
}

void CPU::SWAP_r8(uint8_t* reg)
{
	uint8_t upper = *reg >> 4;
	*reg <<= 4;
	*reg += upper;

	// set flags
	setFlag('Z', *reg == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', false);

	this->cycles += 8;
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
	PUSH_r16(this->PC + 2);	// push address of next instruction

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
	PUSH_r16(this->PC);
	this->PC = vectorAddress;
}

void CPU::RLC_r8(std::uint8_t* reg)
{
	uint8_t value = *reg;

	setFlag('Z', *reg == 0);
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
	setFlag('Z', value == 0);
	setFlag('N', false);
	setFlag('H', false);
	setFlag('C', *reg >= 0b10000000);
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

void CPU::DAA() {
	uint8_t adjustment = 0;
	bool carry = false;

	if (!N_flag()) { // Addition
		if (H_flag() || (A & 0x0F) > 9) {
			adjustment += 0x06;
		}
		if (C_flag() || A > 0x99) {
			adjustment += 0x60;
			carry = true;
		}
	}
	else { // Subtraction
		if (H_flag()) {
			adjustment -= 0x06;
		}
		if (C_flag()) {
			adjustment -= 0x60;
		}
	}

	A += adjustment;

	setFlag('Z', A == 0);
	setFlag('H', false); // H is always cleared
	setFlag('C', carry || C_flag());
}



void CPU::DI()
{
	this->IME = 0;
	this->cycles += 4;
}

void CPU::EI()
{
	this->IME = 1;
	this->cycles += 4;
}

void CPU::HALT()
{
	this->cycles += 4;
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
	setFlag('Z', uint8_t(this->A + reg) == 0);
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
	setFlag('Z', uint8_t(this->A + value) == 0);
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
	setFlag('Z', uint8_t(this->A + value) == 0);
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

	uint32_t result = uint32_t(this->SP) + uint32_t(value);

	// set flags
	setFlag('Z', false);
	setFlag('N', false);
	setFlag('H', ((this->SP ^ value ^ result) & 0x10) != 0);
	setFlag('C', ((this->SP ^ value ^ result) & 0x100) != 0);

	this->SP = result & 0xFFFF;
	this->cycles += 16;
}


void CPU::ADD_HL_r8r8(std::uint8_t* reg1, std::uint8_t* reg2)
{
	uint16_t hlValue = uint16_t(this->H << 8) + uint16_t(this->L);
	uint16_t r1r2Value = uint16_t((*reg1) << 8) + uint16_t(*reg2);
	uint16_t result = hlValue + r1r2Value;

	setFlag('N', false);
	setFlag('H', ((hlValue & 0x0FFF) + (r1r2Value & 0x0FFF)) > 0x0FFF);
	setFlag('C', hlValue + r1r2Value > 0xFFFF);

	hlValue += r1r2Value;
	this->H = uint8_t(hlValue >> 8);
	this->L = uint8_t(hlValue & 0xFF);

	this->cycles += 8;
}

void CPU::ADD_HL_r16(std::uint16_t* reg)
{
	uint16_t HL_value = (this->H << 8) + this->L;

	setFlag('N', false);
	setFlag('H', ((*reg & 0x0FFF) + (HL_value & 0x0FFF)) > 0x0FFF);
	setFlag('C', this->SP + HL_value > 0xFFFF);

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
	setFlag('Z', uint8_t(this->A + reg + cf) == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + (reg & 0x0F) + cf) > 0x0F);
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
	setFlag('Z', uint8_t(this->A + value + cf) == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + (value & 0x0F) + cf) > 0x0F);
	setFlag('C', (uint16_t(this->A) + uint16_t(value)) + cf > 0xFF);

	this->A += value + cf;
	this->cycles += 8;
}

void CPU::ADC_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	uint8_t cf = C_flag();
	setFlag('Z', uint8_t(this->A + value + cf) == 0);
	setFlag('N', false);
	setFlag('H', ((this->A & 0x0F) + (value & 0x0F) + cf) > 0x0F);
	setFlag('C', (uint16_t(this->A) + uint16_t(value)) + cf > 0xFF);

	this->A += value + cf;
	this->cycles += 8;
}

void CPU::SBC_A_r8(std::uint8_t reg)
{
	// set flags
	bool cf = C_flag();
	setFlag('Z', ((this->A - reg - cf) & 0xFF) == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) - (reg & 0x0F) - cf) < 0);
	setFlag('C', this->A - reg - cf < 0);

	this->A -= reg + cf;
	this->cycles += 4;
}

void CPU::SBC_A_ar8r8(std::uint8_t reg1, std::uint8_t reg2)
{
	uint16_t address = (uint16_t(reg1) << 8) + uint16_t(reg2);
	uint8_t value = readMemory(address);

	// set flags
	bool cf = C_flag();
	setFlag('Z', ((this->A - value - cf) & 0xFF) == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) - (value & 0x0F) - cf) < 0);
	setFlag('C', this->A - value - cf < 0);

	this->A -= value + cf;
	this->cycles += 8;
}

void CPU::SBC_A_n8()
{
	uint8_t value = readInstruction(this->PC);
	this->PC++;

	// set flags
	bool cf = C_flag();
	setFlag('Z', ((this->A - value - cf) & 0xFF) == 0);
	setFlag('N', true);
	setFlag('H', ((this->A & 0x0F) - (value & 0x0F) - cf) < 0);
	setFlag('C', this->A - value - cf < 0);

	this->A -= uint8_t(value + cf);
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
	setFlag('Z', (*reg) == 1);
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

	if (reg1 == &this->A && reg2 == &this->F)
	{
		*reg2 &= 0xF0;
	}

	this->cycles += 12;
}

void CPU::RET()
{
	uint16_t popedValue = uint16_t(readMemory(this->SP));	// low
	this->SP += 1;
	popedValue += uint16_t(readMemory(this->SP)) << 8;	// high
	this->SP += 1;
	
	this->PC = popedValue;
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

	writeMemory(address, reg & 0xFF);
	writeMemory(address + 1, reg >> 8);

	this->cycles += 20;
}

void CPU::LD_HL_SPe8()
{
	// read address from instruction set
	int8_t value = readInstruction(this->PC);
	this->PC++;

	uint32_t result = uint32_t(this->SP) + uint32_t(value);

	// set flags
	setFlag('Z', false);
	setFlag('N', false);
	setFlag('H', ((this->SP ^ value ^ result) & 0x10) != 0);
	setFlag('C', ((this->SP ^ value ^ result) & 0x100) != 0);

	uint8_t low = (result & 0xFF);
	uint8_t high = (result & 0xFF00) >> 8;

	this->L = low;
	this->H = high;

	this->cycles += 12;
}

void CPU::LD_SP_HL()
{
	this->SP = (this->H << 8) + this->L;
	this->cycles += 8;
}

void CPU::LDH_ar8_A(uint8_t* reg)
{
	uint16_t address = 0xFF00 + *reg;
	writeMemory(address, this->A);
	this->cycles += 8;
}

void CPU::LDH_A_ar8(uint8_t* reg)
{
	uint16_t address = 0xFF00 + *reg;
	this->A = readMemory(address);
	this->cycles += 8;
}

void CPU::JR_e8()
{
	int8_t offset = readInstruction(this->PC); // read signed offset
	this->PC++;
	
	this->PC += offset;
	
	if (this->debugMode) printf("Jump at address %x to address: %x\n", this->PC - offset - 1, this->PC);

	this->cycles += 12;
}
