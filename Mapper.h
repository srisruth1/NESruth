/**
 * @file Mapper.h
 * @author Srisruth
 *
 *
 */
 
#ifndef MAPPER_H
#define MAPPER_H
#include <cstdint>


class Mapper {
private:

protected:
    uint8_t prgBank = 0;
    uint8_t chrBank = 0;
public:
    Mapper(uint8_t prgBanks, uint8_t chrBanks);
    virtual ~Mapper();

    virtual bool cpuMapRead(uint16_t address, uint32_t& Mappedaddress) = 0;
    virtual bool cpuMapWrite(uint16_t address, uint32_t& Mappedaddress) = 0;
    virtual bool ppuMapRead(uint16_t address, uint32_t& Mappedaddress) = 0;
    virtual bool ppuMapWrite(uint16_t address, uint32_t& Mappedaddress) = 0;
    virtual bool cpuRegWritr(uint16_t address, uint8_t data) = 0;



};



#endif //MAPPER_H
