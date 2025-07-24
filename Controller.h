/**
 * @file Controller1.h
 * @author Srisruth
 *
 *
 */
 
#ifndef CONTROLLER1_H
#define CONTROLLER1_H
#include "Bus.h"


class Controller {
private:



public:
    union
    {
        struct
        {
            uint8_t A: 1;
            uint8_t B: 1;
            uint8_t Select: 1;
            uint8_t Start : 1;
            uint8_t UP : 1;
            uint8_t DOWN : 1;
            uint8_t LEFT : 1;
            uint8_t RIGHT : 1;

        };
        uint8_t reg;

    }Shift;
    bool strobe = true;

    uint8_t cpuRead();
    void cpuWrite(bool data);


};



#endif //CONTROLLER1_H
