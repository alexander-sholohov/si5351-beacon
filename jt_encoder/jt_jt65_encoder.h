#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "jt_mock_encoder.h"



class JT65Encoder : public JTMockEncoder
{
public:
    JT65Encoder();

    //STL and C++11 iteration capability.
    JTEncodeBase::iterator begin() { return JTEncodeBase::iterator(0, this); }
    JTEncodeBase::iterator end()  { return JTEncodeBase::iterator(numSymbols(), this); }

    unsigned numSymbols() const;

private:
    static const unsigned char _sync_vector[16];
    static const unsigned _maxSymbols;

    virtual unsigned char get_packed_symbol(unsigned pos) const;

};
