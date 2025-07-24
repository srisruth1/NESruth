/**
 * @file Cartridge.h
 * @author Srisruth
 *
 *
 */
 
#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "Mapper0.h"


class Cartridge {
private:
    std::vector<uint8_t> PRG;
    std::vector<uint8_t> CHR;
    uint8_t PRG_bank = 0;
    uint8_t CHR_bank = 0;
    uint8_t MapperId = 0;
    std::shared_ptr<Mapper> mapper;
    bool ImageValid = false;

public:
    Cartridge(const std::string& filename);
    ~Cartridge();

    bool cpuRead(uint16_t address, uint8_t &data);
    bool cpuWrite(uint16_t address, uint8_t data);

    bool ppuRead(uint16_t address, uint8_t &data);
    bool ppuWrite(uint16_t address, uint8_t data);

    bool Imagevalid() { return ImageValid; }

    enum
    {
        HOR,
        VERT,
        ONSCREENLO,
        ONSCREENHI,
    }Mirror = HOR;


};



#endif //CARTRIDGE_H
