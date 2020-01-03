//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//
#include "time_slice_gps.h"
#include "rtc_datetime.h"
#include <Stream.h>

//---------------------------------------------------------------------------------
TimeSliceGPS::TimeSliceGPS(Stream& serial)
    : m_uart(serial)
    , m_valid(false)
{
}

//---------------------------------------------------------------------------------
void TimeSliceGPS::initialize()
{
}

//---------------------------------------------------------------------------------
bool TimeSliceGPS::getTime(RtcDatetime& outTime)
{
    if( m_valid )
    {
        outTime.initFromShortString(m_datetime);
    }

    return m_valid;
}

//---------------------------------------------------------------------------------
int TimeSliceGPS::get1PPS()
{
    int v = (m_valid && m_timer.millisecondsElapsed() < 500)? 0 : 1;
    return v;
}


//---------------------------------------------------------------------------------
void TimeSliceGPS::doWork()
{
    while(m_uart.available())
    {
        char ch = m_uart.read();
        m_gpsDataExtract.onCharReceived(ch);
#ifdef ENABLE_MAIDENHEAD_LOCATOR
        m_gpsLatLotExtract.onCharReceived(ch);
#endif
    }

    if( m_gpsDataExtract.isDateTimePresent() )
    {
        m_valid = true;
        m_gpsDataExtract.retrieveDateTime(m_datetime);
        m_timer.resetToNow();
    }
}

#ifdef ENABLE_MAIDENHEAD_LOCATOR
//---------------------------------------------------------------------------------
const char* TimeSliceGPS::calcMaidenheadLocator(unsigned numChars)
{
    return m_gpsLatLotExtract.calcMaidenheadLocator(numChars);
}

bool TimeSliceGPS::isValidLatLonPresent() const
{
    return m_gpsLatLotExtract.isValidLatLonPresent();
}
#endif
