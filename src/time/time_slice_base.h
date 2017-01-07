#pragma once

class RtcDatetime;

class TimeSliceBase
{
public:
    virtual void initialize() = 0;
    virtual bool getTime(RtcDatetime& outTime) = 0;
    virtual int get1PPS() = 0;
    virtual void doWork() = 0;
};
