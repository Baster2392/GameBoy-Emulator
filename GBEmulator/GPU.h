#pragma once
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

class GPU
{
public:
	uint8_t* vram;		// VRAM: 8 KB
	uint8_t* oam;       // OAM: Sprite attribute table
	uint8_t framebuffer[160 * 144]; // Ekran: 160x144 pikseli

	uint8_t lcdc;            // Rejestr sterowania LCD
	uint8_t stat;            // Rejestr statusu GPU
	uint8_t scy, scx;        // Scroll Y, Scroll X
	uint8_t lyc;             // Porównanie z LY
	uint8_t bgp;             // Paleta t³a
	uint8_t obp0, obp1;      // Palety sprite'ów
	uint8_t wy, wx;          // Pozycja okna

	uint8_t color_palette[4] = {255, 127, 63, 0};

	bool ready_to_render = false;

	/*
		GPU mode number:
		0 - horizontal blank
		1 - vertical blank
		2 - scanline (accessing OAM)
		3 - scanline (accessing VRAM)
	*/
	int mode;
	/*
		Clocks spent in current mode.
	*/
	int mode_clock;
	/*
		Number of currently proccessing line.
	*/
	int line;
	/*
		Value of cpu cycles in previous instruction
	*/
	int cpu_cycles;

	GPU(uint8_t* vram, uint8_t* oam)
	{
		this->vram = vram;
		this->oam = oam;
	}

	// Execute single GPU step.
	void step(int cycles)
	{
		// callculate cycles done in last performed instruction
		this->mode_clock += cycles;

		switch (this->mode)
		{
		case 2:	// OAM read mode, scalnine active
			if (this->mode_clock >= 80)
			{
				// Enter scanline mode 3
				this->mode_clock = 0;
				this->mode = 3;
				// this->stat = (this->stat & 0xFC) | this->mode;
			}
			break;
		case 3:	// VRAM read mode, scanline active
			if (this->mode_clock >= 172)
			{
				this->mode_clock = 0;
				this->mode = 0;
				// this->stat = (this->stat & 0xFC) | this->mode;
				render_scanline();
			}
			break;
		case 0:	// horizontal blank
			if (this->mode_clock >= 204)
			{
				this->mode_clock = 0;
				this->line++;

				if (this->line == 143)
				{
					this->mode = 1;
					// this->stat = (this->stat & 0xFC) | this->mode;
				}
				else
				{
					this->mode = 2;
					// this->stat = (this->stat & 0xFC) | this->mode;
				}
			}
			break;
		case 1:	// vertical blank
			if (this->mode_clock >= 456)
			{
				this->mode_clock = 0;
				this->line++;

				if (this->line >= 153)
				{
					// Restart scanning modes
					this->mode = 2;
					this->line = 0;
					// this->stat = (this->stat & 0xFC) | this->mode;

				}
			}
		break;
		default:
			break;
		}
	}

	void render_scanline()
	{
		// get vram sector selected in lcdc register
		uint8_t* tileMap = (this->lcdc & 0x08) ? &vram[0x1C00] : &vram[0x1800];
		uint8_t* tileData = (this->lcdc & 0x10) ? &vram[0x0000] : &vram[0x0800];

		// calculate tile row
		int y = (this->line + this->scy) & 0xFF;
		int tileRow = (y / 8) * 32;

		for (int x = 0; x < 160; x++) {
			// select pixel using scx register
			int pixelX = (x + this->scx) & 0xFF;
			int tileCol = pixelX / 8;
			int tileIndex = tileMap[tileRow + tileCol];
			uint8_t tileLine = (y % 8) * 2;

			uint8_t low = tileData[tileIndex * 16 + tileLine];
			uint8_t high = tileData[tileIndex * 16 + tileLine + 1];

			int colorBit = 7 - (pixelX % 8);
			int color = ((high >> colorBit) & 1) << 1 | ((low >> colorBit) & 1);
			
			uint8_t pixelColor = (bgp >> (color * 2)) & 0x03;
			this->framebuffer[this->line * 160 + x] = color_palette[pixelColor];
		}

		ready_to_render = true;
		printf("Rendered\n");
		// getchar(); getchar();
	}
};

