//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "rtc_datetime.h"
#include <stdio.h>

const unsigned char RtcDatetime::_daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//---------------------------------------------------------------------------------
RtcDatetime::RtcDatetime()
{
    reset();
}

//---------------------------------------------------------------------------------
void RtcDatetime::reset()
{
    second = 0;
    minute = 0;
    hour = 0;
    day = 0;
    month = 0;
    year = 0;
}

//---------------------------------------------------------------------------------
unsigned long RtcDatetime::stamp() const
{
    // protect against incorrect date
    if( month == 0 || day == 0 )
        return 0;

    //
    // Very straightforward algorithm.
    //

    unsigned long tempDay = 0;
    for(unsigned i=0; i<year; i++ )
    {
        tempDay += ((i % 4) == 0 )? 366 : 365 ;
    }


    for(unsigned i=0; i<month-1; i++ )
    {
        tempDay += _daysInMonth[i];
    }

    if( month > 2 )
    {
        if( (year % 4) == 0 )
        {
            tempDay++;
        }
    }

    tempDay += day - 1;

    unsigned long s = tempDay * 86400L + (long)hour * 3600L + (long)minute * 60L + (long)second;
    return s;
}

//---------------------------------------------------------------------------------
void RtcDatetime::initFromStamp(unsigned long stamp)
{
    //
    // straightforward algorithm.
    // exact reverse of stamp()
    // 

    unsigned long numDays = stamp / 86400L;
    unsigned long numSeconds = stamp % 86400L;

    unsigned long tmpDays = 0;
    unsigned xYear = 0;
    for( ;xYear<100; xYear++ )
    {
        unsigned n = ((xYear % 4) == 0 )? 366 : 365 ;
        if( tmpDays + n > numDays )
            break;

        tmpDays += n;
    }

    unsigned xMonth = 0;
    for( ;xMonth<12; xMonth++ )
    {
        unsigned n = _daysInMonth[xMonth];
        if( ((xYear % 4) == 0 ) && xMonth == 1 )
            n++;

        if( tmpDays + n > numDays )
            break;

        tmpDays += n;
    }

    unsigned xDay = numDays - tmpDays;
    if( xDay > 30 )
        xDay = 30;

    year = xYear;
    month = xMonth + 1;
    day = xDay + 1;

    hour = numSeconds / 3600;
    minute = (numSeconds % 3600) / 60;
    second = (numSeconds % 3600) % 60;
}


//---------------------------------------------------------------------------------
long RtcDatetime::subtractFrom(RtcDatetime const& other) const
{
    unsigned long s1 = stamp();
    unsigned long s2 = other.stamp();
    long res = s2 - s1;
    return res;
}

//---------------------------------------------------------------------------------
bool RtcDatetime::isEqualTo(RtcDatetime const& other) const
{
    return (year==other.year) 
        && (month==other.month)
        && (day==other.day)
        && (hour==other.hour)
        && (minute==other.minute)
        && (second==other.second);
}

//---------------------------------------------------------------------------------
bool RtcDatetime::lessThan(const RtcDatetime &other) const
{
    if( year < other.year )
        return true;

    if( month < other.month )
        return true;

    if( day < other.day )
        return true;

    if( hour < other.hour )
        return true;

    if( minute < other.minute )
        return true;

    if( second < other.second )
        return true;

    return false;
}

//---------------------------------------------------------------------------------
bool RtcDatetime::greaterThan(const RtcDatetime &other) const
{
    if( year > other.year )
        return true;

    if( month > other.month )
        return true;

    if( day > other.day )
        return true;

    if( hour > other.hour )
        return true;

    if( minute > other.minute )
        return true;

    if( second > other.second )
        return true;

    return false;
}


//---------------------------------------------------------------------------------
int RtcDatetime::formatStr(char* buf) const
{
    int pos = 0;
    buf[pos++] = '0' + year / 10;
    buf[pos++] = '0' + year % 10;
    buf[pos++] = '-';
    buf[pos++] = '0' + month / 10;
    buf[pos++] = '0' + month % 10;
    buf[pos++] = '-';
    buf[pos++] = '0' + day / 10;
    buf[pos++] = '0' + day % 10;
    buf[pos++] = ' ';
    buf[pos++] = '0' + hour / 10;
    buf[pos++] = '0' + hour % 10;
    buf[pos++] = ':';
    buf[pos++] = '0' + minute / 10;
    buf[pos++] = '0' + minute % 10;
    buf[pos++] = ':';
    buf[pos++] = '0' + second / 10;
    buf[pos++] = '0' + second % 10;
    buf[pos] = 0;

    return pos;

}
