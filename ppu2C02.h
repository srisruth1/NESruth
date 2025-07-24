/**
 * @file ppu2C02.h
 * @author Srisruth
 *
 *
 */
 
#ifndef PPU2C02_H
#define PPU2C02_H
#include <cstdint>
#include <memory>
#include "olcPixelGameEngine.h"
#include "Cartridge.h"


class ppu2C02 {
private:
    std::shared_ptr<Cartridge> cartridge;

    uint8_t PatternT[2][4096];
    uint8_t Palette[32];

    olc::Pixel palScreen[0x40];
    olc::Sprite sprScreen = olc::Sprite(256,250);
    olc::Sprite sprNameT[2] = {olc::Sprite(256,240),olc::Sprite(256,240)};
    olc::Sprite sprPatternT[2] = {olc::Sprite(128,128),olc::Sprite(128,128)};

    int scanline = 0;
    int cycle = 0;
    struct OAM_entry
    {
        uint8_t y;
        uint8_t id;
        uint8_t attr;
        uint8_t x;
    }OAM[64];
    OAM_entry spriteScanline[8];
    uint8_t spriteCount = 0;

public:


    uint8_t NameT[2][1024];
    ppu2C02();
    ~ppu2C02();
    bool nmi = false;
    void Insertcartridge(const std::shared_ptr<Cartridge>& cartridge);
    void clock();


    uint8_t cpuRead(uint16_t address, bool rdonly = false);
    void cpuWrite(uint16_t address, uint8_t data);

    uint8_t ppuRead(uint16_t address, bool rdonly = false);
    void ppuWrite(uint16_t address, uint8_t data);

    //debugging utilities
    olc::Sprite& GetScreen();
    olc::Sprite& GetNameT(uint8_t i);
    olc::Sprite& GetPattern(uint8_t i, uint8_t palete);
    olc::Pixel& GetColor(uint8_t Pal, uint8_t pixel);
    bool frame_complete = false;

    //IO mapped registers
    union
    {
        struct
        {
            uint8_t NX : 1;//nametable x
            uint8_t NY : 1;//nametable y
            uint8_t I :  1;//increment mode
            uint8_t S : 1;//sprite tile select
            uint8_t B : 1;//background tile select
            uint8_t H : 1;//sprite height
            uint8_t P : 1;//master/slave
            uint8_t V : 1;// NMI enable
        };
        uint8_t reg;
    }CTRL;

    union
    {
        struct
        {
            uint8_t G:1; //Greyscale
            uint8_t m:1; //background left column enable
            uint8_t M:1; //sprite left column enable
            uint8_t b:1; //background enable
            uint8_t s:1; //sprite enable
            uint8_t ER:1; //enhance red
            uint8_t EG:1; //enhance green
            uint8_t EB:1; //enhance blue



        };
        uint8_t reg;
    }MASK;

    union
    {
        struct
        {
            uint8_t Blank: 5;
            uint8_t O:1; //overflow
            uint8_t S:1; //sprite0 hit
            uint8_t VB:1; //vblank


        };
        uint8_t reg;
    }STATUS;
    union Scroll
    {
        struct
        {
            uint16_t coarsex: 5;
            uint16_t coarsey: 5;
            uint16_t NTx: 1;
            uint16_t NTy: 1;
            uint16_t FineY: 3;
            uint16_t U: 1;
        };
        uint16_t reg;
    };

    uint8_t address_latch = 0;
    uint8_t address_buff = 0;

    Scroll vram_address;
    Scroll tram_address;

    uint8_t fine_x = 0;

    uint8_t bg_next_tileid, bg_next_attrib, bg_next_tilelsb, bg_next_tilemsb = 0x0000;

    uint16_t bg_shi_pl, bg_shi_ph, bg_attribl , bg_attribh = 0x0000;

   uint8_t* DRMS = (uint8_t*)(OAM);

    uint8_t OAMaddr = 0x00;

    uint8_t sprite_shifter_patter_lo[8];
    uint8_t sprite_shifter_patter_hi[8];
    bool sp0poss = false;
    bool sp0r = false;
    bool sprh = false;




};



#endif //PPU2C02_H
