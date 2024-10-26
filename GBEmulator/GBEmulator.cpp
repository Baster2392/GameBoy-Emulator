#include <iostream>
#include <cstddef>
#include <cstdint>
#include "CPU.h"

using namespace std;

void load_ROM_procedure()
{
	FILE* file;
	fopen_s(&file, "ROMs/cpu_instrs.gb", "rb");

	if (!file)
	{
		cout << "File not found!" << endl;
		return;
	}

	CPU cpu = CPU();
	uint8_t buffer[0xFFFF];
	fread(&buffer, sizeof(uint8_t), 0xFFFF, file);
	fseek(file, 0L, SEEK_END);
	int file_size = ftell(file);

	for (uint16_t i = 0; i < 100; i++)
	{
		for (uint16_t j = 0; j < 15; j++)
		{
			printf("%02x ", buffer[i * 16 + j]);
		}

		printf("\n");
	}

	printf("Loading ROM to GB memory...\n");
	for (int i = 0; i < file_size; i++)
	{
		cpu.writeMemory(i, buffer[i]);
		printf("\rLoaded %d from %d elements.", i, file_size);
	}

	printf("\nROM loaded.\nROM size: %d\n", file_size);

	cpu.PC = 0x100;
	while (true)
	{
		cpu.step();
	}
}

int main()
{
	load_ROM_procedure();
}