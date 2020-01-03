#pragma once
//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "gps_config.h"
#include "time_slice_base.h"
#include "gps_datetime_extract.h"
#include "gps_latlon_extract.h"
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
#ifdef ENABLE_MAIDENHEAD_LOCATOR
    const char* calcMaidenheadLocator(unsigned numChars);
    bool isValidLatLonPresent() const;
#endif

private:
    TimeSliceGPS();
    Stream& m_uart;
    GpsDataExtract m_gpsDataExtract;
#ifdef ENABLE_MAIDENHEAD_LOCATOR
    GpsLatLonExtract m_gpsLatLotExtract;
#endif
    CTimeRange m_timer;
    bool m_valid;
    char m_datetime[12];
};
