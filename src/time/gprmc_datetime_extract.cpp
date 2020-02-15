//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "gprmc_datetime_extract.h"
#include <string.h>

#define MYSWAP(A, B) do{char tmp=A; A=B; B=tmp;}while(0)

//---------------------------------------------------------------------------------
GPRMCDatetimeExtract::GPRMCDatetimeExtract()
    : GpsDatetimeExtractBase()
    , m_state(stWaitLine)
    , m_numCommas(0)
    , m_pos(0)
{
}

//---------------------------------------------------------------------------------
void GPRMCDatetimeExtract::onCharReceived(char ch)
{
    for (size_t idx = 1; idx < 6; idx++)
    {
        m_rmc[idx - 1] = m_rmc[idx];
    }
    m_rmc[5] = ch;

    if (strncmp(m_rmc, "$GPRMC", 6) == 0)
    {
        m_state = stWaitTime;
        m_numCommas = 0;
        memset(m_tmpDatetime, '0', 12);
        return;
    }

    // "$GPRMC,225446,A,5321.60231,N,08440.82524,W,000.5,054.7,191194,020.3,E*68";

    if (m_state == stWaitTime)
    {
        if (ch != ',')
        {
            invalidate();
            m_state = stWaitLine;
            return;
        }
        m_numCommas++;
        m_state = stGrabTime;
        m_pos = 0;
    }
    else if (m_state == stGrabTime)
    {
        if (ch == ',')
        {
            // unexpected
            invalidate();
            m_state = stWaitLine;
            return;
        }

        m_tmpDatetime[6 + m_pos] = ch;
        m_pos++;
        if (m_pos == 6)
        {
            m_state = stWaitDate;
        }
    }
    else if (m_state == stWaitDate)
    {
        if (ch == ',')
            m_numCommas++;

        if (m_numCommas == 9)
        {
            m_state = stGrabDate;
            m_pos = 0;
        }
    }
    else if (m_state == stGrabDate)
    {
        if (ch == ',')
        {
            // unexpected
            invalidate();
            m_state = stWaitLine;
            return;
        }

        m_tmpDatetime[m_pos] = ch;
        m_pos++;
        if (m_pos == 6)
        {
            // convert date. example 311216 -> 161231
            MYSWAP(m_tmpDatetime[0], m_tmpDatetime[4]);
            MYSWAP(m_tmpDatetime[1], m_tmpDatetime[5]);

            // put catched datetiome to base class
            putDateTime(m_tmpDatetime);
            m_state = stWaitLine;
        }
    }

}