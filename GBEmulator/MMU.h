#pragma once

#include <cstdint>
#include <stdio.h>
#include <memory>
#include <mutex>
#include "KeyboardHandler.h"

class MMU
{
public:
    uint8_t ROM_bank_0[0x4000];
    uint8_t ROM_bank_1[0x4000];
    uint8_t Graphic_RAM[0x2000];
    uint8_t Cardridge_RAM[0x2000];
    uint8_t Working_RAM[0x2000];
    uint8_t Working_RAM_shadow[0x1E00];
    uint8_t Graphics_sprite_information[0xA0];
    uint8_t Memory_mapped_IO[0x80];
    uint8_t Zero_page_RAM[0x80];
    KeyboardHandler* keyboardHandler;

    MMU(KeyboardHandler* keyboardHandler)
    {
        this->keyboardHandler = keyboardHandler;
    }

    uint8_t read_memory(uint16_t address) {
        if (address <= 0x3FFF) {  // [0000-3FFF] Cartridge ROM, bank 0
            return ROM_bank_0[address];
        }
        else if (address >= 0x4000 && address <= 0x7FFF) {  // [4000-7FFF] Cartridge ROM, other banks
            return ROM_bank_1[address - 0x4000];
        }
        else if (address >= 0x8000 && address <= 0x9FFF) {  // [8000-9FFF] Graphics RAM
            return Graphic_RAM[address - 0x8000];
        }
        else if (address >= 0xA000 && address <= 0xBFFF) {  // [A000-BFFF] Cartridge (External) RAM
            return Cardridge_RAM[address - 0xA000];
        }
        else if (address >= 0xC000 && address <= 0xDFFF) {  // [C000-DFFF] Working RAM
            return Working_RAM[address - 0xC000];
        }
        else if (address >= 0xE000 && address <= 0xFDFF) {  // [E000-FDFF] Working RAM (shadow)
            return Working_RAM_shadow[address - 0xE000];
        }
        else if (address >= 0xFE00 && address <= 0xFE9F) {  // [FE00-FE9F] Graphics: sprite information
            return Graphics_sprite_information[address - 0xFE00];
        }
        else if (address >= 0xFF00 && address <= 0xFF7F) {  // [FF00-FF7F] Memory-mapped I/O
            if (address == 0xFF00)
            {
                return keyboardHandler->read_value();
            }

            return Memory_mapped_IO[address - 0xFF00];
        }
        else if (address >= 0xFF80 && address <= 0xFFFF) {  // [FF80-FFFF] Zero-page RAM
            return Zero_page_RAM[address - 0xFF80];
        }
        else {
            // Incorrect address
            return 0;
        }
    }

    void write_memory(uint16_t address, uint8_t value) {
        if (address <= 0x3FFF) {
            ROM_bank_0[address] = value;
        }
        else if (address >= 0x4000 && address <= 0x7FFF) {
            ROM_bank_1[address - 0x4000] = value;
            return;
        }
        else if (address >= 0x8000 && address <= 0x9FFF) {  // [8000-9FFF] Graphics RAM
            Graphic_RAM[address - 0x8000] = value;
        }
        else if (address >= 0xA000 && address <= 0xBFFF) {  // [A000-BFFF] Cartridge (External) RAM
            Cardridge_RAM[address - 0xA000] = value;
        }
        else if (address >= 0xC000 && address <= 0xDFFF) {  // [C000-DFFF] Working RAM
            Working_RAM[address - 0xC000] = value;
        }
        else if (address >= 0xE000 && address <= 0xFDFF) {  // [E000-FDFF] Working RAM (shadow)
            Working_RAM_shadow[address - 0xE000] = value;
        }
        else if (address >= 0xFE00 && address <= 0xFE9F) {  // [FE00-FE9F] Graphics: sprite information
            Graphics_sprite_information[address - 0xFE00] = value;
        }
        else if (address >= 0xFF00 && address <= 0xFF7F) {  // [FF00-FF7F] Memory-mapped I/O
            if (address == 0xFF00)
            {
                keyboardHandler->write_value(value);
                return;
            }

            Memory_mapped_IO[address - 0xFF00] = value;
        }
        else if (address >= 0xFF80 && address <= 0xFFFF) {  // [FF80-FFFF] Zero-page RAM
            Zero_page_RAM[address - 0xFF80] = value;
        }
    }

    void print_memory(uint16_t startAddress, uint16_t endAddress)
    {
        printf("Printing memory starting at: %d\n", startAddress);

        for (uint16_t i = startAddress; i < endAddress + 1; i++)
        {
            printf("%04x	%04x\n", i, read_memory(i));
        }
    }
};	

