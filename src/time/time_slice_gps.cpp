//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//
#include "time_slice_gps.h"
#include "rtc_datetime.h"
#include <Stream.h>

//---------------------------------------------------------------------------------
TimeSliceGPS::TimeSliceGPS()
    : m_1ppsValid(false)
{
}

//---------------------------------------------------------------------------------
void TimeSliceGPS::initialize()
{
}

//---------------------------------------------------------------------------------
bool TimeSliceGPS::getTime(RtcDatetime& outTime)
{
    if (!m_gpsDatatimeExtract.isDateTimePresent())
        return false;

    outTime.initFromShortString(m_gpsDatatimeExtract.getDateTimeAsShortString());

    return true;
}

//---------------------------------------------------------------------------------
int TimeSliceGPS::get1PPS()
{
    // emulate 1pps signal using m_timer and event logic.
    // 000 ms - 499 ms - 0
    // 500 ms - 999+ ms - 1

    int v = (m_1ppsValid && m_timer.millisecondsElapsed() < 500)? 0 : 1;
    return v;
}

//---------------------------------------------------------------------------------
void TimeSliceGPS::putNMEAChar(char ch)
{
    m_gpsDatatimeExtract.onCharReceived(ch);
#ifdef ENABLE_MAIDENHEAD_LOCATOR
    m_gpsLatLotExtract.onCharReceived(ch);
#endif
}

//---------------------------------------------------------------------------------
void TimeSliceGPS::doWork()
{
    // We expect event raises every second, not faster!!!
    if (m_gpsDatatimeExtract.isEventPresent())
    {
        m_gpsDatatimeExtract.resetEvent(); // prevent reenter
        m_1ppsValid = true;
        m_timer.resetToNow();
    }
    else
    {
        // if GPS does not provide fresh time within 2 seconds then mark as not valid.
        if (m_1ppsValid && m_timer.millisecondsElapsed() > 2000)
        {
            m_1ppsValid = false;
        }
    }

}

