/**
 * @file Bus.h
 * @author Srisruth
 *
 *
 */
 
#ifndef BUS_H
#define BUS_H
#include <array>
#include <cstdint>
#include <memory>

#include "Cartridge.h"
#include "Controller.h"
#include "cpu6502.h"
#include "ppu2C02.h"
class Controller;
class Bus {
private:
    uint32_t systemclock = 0;


public:
    Bus();
    ~Bus();
    cpu6502 cpu;

    ppu2C02 ppu;
    Controller* controller1 = nullptr;
    Controller* controller2 = nullptr;
    bool ActDMA = false;
    bool DummYDMA = true;
    uint8_t DMA_page = 0x00;
    uint8_t DMA_addr = 0x00;
    uint8_t DMA_data = 0x00;
    std::array<uint8_t, 2048> ram{};

    std::shared_ptr<Cartridge> cartridge;

    void cpuwrite(uint16_t address, uint8_t data); //< write data

    uint8_t cpuread(uint16_t address, bool Readonly = false); //< read data

    void InsertCartridge(const std::shared_ptr<Cartridge>& cartridge);
    void InsertController1(Controller* controller){this->controller1 = controller;}
    void InsertController2(Controller* controller){this->controller2 = controller;}
    void reset();
    void clock();
};



#endif //BUS_H
