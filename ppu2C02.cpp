/**
 * @file ppu2C02.cpp
 * @author Srisruth
 */
 
#include "ppu2C02.h"

ppu2C02::ppu2C02()
{
	STATUS.reg = 0;
	palScreen[0x00] = olc::Pixel(84, 84, 84);
	palScreen[0x01] = olc::Pixel(0, 30, 116);
	palScreen[0x02] = olc::Pixel(8, 16, 144);
	palScreen[0x03] = olc::Pixel(48, 0, 136);
	palScreen[0x04] = olc::Pixel(68, 0, 100);
	palScreen[0x05] = olc::Pixel(92, 0, 48);
	palScreen[0x06] = olc::Pixel(84, 4, 0);
	palScreen[0x07] = olc::Pixel(60, 24, 0);
	palScreen[0x08] = olc::Pixel(32, 42, 0);
	palScreen[0x09] = olc::Pixel(8, 58, 0);
	palScreen[0x0A] = olc::Pixel(0, 64, 0);
	palScreen[0x0B] = olc::Pixel(0, 60, 0);
	palScreen[0x0C] = olc::Pixel(0, 50, 60);
	palScreen[0x0D] = olc::Pixel(0, 0, 0);
	palScreen[0x0E] = olc::Pixel(0, 0, 0);
	palScreen[0x0F] = olc::Pixel(0, 0, 0);

	palScreen[0x10] = olc::Pixel(152, 150, 152);
	palScreen[0x11] = olc::Pixel(8, 76, 196);
	palScreen[0x12] = olc::Pixel(48, 50, 236);
	palScreen[0x13] = olc::Pixel(92, 30, 228);
	palScreen[0x14] = olc::Pixel(136, 20, 176);
	palScreen[0x15] = olc::Pixel(160, 20, 100);
	palScreen[0x16] = olc::Pixel(152, 34, 32);
	palScreen[0x17] = olc::Pixel(120, 60, 0);
	palScreen[0x18] = olc::Pixel(84, 90, 0);
	palScreen[0x19] = olc::Pixel(40, 114, 0);
	palScreen[0x1A] = olc::Pixel(8, 124, 0);
	palScreen[0x1B] = olc::Pixel(0, 118, 40);
	palScreen[0x1C] = olc::Pixel(0, 102, 120);
	palScreen[0x1D] = olc::Pixel(0, 0, 0);
	palScreen[0x1E] = olc::Pixel(0, 0, 0);
	palScreen[0x1F] = olc::Pixel(0, 0, 0);

	palScreen[0x20] = olc::Pixel(236, 238, 236);
	palScreen[0x21] = olc::Pixel(76, 154, 236);
	palScreen[0x22] = olc::Pixel(120, 124, 236);
	palScreen[0x23] = olc::Pixel(176, 98, 236);
	palScreen[0x24] = olc::Pixel(228, 84, 236);
	palScreen[0x25] = olc::Pixel(236, 88, 180);
	palScreen[0x26] = olc::Pixel(236, 106, 100);
	palScreen[0x27] = olc::Pixel(212, 136, 32);
	palScreen[0x28] = olc::Pixel(160, 170, 0);
	palScreen[0x29] = olc::Pixel(116, 196, 0);
	palScreen[0x2A] = olc::Pixel(76, 208, 32);
	palScreen[0x2B] = olc::Pixel(56, 204, 108);
	palScreen[0x2C] = olc::Pixel(56, 180, 204);
	palScreen[0x2D] = olc::Pixel(60, 60, 60);
	palScreen[0x2E] = olc::Pixel(0, 0, 0);
	palScreen[0x2F] = olc::Pixel(0, 0, 0);

	palScreen[0x30] = olc::Pixel(236, 238, 236);
	palScreen[0x31] = olc::Pixel(168, 204, 236);
	palScreen[0x32] = olc::Pixel(188, 188, 236);
	palScreen[0x33] = olc::Pixel(212, 178, 236);
	palScreen[0x34] = olc::Pixel(236, 174, 236);
	palScreen[0x35] = olc::Pixel(236, 174, 212);
	palScreen[0x36] = olc::Pixel(236, 180, 176);
	palScreen[0x37] = olc::Pixel(228, 196, 144);
	palScreen[0x38] = olc::Pixel(204, 210, 120);
	palScreen[0x39] = olc::Pixel(180, 222, 120);
	palScreen[0x3A] = olc::Pixel(168, 226, 144);
	palScreen[0x3B] = olc::Pixel(152, 226, 180);
	palScreen[0x3C] = olc::Pixel(160, 214, 228);
	palScreen[0x3D] = olc::Pixel(160, 162, 160);
	palScreen[0x3E] = olc::Pixel(0, 0, 0);
	palScreen[0x3F] = olc::Pixel(0, 0, 0);


}

