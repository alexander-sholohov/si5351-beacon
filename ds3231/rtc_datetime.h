#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//


class RtcDatetime
{
public:
    unsigned second;
    unsigned minute;
    unsigned hour;
    unsigned day;
    unsigned month;
    unsigned year;

    RtcDatetime();
    void reset();
    unsigned long stamp() const;
    void initFromStamp( unsigned long stamp );
    long subtractFrom( const RtcDatetime &other ) const;
    bool isEqualTo( const RtcDatetime &other ) const;
    bool lessThan( const RtcDatetime &other ) const;
    bool greaterThan( const RtcDatetime &other ) const;

    int formatStr(char* buf) const;

    bool operator == ( const RtcDatetime &other ) const { return isEqualTo( other ); }
    bool operator < ( const RtcDatetime &other ) const { return lessThan( other ); }
    bool operator > ( const RtcDatetime &other ) const { return greaterThan( other ); }
    bool operator >= ( const RtcDatetime &other ) const { return !lessThan( other ); }
private:
    static const unsigned char _daysInMonth[12];
};
