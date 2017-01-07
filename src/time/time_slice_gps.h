#pragma once
//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "time_slice_base.h"
#include "gps_data_extract.h"
#include "../utils/timerange.h"

class RtcDatetime;
class Stream;

class TimeSliceGPS : public TimeSliceBase
{
public:
    TimeSliceGPS(Stream& serial);
    void initialize();
    bool getTime(RtcDatetime& outTime);
    int get1PPS();
    void doWork();
private:
    TimeSliceGPS();
    GpsDataExtract m_gpsDataExtract;
    CTimeRange m_timer;
    bool m_valid;
    char m_datetime[12];
};
