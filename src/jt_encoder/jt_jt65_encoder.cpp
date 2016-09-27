//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "jt_jt65_encoder.h"


// JT65 sync vector 
// 100110001111110101000101100100011100111101101111000110101011001
// 101010100100000011000000011010010110101010011001001000011111111

// sync vector as hex
const unsigned char JT65Encoder::_sync_vector[] = { 0x98,0xfd,0x45,0x91,0xcf,0x6f,0x1a,0xb3,0x54,0x81,0x80,0xd2,0xd5,0x32,0x43,0xff};

// jt65 has 126 symbols
const unsigned JT65Encoder::_maxSymbols = 126;

//-----------------------------------------------------------------------
JT65Encoder::JT65Encoder()
    : JTMockEncoder()
{
}


//-----------------------------------------------------------------------
unsigned JT65Encoder::numSymbols() const
{
    if( JTMockEncoder::numSymbols() == 0 )
        return 0;

    return _maxSymbols;
}

//-----------------------------------------------------------------------
unsigned char JT65Encoder::get_packed_symbol( unsigned pos ) const
{
    unsigned numChar = pos / 8;
    unsigned shiftChar = pos % 8;
    if( numChar > sizeof(_sync_vector) )
        return 0;

    unsigned cnt = 0;
    unsigned char ch = _sync_vector[numChar];
    for( unsigned j=0; j<shiftChar; j++ )
    {
        // calculate sum of zeros at left
        if( ( ch & 0x80 ) == 0 )
        {
            cnt++;
        }
        ch <<= 1;
    }

    if( ch & 0x80 )
    {
        // "1" means send sync
        return 0;
    }

    // a little bit optimized algorithm to calculate summ of zeros
    for( unsigned j=0; j<numChar; j++ )
    {
        unsigned char tmp = ~_sync_vector[j];
        while(tmp)
        {
            tmp = tmp & (tmp - 1);
            cnt++;
        }
    }

    unsigned char res = internalGetPackedSymbol( cnt ) + 2;
    return res;
}
