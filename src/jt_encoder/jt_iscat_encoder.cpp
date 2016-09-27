//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include <stdlib.h>
#include <string.h>

#include "jt_iscat_encoder.h"



//-----------------------------------------------------------------------
JTISCATEncoder::JTISCATEncoder()
    : JTEncodeBase()
    , _numPackets( 2 )
    , _msgLength( 0 )
{
}

//-----------------------------------------------------------------------
unsigned char JTISCATEncoder::charToCode( char ch ) const
{
    unsigned char res;

    if( ch >= '0' && ch <= '9' )
    {
        res = ch - '0';
    }
    else if( ch >= 'A' && ch <= 'Z' )
    {
        res = ch - 'A' + 10;
    }
    else if( ch >= 'a' && ch <= 'z' )
    {
        res = ch - 'a' + 10;
    }
    else if( ch == ' ' )
    {
        res = 36;
    }
    else if( ch == '/' )
    {
        res = 37;
    }
    else if( ch == '.' )
    {
        res = 38;
    }
    else if( ch == '?' )
    {
        res = 39;
    }
    else if( ch == '@' )
    {
        res = 40;
    }
    else if( ch == '-' )
    {
        res = 41;
    }
    else
    {
        res = 36; // space - illegal char
    }

    return res;
}

//-----------------------------------------------------------------------
void JTISCATEncoder::encodeMessage( const char* message, unsigned numRepeats )
{
    size_t len = strlen( message );
    if( len > sizeof(_message) - 1 )
    {
        len = sizeof(_message) - 1;
    }
    _message[0] = charToCode('@');
    for( unsigned i=0; i<len; i++ )
    {
        _message[i+1] = charToCode( message[i] );
    }
    _msgLength = len + 1;

    unsigned char l1 = _msgLength;
    unsigned char l2 = _msgLength + 5;
    if( l2 > 41 )
    {
        l2 -= 42;
    }


    unsigned pos=0;
    _header[pos++] = 0;
    _header[pos++] = 1;
    _header[pos++] = 3;
    _header[pos++] = 2;

    _header[pos++] = l1;
    _header[pos++] = l2;

    unsigned n = numRepeats * _msgLength;
    _numPackets = ( n % 18 )? (n / 18) + 1 : n / 18 ;
}

//-----------------------------------------------------------------------
void JTISCATEncoder::setNumPackets( unsigned numPackets )
{
    _numPackets = numPackets;
}



//-----------------------------------------------------------------------
unsigned JTISCATEncoder::numSymbols() const
{
    if( _msgLength == 0 )
        return 0;

    return _numPackets * 24; 
}

//-----------------------------------------------------------------------
unsigned char JTISCATEncoder::get_packed_symbol( unsigned pos ) const
{
    unsigned r = pos % 24;
    if( r < 6 )
    {
        return _header[r];
    }

    r -= 6;

    unsigned n = pos / 24;
    unsigned k = n * 18 + r;
    unsigned j = k % _msgLength;
    return _message[j];
}
