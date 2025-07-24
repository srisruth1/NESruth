// Test Cpu 6502 – **stand‑alone** functional + unit test harness
// -----------------------------------------------------------------------------
// This version eliminates the linker errors you just hit by:
//   • **Not inheriting from the full-fat `Bus`** implementation (which drags in
//     `ppu2C02`, `Cartridge`, and the OLC Pixel Game Engine).
//   • Providing a **minimal stub of Bus itself** – just enough to satisfy
//     `cpu6502` at link‑time – plus a tiny `olc::Pixel`, `ppu2C02`, and
//     `Cartridge` shim so we don’t need any other source files.
//
// The stubbed‑out functions simply read/write from a 64 KiB RAM array that
// lives in `TestCPU6502`, so the emulator core exercises real memory while the
// heavy NES subsystems sit out.
//
// Build:
//   g++ -std=c++17 -O2 test_cpu_6502.cpp cpu6502.cpp -o tests
//   ./tests                             # run mini tests
//   ./tests 6502_functional_test.bin    # plus Klaus functional ROM
// -----------------------------------------------------------------------------

#include <array>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <iterator>
#include <iostream>
#include <iomanip>

// ---------------------------------------------------------------------------
// **Tiny shim for olc::Pixel** – avoids pulling in the whole Pixel Game Engine
// ---------------------------------------------------------------------------
namespace olc {
    struct Pixel {
        uint8_t r, g, b, a;
        Pixel(uint8_t R=0, uint8_t G=0, uint8_t B=0, uint8_t A=255)
            : r(R), g(G), b(B), a(A) {}
    };
}

// ---------------------------------------------------------------------------
// **Forward‑declare cpu6502** so we can craft a stub Bus before pulling in the
// real header (which only forward‑declares Bus).  The order matters!
// ---------------------------------------------------------------------------
class cpu6502;

// ---------------------------------------------------------------------------
// **Minimal Bus stub** – just the interface cpu6502 cares about.  No PPU, no
// cartridge.  Implementations are provided further down, _after_ we define
// `TestCPU6502`, so they can down‑cast `this` to get at the 64 KiB RAM.
// ---------------------------------------------------------------------------
class Bus {
public:
    Bus();                // defined later
    ~Bus();               // defined later
    uint8_t cpuread (uint16_t addr, bool readonly=false); // ditto
    void    cpuwrite(uint16_t addr, uint8_t data);        // ditto
};

// Dummy stand‑ins so the real `cpu6502.h` doesn’t choke on missing types.
class ppu2C02 {};
class Cartridge {};

// Bring in the real core now that the stubs are in place.
#include "cpu6502.h"

// ---------------------------------------------------------------------------
// TestCPU6502 – owns 64 KiB RAM + a cpu6502 and wires the stub Bus into the
// core.  No heavy NES components are instantiated.
// ---------------------------------------------------------------------------
class TestCPU6502 : public cpu6502, public Bus {
public:
    std::array<uint8_t, 65536> ram{};

    TestCPU6502() {
        ram.fill(0);
        connectBus(this);          // cpu6502 → Bus* hookup
        Reset();                   // prime PC from vector (which we’ll patch)
    }

    // ---------------- Convenience helpers ---------------------------------
    void loadProgram(const std::vector<uint8_t>& bytes,
                     uint16_t start = 0x8000,
                     bool patchVector = true)
    {
        std::copy(bytes.begin(), bytes.end(), ram.begin() + start);
        if (patchVector) {
            ram[0xFFFC] = static_cast<uint8_t>(start & 0x00FF);
            ram[0xFFFD] = static_cast<uint8_t>(start >> 8);
        }
        PC = start;   // override reset so tests start at first instruction
    }

    void loadBinary(const std::vector<uint8_t>& bytes) {
        std::copy(bytes.begin(), bytes.end(), ram.begin());
        Reset();                      // vector comes from the image itself
    }

    void runUntilPC(uint16_t targetPC, size_t maxCycles = 1'000'000) {
        while (PC != targetPC && maxCycles--) Clock();
        if (PC != targetPC)
            throw std::runtime_error("runUntilPC() timed‑out");
    }
};

// ---------------------------------------------------------------------------
// **Implement the Bus stub now** – they down‑cast to `TestCPU6502` so the core
// really hits the 64 KiB buffer.
// ---------------------------------------------------------------------------
inline Bus::Bus()  {}
inline Bus::~Bus() {}
inline uint8_t Bus::cpuread(uint16_t addr, bool)  {
    return static_cast<TestCPU6502*>(this)->ram[addr];
}
inline void Bus::cpuwrite(uint16_t addr, uint8_t data) {
    static_cast<TestCPU6502*>(this)->ram[addr] = data;
}

// ---------------------------------------------------------------------------
// Mini smoke‑test + Klaus functional harness (unchanged API)
// ---------------------------------------------------------------------------
static bool sampleMiniTests() {
    TestCPU6502 cpu;

    // LDA #$00 → Z=1, N=0
    cpu.loadProgram({ 0xA9, 0x00, 0x00 });   // LDA #$00  / BRK
    cpu.runUntilPC(0x8003);
    if (cpu.GetFlag(cpu6502::Z) == 0 || cpu.GetFlag(cpu6502::N) != 0) {
        std::cerr << "[FAIL] LDA immediate flag behaviour\n";
        return false;
    }

    // INC $10 increments memory
    cpu.loadProgram({ 0xE6, 0x10, 0x00 });   // INC $10   / BRK
    cpu.ram[0x0010] = 0x7F;
    cpu.runUntilPC(0x8003);
    if (cpu.ram[0x0010] != 0x80 || cpu.GetFlag(cpu6502::N) == 0) {
        std::cerr << "[FAIL] INC zeropage\n";
        return false;
    }

    std::cout << "[PASS] mini‑tests\n";
    return true;
}

static bool runKlaus(const char* path) {
    std::ifstream rom(path, std::ios::binary);
    if (!rom) { std::cerr << "Cannot open ROM '" << path << "'\n"; return false; }
    std::vector<uint8_t> buf{ std::istreambuf_iterator<char>(rom), {} };
    if (buf.size() > 65536) { std::cerr << "ROM too large\n"; return false; }

    TestCPU6502 cpu;
    cpu.loadBinary(buf);                // reset vector inside image

    try { cpu.runUntilPC(0x3469, 100'000'000); }
    catch (const std::exception& e) {
        std::cerr << "[FAIL] Klaus ROM – " << e.what() << "\n"; return false; }

    if (cpu.ram[0x0002] == 0) {
        std::cout << "[PASS] Klaus functional test\n"; return true; }
    else {
        std::cerr << "[FAIL] Klaus functional test – error="
                  << static_cast<int>(cpu.ram[0x0002]) << "\n"; return false; }
}

int main(int argc, char** argv) {
    bool ok = sampleMiniTests();
    if (argc == 2) ok &= runKlaus(argv[1]);
    return ok ? 0 : 1;
}
