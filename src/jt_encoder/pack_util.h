#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//
#include <stdlib.h>
#include <stdint.h>

class PackUtil
{
public:
    static void pack1b(unsigned char* buf, size_t pos, unsigned char val);
    static unsigned char unpack1b(const unsigned char* buf, size_t pos);
    static void pack2b(unsigned char* buf, size_t pos, unsigned char val);
    static unsigned char unpack2b(const unsigned char* buf, size_t pos);
};

