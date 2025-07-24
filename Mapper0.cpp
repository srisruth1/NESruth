/**
 * @file Mapper0.cpp
 * @author Srisruth
 */
 
#include "Mapper0.h"


Mapper0::Mapper0(uint8_t prgBanks, uint8_t chrBanks):Mapper(prgBanks,chrBanks)
{
}

Mapper0::~Mapper0()
{
}

bool Mapper0::cpuMapRead(uint16_t address, uint32_t& Mappedaddress)
{
    if (address >= 0x8000 && address <= 0xFFFF)
    {
        Mappedaddress = address & (prgBank > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper0::cpuMapWrite(uint16_t address, uint32_t& Mappedaddress)
{
     if (address >= 0x8000 && address <= 0xFFFF)
     {
         Mappedaddress = address & (prgBank > 1 ? 0x7FFF : 0x3FFF);
         return true;
     }
    return false;
}

bool Mapper0::ppuMapRead(uint16_t address, uint32_t& Mappedaddress)
{
    if (address >= 0x0000 && address <= 0x1FFF)
    {
        Mappedaddress = address;

        return true;
    }
    return false;
}

bool Mapper0::ppuMapWrite(uint16_t address, uint32_t& Mappedaddress)
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

bool Mapper0::cpuRegWritr(uint16_t address, uint8_t data)
{
    return false;
}
