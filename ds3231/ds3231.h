#pragma once

//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//


#include "rtc_datetime.h"

class Ds3231
{
public:

    enum ResultCode
    {
        rcSuccess = 0,
        rcFail
    };

    static ResultCode getTime(RtcDatetime& outTime);
    static void setTime(RtcDatetime& time);
    static int getTemperature();
    static void enable1PPS(bool enable);

};

