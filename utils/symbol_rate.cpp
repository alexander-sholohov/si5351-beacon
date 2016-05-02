//
//Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//


#include "symbol_rate.h"
#include <Arduino.h>



//--------------------------------------------------------
SymbolRate::SymbolRate()
{
    resetToNow();

    // wspr2 by default
    _dividend = 12;
    _divisor = 8192;
}


//--------------------------------------------------------
void SymbolRate::resetToNow()
{
    _stamp = millis();
}

//--------------------------------------------------------
void SymbolRate::initFromParams(long dividend, long divisor)
{
    _dividend = dividend;
    _divisor = divisor;
}

//--------------------------------------------------------
long SymbolRate::millisecondsElapsed() const
{
    unsigned long current = millis();
    return current - _stamp;
}

//--------------------------------------------------------
unsigned SymbolRate::currentSymbolIndex() const
{
    long elapsedInMs = millisecondsElapsed();
    return elapsedInMs * _dividend / _divisor;
}
