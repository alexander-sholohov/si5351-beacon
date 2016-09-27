//
//
// Original fortran code by Joseph Taylor, K1JT
//
// Ported to C/C++ by Alexander Sholohov, RA9YER <ra9yer@yahoo.com>
//
// License: MIT
//

#include <string.h>
#include <stdlib.h>

#include "jt_wspr_encoder.h"


#ifndef UINT32_MAX
#define UINT32_MAX (0xffffffff)
#endif


#define INRANGE(X, L, R)  (X >= L && X <= R)
#define EXACT(X, VAL) (X == VAL)


const unsigned char JTWSPREncoder::_sync[] = {
    0xc0, 0x8e, // 0b11000000, 0b10001110,
    0x25, 0xe0, // 0b00100101, 0b11100000,
    0x25, 0x02, // 0b00100101, 0b00000010,
    0xcd, 0x1a, // 0b11001101, 0b00011010,
    0x1a, 0xa9, // 0b00011010, 0b10101001,
    0x2c, 0x6a, // 0b00101100, 0b01101010,
    0x20, 0x93, // 0b00100000, 0b10010011,
    0xb3, 0x47, // 0b10110011, 0b01000111,
    0x05, 0x30, // 0b00000101, 0b00110000,
    0x1a, 0xc6, // 0b00011010, 0b11000110,
    0x0  //0b00000000
};

//-----------------------------------------------------------------------
JTWSPREncoder::JTWSPREncoder(void)
    : JTEncodeBase()
    , _ok(false)
    , _num_symbols(0)
{
}


//-----------------------------------------------------------------------
JTWSPREncoder::~JTWSPREncoder(void)
{
}

//----------------------------------------------------------------------------------------------
static int nchar(char ch)
{
    if( ch >='0' && ch <='9' )
    {
        return ch - '0';
    }
    else if( ch >='A' && ch <='Z' )
    {
        return ch - 'A' + 10;
    }
    else if( ch >='a' && ch <='z' )
    {
        return ch - 'a' + 10;
    }
    else if( ch == ' ' )
    {
        return 36;
    }

    // invalid char. should never happen
    return -1;
}

//----------------------------------------------------------------------------------------------
static int nchar2(char ch)
{
    if( ch >='0' && ch <='9' )
    {
        return ch - '0';
    }
    else if( ch >='A' && ch <='Z' )
    {
        return ch - 'A' + 10;
    }

    return 37;
}


//----------------------------------------------------------------------------------------------
static void decodeGrid6(const char* grid, int& outLon, int& outLat )
{
    // grid must be valid! like AA00aa

    // TODO: not exact fortran code due to lack of double type

    // grid4
    int nlon = 180 - 20 * ( grid[0] - 'A' ) - 2 * ( grid[2] - '0' ) ;
    int nlat = -90 + 10 * ( grid[1] - 'A' ) + ( grid[3] - '0' );

    // grid6
    nlon -= 50 * (grid[4] - 'a') / 600;
    nlat += 25 * (grid[5] - 'a') / 600;

    outLon = nlon;
    outLat = nlat;
}

//----------------------------------------------------------------------------------------------
static int parity(uint32_t data)
{
    int parity = 0;
    uint32_t ldata = data;
    while( ldata )
    {
        parity = 1 - parity;
        ldata = ldata & (ldata - 1);
    }

    return parity;
}

