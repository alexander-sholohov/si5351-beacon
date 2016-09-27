#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include <stdint.h>


// how to use
//
// Si5351 obj(0);
// obj.initialize();
// obj.setupMultisyncParams(Si5351::OUT_0, 30, 1);
// obj.setupPLLParams(Si5351::PLL_A, 31, 154287, 614418);
// obj.enableOutput(Si5351::OUT_0);
//
//
// online configurator:  http://ra9yer.blogspot.com/p/si5351-configurator.html
//


class Si5351
{
public:
    enum OutPin
    {
        OUT_0 = 0,
        OUT_1 = 1,
        OUT_2 = 2
    };

    enum PLL
    {
        PLL_A = 0,
        PLL_B = 1
    };

    enum PowerLevel
    {
        Power_2ma = 0,
        Power_4ma = 1,
        Power_5ma = 2,
        Power_8ma = 3
    };

    Si5351(int instance);
    void initialize();

    void enableOutput(OutPin outputNumber, bool enable);
    void enableShutDown(OutPin outputNumber, bool shutdown);
    void setupPower(OutPin outputNumber, PowerLevel powerLevel);

    void setupPLLParams(PLL pllNumber, uint16_t a, uint32_t b, uint32_t c);
    void setupMultisyncParams(OutPin multisyncNumber, unsigned multisyncDivider, unsigned r);
    void resetPLL(PLL pllNumber);

private:
};

