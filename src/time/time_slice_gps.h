#pragma once
//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "gps_config.h"
#include "time_slice_base.h"

#ifdef ENABLE_MAIDENHEAD_LOCATOR
#include "gngll_latlon_extract.h"
#include "gprmc_latlon_extract.h"
#endif

#include "gngll_datetime_extract.h"
#include "gprmc_datetime_extract.h"

#include "../utils/timerange.h"

class RtcDatetime;
class Stream;

class TimeSliceGPS : public TimeSliceBase
{
public:
    TimeSliceGPS();
    void initialize();
    bool getTime(RtcDatetime& outTime);
    int get1PPS();
    void doWork();
    void putNMEAChar(char ch);

#ifdef ENABLE_MAIDENHEAD_LOCATOR
    const char* calcMaidenheadLocator(unsigned numChars) { return m_gpsLatLotExtract.calcMaidenheadLocator(numChars); }
    bool isValidLatLonPresent() const { return m_gpsLatLotExtract.isValidLatLonPresent(); }
#endif

private:
    bool m_1ppsValid;
    CTimeRange m_timer;

    // Arduino-stype factory
#ifdef USE_GPRMC
    GPRMCDatetimeExtract m_gpsDatatimeExtract;
#endif // USE_GPRMC
#ifdef USE_GNGLL
    GNGLLDatetimeExtract m_gpsDatatimeExtract;
#endif // USE_GNGLL

#ifdef ENABLE_MAIDENHEAD_LOCATOR
#ifdef USE_GPRMC
    GPRMCLatLonExtract m_gpsLatLotExtract;
#endif // USE_GPRMC
#ifdef USE_GNGLL
    GNGLLLatLonExtract m_gpsLatLotExtract;
#endif // USE_GNGLL
#endif // ENABLE_MAIDENHEAD_LOCATOR

};
