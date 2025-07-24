/**
 * @file cpu6502.cpp
 * @author Srisruth
 */
 
#include "cpu6502.h"

#include <map>

#include "Bus.h"
// Beginning of the Stack is hard coded in 6502 to be 0x0100
uint16_t const STACK = 0x0100;
cpu6502::cpu6502()
{
	//**Taken from Javid9x on youtube writing the table is kinda simple but very tedious as 6502 has 256 op codes**
	// Assembles the translation table. It's big, it's ugly, but it yields a convenient way
	// to emulate the 6502. I'm certain there are some "code-golf" strategies to reduce this
	// but I've deliberately kept it verbose for study and alteration

	// It is 16x16 entries. This gives 256 instructions. It is arranged to that the bottom
	// 4 bits of the instruction choose the column, and the top 4 bits choose the row.

	// For convenience to get function pointers to members of this class

	// The table is one big initialiser list of initialiser lists...
	using a = cpu6502; //using directive for readable code
	lookup =
	lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

cpu6502::~cpu6502()
{

}
// Reading from the main bus
uint8_t cpu6502::read(uint16_t addr)
{
    return mBus->cpuread(addr,false);
}
// writing to the bus
void cpu6502::write(uint16_t addr, uint8_t data)
{

    mBus->cpuwrite(addr,data);
}
//Getting flag from the status register
uint8_t cpu6502::GetFlag(Flags f)
{
	if (status & f)
	{
		return 1;
	}
	return 0;
}
// setting a flag of the status register used by a lot of functions
void cpu6502::SetFlag(Flags f, bool v)
{
	if (v) status |= f;
	else status &= ~f;
}
// runs the CPU and adds additional "cycles" to emulate clock cycles based on the operation and addressing mode
void cpu6502::Clock()
{
	if (cycle == 0)
	{
		opcode = read(PC);
		PC++;
		cycle = lookup[opcode].cycles;
		uint8_t addclock1 = (this->*lookup[opcode].addrmode)();
		uint8_t addclock2 = (this->*lookup[opcode].operation)();
		cycle += addclock1 & addclock2;
	}
	cycle--;
}
// Addressing_mode
//implied addressing mode usually means the operation us done on the accumulator
uint8_t cpu6502::IMP()
{
	fetched = mAR;
	return 0;
}
// immediate addressing the address in the next program byte
uint8_t cpu6502::IMM(){

	addressabs = PC++;
	return 0;
}
// zero page addressing only consider the offset
uint8_t cpu6502::ZP0(){

	addressabs = read(PC);
	PC++;
	addressabs &= 0x00FF; //we only care about the offset
	return 0;
}
// zero page addressing with the contents of X register added to the offset
uint8_t cpu6502::ZPX()
{
	addressabs = read(PC) + mXR;
	PC++;
	addressabs &= 0x00FF;
	return 0;
}
//zero page addressing with the contents of Y register added to the offset
uint8_t cpu6502::ZPY()
{
	addressabs = (read(PC) + mYR);
	PC++;
	addressabs &= 0x00FF;
	return 0;
}
//absolute addressing : the 16bit address is in the next two bytes of the program code
//6502 architecture is little Endian, the smaller address is stored first
uint8_t cpu6502::ABS()
{
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;
	addressabs = (hi <<	8) | lo;
	return 0;
}
//absolute addressing : the 16bit address from the program code + the contents of the X register
//additional clock cycle is needed if the addition of X changes the page
uint8_t cpu6502::ABX()
{
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;
	addressabs = (hi <<	8) | lo;
	addressabs += mXR;
	if ((addressabs & 0xFF00) != (hi << 8)) // checking if the high byte is changed for possible page change
		return 1; // indicate an additional clock cycle
	return 0;
}
//absolute addressing : the 16bit address from the program code + the contents of the Y register
//additional clock cycle is needed if the addition of X changes the page
uint8_t cpu6502::ABY()
{
	uint16_t lo = read(PC);
	PC++;
	uint16_t hi = read(PC);
	PC++;
	addressabs = (hi <<	8) | lo;
	addressabs += mYR;
	if ((addressabs & 0xFF00) != (hi << 8))
		return 1;
	return 0;
}
//indirect addressing : used when reading data from a pointer
//if the start of the pointer is at the end of a page 6502 wraps around it instead of changing the page
uint8_t cpu6502::IND()
{
	uint16_t ptr_lo = read(PC);
	PC++;
	uint16_t ptr_hi = read(PC);
	PC++;
	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == (0x00FF))
	{
		addressabs = (read(0xFF00&ptr) << 8) | read(ptr);  //wrap around
	}else
	{
		addressabs = (read(ptr + 1) << 8) | read(ptr); //reading the pointer gives us 8 byte data
															// bit wise operations are used to get the full 16 bit address
	}
	return 0;
}
//indirect addressing with contents of X register: Kinda different from how the other X and Y variants worked
// contents of the X register is used to offset a location in the 0th page of the memory
//Ret@rded asf
uint8_t cpu6502::IZX()
{
	uint16_t t = read(PC);
	PC++;

	uint16_t ptr_lo = read((uint16_t)(t + (uint16_t)mXR) & 0x00FF);
	uint16_t ptr_hi = read((uint16_t)(t + (uint16_t)mXR + 1) & 0x00FF);

	addressabs = (ptr_hi <<	8) | ptr_lo;
	return 0;

}
//indirect addressing with Y : acts similar to the other variants we get the pointer from the machine code
//The pointer is then offset by the contents of Y register to get the actual address
//similar to abs addressing x and y variants additional clock cycle is required for a page change
uint8_t cpu6502::IZY()
{
	uint16_t t = read(PC);
	PC++;

	uint16_t ptr_lo = read(t & 0x00FF);
	uint16_t ptr_hi = read((t + 1)&0x00FF) ;

	addressabs = (ptr_hi << 8 )| ptr_lo;
	addressabs += mYR;

	if ((addressabs & 0xFF00) != (ptr_hi << 8))
		return 1;
	return 0;
}
// Address Mode: Relative
// This address mode is exclusive to branch instructions. The address
// must reside within -128 to +127 of the branch instruction, i.e.
// you cant directly branch to any address in the addressable range.
uint8_t cpu6502::REL()
{
	addressrel = read(PC);
	PC++;
	if (addressrel & 0x80)
		addressrel |= 0xFF00;
	return 0;

}


