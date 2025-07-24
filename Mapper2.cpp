/**
 * @file Mapper2.cpp
 * @author Srisruth
 */
 
#include "Mapper2.h"

Mapper2::Mapper2(uint8_t prgBanks, uint8_t chrBanks): Mapper(prgBanks, chrBanks), reg(0)
{
    high = prgBanks - 1;
}

Mapper2::~Mapper2()
{
}

bool Mapper2::cpuMapRead(uint16_t address, uint32_t& Mappedaddress)
{
    if (address >= 0x8000 && address <= 0xBFFF)
    {
        Mappedaddress = 0x4000 * reg + (address & 0x3FFF);

        return true;
    }else if (address >= 0xC000 && address <= 0xFFFF)
    {
        Mappedaddress = 0x4000 * 7 + (address & 0x3FFF);
        return true;
    }


    return false;
}

bool Mapper2::cpuMapWrite(uint16_t address, uint32_t& Mappedaddress)
{
    return false;
}

bool Mapper2::ppuMapRead(uint16_t address, uint32_t& Mappedaddress)
{
    if (address <= 0x1FFF)
    {
        Mappedaddress = address;

        return true;
    }
    return false;
}

bool Mapper2::ppuMapWrite(uint16_t address, uint32_t& Mappedaddress)
{
    if (address >= 0x0000 && address <= 0x1FFF)
    {
        if (chrBank == 0)
        {
            // Treat as RAM
            Mappedaddress = address;
            return true;
        }
    }

    return false;
}

bool Mapper2::cpuRegWritr(uint16_t address, uint8_t data)
{
    if (address >= 0x8000 && address <= 0xFFFF)
    {
        reg = data & 0xF;
        return true;
    }
    return false;
}
