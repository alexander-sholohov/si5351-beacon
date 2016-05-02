#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include <stdlib.h>
#include <stdint.h>

#include "jt_encoder_base.h"


class JTWSPREncoder : public JTEncodeBase
{
public:

    JTWSPREncoder(void);
    ~JTWSPREncoder(void);

    bool encodeMessage( const char* callsign, const char* location, int dbm ); 
    bool encodeExtendedCallsign( const char* callsign, int dbm ); 
    bool encodeExtendedLocation( const char* callsign, const char* location, int dbm ); 

    bool isInitialized() const { return _ok; }
    int ntype() const { return _ntype; }


    //STL and C++11 iteration capability.
    JTEncodeBase::iterator begin() { return JTEncodeBase::iterator(0, this); }
    JTEncodeBase::iterator end() { return JTEncodeBase::iterator(_num_symbols, this); }

private:
    static const unsigned char _sync[21];
    bool _ok;
    int _ntype;
    unsigned _num_symbols;
    unsigned char _internal_symbols[41];

    uint32_t packcall(const char* callsign) const;
    int packgrid(const char* grid) const;
    int packdbm(int dbm) const;
    void pack50(uint32_t n, uint32_t m, unsigned char* out);
    void encode232(const unsigned char* dat, size_t nbytes, unsigned char* symbol) const;
    void inter_mept(const unsigned char* dat, int direction, unsigned char* res) const; 
    void apply_sync(const unsigned char* dat, unsigned char* res) const; 
    void make_channel_symbols(const unsigned char* dat, unsigned char* res) const;
    void internalEncodeData(const unsigned char* data, int num_bits);
    void store_as_internal_symbols(const unsigned char* channel_symbols);

    virtual unsigned char get_packed_symbol(unsigned pos) const;
};