//instructions

//fetches the data depending on the  addressing mode
//unless its implied which means the fetched variable is already set
uint8_t cpu6502::fetch()
{
	if (lookup[opcode].addrmode != &cpu6502::IMP)
		fetched = read(addressabs);
	return fetched;
}
// Instruction: Bitwise Logic AND
// Function:    A = A & M
// Flags Out:   N, Z
uint8_t cpu6502::AND()
{
	fetch();
	mAR &= fetched;
	SetFlag(Z, mAR == 0);
	SetFlag(N, mAR & 0x80);
	return 1;
}

// Instruction: Branch if Carry Set
// Function:    if(C == 1) pc = address
uint8_t cpu6502::BCS()
{
	if (GetFlag(C) == 1)
	{
		cycle++;
		addressabs = PC + addressrel;
		if ((addressabs & 0xFF00) != (PC & 0xFF00))
			cycle++;
		PC = addressabs;
	}
	return 0;
}
// Instruction: Branch if Carry Clear
// Function:    if(C == 0) pc = address
uint8_t cpu6502::BCC()
{
	if (GetFlag(C) == 0)
	{
		cycle++;
		addressabs = PC + addressrel;
		if ((addressabs & 0xFF00) != (PC & 0xFF00))
			cycle++;
		PC = addressabs;
	}
	return 0;
}

uint8_t cpu6502::BEQ()
{
	if (GetFlag(Z))
	{
		cycle++;
		addressabs = PC + addressrel;
		if ((addressabs & 0xFF00) != (PC & 0xFF00))
			cycle++;
		PC = addressabs;

	}
	return 0;
}

