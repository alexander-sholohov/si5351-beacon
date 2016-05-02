#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

class SymbolRate
{
public:

    SymbolRate();
    void initFromParams(long dividend, long divisor);

    void resetToNow();
    unsigned currentSymbolIndex() const;

private:
    unsigned long _stamp;
    long _dividend;
    long _divisor;

    long millisecondsElapsed() const;
};