ppu2C02::~ppu2C02()
{
}

void ppu2C02::Insertcartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    this->cartridge = cartridge;
}


uint8_t ppu2C02::cpuRead(uint16_t address, bool rdonly)
{
    uint8_t data = 0;
    switch (address)
    {
        case 0x0:
            break;
        case 0x1:
            break;
        case 0x2:
        	data  = ((STATUS.reg & 0xE0) | (address_buff & 0x1F));
    		STATUS.VB = 0;
    		address_latch = 0;
            break;
        case 0x3:

            break;
        case 0x4:
        	data = DRMS[OAMaddr];
            break;
        case 0x5:
            break;
        case 0x6:
            break;
        case 0x7:
        	data = address_buff;
    		address_buff = ppuRead(vram_address.reg);

    		if (vram_address.reg > 0x3f00)
    		{
    			data = address_buff;
    		}
    		vram_address.reg+= (CTRL.I)? 32:1;
            break;
    }
    return data;
}

void ppu2C02::cpuWrite(uint16_t address, uint8_t data)
{
    switch (address)
    {
    case 0x0:
    	CTRL.reg = data;
    	tram_address.NTx = CTRL.NX;
    	tram_address.NTy = CTRL.NY;
    	break;
    case 0x1:
    	MASK.reg = data;
    	break;
    case 0x2:
    	break;
    case 0x3:
    	OAMaddr = data;
    	break;
    case 0x4:
    	DRMS[OAMaddr] = data;
     	break;
    case 0x5:
    	if (address_latch == 0)
    	{
    		fine_x = data & 0x07;
    		tram_address.coarsex = data >> 3;
    		address_latch = 1;
    	}else
    	{
    		tram_address.FineY = data & 0x07;
    		tram_address.coarsey = data >> 3;
    		address_latch = 0;
    	}
        break;
    case 0x6:
    	 if (address_latch == 0)
    	 {
    	 	tram_address.reg = (tram_address.reg & 0x00FF) | data << 8;
    	 	address_latch = 1;
    	 }
    	else
    	{
    		tram_address.reg  = (tram_address.reg & 0xFF00) | data;
    		vram_address.reg = tram_address.reg;
    		address_latch = 0;
    	}

        break;
    case 0x7:
    	ppuWrite(vram_address.reg, data);
    	vram_address.reg+= (CTRL.I)? 32:1;
        break;
    }
}

uint8_t ppu2C02::ppuRead(uint16_t address, bool rdonly)
{
    uint8_t data = 0;
	address &= 0x3fff;
    if (cartridge->ppuRead(address, data))
    {

    }

	else if (address >= 0x000 && address <= 0x1FFF)
	{
		data = PatternT[(address & 0x1000) >> 12][address & 0x0FFF];
	}
	else if (address >= 0x2000 && address <= 0x3EFF)
	{
		uint16_t compar = 0x0400;
		if (cartridge->Mirror == Cartridge::HOR) compar = 0x0800;
		else if (cartridge->Mirror == Cartridge::VERT) compar = 0x0400;
		data = NameT[(address & compar) ? 1 : 0][address & 0x03FF];

	}
	else if ((address >= 0x3F00) && (address < 0x3FFF))
	{
		address&= 0x001F;
		if (address == 0x0010) address = 0x0000;
		if (address == 0x0014) address = 0x0004;
		if (address == 0x0018) address = 0x0008;
		if (address == 0x001C) address = 0x000C;
		data =  Palette[address];

	}

    return data;
}

void ppu2C02::ppuWrite(uint16_t address, uint8_t data)
{
	address &= 0x3fff;
    if (cartridge->ppuWrite(address, data))
    {

    }
	else if (address >= 0x0000 && address <= 0x1FFF)
	{
		PatternT[(address & 0x1000) >> 12][address & 0x0FFF] = data;

	}
	else if (address >= 0x2000 && address <= 0x3EFF)
	{
		uint16_t compar = 0x0400;
		if (cartridge->Mirror == Cartridge::HOR) compar = 0x0800;
		else if (cartridge->Mirror == Cartridge::VERT) compar = 0x0400;
		NameT[(address & compar) ? 1 : 0][address & 0x03FF] = data;

	}
	else if ((address >= 0x3F00) && (address < 0x3FFF))
	{
		address&= 0x001F;
		if (address == 0x0010) address = 0x0000;
		if (address == 0x0014) address = 0x0004;
		if (address == 0x0018) address = 0x0008;
		if (address == 0x001C) address = 0x000C;
		Palette[address] = data;

	}

}