uint8_t cpu6502::BNE()
{
	if (GetFlag(Z) ==	0)
	{
		cycle++;
		addressabs = PC + addressrel;
		if ((addressabs & 0xFF00) != (PC & 0xFF00))
			cycle++;
		PC = addressabs;
	}
	return 0;
}

uint8_t cpu6502::BMI()
{
	if (GetFlag(N))
	{
		cycle++;
		addressabs = PC + addressrel;
		if ((addressabs & 0xFF00) != (PC & 0xFF00))
			cycle++;
		PC = addressabs;
	}
	return 0;
}
uint8_t cpu6502::BVS()
{
	if (GetFlag(V))
	{
		cycle++;
		addressabs = PC + addressrel;
		if ((addressabs & 0xFF00) != (PC & 0xFF00))
			cycle++;
		PC = addressabs;
	}
	return 0;
}
uint8_t cpu6502::BVC()
{
	if (!GetFlag(V))
	{
		cycle++;
		addressabs = PC + addressrel;
		if ((addressabs & 0xFF00) != (PC & 0xFF00))
			cycle++;
		PC = addressabs;
	}
	return 0;
}
uint8_t cpu6502::BPL()
{
	if (!GetFlag(N))
	{
		cycle++;
		addressabs = PC + addressrel;
		if ((addressabs & 0xFF00) != (PC & 0xFF00))
			cycle++;
		PC = addressabs;
	}
	return 0;
}

uint8_t cpu6502::CLC()
{
	SetFlag(C, false);
	return 0;
}
uint8_t cpu6502::CLD()
{
	SetFlag(D, false);
	return 0;
}

uint8_t cpu6502::CLI()
{
	SetFlag(I, false);
	return 0;
}
uint8_t cpu6502::CLV()
{
	SetFlag(V, false);
	return 0;
}


uint8_t cpu6502::ADC()
{
	fetch();

	uint16_t temp = (uint16_t)mAR + (uint16_t)fetched + (uint16_t)GetFlag(C);

	SetFlag(C, temp > 0x00FF);
	SetFlag(N, temp & 0x0080);
	SetFlag(Z, (temp & 0x00FF)==0);
	SetFlag(V, (~((uint16_t)mAR ^ (uint16_t)fetched) & ((uint16_t)mAR ^ (uint16_t)temp)) & 0x0080);
	mAR = temp & 0x00FF;
	return 1;
}

uint8_t cpu6502::SBC()
{
	fetch();
	uint16_t result = (uint16_t)fetched ^ 0x00FF;
	uint16_t temp = (uint16_t)mAR + result + (uint16_t)GetFlag(C);
	SetFlag(C, temp &0xFF00)	;
	SetFlag(N, temp & 0x0080);
	SetFlag(Z, (temp & 0x00FF)==0);
	SetFlag(V, (temp ^ (uint16_t)mAR) & (temp ^ result) & 0x0080);
	mAR = temp & 0x00FF;
	return 1;

}

uint8_t cpu6502::ASL()
{
	fetch();
	SetFlag(C, fetched & 0x80);
	fetched = fetched << 1;
	SetFlag(N, fetched & 0x80);
	SetFlag(Z, !fetched);
	if (lookup[opcode].addrmode == &cpu6502::IMP)
		mAR = fetched;
	else
		write(addressabs, fetched);
	return 0;
}
uint8_t cpu6502::BIT()
{
	fetch();
	uint8_t temp = mAR  & fetched;
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, fetched & (1 << 7));
	SetFlag(V, fetched & (1 << 6));
	return 0;
}
uint8_t cpu6502::PHA()
{
	write(STACK + SP, mAR);
	SP--;
	return 0;
}

uint8_t cpu6502::PLA()
{
	SP++;
	mAR = read(STACK + SP);
	SetFlag(Z, mAR == 0);
	SetFlag(N, mAR & 0x80);
	return 0;
}

