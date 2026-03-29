#define SDL_MAIN_HANDLED
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "CPU.h"
#include "KeyboardHandler.h"
#include <cstddef>
#include <cstdint>
#include <SDL.h>
#include <thread>
#include <functional>
#include <atomic>
#include <string>
#include <windows.h>
#include <commdlg.h>

#pragma comment(lib, "comdlg32.lib")

#define DEBUG_MODE false

using namespace std;

atomic<bool> running(true);
atomic<bool> romLoaded(false);

string openFileDialog()
{
	OPENFILENAMEA ofn;
	char szFile[260] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "GameBoy ROMs (*.gb;*.gbc)\0*.gb;*.gbc\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = "Select a GameBoy ROM";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn))
	{
		return string(szFile);
	}
	return "";
}

bool load_ROM(CPU& cpu, const string& path)
{
	FILE* file;
	fopen_s(&file, path.c_str(), "rb");

	if (!file)
	{
		cout << "File not found: " << path << endl;
		return false;
	}

	fseek(file, 0L, SEEK_END);
	int file_size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	uint8_t buffer[0xFFFF];
	fread(&buffer, sizeof(uint8_t), 0xFFFF, file);

	printf("Loading ROM to GB memory...\n");
	for (int i = 0; i < file_size; i++)
	{
		cpu.writeMemory(i, buffer[i]);
	}

	fclose(file);
	printf("ROM loaded. Size: %d bytes\n", file_size);
	return true;
}

void cpu_loop(CPU& cpu)
{
	while (running)
	{
		if (!cpu.renderer.paused && romLoaded)
		{
			cpu.step();
		}
		else
		{
			// Keep rendering the UI when paused or no ROM
			cpu.renderer.render(cpu.gpu.framebuffer);
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	}
}

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}

	CPU cpu = CPU(DEBUG_MODE);
	cpu.renderer.paused = true;
	cpu.renderer.romLoaded = false;

	std::thread cpuThread(cpu_loop, std::ref(cpu));

	// Main event loop
	while (running) {
		UIAction action = cpu.keyboardHandler.pollEvents(
			&running, cpu.renderer.buttons, cpu.renderer.mouseX, cpu.renderer.mouseY);

		switch (action) {
		case UIAction::LoadROM:
		{
			bool wasPaused = cpu.renderer.paused.load();
			cpu.renderer.paused = true;

			string path = openFileDialog();
			if (!path.empty())
			{
				cpu.reset();
				if (load_ROM(cpu, path))
				{
					cpu.PC = 0x100;
					romLoaded = true;
					cpu.renderer.romLoaded = true;
					cpu.renderer.paused = false;

					// Update window title with ROM name
					string filename = path.substr(path.find_last_of("\\/") + 1);
					string title = "GameBoy Emulator - " + filename;
					SDL_SetWindowTitle(cpu.renderer.getWindow(), title.c_str());
				}
				else
				{
					cpu.renderer.paused = wasPaused;
				}
			}
			else
			{
				cpu.renderer.paused = wasPaused;
			}
			break;
		}
		case UIAction::TogglePause:
			if (romLoaded) {
				cpu.renderer.paused = !cpu.renderer.paused.load();
			}
			break;
		case UIAction::Reset:
			if (romLoaded) {
				cpu.renderer.paused = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				cpu.reset();
				cpu.PC = 0x100;
				cpu.renderer.paused = false;
			}
			break;
		case UIAction::Quit:
			running = false;
			break;
		default:
			break;
		}

		SDL_Delay(1);
	}

	cpuThread.join();
	SDL_Quit();
	return 0;
}