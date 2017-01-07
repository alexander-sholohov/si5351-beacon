//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "time_slice_ds3231.h"
#include "ds3231.h"
#include <Arduino.h>

//---------------------------------------------------------------------------------
TimeSliceDS3231::TimeSliceDS3231(int pinPPS)
    : m_pinPPS(pinPPS)
{
}

//---------------------------------------------------------------------------------
void TimeSliceDS3231::initialize()
{
    pinMode(m_pinPPS, INPUT_PULLUP);
    Ds3231::enable1PPS(true);
}

//---------------------------------------------------------------------------------
bool TimeSliceDS3231::getTime(RtcDatetime& outTime)
{
    return (Ds3231::getTime(outTime) == Ds3231::rcSuccess);
}

//---------------------------------------------------------------------------------
int TimeSliceDS3231::get1PPS()
{
    return digitalRead(m_pinPPS);
}

//---------------------------------------------------------------------------------
void TimeSliceDS3231::doWork()
{
}

