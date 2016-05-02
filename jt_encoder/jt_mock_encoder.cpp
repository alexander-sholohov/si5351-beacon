
//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//


#include "jt_mock_encoder.h"

//-----------------------------------------------------------------------
JTMockEncoder::JTMockEncoder()
    : JTEncodeBase()
    , _bitsPerSymbol(1)
    , _symbolsTotal(0)
{
}

//-----------------------------------------------------------------------
JTMockEncoder::~JTMockEncoder()
{
}

//-----------------------------------------------------------------------
void JTMockEncoder::initFromArray( unsigned char* buf, unsigned numberOfSymbols, unsigned bitsPerSymbol )
{
    _bitsPerSymbol = 1;
    _symbolsTotal = 0;

    if( bitsPerSymbol > 8 )
    {
        // TODO: assert
        return;
    }

    unsigned neededBytesTotal = numberOfSymbols * bitsPerSymbol / 8;
    if( neededBytesTotal > sizeof(_internal_symbols) )
    {
        // TODO: assert
        return;
    }

    _bitsPerSymbol = bitsPerSymbol;
    _symbolsTotal = numberOfSymbols;


    unsigned mask = 1;
    for(size_t i=0; i<_bitsPerSymbol-1; i++)
    {
        mask = (mask << 1) | 1;
    }


    // zero init
    for(size_t i=0; i<sizeof(_internal_symbols); i++)
    {
        _internal_symbols[i] = 0;
    }


    for(size_t pos=0; pos<numberOfSymbols; pos++)
    {
        unsigned ch = buf[pos];

        unsigned byteNum = pos * _bitsPerSymbol / 8;
        unsigned bitShift = (pos * _bitsPerSymbol) % 8;

        unsigned value = *((unsigned*)&_internal_symbols[byteNum]);
        value = value | ((ch & mask) << bitShift);

        *((unsigned*)&_internal_symbols[byteNum]) = value;
    }

}

//-----------------------------------------------------------------------
unsigned char JTMockEncoder::internalGetPackedSymbol( unsigned pos ) const
{
    if( pos >= _symbolsTotal )
    {
        return 0;
    }

    unsigned byteNum = pos * _bitsPerSymbol / 8;
    unsigned bitShift = (pos * _bitsPerSymbol) % 8;
    unsigned mask = 1;
    for(size_t i=0; i<_bitsPerSymbol-1; i++)
    {
        mask = (mask << 1) | 1;
    }

    unsigned value = *((unsigned*)&_internal_symbols[byteNum]);

    value >>= bitShift;
    value &= mask;

    return value;
}

//-----------------------------------------------------------------------
unsigned char JTMockEncoder::get_packed_symbol( unsigned pos ) const
{
    return internalGetPackedSymbol( pos );
}

//-----------------------------------------------------------------------
unsigned JTMockEncoder::numSymbols() const
{
    return _symbolsTotal;
}
