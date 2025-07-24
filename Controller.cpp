/**
 * @file Controller1.cpp
 * @author Srisruth
 */
 
#include "Controller.h"


uint8_t Controller::cpuRead()
{
    auto tr = Shift.A;
    if (strobe == false)
    {
        Shift.reg >>= 1;
    }


    return tr;

}

void Controller::cpuWrite(bool data)
{
    strobe = data;
}