uint8_t cpu6502::PHP()
{
	write(STACK + SP, status | B | U );
	SetFlag(B, 0);
	SetFlag(U, 0);
	SP--;
	return 0;
}
uint8_t cpu6502::PLP()
{
	SP++;
	status = read(STACK + SP);
	SetFlag(U, 1);
	return 0;
}

uint8_t cpu6502::RTI()
{
	SP++;
	status = read(STACK + SP);
	status &= ~B;
	status &= ~U;
	SP++;
	uint16_t lo = read(STACK + SP);
	SP++;
	uint16_t hi = read(STACK + SP);
	PC = hi << 8 | lo;
	return 0;
}

uint8_t cpu6502::RTS()
{
	SP++;
	PC = (uint16_t)read(STACK + SP);
	SP++;
	PC |= (uint16_t)read(STACK + SP) << 8;;
	PC++;
	return 0;
}


uint8_t cpu6502::BRK()
{
	PC++;
	SetFlag(I, 1);
	write(STACK + SP, (PC >> 8) & 0x00FF);
	SP--;
	write(STACK + SP, PC & 0x00FF);
	SP--;
	SetFlag(B, 1);
	write(STACK + SP, status);
	SP--;
	SetFlag(B, 0);
	PC = (uint16_t)read(0xFFFE) | ((uint16_t)read(0xFFFF) << 8);
	return 0;
}

uint8_t cpu6502::CMP()
{
	fetch();
	uint16_t temp = (uint16_t)mAR - (uint16_t)fetched;
	SetFlag(C, mAR >= fetched);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	return 1;
}

uint8_t cpu6502::CPX()
{
	fetch();
	uint16_t temp = (uint16_t)mXR - (uint16_t)fetched;
	SetFlag(C, mXR >= fetched);
	SetFlag(N, temp & 0x0080);
	SetFlag(Z, !(temp & 0x00FF));
		return 0;

}

uint8_t cpu6502::CPY()
{
	fetch();
	uint16_t temp = (uint16_t)mYR - (uint16_t)fetched;
	SetFlag(C, mYR >= fetched);
	SetFlag(N, temp & 0x0080);
	SetFlag(Z, (temp & 0x00FF) == 0);
	return 0;

}
uint8_t cpu6502::DEC()
{
	fetch();
	uint8_t temp = fetched - 1;
	write(addressabs, temp );
	SetFlag(N, temp & 0x80);
	SetFlag(Z, temp == 0);
	return 0;
}

uint8_t cpu6502::DEX()
{
	mXR--;
	SetFlag(N, mXR & 0x80);
	SetFlag(Z, mXR == 0);
	return 0;
}

uint8_t cpu6502::DEY()
{
	mYR--;
	SetFlag(N, mYR & 0x80);
	SetFlag(Z, mYR == 0);
	return 0;
}
uint8_t cpu6502::EOR()
{
	fetch();
	mAR = mAR ^ fetched;
	SetFlag(N, mAR & 0x80);
	SetFlag(Z, mAR == 0);
	return 1;
}

uint8_t cpu6502::INC()
{
	fetch();
	uint16_t temp = fetched + 1;
	write(addressabs, temp );
	SetFlag(N, temp & 0x80);
	SetFlag(Z, (temp & 0x00FF) == 0);
	return 0;
}

uint8_t cpu6502::INX()
{
	mXR++;
	SetFlag(N, mXR & 0x80);
	SetFlag(Z, mXR == 0);
	return 0;
}

uint8_t cpu6502::INY()
{
	mYR++;
	SetFlag(N, mYR & 0x80);
	SetFlag(Z, mYR == 0);
	return 0;
}

uint8_t cpu6502::JMP()
{
	PC = addressabs;
	return 0;
}

uint8_t cpu6502::JSR()
{
	PC--;
	write(STACK + SP, (PC >> 8) & 0x00FF);
	SP--;
	write(STACK + SP, PC & 0x00FF);
	SP--;

	PC = addressabs;
	return 0;
}