olc::Sprite& ppu2C02::GetScreen()
{
    return sprScreen;
}

olc::Sprite& ppu2C02::GetNameT(uint8_t i)
{
	for (int y = 0; y < 30; y++)
	{
		for (int x = 0; x < 32; x++)
		{
			uint16_t offset = NameT[i][y*32 + x];
			for (int row = 0; row < 8; row++)
			{
				uint8_t LSB = ppuRead((1 << 12) + (offset*16) + row);
				uint8_t MSB = ppuRead((1 << 12) + (offset*16) + row + 8);
				for (int col = 0; col < 8; col++)
				{
					uint8_t pixel = (MSB & 0x01 ) << 1 | LSB & 0x01;
					MSB = MSB >> 1;
					LSB = LSB >> 1;
					sprNameT[i].SetPixel(x*8 + (7- col), y*8 + row, GetColor(0, pixel));
				}
			}


		}
	}
    return sprNameT[i];
}

olc::Sprite& ppu2C02::GetPattern(uint8_t i, uint8_t palete)
{
	for (uint16_t TileY = 0; TileY < 16; TileY++)
	{
		for (uint16_t TileX = 0; TileX < 16; TileX++)
		{
			uint16_t offset = TileY*256 + TileX*16;
			for (uint8_t row = 0; row < 8; row++)
			{
				uint8_t LSB = ppuRead((i*0x1000)+offset+row);
				uint8_t MSB = ppuRead((i*0x1000) +offset+ row +0x0008);
				for (uint8_t col = 0; col < 8; col++)
				{
					uint8_t pixel = (MSB & 0x01 ) << 1 | LSB & 0x01;
					MSB = MSB >> 1;
					LSB = LSB >> 1;
					sprPatternT[i].SetPixel(TileX*8 + (7- col), TileY*8 + row, GetColor(palete, pixel));
				}
			}
		}
	}

    return sprPatternT[i];
}