//----------------------------------------------------------------------------------------------
uint32_t JTWSPREncoder::packcall( const char* callsign ) const
{
    size_t len = strlen(callsign);
    if( len > 6 )
        return false;

    uint32_t nbase = (uint32_t)37 * 36 * 10 * 27 * 27 * 27;

    char lcallsign[7];
    memset(lcallsign, ' ', 6);
    lcallsign[6] = 0;


    // special condition

    if( !strncmp(callsign, "3DA0", 4) )
    {
        // Work-around for Swaziland prefix
        strncpy(lcallsign, "3D0", 3);
        strncpy(&lcallsign[3], &callsign[4], len-4);
    }
    else if( !strncmp(callsign, "CQ ", 3) )
    {
        strncpy(lcallsign, callsign, len);
        int ncall = nbase + 1;
        if( lcallsign[3]>='0' && lcallsign[3]<='9'
            && lcallsign[4]>='0' && lcallsign[4]<='9'
            && lcallsign[5]>='0' && lcallsign[5]<='9' )
        {
            int nfreq = atoi( &lcallsign[3] );
            ncall = nbase + 3 + nfreq;
        }
        return ncall;
    }
    else if( !strncmp(callsign, "QRZ ", 4) )
    {
        int ncall = nbase + 2;
        return ncall;
    }
    else
    {
        strncpy(lcallsign, callsign, len);
    }

    // short callsign modification

    if( lcallsign[2] >= '0' && lcallsign[2] <= '9' )
    {
        // valid
    }
    else if( lcallsign[1] >= '0' && lcallsign[1] <= '9' )
    {
        if( lcallsign[5] == ' ' )
        {
            // move right
            memcpy(&lcallsign[1], &lcallsign[0], 5);
            lcallsign[0] = ' ';
        }
        else
        {
            // not valid
            return UINT32_MAX;
        }
    }

    // convert to uppercase
    for( size_t i=0; i<6; i++ )
    {
        if( lcallsign[i] >= 'a' && lcallsign[i] <= 'z' )
        {
            lcallsign[i] &= ~0x20;
        }
    }

    // validation
    if( !(INRANGE(lcallsign[0], 'A', 'Z') || INRANGE(lcallsign[0], '0', '9') || EXACT(lcallsign[0], ' ') ) )
        return UINT32_MAX;
    if( !(INRANGE(lcallsign[1], 'A', 'Z') || INRANGE(lcallsign[1], '0', '9') ) )
        return UINT32_MAX;
    if( !(INRANGE(lcallsign[2], '0', '9') ) )
        return UINT32_MAX;
    if( !(INRANGE(lcallsign[3], 'A', 'Z') || EXACT(lcallsign[3], ' ') ) )
        return UINT32_MAX;
    if( !(INRANGE(lcallsign[4], 'A', 'Z') || EXACT(lcallsign[4], ' ') ) )
        return UINT32_MAX;
    if( !(INRANGE(lcallsign[4], 'A', 'Z') || EXACT(lcallsign[4], ' ') ) )
        return UINT32_MAX;

    // extra validation
    for( size_t i=0; i<6; i++ )
    {
        if( nchar( lcallsign[i] ) == -1 )
            return UINT32_MAX;
    }

    // calculate result
    uint32_t res;
    res =          nchar(lcallsign[0]);
    res = 36*res + nchar(lcallsign[1]);
    res = 10*res + nchar(lcallsign[2]);
    res = 27*res + nchar(lcallsign[3]) - 10;
    res = 27*res + nchar(lcallsign[4]) - 10;
    res = 27*res + nchar(lcallsign[5]) - 10;

    return res;

}

//-----------------------------------------------------------------------
int JTWSPREncoder::packgrid( const char* grid ) const
{

    int nbase = 180*180;

    if( !strcmp(grid, "    ") )
        return nbase + 1;

    // Test for numerical signal report, etc.
    if( !strncmp(grid, "-", 1) )
    {
        int n = atoi( &grid[1] );
        return nbase + 1 + n;
    }
    else if( !strncmp(grid, "R-", 2) )
    {
        int n = atoi( &grid[2] );
        if( n == 0 )
        {
            return nbase + 1;
        }
        else
        {
            return nbase + 31 + n;
        }
    }
    else if( !strncmp(grid, "RO", 2) )
    {
        return nbase + 62;
    }
    else if( !strncmp(grid, "RRR", 3) )
    {
        return nbase + 63;
    }
    else if( !strncmp(grid, "73", 2) )
    {
        return nbase + 64;
    }

    // validation
    size_t len = strlen(grid);
    if( len!=4 )
        return UINT32_MAX;

    if( !(INRANGE(grid[0], 'A', 'R') ) )
        return UINT32_MAX;
    if( !(INRANGE(grid[1], 'A', 'R') ) )
        return -1;
    if( !(INRANGE(grid[2], '0', '9') ) )
        return -1;
    if( !(INRANGE(grid[3], '0', '9') ) )
        return -1;

    char lgrid[7];
    strncpy(lgrid, grid, len);
    strncpy(&lgrid[len], "mm\0", 3);

    int lon = 0;
    int lat = 0;
    decodeGrid6( lgrid, lon, lat );

    int res = ((lon+180)/2)*180 + (lat + 90) ;
    return res;

}

