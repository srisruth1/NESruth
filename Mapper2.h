/**
 * @file Mapper2.h
 * @author Srisruth
 *
 *
 */
 
#ifndef MAPPER2_H
#define MAPPER2_H
#include "Mapper.h"


class Mapper2: public Mapper {
private:
    uint8_t reg;
    uint8_t high;

public:
    Mapper2(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper2();
    bool cpuMapRead(uint16_t address, uint32_t& Mappedaddress) override;
    bool cpuMapWrite(uint16_t address, uint32_t& Mappedaddress) override;
    bool ppuMapRead(uint16_t address, uint32_t& Mappedaddress) override;
    bool ppuMapWrite(uint16_t address, uint32_t& Mappedaddress) override;
    bool cpuRegWritr(uint16_t address, uint8_t data) override;
};



#endif //MAPPER2_H
