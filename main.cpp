#define OLC_PGE_APPLICATION
#include "Bus.h"
#include "olcPixelGameEngine.h"

class TextDemo : public olc::PixelGameEngine {
private:
    std::shared_ptr<Cartridge> cartridge;
    Bus NES;
    Controller controller1;
    Controller controller2;
    bool run = false;
    uint8_t nSelectedPalette = 0x00;

    float curr = 0;
    std::map<uint16_t, std::string> mapAsm;
public:
    static std::string hex(uint32_t n, uint8_t d) {
        std::string s(d, '0');
        for (int i = d - 1; i >= 0; i--, n >>= 4)
            s[i] = "0123456789ABCDEF"[n & 0xF];
        return s;
    }

    TextDemo() {
        sAppName = "NESruth";
    }
    void displayram(uint16_t start)
    {
        for (int j = 128; j < 128*2; j+= 16)
        {
            int h = 0;
            for (int i = j; i < j + 16; i++)
            {
                DrawString(h*20, (j/16)*20, hex(NES.ram[start + i], 2) , olc::WHITE, 1);
                h++;
            }
        }

    }
    void UpdateInput()
    {
        if (GetKey(olc::UP).bHeld)
        {
            controller1.Shift.UP = 1;
            controller2.Shift.UP = 1;
        }
        if (GetKey(olc::DOWN).bHeld)
        {
            controller1.Shift.DOWN = 1;
            controller2.Shift.DOWN = 1;
        }
        if (GetKey(olc::LEFT).bHeld)
        {
            controller1.Shift.LEFT = 1;
            controller2.Shift.LEFT = 1;
        }
        if (GetKey(olc::RIGHT).bHeld)
        {
            controller1.Shift.RIGHT = 1;
            controller2.Shift.RIGHT = 1;
        }
        if (GetKey(olc::A).bHeld)
        {
            controller1.Shift.A = 1;
            controller2.Shift.A = 1;
        }
        if (GetKey(olc::S).bHeld)
        {
            controller1.Shift.B = 1;
            controller2.Shift.B = 1;
        }
        if (GetKey(olc::ENTER).bHeld)
        {
            controller1.Shift.Start = 1;
            controller2.Shift.Start = 1;
        }
        if (GetKey(olc::SHIFT).bHeld)
        {
            controller1.Shift.Select = 1;
            controller2.Shift.Select = 1;
        }
    }
    void DrawCode(int x, int y, int nLines)
    {
        auto it_a = mapAsm.find(NES.cpu.PC);
        int nLineY = (nLines >> 1) * 10 + y;
        if (it_a != mapAsm.end())
        {
            DrawString(x, nLineY, (*it_a).second, olc::CYAN);
            while (nLineY < (nLines * 10) + y)
            {
                nLineY += 10;
                if (++it_a != mapAsm.end())
                {
                    DrawString(x, nLineY, (*it_a).second);
                }
            }
        }

    }
    void displayregisters()
    {
        DrawString(0,400, "A: " , olc::WHITE, 2);
        DrawString(40,400, hex(NES.cpu.mAR,2) , olc::WHITE, 2);
        DrawString(0,420, "X: " , olc::WHITE, 2);
        DrawString(40,420, hex(NES.cpu.mXR,2) , olc::WHITE, 2);
        DrawString(0,440, "Y: " , olc::WHITE, 2);
        DrawString(40,440, hex(NES.cpu.mYR,2) , olc::WHITE, 2);

    }

    bool OnUserCreate() override {

        auto cart = std::make_shared<Cartridge>("ex.nes");

        NES.InsertCartridge(cart);
        NES.InsertController1(&controller1);
        NES.InsertController2(&controller2);
        controller1.Shift.reg = 0;
        mapAsm = NES.cpu.disassemble(0x0000,0xFFFF);
        run = false;

        NES.reset();
     //   NES.cpu.PC = 0xC000;



        return true;

    }
    void log()
    {
       std::cout << mapAsm[NES.cpu.PC] << std::endl;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        Clear(olc::BLACK);
        if (!run)
        {
            DrawString(0, 0, "NESruth", olc::WHITE, 2);
            DrawString (0,20,"Press C to start", olc::WHITE, 2);
        }
        if (GetKey(olc::Key::C).bPressed)
                run = !run;
        if (GetKey(olc::Key::P).bPressed) (++nSelectedPalette) &= 0x07;
        if (run)
        {
            if (curr > 0.0f)
                curr -= fElapsedTime;
            else
            {
                curr += (1.0f / 60.0f) - fElapsedTime;
                do
                {
                    NES.clock();
                    if (controller1.strobe)
                    {
                        UpdateInput();
                    }


                } while (!NES.ppu.frame_complete);
                //DrawSprite(516,348,&NES.ppu.GetNameT(0));
                NES.ppu.frame_complete = false;
            }
        }
        else
        {
            if (GetKey(olc::Key::F).bPressed)
            {
                // Clock enough times to draw a single frame
                do
                {
                    NES.clock();

                } while (!NES.ppu.frame_complete);
                // Use residual clock cycles to complete current instruction
                do
                {
                    NES.clock();

                } while (!NES.cpu.complete());
                // Reset frame completion flag
                NES.ppu.frame_complete = false;
            }
        }
        if (GetKey(olc::Key::SPACE).bPressed)
        {
            NES.clock();

        }
        // Draw Palettes & Pattern Tables ==============================================
        const int nSwatchSize = 6;
        for (int p = 0; p < 8; p++) // For each palette
            for(int s = 0; s < 4; s++) // For each index
                FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340,
                    nSwatchSize, nSwatchSize, NES.ppu.GetColor(p, s));

        // Draw selection reticule around selected palette
       // DrawRect(516 + nSelectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);
       DrawSprite(0,0, &NES.ppu.GetScreen(), 2);
        DrawSprite(516,348,&NES.ppu.GetNameT(0));
        //DrawSprite(648,348,&NES.ppu.GetPattern(1,nSelectedPalette));

        DrawCode(516, 72, 20);




        return true;

    }
};

int main() {
    TextDemo demo;
    if (demo.Construct(800, 600, 2, 2))
        demo.Start();
    return 0;
}
