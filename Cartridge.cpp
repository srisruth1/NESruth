/**
 * @file Cartridge.cpp
 * @author Srisruth
 */
 
#include "Cartridge.h"
#include <fstream>

#include "Mapper2.h"
#include "olcPixelGameEngine.h"

Cartridge::Cartridge(const std::string& filename)
{
    struct sHeader
    {
        char name[4];
        uint8_t prg_chunks;
        uint8_t chr_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t TVinfo;
        uint8_t TVinfo2;
        char unused[5];
    } header;
    std::ifstream ifs;
    ifs.open(filename, std::ifstream::binary);
    if (ifs.is_open())
    {
        ifs.read(reinterpret_cast<char*>(&header), sizeof(sHeader));

        if (header.mapper1 & 0x04)
            ifs.seekg(512, std::ios_base::cur);

        MapperId = ((header.mapper2 >> 4 ) << 4 )| (header.mapper1  >> 4);

        Mirror = (header.mapper1 &0x1) ? VERT: HOR;




        uint8_t fileformat = 1;
        if ((header.mapper2 & 0x0C) == 0x08) fileformat = 2;
        if (fileformat == 0)
        {

        }
        else if (fileformat == 1)
        {
            PRG_bank = header.prg_chunks;
            PRG.resize(PRG_bank*16384);
            ifs.read((char*)(PRG.data()), PRG.size());

            CHR_bank = header.chr_chunks;
            if (CHR_bank == 0)
            {
                // Create CHR RAM
                CHR.resize(8192);
            }else
            {
                // Allocate for ROM
                CHR.resize(CHR_bank * 8192);
            }
            ifs.read((char*)(CHR.data()), CHR.size());
        }
        else if (fileformat == 2)
        {
            PRG_bank = ((header.prg_ram_size & 0x07) << 8) | header.prg_chunks;
            PRG.resize(PRG_bank * 16384);
            ifs.read((char*)PRG.data(), PRG.size());

            CHR_bank = ((header.prg_ram_size & 0x38) << 8) | header.chr_chunks;
            CHR.resize(CHR_bank* 8192);
            ifs.read((char*)CHR.data(), CHR.size());

        }
        switch (MapperId)
        {
        case 0:
            mapper = std::make_shared<Mapper0>(header.prg_chunks, header.chr_chunks);
            break;
        case 0x2:
            mapper = std::make_shared<Mapper2>(header.prg_chunks, header.chr_chunks);
            break;
        }
        ImageValid = true;
        ifs.close();

    }
}

Cartridge::~Cartridge()
{
}

bool Cartridge::cpuRead(uint16_t address, uint8_t& data)
{
    uint32_t mapped = 0;
    if (mapper->cpuMapRead(address, mapped))
    {
        data = PRG[mapped];
        return true;
    }
    return false;
}

bool Cartridge::cpuWrite(uint16_t address, uint8_t data)
{
    uint32_t mapped = 0;
    if (MapperId == 0x02)
    {
        return mapper->cpuRegWritr(address, data);
    }
    if (mapper->cpuMapWrite(address, mapped))
    {
        PRG[mapped] = data;
        return true;
    }
    return false;
}

bool Cartridge::ppuRead(uint16_t address, uint8_t& data)
{
    uint32_t mapped = 0;
    if (mapper->ppuMapRead(address, mapped))
    {
        data = CHR[mapped];
        return true;
    }
    return false;
}

bool Cartridge::ppuWrite(uint16_t address, uint8_t data)
{
    uint32_t mapped = 0;
    if (mapper->ppuMapWrite(address, mapped))
    {
        CHR[mapped] = data;
        return true;
    }
    return false;
}