uint8_t cpu6502::LDA()
{
	fetch();
	mAR = fetched;
	SetFlag(N, mAR & 0x80);
	SetFlag(Z, (mAR) == 0);
	return 1;
}
uint8_t cpu6502::LDX()
{
	fetch();
	mXR = fetched;
	SetFlag(N, mXR & 0x80);
	SetFlag(Z, !(mXR));
	return 1;

}
uint8_t cpu6502::LDY()
{
	fetch();
	mYR = fetched;
	SetFlag(N, mYR & 0x80);
	SetFlag(Z, (mYR) == 0);
	return 1;

}

uint8_t cpu6502::LSR()
{
	fetch();
	SetFlag(C, fetched & 0x0001);
	fetched = fetched >> 1;
	SetFlag(Z, (fetched & 0x00FF) == 0x0000);
	SetFlag(N, fetched & 0x0080);
	if (lookup[opcode].addrmode == &cpu6502::IMP)
		mAR = fetched & 0x00FF;
	else
		write(addressabs, fetched & 0x00FF);
	return 0;
}


uint8_t cpu6502::NOP()
{
	// Not all NOPs are equal, Ive added a few here
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
	switch (opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}
uint8_t cpu6502::ORA()
{
	fetch();
	mAR |= fetched;
	SetFlag(N, mAR & 0x80);
	SetFlag(Z, (mAR) == 0		);
	return 1;
}
uint8_t cpu6502::ROL()
{
	fetch();
	int16_t temp = (uint16_t)(fetched << 1) | GetFlag(C);
	SetFlag(C, temp & 0xFF00);
	SetFlag(Z, (temp & 0x00FF) == 0x0000);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &cpu6502::IMP)
		mAR = temp & 0x00FF;
	else
		write(addressabs, temp & 0x00FF);
	return 0;
}


uint8_t cpu6502::ROR()
{
	fetch();
	uint16_t temp = (uint16_t)(GetFlag(C) << 7) | (fetched >> 1);
	SetFlag(C, fetched & 0x01);
	SetFlag(Z, (temp & 0x00FF) == 0x00);
	SetFlag(N, temp & 0x0080);
	if (lookup[opcode].addrmode == &cpu6502::IMP)
		mAR = temp & 0x00FF;
	else
		write(addressabs, temp & 0x00FF);
	return 0;
}


uint8_t cpu6502::SEC()
{
	SetFlag(C,1);
	return 0;
}


uint8_t cpu6502::SED()
{
	SetFlag(D,1);
	return 0;
}


uint8_t cpu6502::SEI()
{
	SetFlag(I,1);
	return 0;
}

uint8_t cpu6502::STA()
{
	if ((addressabs != 511) &(addressabs != 510))
	write(addressabs, mAR);
	return 0;
}


uint8_t cpu6502::STX()
{
	write(addressabs, mXR);
	return 0;
}


uint8_t cpu6502::STY()
{
	write(addressabs, mYR);
	return 0;
}

uint8_t cpu6502::TAX()
{
	mXR = mAR;
	SetFlag(N, mXR & 0x80);
	SetFlag(Z, (mXR) == 0);
	return 0;
}


uint8_t cpu6502::TAY()
{
	mYR = mAR;
	SetFlag(N, mYR & 0x80);
	SetFlag(Z, (mYR) == 0);
	return 0;
}


uint8_t cpu6502::TSX()
{
	mXR = SP;
	SetFlag(N, mXR & 0x80);
	SetFlag(Z, !(mXR));
	return 0;
}


uint8_t cpu6502::TXA()
{
	mAR = mXR;
	SetFlag(N, mAR & 0x80);
	SetFlag(Z, (mAR) == 0);
	return 0;
}


uint8_t cpu6502::TXS()
{
	SP = mXR;
	return 0;
}

uint8_t cpu6502::TYA()
{
	mAR = mYR;
	SetFlag(N, mAR & 0x80);
	SetFlag(Z, !(mAR));
	return 0;
}

uint8_t cpu6502::XXX()
{
	return 0;
}





