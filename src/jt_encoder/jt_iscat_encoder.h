#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "jt_encoder_base.h"



class JTISCATEncoder : public JTEncodeBase
{
public:
    JTISCATEncoder();
    void encodeMessage( const char* message, unsigned numRepeats ); 
    void setNumPackets( unsigned numPackets );

    //STL and C++11 iteration capability.
    JTEncodeBase::iterator begin() { return JTEncodeBase::iterator(0, this); }
    JTEncodeBase::iterator end()  { return JTEncodeBase::iterator(numSymbols(), this); }

    unsigned numSymbols() const;

private:
    unsigned _numPackets;
    unsigned _msgLength;
    unsigned char _header[6];
    char _message[28];

    unsigned char charToCode( char ch ) const;

    virtual unsigned char get_packed_symbol(unsigned pos) const;

};
