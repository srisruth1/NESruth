cmake_minimum_required(VERSION 3.30)
project(NESruth)

set(CMAKE_CXX_STANDARD 20)
add_executable(NESruth main.cpp
        Bus.cpp
        Bus.h
        cpu6502.cpp
        cpu6502.h
        ppu2C02.cpp
        ppu2C02.h
        Cartridge.cpp
        Cartridge.h
        Mapper.cpp
        Mapper.h
        Mapper0.cpp
        Mapper0.h
        Controller.cpp
        Controller.h
        Mapper2.cpp
        Mapper2.h)
target_link_libraries(NESruth opengl32 dwmapi gdiplus shlwapi)

