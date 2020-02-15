//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "gngll_datetime_extract.h"
#include <string.h>


//---------------------------------------------------------------------------------
GNGLLDatetimeExtract::GNGLLDatetimeExtract()
    : GpsDatetimeExtractBase()
    , m_state(stWaitLine)
    , m_numCommas(0)
    , m_pos(0)
{
}

//---------------------------------------------------------------------------------
void GNGLLDatetimeExtract::onCharReceived(char ch)
{
    for (size_t idx = 1; idx < 6; idx++)
    {
        m_rmc[idx - 1] = m_rmc[idx];
    }
    m_rmc[5] = ch;

    if (strncmp(m_rmc, "$GNGLL", 6) == 0)
    {
        m_state = stWaitTime;
        m_numCommas = 0;
        memset(m_tmpDatetime, '0', 12);
        return;
    }

    // "$GNGLL,5046.4661,N,03044.4404,E,104232.000,A,A*11";

    if (m_state == stWaitTime)
    {
        if (ch == ',')
            m_numCommas++;

        if(m_numCommas == 5)
        {
            m_state = stGrabTime;
            m_pos = 0;
        }
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
            // fill date as January 1'st of 2020
            memcpy(m_tmpDatetime, "200101", 6);

            // put catched datetime to base class
            putDateTime(m_tmpDatetime);
            m_state = stWaitLine;
        }
    }
}
