/**
 * @file Mapper0.h
 * @author Srisruth
 *
 *
 */
 
#ifndef MAPPER0_H
#define MAPPER0_H
#include "Mapper.h"


class Mapper0: public Mapper{
private:

public:
    Mapper0(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper0();
    bool cpuMapRead(uint16_t address, uint32_t& Mappedaddress) override;
    bool cpuMapWrite(uint16_t address, uint32_t& Mappedaddress) override;
    bool ppuMapRead(uint16_t address, uint32_t& Mappedaddress) override;
    bool ppuMapWrite(uint16_t address, uint32_t& Mappedaddress) override;
    bool cpuRegWritr(uint16_t address, uint8_t data) override;

};



#endif //MAPPER0_H
