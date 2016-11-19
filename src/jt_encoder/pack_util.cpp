//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "pack_util.h"


//-----------------------------------------------------------------------
void PackUtil::pack1b(unsigned char* buf, size_t pos, unsigned char val)
{
    size_t n = pos / 8; // 8 symbols per byte
    size_t shift = pos % 8;
    unsigned char mask = 0x80 >> shift;
    unsigned char upd =  val << (7 - shift);

    buf[n] = (buf[n] & ~mask) | upd;
}

//-----------------------------------------------------------------------
unsigned char PackUtil::unpack1b(const unsigned char* buf, size_t pos)
{
    size_t n = pos / 8; // 8 symbols per byte
    size_t shift = pos % 8;

    unsigned char res = buf[n];
    res = (res >> (7 - shift)) & 1;
    return res;
}


//-----------------------------------------------------------------------
void PackUtil::pack2b(unsigned char* buf, size_t pos, unsigned char val)
{
    size_t n = pos / 4; // 4 symbols per byte
    size_t shift = pos % 4;
    unsigned char mask = 0xc0 >> (shift * 2);
    unsigned char upd = val << (6 - shift * 2);

    buf[n] = (buf[n] & ~mask) | upd;
}


//-----------------------------------------------------------------------
unsigned char PackUtil::unpack2b(const unsigned char* buf, size_t pos)
{
    size_t n = pos / 4; // 4 symbols per byte
    size_t shift = pos % 4;

    unsigned char res = buf[n];
    res = (res >> (6 - shift * 2)) & 3;
    return res;
}