//-----------------------------------------------------------------------
int JTWSPREncoder::packdbm( int dbm ) const
{
    int res = (dbm < 0)? 0 : dbm;
    res = (res > 60)? 60 : res;

    int corr[10] = {0,-1,1,0,-1,2,1,0,-1,1};
    res = res + corr[ res % 10 ];
    return res;
}

//-----------------------------------------------------------------------
void JTWSPREncoder::pack50( uint32_t n, uint32_t m, unsigned char* out )
{
    out[0] = n >> 20;
    out[1] = n >> 12;
    out[2] = n >> 4;
    out[3] = (n & 0xf) << 4 | ((m >> 18) & 0xf);
    out[4] = m >> 10;
    out[5] = m >> 2;
    out[6] = (m & 0x3) << 6;
    out[7] = 0;
    out[8] = 0;
    out[9] = 0;
    out[10] = 0;
}


//-----------------------------------------------------------------------
void JTWSPREncoder::encode232(const unsigned char* dat, size_t nbytes, unsigned char* symbol) const
{
    const uint32_t npoly1 = -221228207;
    const uint32_t npoly2 = -463389625;
    uint32_t nstate=0;
    size_t k=0;
    for(size_t j=0; j<nbytes; j++)
    {
        for(size_t i=0; i<8; i++)
        {
            unsigned char i4 = dat[j];
            nstate = (nstate << 1) | ((i4 >> (7-i)) & 0x1);
            symbol[k] = parity( nstate & npoly1 );
            k += 1;
            symbol[k] = parity( nstate & npoly2 );
            k += 1;
        }
    }
    
}


//-----------------------------------------------------------------------
void JTWSPREncoder::inter_mept(const unsigned char* dat, int direction, unsigned char* res) const
{

    // Compute the interleave table using bit reversal.
    // TODO: store the result

    unsigned char table[162];
    memset(table, 0, sizeof(table));
    size_t k=0;
    for(unsigned i=0; i<255; i++)
    {
        unsigned n=0;
        unsigned ii=i;
        for(size_t j=0; j<8; j++ )
        {
            n = n + n;
            if(ii & 0x1)
            {
                n += 1;
            }
            ii = ii >> 1;
        }

        if( n < 162 )
        {
            table[k] = n;
            k += 1;
        }
    }

    if( direction )
    {
        for(size_t i=0; i<162; i++ )
        {
            res[ table[i] ] = dat[i];
        }
    }
    else
    {
        for(size_t i=0; i<162; i++ )
        {
            res[i] = dat[ table[i] ];
        }
    }


}

//-----------------------------------------------------------------------
void JTWSPREncoder::make_channel_symbols(const unsigned char* dat, unsigned char* res) const
{
    size_t pos=0;
    size_t k=0;
    unsigned char s=_sync[pos];
    for(size_t i=0; i<162; i++)
    {
        res[i] = 2 * dat[i] + ((s & 0x80)? 1 : 0) ;

        s = s << 1;
        k++;
        if(k==8)
        {
            k = 0;
            pos++;
            s = _sync[pos];
        }
    }
}


//-----------------------------------------------------------------------
void JTWSPREncoder::store_as_internal_symbols(const unsigned char* channel_symbols)
{
    size_t pos = 0;
    unsigned char r = 0;
    size_t k = 0;
    for( size_t i=0; i<162; i++ )
    {
        r = (r << 2) | (channel_symbols[i] & 0x3);
        k+=2;
        if( k == 8 )
        {
            _internal_symbols[pos] = r;
            pos++;
            r = 0;
            k = 0;
        }
    }

    // save rest
    if( k != 0 )
    {
        _internal_symbols[pos] = r << (8-k);
    }

}



//-----------------------------------------------------------------------
unsigned char JTWSPREncoder::get_packed_symbol( unsigned pos ) const
{
    size_t n = pos / 4; // 4 symbols per byte
    size_t shift = pos % 4;

    unsigned char ch = _internal_symbols[n];
    ch = ch >> ( 6 - shift * 2 );
    ch = ch & 0x3;
    return ch;
}

//-----------------------------------------------------------------------
void JTWSPREncoder::internalEncodeData(const unsigned char* data, int num_bits)
{
    size_t nbits = 50 + 31;
    size_t nbytes = (nbits % 8 == 0)? nbits/8 : nbits/8 + 1;

    unsigned char dat[176];  // 11*8*2

    memset(dat,0, sizeof(dat));
    encode232(data, nbytes, dat);

    unsigned char dat_interlived[162];

    memset(dat_interlived,0, sizeof(dat_interlived));
    inter_mept(dat, 1, dat_interlived);

    unsigned char dat_channel[162];
    make_channel_symbols(dat_interlived, dat_channel);
    
    store_as_internal_symbols(dat_channel); //

    _ok = true;
    _num_symbols = 162;

}


