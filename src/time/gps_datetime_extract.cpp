//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "gps_datetime_extract.h"
#include <Arduino.h>

#define MYSWAP(A, B) do{char tmp=A; A=B; B=tmp;}while(0)

//---------------------------------------------------------------------------------
GpsDataExtract::GpsDataExtract()
    : m_state(stWaitLine)
    , m_numCommas(0)
    , m_pos(0)
    , m_isDateTimePresent(false)
{
}

//---------------------------------------------------------------------------------
bool GpsDataExtract::isDateTimePresent() const
{
    return m_isDateTimePresent;
}

//---------------------------------------------------------------------------------
void GpsDataExtract::retrieveDateTime(char* buf)
{
    strncpy(buf, m_datetime, 12);
    m_isDateTimePresent = false; // mark data retrieved
}



//---------------------------------------------------------------------------------
void GpsDataExtract::onCharReceived(char ch)
{
    for(size_t idx=1; idx<6; idx++)
    {
        m_rmc[idx-1] = m_rmc[idx];
    }
    m_rmc[5] = ch;

    if( strncmp(m_rmc, "$GPRMC", 6) == 0 )
    {
        m_state = stWaitTime;
        m_numCommas = 0;
        memset(m_datetime, '0', 12);
        m_isDateTimePresent = false;
        return;
    }

    if( m_state == stWaitTime )
    {
        if( ch != ',' )
        {
            m_state = stWaitLine;
            return;
        }
        m_numCommas++;
        m_state = stGrabTime;
        m_pos = 0;
    }
    else if( m_state == stGrabTime )
    {
        if( ch == ',' )
        {
            // unexpected
            m_state = stWaitLine;
            return;
        }

        m_datetime[6+m_pos] = ch;
        m_pos++;
        if( m_pos == 6 )
        {
            m_state = stWaitDate;
        }
    }
    else if( m_state == stWaitDate )
    {
        if( ch == ',' )
            m_numCommas++;

        if( m_numCommas == 9 )
        {
            m_state = stGrabDate;
            m_pos = 0;
        }
    }
    else if( m_state == stGrabDate )
    {
        if( ch == ',' )
        {
            // unexpected
            m_state = stWaitLine;
            return;
        }

        m_datetime[m_pos] = ch;
        m_pos++;
        if( m_pos == 6 )
        {
            // convert date. example 311216 -> 161231
            MYSWAP(m_datetime[0], m_datetime[4]);
            MYSWAP(m_datetime[1], m_datetime[5]);

            // validation
            bool valid = true;
            for( size_t idx=0; idx<12; idx++ )
            {
                if( m_datetime[idx]<'0' || m_datetime[idx]>'9' )
                {
                    valid = false;
                    break;
                }
            }

            // Done
            m_isDateTimePresent = valid;
            m_state = stWaitLine;
        }
    }
}
