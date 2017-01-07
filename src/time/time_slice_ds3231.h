#pragma once

#include "time_slice_base.h"

class RtcDatetime;

class TimeSliceDS3231 : public TimeSliceBase
{
public:
    TimeSliceDS3231(int pinPPS);
    void initialize();
    bool getTime(RtcDatetime& outTime);
    int get1PPS();
    void doWork();
private:
    TimeSliceDS3231();
    int m_pinPPS;
};
