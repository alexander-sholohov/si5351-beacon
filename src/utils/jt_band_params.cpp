//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include <stdint.h>
#include "jt_band_params.h"

#define XTAL_FREQUENCY_IN_KHZ (27000)

//--------------------------------------------------------------------------------
JTBandParams::JTBandParams()
    : _jt_mode(0)
    , _pll_a(0)
    , _pll_b(0)
    , _pll_c(1)
    , _msync_div(1)
    , _r_div(1)
    , _baud_rate_dividend(1)
    , _baud_rate_divisor(1)
    , _tr_interval(0)
{

}

//--------------------------------------------------------------------------------
void JTBandParams::initFromJTBandDescr(JTBandDescr& bandDescr)
{
    _jt_mode = bandDescr.mode & 0x3f;
    _pll_a = bandDescr.pll_a;
    _pll_b = bandDescr.pll_b;
    _pll_c = bandDescr.pll_c;
    _msync_div = bandDescr.msync_div;
    _r_div = bandDescr.r_div;
    _baud_rate_dividend = bandDescr.baud_rate_dividend;
    _baud_rate_divisor = bandDescr.baud_rate_divisor;
    _tr_interval = bandDescr.tr_interval;
}


//--------------------------------------------------------------------------------
void JTBandParams::getPLLParamsForSymbol(unsigned symbol, uint16_t& a, uint32_t& b, uint32_t& c) const
{
    a = _pll_a;
    b = _pll_b + symbol;
    c = _pll_c;
}

//--------------------------------------------------------------------------------
unsigned JTBandParams::approxFrequencyInMHz() const
{
    unsigned long res = getFrequencyInKHz() / 1000;
    return static_cast<unsigned>( res );
}

//--------------------------------------------------------------------------------
unsigned long JTBandParams::getFrequencyInKHz() const
{
    if(_pll_c==0 || _msync_div==0 || _r_div==0)
        return 0;

    unsigned long xtalFreq = XTAL_FREQUENCY_IN_KHZ;
    unsigned long m = 1000;
    unsigned long f = m * _pll_b / _pll_c;
    unsigned long res = ( xtalFreq * _pll_a + xtalFreq * f / m ) / _msync_div / _r_div ;

    return res;
}

//--------------------------------------------------------------------------------
RF_Band JTBandParams::getBand() const
{
    
    unsigned long freq = getFrequencyInKHz();
    RF_Band res;

    if( freq < 138 )
    {
        res = RF_Band_2200m;
    }
    else if( freq < 480 )
    {
        res = RF_Band_600m;    
    }
    else if( freq < 2000 )
    {
        res = RF_Band_160m;    
    }
    else if( freq < 3800 )
    {
        res = RF_Band_80m;    
    }
    else if( freq < 5450 )
    {
        res = RF_Band_60m;    
    }
    else if( freq < 7300 )
    {
        res = RF_Band_40m;    
    }
    else if( freq < 10150 )
    {
        res = RF_Band_30m;    
    }
    else if( freq < 14350 )
    {
        res = RF_Band_20m;    
    }
    else if( freq < 18168 )
    {
        res = RF_Band_17m;    
    }
    else if( freq < 21450 )
    {
        res = RF_Band_15m;    
    }
    else if( freq < 24990 )
    {
        res = RF_Band_12m;    
    }
    else if( freq < 29700 )
    {
        res = RF_Band_10m;    
    }
    else if( freq < 54000 )
    {
        res = RF_Band_6m;    
    }
    else if( freq < 146000 )
    {
        res = RF_Band_2m;    
    }
    else
    {
        res = RF_Band_Unknown;
    }

    return res;
}



//--------------------------------------------------------------------------------
JTMode JTBandParams::getJTMode() const
{
    return static_cast<JTMode>( _jt_mode );
}

//--------------------------------------------------------------------------------
uint32_t JTBandParams::getStepInHZx1000() const
{
    uint32_t step = 0;
    switch( getJTMode() )
    {
        case Mode_Unknown:
            step = 0;
            break;
        case Mode_WSPR2:
            step = 1465;
            break;
        case Mode_WSPR15:
            step = 183;
            break;

        case Mode_JT65_A:
            step = 2692;
            break;
        case Mode_JT65_B:
            step = 5383;
            break;
        case Mode_JT65_C:
            step = 10766;
            break;
        case Mode_JT65_B2:
            step = 5383;
            break;
        case Mode_JT65_C2:
            step = 10766;
            break;

        case Mode_JT9_1:
            step = 1736;
            break;
        case Mode_JT9_2:
            step = 781;
            break;
        case Mode_JT9_5 :
            step = 293;
            break;
        case Mode_JT9_10 :
            step = 148;
            break;
        case Mode_JT9_30 :
            step = 48;
            break;

        case Mode_JT4_A :
            step = 4375;
            break;
        case Mode_JT4_B :
            step = 8750;
            break;
        case Mode_JT4_C :
            step = 17500;
            break;
        case Mode_JT4_D :
            step = 39375;
            break;
        case Mode_JT4_E :
            step = 78750;
            break;
        case Mode_JT4_F :
            step = 157500;
            break;
        case Mode_JT4_G :
            step = 315000;
            break;

        case Mode_JT6M :
            step = 21533;
            break;
        case Mode_ISCAT_A :
            step = 21533;
            break;
        case Mode_ISCAT_B :
            step = 43066;
            break;

        case Mode_Morse :
            step = 0;
            break;

        default:
            step = 0;

    }

    return step;
}
