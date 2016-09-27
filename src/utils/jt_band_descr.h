#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include <stdint.h>

// example initial vector {13, 32, 114285, 1028571, 6, 1, 120} // 144.5 MHz (JT4A)

struct JTBandDescr
{
    uint16_t mode;
    uint16_t pll_a;
    uint32_t pll_b;
    uint32_t pll_c;
    uint16_t msync_div;
    uint16_t r_div;
    uint32_t baud_rate_dividend;
    uint32_t baud_rate_divisor;
    uint16_t tr_interval;
};

