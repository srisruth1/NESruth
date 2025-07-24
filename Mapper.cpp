/**
 * @file Mapper.cpp
 * @author Srisruth
 */
 
#include "Mapper.h"

Mapper::Mapper(uint8_t prgBanks, uint8_t chrBanks)
{
    prgBank = prgBanks;
    chrBank = chrBanks;
}

Mapper::~Mapper()
{

}
