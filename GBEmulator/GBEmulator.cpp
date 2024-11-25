#define SDL_MAIN_HANDLED

#include "CPU.h"
#include <cstddef>
#include <cstdint>
#include <SDL.h>

using namespace std;

void load_ROM_procedure(CPU& cpu)
{
	FILE* file;
	fopen_s(&file, "ROMs/opus_tests/opus5.gb", "rb");
	// fopen_s(&file, "ROMs/instruction_tests/05-op rp.gb", "rb");

	if (!file)
	{
		cout << "File not found!" << endl;
		return;
	}

	uint8_t buffer[0xFFFF];
	fread(&buffer, sizeof(uint8_t), 0xFFFF, file);
	fseek(file, 0L, SEEK_END);
	int file_size = ftell(file);

	printf("Loading ROM to GB memory...\n");
	for (int i = 0; i < file_size; i++)
	{
		cpu.writeMemory(i, buffer[i]);
		printf("\rLoaded %d from %d elements.", i, file_size);
	}

	fclose(file);
	printf("\nROM loaded.\nROM size: %d\n", file_size);
}

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Nie udało się zainicjować SDL: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	CPU cpu = CPU();
	load_ROM_procedure(cpu);
	cpu.PC = 0x100;

	while (true)
	{
		cpu.step();
		// getchar();
	}

	SDL_Quit();
	return 0;
}