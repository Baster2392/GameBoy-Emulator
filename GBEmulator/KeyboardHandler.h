#pragma once
#include <cstdint>
#include <atomic>
#include <SDL.h>
#include "UI.h"

using namespace std;

class KeyboardHandler
{
	atomic<uint8_t> rows_values[2] = { 0x0F, 0x0F };
	atomic<uint8_t> column_value = 0;

public:
	atomic<bool> interrupt_happened = false;

	void write_value(uint8_t value)
	{
		this->column_value = value & 0x30;
	}

	uint8_t read_value()
	{
		if (this->column_value == 0x10)
		{
			return this->rows_values[0];
		}
		else if (this->column_value == 0x20)
		{
			return this->rows_values[1];
		}

		return 0;
	}

	UIAction pollEvents(atomic<bool>* running,
		const vector<UIButton>& buttons,
		atomic<int>& mouseX, atomic<int>& mouseY)
	{
		UIAction result = UIAction::None;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				*running = false;
				result = UIAction::Quit;
				break;

			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					for (const auto& btn : buttons) {
						if (btn.contains(event.button.x, event.button.y)) {
							result = btn.action;
							break;
						}
					}
				}
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_F5:
					result = UIAction::LoadROM;
					break;
				case SDL_SCANCODE_F6:
					result = UIAction::TogglePause;
					break;
				case SDL_SCANCODE_F7:
					result = UIAction::Reset;
					break;
				case SDL_SCANCODE_ESCAPE:
					result = UIAction::Quit;
					break;
				case SDL_SCANCODE_D:
					this->rows_values[1] &= 0xE;
					this->interrupt_happened = true;
					break;
				case SDL_SCANCODE_A:
					this->rows_values[1] &= 0xD;
					this->interrupt_happened = true;
					break;
				case SDL_SCANCODE_W:
					this->rows_values[1] &= 0xB;
					this->interrupt_happened = true;
					break;
				case SDL_SCANCODE_S:
					this->rows_values[1] &= 0x7;
					this->interrupt_happened = true;
					break;
				case SDL_SCANCODE_K:
					this->rows_values[0] &= 0xE;
					this->interrupt_happened = true;
					break;
				case SDL_SCANCODE_L:
					this->rows_values[0] &= 0xD;
					this->interrupt_happened = true;
					break;
				case SDL_SCANCODE_KP_ENTER:
					this->rows_values[0] &= 0xB;
					this->interrupt_happened = true;
					break;
				case SDL_SCANCODE_SPACE:
					this->rows_values[0] &= 0x7;
					this->interrupt_happened = true;
					break;
				default:
					break;
				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_D:
					this->rows_values[1] |= 0x1;
					break;
				case SDL_SCANCODE_A:
					this->rows_values[1] |= 0x2;
					break;
				case SDL_SCANCODE_W:
					this->rows_values[1] |= 0x4;
					break;
				case SDL_SCANCODE_S:
					this->rows_values[1] |= 0x8;
					break;
				case SDL_SCANCODE_K:
					this->rows_values[0] |= 0x1;
					break;
				case SDL_SCANCODE_L:
					this->rows_values[0] |= 0x2;
					break;
				case SDL_SCANCODE_KP_ENTER:
					this->rows_values[0] |= 0x4;
					break;
				case SDL_SCANCODE_SPACE:
					this->rows_values[0] |= 0x8;
					break;
				default:
					break;
				}
				break;
			}
		}
		return result;
	}
};

