//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "gps_datetime_extract_base.h"
#include <string.h>

//---------------------------------------------------------------------------------
GpsDatetimeExtractBase::GpsDatetimeExtractBase()
    : m_isDateTimePresent(false)
{
}

//---------------------------------------------------------------------------------
void GpsDatetimeExtractBase::invalidate()
{
    m_isDateTimePresent = false;
}

//---------------------------------------------------------------------------------
bool GpsDatetimeExtractBase::isDateTimePresent() const
{
    return m_isDateTimePresent;
}

//---------------------------------------------------------------------------------
const char* GpsDatetimeExtractBase::getDateTimeAsShortString() const
{
    return m_datetime;
}

//---------------------------------------------------------------------------------
void GpsDatetimeExtractBase::putDateTime(char * datetime)
{
    // validation
    bool valid = true;
    for (size_t idx = 0; idx < 12; idx++)
    {
        if (datetime[idx]<'0' || datetime[idx]>'9')
        {
            valid = false;
            break;
        }
    }

    if (!valid)
    {
        m_isDateTimePresent = false;
        return;
    }

    m_isDateTimePresent = true;
    memcpy(m_datetime, datetime, 12);
    m_datetime[12] = 0; // ASCIIZ null terminated 

    m_isEventPresent = true; // set flag of event logic
}

//---------------------------------------------------------------------------------
bool GpsDatetimeExtractBase::isEventPresent() const
{
    return m_isEventPresent;
}

//---------------------------------------------------------------------------------
void GpsDatetimeExtractBase::resetEvent()
{
    m_isEventPresent = false;
}
