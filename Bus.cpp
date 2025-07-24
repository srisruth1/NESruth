/**
 * @file Bus.cpp
 * @author Srisruth
 */
 
#include "Bus.h"
Bus::Bus()
{
    // Connect CPU to communication bus
    cpu.connectBus(this);

    // Clear RAM contents
    for (auto &i : ram) i = 0x00;
}

Bus::~Bus()
{
}

void Bus::cpuwrite(uint16_t address, uint8_t data)
{
    if (cartridge->cpuWrite(address, data))
    {

    }
    else if (address >= 0x0 && address <= 0x1FFF)
        ram[address & 0x07FF] = data;
    else if (address >= 0x2000 && address <= 0x3FFF)
        ppu.cpuWrite(address & 0x0007, data);
    else if (address == 0x4016)
    {
        if (controller1) controller1->cpuWrite(data&0x0001);
        if (controller2) controller2->cpuWrite(data&0x0001);

    }
    else if (address == 0x4014)
    {
        ActDMA = true;
        DMA_addr = 0x00;
        DMA_page = data;
    }
}

uint8_t Bus::cpuread(uint16_t address, bool Readonly)
{
    uint8_t data = 0x00;
    if (cartridge->cpuRead(address,data))
    {

    }
    else if (address >= 0x0 && address <= 0x01FFF)
        data = ram[address & 0x07FF];
    else if (address >= 0x2000 && address <= 0x3FFF)
        data = ppu.cpuRead(address & 0x0007, Readonly);
    else if (address == 0x4016)
    {
        if (controller1)
        {
            data = controller1->cpuRead();
        }

    }else if (address == 0x4017)
    {
        if (controller2)
        {
            data = controller2->cpuRead();
        }
    }

    return data;
}

void Bus::InsertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
    this->cartridge = cartridge;
    ppu.Insertcartridge(cartridge);
}

void Bus::reset()
{
    cpu.Reset();
    systemclock = 0;
}

void Bus::clock()
{
    ppu.clock();
    if (systemclock % 3 == 0)
    {
        if (ActDMA)
        {
            if ((DummYDMA)&& (systemclock % 2))
            {
                DummYDMA = false;
            }
            else if (systemclock % 2  == 0)
            {
                DMA_data = cpuread(DMA_page << 8 | DMA_addr);
            }else
            {
                ppu.DRMS[DMA_addr] = DMA_data;
                DMA_addr++;
                if (DMA_addr == 0x00)
                {
                    ActDMA = false;
                    DummYDMA = true;
                }
            }


        }
    }else
    {
        cpu.Clock();
    }
     if (ppu.nmi)
    {
        ppu.nmi = false;
        cpu.nmi();
   }
   systemclock++;
}


