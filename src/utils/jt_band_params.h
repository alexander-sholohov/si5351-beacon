#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//


#include <stdint.h>
#include "jt_band_descr.h"

enum JTMode
{
    Mode_Unknown = 0,
    Mode_WSPR2 = 1,
    Mode_WSPR15 = 2,

    Mode_JT65_A = 3,
    Mode_JT65_B = 4,
    Mode_JT65_C = 5,
    Mode_JT65_B2 = 6,
    Mode_JT65_C2 = 7,

    Mode_JT9_1 = 8,
    Mode_JT9_2 = 9,
    Mode_JT9_5 = 10,
    Mode_JT9_10 = 11,
    Mode_JT9_30 = 12,

    Mode_JT4_A = 13,
    Mode_JT4_B = 14,
    Mode_JT4_C = 15,
    Mode_JT4_D = 16,
    Mode_JT4_E = 17,
    Mode_JT4_F = 18,
    Mode_JT4_G = 19,

    Mode_JT6M = 20,
    Mode_ISCAT_A = 21,
    Mode_ISCAT_B = 22,

    Mode_Morse = 30,

};


enum RF_Band
{
    RF_Band_Unknown = 0,
    RF_Band_2200m,
    RF_Band_600m,
    RF_Band_160m,
    RF_Band_80m,
    RF_Band_60m,
    RF_Band_40m,
    RF_Band_30m,
    RF_Band_20m,
    RF_Band_17m,
    RF_Band_15m,
    RF_Band_12m,
    RF_Band_10m,
    RF_Band_6m,
    RF_Band_2m
};



class JTBandParams
{
public:
    JTBandParams();
    void initFromJTBandDescr(JTBandDescr& bandDescr);
    void getPLLParamsForSymbol(unsigned symbol, uint16_t& a, uint32_t& b, uint32_t& c) const;
    JTMode   getJTMode() const;
    uint16_t getMsyncDiv() const { return _msync_div; }
    uint16_t getRDiv() const { return _r_div; }
    uint32_t getBaudRateDividend() const { return _baud_rate_dividend; }
    uint32_t getBaudRateDivisor() const { return _baud_rate_divisor; }
    uint16_t getTRInterval() const { return _tr_interval; }
    unsigned approxFrequencyInMHz() const;
    unsigned long getFrequencyInKHz() const;
    RF_Band getBand() const;
    uint32_t getStepInHZx1000() const;

private:
    uint16_t _jt_mode;
    uint16_t _pll_a;
    uint32_t _pll_b;
    uint32_t _pll_c;
    uint16_t _msync_div;
    uint16_t _r_div;
    uint32_t _baud_rate_dividend;
    uint32_t _baud_rate_divisor;
    uint16_t _tr_interval;

};