//-----------------------------------------------------------------------
bool JTWSPREncoder::encodeExtendedLocation( const char* callsign, const char* location, int dbm )
{
    // callsign - callsign with or without prefix. Callsign will be send as hash.
    // location - 6 chars example: NO13ui
    // dbm - output power. 0 .. 60

    _ok = false;
    _num_symbols = 0;

    // TODO: implement

    return _ok;
}


//-----------------------------------------------------------------------
bool JTWSPREncoder::encodeMessage( const char* callsign, const char* location, int dbm )
{
    // callsign - valid callsign without prefix/suffix. Examples: AA9AAA, A9AAA, 0A9AAA, AA0AA, A0A, A0
    // location - 4 chars example: NO13
    // dbm - output power. 0 .. 60

    _ok = false;
    _num_symbols = 0;

    uint32_t res_N = packcall( callsign );
    int res2 = packgrid( location );
    int res3 = packdbm( dbm );

    if( res_N == UINT32_MAX || res2 == -1 )
        return false;

    uint32_t res_M = (uint32_t)128 * (uint32_t)res2 + (res3 + 64);

    unsigned char data0[11];
    memset(data0,0, sizeof(data0));
    pack50(res_N, res_M, data0 );

    internalEncodeData(data0, 50);
    _ntype = res3;


    return _ok;
}


//-----------------------------------------------------------------------
bool JTWSPREncoder::encodeExtendedCallsign( const char* callsign, int dbm )
{
    // callsign - valid callsign with prefix/suffix. Examples: ABC/AA9AAA, 000/AA9AAA, 0/AA9AAA, AA9AAA/M, AA9AAA/10
    // dbm - output power. 0 .. 60

    _ok = false;
    _num_symbols = 0;

    const char* pos1 = strstr(callsign, "/");
    if( !pos1 )
        return false;

    char lcallsign[7];
    
    uint32_t res_N = 0;
    uint32_t ng = 0;
    int nadd = 0;

    size_t lenAfterSlash = strlen(pos1) - 1;
    size_t lenBeforeSlash = pos1 - callsign;

    if( lenAfterSlash == 1)
    {
        // single char suffix
        size_t len = lenBeforeSlash;

        strncpy(lcallsign, callsign, len);
        lcallsign[len] = 0;
        

        int n = nchar2( *(pos1+1) );
        ng = (uint32_t)60000 - 32768 + n; // 

        nadd = 1;
    }
    else if( lenAfterSlash == 2 )
    {
        // Two-character numerical suffix, /10 to /99
        size_t len = lenBeforeSlash;

        strncpy(lcallsign, callsign, len);
        lcallsign[len] = 0;
        
        int n = atoi( pos1 + 1 );
        ng = (uint32_t)60000 + 26 + n;
        nadd = 1;
    }
    else if( lenBeforeSlash > 0 && lenBeforeSlash < 4 )
    {
        // Prefix of 1 to 3 characters
        size_t len = lenAfterSlash;
        strncpy(lcallsign, pos1+1, len);
        lcallsign[len] = 0;

        char pfx[3];
        memset(pfx, ' ', sizeof(pfx));
        strncpy( pfx + 3 - lenBeforeSlash, callsign, lenBeforeSlash );

        for(size_t i=0; i<3; i++ )
        {
            ng = 37 * ng + nchar( pfx[i] );
        }

        if( ng < 32768 )
        {
            nadd = 0;
        }
        else
        {
            ng = ng - 32768;
            nadd = 1;
        }
    }
    else
    {
        return false;
    }

    res_N = packcall( lcallsign ); //
    if( res_N == UINT32_MAX )
        return false;

    int res3 = packdbm( dbm );
    if( res3 == -1 )
        return false;

    int ntype = res3 + 1 + nadd;
    uint32_t n2 = (uint32_t)128 * ng + ntype + 64;
    unsigned char data0[11];
    memset(data0, 0, sizeof(data0));
    pack50(res_N, n2, data0 );

    internalEncodeData(data0, 50);
    _ntype = ntype;


    return _ok;
}