olc::Pixel& ppu2C02::GetColor(uint8_t Pal, uint8_t pixel){
	uint8_t data = ppuRead(0x3F00 + (Pal << 2) + pixel) & 0x3F;
	return palScreen[data];
}
void ppu2C02::clock()
{

	auto IncrementScrollX = [&]()
	{
		if (MASK.b || MASK.s)
		{
			if (vram_address.coarsex == 31)
			{
				vram_address.coarsex = 0;

				vram_address.NTx = ~vram_address.NTx;

			}
			else
			{
				vram_address.coarsex++;
			}

		}
	};

	auto IncrementScrollY = [&]()
	{
		if (MASK.b || MASK.s)
		{
			if (vram_address.FineY < 7)
			{
				vram_address.FineY++;
			}else
			{
				vram_address.FineY = 0;
				if (vram_address.coarsey == 29)
				{
					vram_address.coarsey = 0;

					vram_address.NTy = ~vram_address.NTy;
				}
				else if (vram_address.coarsey == 31)
				{

					vram_address.coarsey = 0;
				}
				else
				{
					vram_address.coarsey++;
				}
			}

		}
	};

	auto TransferX = [&]()
	{
		if (MASK.b || MASK.s)
		{
			vram_address.NTx = tram_address.NTx;
			vram_address.coarsex = tram_address.coarsex;

		}

	};

	auto TransferY = [&]()
	{
		if (MASK.b || MASK.s)
		{
			vram_address.NTy = tram_address.NTy;
			vram_address.coarsey = tram_address.coarsey;
			vram_address.FineY = tram_address.FineY;
		}
	};
	auto LoadBackgroundShifters = [&]()
	{
		bg_shi_ph = ((bg_shi_ph & 0xFF00) | (bg_next_tilemsb));
		bg_shi_pl = ((bg_shi_pl & 0xFF00) | (bg_next_tilelsb));
		bg_attribh = ((bg_attribh & 0xFF00) | ((bg_next_attrib & 0b10) ? 0xFF : 0x00));
		bg_attribl = ((bg_attribl & 0xFF00) | ((bg_next_attrib & 0b01 )? 0xFF : 0x00));
	};

	auto UpdateShifters = [&]()
	{
		if (MASK.b)
		{
			bg_shi_ph <<= 1;
			bg_shi_pl <<= 1;
			bg_attribh <<= 1;
			bg_attribl <<= 1;
		}
		if (MASK.s && cycle >=1 && cycle < 258)
		{
			for (int i =0; i < spriteCount; i++)
			{
				if (spriteScanline[i].x > 0){
					spriteScanline[i].x--;
				}
				else
				{
					sprite_shifter_patter_lo[i] = sprite_shifter_patter_lo[i] << 1;
					sprite_shifter_patter_hi[i] = sprite_shifter_patter_hi[i] << 1;
				}
			}
		}
	};
	if (scanline >= -1 && scanline < 240)
	{
		if (scanline == 0 && cycle == 0)
		{
			cycle = 1;
		}

		if ((scanline == -1)&&(cycle ==1))
		{

			STATUS.S = 0;
			STATUS.VB = 0;
			STATUS.O = 0;
			for (int i =0; i < 8; i++)
			{
				sprite_shifter_patter_lo[i] = 0;
				sprite_shifter_patter_hi[i] = 0;
			}
		}
		if ((cycle >= 2 && cycle < 258) ||(cycle >= 321 && cycle < 338 ))
		{
			UpdateShifters();

			switch((cycle -1) % 8)
			{
			case 0:
				LoadBackgroundShifters();
				bg_next_tileid = ppuRead(0x2000 | (vram_address.reg&0x0FFF));
				break;
			case 2:
				bg_next_attrib = ppuRead(0x23C0 | (vram_address.NTy) << 11 | (vram_address.NTx << 10)
					|( (vram_address.coarsey >> 2) << 3) | (vram_address.coarsex >> 2));

				if (vram_address.coarsey & 0x02) bg_next_attrib >>= 4;
				if (vram_address.coarsex & 0x02) bg_next_attrib >>= 2;
				bg_next_attrib &= 0x03;
				break;
			case 4:
				bg_next_tilelsb = ppuRead((CTRL.B << 12) + (uint16_t)(bg_next_tileid << 4) + vram_address.FineY);
				break;
			case 6:
				bg_next_tilemsb = ppuRead((CTRL.B << 12) + (uint16_t)(bg_next_tileid << 4) + (vram_address.FineY + 8));
				break;
			case 7:
				IncrementScrollX();
			}
		}
		if (cycle == 256)
		{
			IncrementScrollY();
		}
		if (cycle == 257)
		{
			LoadBackgroundShifters();
			TransferX();
		}
		if (cycle == 338 || cycle == 340)
		{
			bg_next_tileid = ppuRead(0x2000 | (vram_address.reg & 0x0FFF));
		}

		if (scanline == -1 && cycle >= 280 && cycle < 305)
		{
			TransferY();

		}

	}

	if (cycle == 257 && scanline >= 0)
	{
		std::memset(spriteScanline, 0xFF, 8*sizeof(OAM_entry));
		spriteCount = 0;
		for (uint8_t i = 0; i < 8; i++)
		{
			sprite_shifter_patter_lo[i] = 0;
			sprite_shifter_patter_hi[i] = 0;
		}
		uint8_t OAMe = 0;
		sp0poss = false;
		while (OAMe < 64 && spriteCount < 9)
		{
			int16_t diff = (int16_t)scanline - (int16_t)OAM[OAMe].y;
			if (diff >= 0 && diff < (CTRL.H ? 16 : 8) )
			{

				if (spriteCount < 8)
				{
					if (OAMe == 0)
					{
						sp0poss = true;
					}
					spriteScanline[spriteCount] =  OAM[OAMe];
					spriteCount++;
				}
			}
			OAMe++;
			STATUS.O = spriteCount > 8;
		}
	}

	if (cycle == 340)
	{
		for (int i = 0; i < spriteCount; i++){
			uint8_t sprite_pattern_lo, sprite_pattern_hi;
			uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;
			if (!CTRL.H)
			{
				if (!((spriteScanline[i].attr) & 0x80))
				{
					sprite_pattern_addr_lo = ((CTRL.S << 12) |( spriteScanline[i].id << 4 )| (scanline - spriteScanline[i].y));
				}
				else
				{
					sprite_pattern_addr_lo = ((CTRL.S << 12) |( spriteScanline[i].id << 4 )| (7 - (scanline - spriteScanline[i].y)));
				}
			}else
			{
				if (!((spriteScanline[i].attr) & 0x80))
				{
					sprite_pattern_addr_lo = (((spriteScanline[i].id & 0x01) << 12)
						| (((spriteScanline[i].id & 0xFE) + (scanline - spriteScanline[i].y < 8? 0 : 1)) << 4) |
						((scanline - spriteScanline[i].y) &0x07));

				}
				else
				{
					sprite_pattern_addr_lo = (((spriteScanline[i].id & 0x01) << 12)
											| (((spriteScanline[i].id & 0xFE) + (scanline - spriteScanline[i].y < 8? 1 : 0)) << 4) |
											( 7 -(scanline - spriteScanline[i].y) &0x07));

				}
			}
			sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
			sprite_pattern_hi = ppuRead(sprite_pattern_addr_hi);
			sprite_pattern_lo = ppuRead(sprite_pattern_addr_lo);
			//function from https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
			if (spriteScanline[i].attr & 0x40)
			{
				auto flipbyte = [](uint8_t b)
				{
					b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
					b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
					b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
					return b;
				};
				sprite_pattern_hi = flipbyte(sprite_pattern_hi);
				sprite_pattern_lo = flipbyte(sprite_pattern_lo);
			}
			sprite_shifter_patter_hi[i] = sprite_pattern_hi;
			sprite_shifter_patter_lo[i] = sprite_pattern_lo;
		}

	}
	uint8_t bgpixel = 0x00;
	uint8_t bgpal = 0x00;
	if (MASK.b)
	{
		// depending upon fine x scolling. This has the effect of
		// offsetting ALL background rendering by a set number
		// of pixels, permitting smooth scrolling
		uint16_t bit_mux = 0x8000 >> fine_x;
		// Select Plane pixels by extracting from the shifter
		// at the required location.
		uint8_t p0_pixel = (bg_shi_pl & bit_mux) > 0;
		uint8_t p1_pixel = (bg_shi_ph & bit_mux) > 0;
		// Combine to form pixel index
		bgpixel = (p1_pixel << 1) | p0_pixel;
		// Get palette
		uint8_t bg_pal0 = (bg_attribl & bit_mux) > 0;
		uint8_t bg_pal1 = (bg_attribh & bit_mux) > 0;
		bgpal = (bg_pal1 << 1) | bg_pal0;
	}
	uint8_t fgpixel = 0x00;
	uint8_t fgpal = 0x00;
	uint8_t fgpriority = 0x00;
	if (MASK.s)
	{
		sp0r = false;
		for (int i =0 ; i < spriteCount; i++)
		{
			if (spriteScanline[i].x == 0)
			{
				uint8_t fg_pixel_lo =  ((sprite_shifter_patter_lo[i] & 0x80) > 0);
				uint8_t fg_pixel_hi = ((sprite_shifter_patter_hi[i] & 0x80) > 0);
				fgpixel  = (fg_pixel_hi << 1) | fg_pixel_lo;

				fgpal = (spriteScanline[i].attr & 0x03) + 0x04;
				fgpriority = (spriteScanline[i].attr & 0x20)  == 0 ;
				if (fgpixel != 0)
				{
					if (i == 0)
					{
						sp0r = true;
					}
					break;
				}

			}
		}
	}
	uint8_t pixel = 0x0;
	uint8_t pal = 0x0;

	if ((bgpixel == 0) && (fgpixel == 0))
	{
		pixel = 0;
		pal = 0;
	}else if ((bgpixel == 0) && (fgpixel > 0))
	{
		pixel = fgpixel;
		pal = fgpal;
	}else if ((bgpixel > 0) && (fgpixel == 0)){
		pixel = bgpixel;
		pal = bgpal;
	}
	else if ((bgpixel > 0) && (fgpixel > 0))
	{
		if (fgpriority)
		{
			pixel = fgpixel;
			pal = fgpal;
		}
		else
		{
			pixel = bgpixel;
			pal = bgpal;
		}


		if (sp0poss && sp0r)
		{


			if ((MASK.b)&&(MASK.s))
			{
				if (~(MASK.m|MASK.M))
				{
					if (cycle >= 9 && cycle <= 258)
					{
						STATUS.S = 1;
					}
				}else if (cycle >= 1 && cycle < 258)
				{
					STATUS.S = 1;
				}
			}
		};
	}
if(scanline>= 241 && scanline <261){
	if ((scanline == 241)&&(cycle == 1))
	{
		STATUS.VB = 1;
		if (CTRL.V)
		{
			nmi = true;
		}

	}
}
 	sprScreen.SetPixel(cycle - 1, scanline, GetColor(pal, pixel));
	cycle++;
    if (cycle >= 341)
    {
        cycle = 0;
        scanline++;
    if (scanline >= 261)
    {
        scanline = -1;
        frame_complete = true;
    }
}
}