// Interrupt sequences
void cpu6502::Reset()
{
	//Getting PC from a fixed location set by the program
	addressabs = 0xFFFC;
	uint16_t lo = read(addressabs);
	uint16_t hi = read(addressabs + 1);

	PC = (hi << 8) | lo;

	//reseting everything to zero
	mAR = 0;
	mXR = 0;
	mYR = 0;
	SP = 0xFD;
	status = 0x00 | U;


	addressabs = 0;
	addressrel = 0;
	fetched = 0;

	cycle = 8;

}

void cpu6502::irq()
{
	// checking the interupt flag
	if (GetFlag(I) == 0)
	{
		//pushing the current Program counter into stack
		write(STACK + SP, ((PC >> 8) &(0x00FF)));
		SP--;
		write(STACK + SP, PC & 0x00FF);
		SP--;
		//pushing status register into the stack
		SetFlag(B, false);
		SetFlag(U, true);
		SetFlag(I, true);
		write(STACK + SP, status);
		SP--;

		//NES Interrupt sequences was always at the same location set by the program
		addressabs = 0xFFFE;
		uint16_t lo = read(addressabs);
		uint16_t hi = read(addressabs + 1);
		PC = (hi << 8) | lo;

		//IRQs take 7 cycles
		cycle = 7;

	}
}

void cpu6502::nmi()
{
	write(STACK + SP, ((PC >> 8) &(0x00FF)));
	SP--;
	write(STACK + SP, PC & 0x00FF);
	SP--;
	SetFlag(B, false);
	SetFlag(U, true);
	SetFlag(I, true);
	write(STACK + SP, status);
	SP--;

	addressabs = 0xFFFA;
	uint16_t lo = read(addressabs);
	uint16_t hi = read(addressabs + 1);
	PC = (hi << 8) | lo;
	cycle = 8;

}


//DISASSEMBLY FUNCTION TAKEN FROM OLC FOR CONVENIENT DEBUGGING

// This is the disassembly function. Its workings are not required for emulation.
// It is merely a convenience function to turn the binary instruction code into
// human readable form. Its included as part of the emulator because it can take
// advantage of many of the CPUs internal operations to do this.
std::map<uint16_t, std::string> cpu6502::disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into
	// hex strings because "modern C++"'s method with
	// streams is atrocious
	auto hex = [](uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	// Starting at the specified address we read an instruction
	// byte, which in turn yields information from the lookup table
	// as to how many additional bytes we need to read and what the
	// addressing mode is. I need this info to assemble human readable
	// syntax, which is different depending upon the addressing mode

	// As the instruction is decoded, a std::string is assembled
	// with the readable output
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		// Prefix line with instruction address
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t opcode = mBus->cpuread(addr, true); addr++;
		sInst += lookup[opcode].name + " ";

		// Get oprands from desired locations, and form the
		// instruction based upon its addressing mode. These
		// routines mimmick the actual fetch routine of the
		// 6502 in order to get accurate data as part of the
		// instruction
		if (lookup[opcode].addrmode != &cpu6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::IMM)
		{
			value = mBus->cpuread(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ZP0)
		{
			lo = mBus->cpuread(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ZPX)
		{
			lo = mBus->cpuread(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ZPY)
		{
			lo = mBus->cpuread(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::IZX)
		{
			lo = mBus->cpuread(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::IZY)
		{
			lo = mBus->cpuread(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ABS)
		{
			lo = mBus->cpuread(addr, true); addr++;
			hi = mBus->cpuread(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ABX)
		{
			lo = mBus->cpuread(addr, true); addr++;
			hi = mBus->cpuread(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::ABY)
		{
			lo = mBus->cpuread(addr, true); addr++;
			hi = mBus->cpuread(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::IND)
		{
			lo = mBus->cpuread(addr, true); addr++;
			hi = mBus->cpuread(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (lookup[opcode].addrmode == &cpu6502::REL)
		{
			value = mBus->cpuread(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instruction's
		// address as the key. This makes it convenient to look for later
		// as the instructions are variable in length, so a straight up
		// incremental index is not sufficient.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}