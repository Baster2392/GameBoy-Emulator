#pragma once
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include "MMU.h"

class GPU
{
public:
	MMU* mmu;
	uint8_t framebuffer[160 * 144]; // Ekran: 160x144 pikseli

	/*
		Lcd control register:
		bit
		0 - Background: on/off
		1 - Sprites: on/off
		2 - Sprites: size (pixels)
		3 - Background: tile map
		4 - Background: tile set
		5 - Window: on/off
		6 - Window: tile map
		7 - Display: on/off	
	*/
	uint8_t lcdc;
	/*
		GPU status register:
		bit
		0, 1 - PPU status
		2 - lyc == line
		3 - gpu in 0 mode
		4 - gpu in 1 mode
		5 - gpu in 2 mode
		6 - check lyc == line
	*/
	uint8_t stat;
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

	GPU(MMU* mmu)
	{
		this->mmu = mmu;
	}

	// Execute single GPU step.
	void step(int cycles)
	{
		// callculate cycles done in last performed instruction
		this->mode_clock += cycles;

		if (this->stat & 0x40)	// if bit is set, check condition lyc == line
		{
			this->stat = (this->lyc == this->line) ? this->stat | 0x04 : this->stat & 0xFB;
		}

		switch (this->mode)
		{
		case 2:	// OAM read mode, scanline active
			if (this->mode_clock >= 80)
			{
				uint16_t sprite_info_address = this->mmu->read_memory(0xFF46) << 8;	// read location of sprite attributes in working memory

				// transfer sprite attributes to OAM
				for (int i = 0; i < 0xA0; i++)
				{
					this->mmu->Graphics_sprite_information[i] = this->mmu->read_memory(sprite_info_address);
					sprite_info_address++;
				}

				// Enter scanline mode 3
				this->mode_clock = 0;
				this->stat += 1;
				this->mode = 3;
			}
			break;
		case 3:	// VRAM read mode, scanline active
			if (this->mode_clock >= 172)
			{
				render_scanline();
				this->mode_clock = 0;
				this->stat -= 3;
				this->mode = 0;
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
					this->stat += 1;
					this->ready_to_render = true;
				}
				else
				{
					this->stat += 2;
					this->mode = 2;
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
					this->stat += 1;
					this->line = 0;
					// set v-blank happened flag
					this->mmu->Memory_mapped_IO[0xF] |= 0x1;
					// set stat interrupt happened flag
					this->mmu->Memory_mapped_IO[0xF] |= 0x2;

				}
			}
		break;
		default:
			break;
		}
	}

	void render_scanline()
	{
		if (this->lcdc & 0x1)
		{
			renderscan_background();
		}

		if (this->lcdc & 0x20)
		{
			renderscan_window();
		}

		if (this->lcdc & 0x2)
		{
			renderscan_sprites();
		}
	}

	void renderscan_sprites()
	{
		uint16_t oamIterator = 0;
		// for each sprite
		for (int i = 0; i < 40; i++)
		{
			// read sprite from memory
			uint8_t y = this->mmu->Graphics_sprite_information[oamIterator++] - 8;
			uint8_t x = this->mmu->Graphics_sprite_information[oamIterator++] - 16;
			uint8_t tileIndex = this->mmu->Graphics_sprite_information[oamIterator++];
			uint8_t options = this->mmu->Graphics_sprite_information[oamIterator++];

			// check if sprite is in this scanline
			if (y <= this->line && y + 8 > this->line)
			{
				uint8_t palette = (options & 0x20 ? this->obp1 : this->obp0);
				
				for (int pix = 0; pix < 8; pix++)
				{
					if (x + pix >= 0 && x + pix < 160) // TODO: to extend
					{
						// select pixel
						int pixelX = (options & 0x20) ? 7 - pix : pix;
						// select line
						int tileLine = (options & 0x40) ? 7 - (this->line - y) : this->line - y;

						uint8_t low = this->mmu->Graphic_RAM[(tileIndex * 16) + tileLine * 2];
						uint8_t high = this->mmu->Graphic_RAM[(tileIndex * 16) + tileLine * 2 + 1];

						// decode color value
						int colorBit = 7 - (pixelX % 8);
						int color = ((high >> colorBit) & 1) << 1 | ((low >> colorBit) & 1);

						// write to buffer
						int buffer_index = this->line * 160 + x + pix;
						if (options & 0x80)
						{
							// write to buffer only, when background pixel is 0
							if (this->framebuffer[buffer_index] == 0)
							{
								uint8_t pixelColor = (palette >> (color * 2)) & 0x03;
								this->framebuffer[buffer_index] = this->color_palette[pixelColor];
							}
						}
						else
						{
							uint8_t pixelColor = (palette >> (color * 2)) & 0x03;
							if (pixelColor == 3) continue;
							this->framebuffer[buffer_index] = this->color_palette[pixelColor];
						}
					}
				}
			}
		}
	}

	void renderscan_background() {
		uint8_t* tileMap = (this->lcdc & 0x08) ? &this->mmu->Graphic_RAM[0x1C00] : &this->mmu->Graphic_RAM[0x1800];
		uint8_t* tileData = (this->lcdc & 0x10) ? &this->mmu->Graphic_RAM[0x0000] : &this->mmu->Graphic_RAM[0x0800];

		int y = (this->line + this->scy) & 0xFF;
		int tileRow = (y / 8) * 32;

		for (int x = 0; x < 160; x++)
		{
			int pixelX = (x + this->scx) & 0xFF;
			int tileCol = pixelX / 8;
			int tileIndex = tileMap[tileRow + tileCol];

			uint8_t tileLine = (y % 8) * 2;
			int byteIndex;
			if (!(this->lcdc & 0x10))
			{ // Signed indexing
				byteIndex = ((int8_t)tileIndex) * 16 + tileLine;
			}
			else
			{ // Unsigned indexing
				byteIndex = tileIndex * 16 + tileLine;
			}

			uint8_t low = tileData[byteIndex];
			uint8_t high = tileData[byteIndex + 1];

			int colorBit = 7 - (pixelX % 8);
			int color = ((high >> colorBit) & 1) << 1 | ((low >> colorBit) & 1);

			uint8_t pixelColor = (this->bgp >> (color * 2)) & 0x03;
			this->framebuffer[this->line * 160 + x] = this->color_palette[pixelColor];
		}
	}


	void renderscan_window()
	{
		if (this->line < this->wy)
		{
			return;
		}

		// get vram sector selected in lcdc register
		uint8_t* tileMap = (this->lcdc & 0x40) ? &this->mmu->Graphic_RAM[0x1C00] : &this->mmu->Graphic_RAM[0x1800];
		uint8_t* tileData = (this->lcdc & 0x10) ? &this->mmu->Graphic_RAM[0x0000] : &this->mmu->Graphic_RAM[0x0800];

		// calculate tile row
		int y = (this->line + this->wy) & 0xFF;
		int tileRow = (y / 8) * 32;

		for (int x = 0; x < 160; x++)
		{
			// select pixel using scx register
			int pixelX = (x + this->wy) & 0xFF;
			int tileCol = pixelX / 8;
			int tileIndex = tileMap[tileRow + tileCol];
			uint8_t tileLine = (y % 8) * 2;

			uint8_t low = tileData[tileIndex * 16 + tileLine];
			uint8_t high = tileData[tileIndex * 16 + tileLine + 1];

			int colorBit = 7 - (pixelX % 8);
			int color = ((high >> colorBit) & 1) << 1 | ((low >> colorBit) & 1);

			uint8_t pixelColor = (this->bgp >> (color * 2)) & 0x03;
			this->framebuffer[this->line * 160 + x] = this->color_palette[pixelColor];
		}
	}
};

