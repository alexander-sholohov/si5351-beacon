#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include <stdlib.h>
#include <stdint.h>

#include "jt_encoder_base.h"

// WSPR - 162 symbols * 2bits = 41bytes
// JT65 - 63symbols * 6bits = 48bytes
// JT9 - 85symbols * 4bits = 43bytes
// JT4 - 206symbols * 2bits = 52bytes
//          so, 54 bytes - enough

#define JT_MOCK_ENCODER_NUM_BYTES (54)

class JTMockEncoder : public JTEncodeBase
{
public:

    JTMockEncoder(void);
    ~JTMockEncoder(void);

    void initFromArray(unsigned char* buf, unsigned numberOfSymbols, unsigned bitsPerSymbol);
    unsigned numSymbols() const;

    //STL and C++11 iteration capability.
    JTEncodeBase::iterator begin() { return JTEncodeBase::iterator(0, this); }
    JTEncodeBase::iterator end()  { return JTEncodeBase::iterator(numSymbols(), this); }

protected:
    unsigned char internalGetPackedSymbol(unsigned pos) const;

private:
    unsigned _bitsPerSymbol;
    unsigned _symbolsTotal;
    unsigned char _internal_symbols[JT_MOCK_ENCODER_NUM_BYTES]; // bit packed storage

    virtual unsigned char get_packed_symbol(unsigned pos) const;
 
};
