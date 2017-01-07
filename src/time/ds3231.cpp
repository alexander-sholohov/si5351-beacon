//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//


#include "ds3231.h"

#define DS3231_I2C_ADDRESS (0x68)

#define USE_ARDUINO_I2C


#ifdef USE_ARDUINO_I2C
#include <Wire.h>

//---------------------------------------------------------------------------------
static void i2c_write(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS); 
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

//---------------------------------------------------------------------------------
static void i2c_bust_write(uint8_t reg, uint8_t* buf, size_t count)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS); 
    Wire.write(reg);
    for( size_t i=0; i<count; i++ )
    {
        Wire.write(buf[i]);
    }
    Wire.endTransmission();
}



//---------------------------------------------------------------------------------
static size_t i2c_bust_read(uint8_t reg, uint8_t* buf, size_t count)
{
    Wire.beginTransmission(DS3231_I2C_ADDRESS); 
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_I2C_ADDRESS, count); 
    size_t pos = 0;
    while (Wire.available())
    {
        buf[pos] = Wire.read(); 
        pos++;
        if (pos >= count)
            break;
    }
    return pos;
}


#endif



#define FROM_BCD(X, MASK) (((X >> 4) & MASK)*10 + (X & 0xf))
#define TO_BCD(X) (((X / 10) << 4) | (X % 10))

//---------------------------------------------------------------------------------
Ds3231::ResultCode Ds3231::getTime(RtcDatetime& outTime)
{
    unsigned char buf[7];
    memset(buf, 0, sizeof(buf));
    i2c_bust_read(0, buf, 7);

    bool allZero = true;
    for(size_t i=0; i<7; i++ )
    {
        if( buf[i] != 0 )
            allZero = false;
    }

    if( allZero )
        return rcFail;

    // day and month should be not zero
    if( FROM_BCD(buf[4], 3) == 0 || FROM_BCD(buf[5], 1) == 0 )
        return rcFail;

    outTime.second = FROM_BCD(buf[0], 7);
    outTime.minute = FROM_BCD(buf[1], 7);
    outTime.hour = FROM_BCD(buf[2], 3);
    // #TODO: day of week
    outTime.day = FROM_BCD(buf[4], 3);
    outTime.month = FROM_BCD(buf[5], 1);
    outTime.year = FROM_BCD(buf[6], 15);

    return rcSuccess;
}


//---------------------------------------------------------------------------------
void Ds3231::setTime(RtcDatetime& time)
{
    unsigned char buf[7];
    memset(buf, 0, sizeof(buf));
    buf[0] = TO_BCD( time.second );
    buf[1] = TO_BCD( time.minute );
    buf[2] = TO_BCD( time.hour );
    buf[3] = 1; // #TODO: day of week
    buf[4] = TO_BCD( time.day );
    buf[5] = TO_BCD( time.month );
    buf[6] = TO_BCD( time.year );
    i2c_bust_write(0, buf, 7);
}

//---------------------------------------------------------------------------------
int Ds3231::getTemperature()
{
    unsigned char buf[2];
    memset(buf, 0, sizeof(buf));
    i2c_bust_read(0x11, buf, 2);

    bool isNegative = (buf[0] & 0x80) != 0;
    int integerValue = buf[0] & 0x7f;
    int fraqtionalValue = buf[1] >> 6;
    int res = integerValue * 100 + fraqtionalValue * 25;
    if(isNegative)
        res = -1 * res;
    return res;
}


//---------------------------------------------------------------------------------
void Ds3231::enable1PPS(bool enable)
{
    unsigned char buf[1];
    memset(buf, 0, sizeof(buf));
    i2c_bust_read(0x0e, buf, 1);
    if (enable)
    {
        buf[0] &= ~0x1c;
    }
    else
    {
        buf[0] |= 0x1c;
    }
    i2c_bust_write(0x0e, buf, 1);
}
