//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "gngll_latlon_extract.h"
#include <string.h>

//---------------------------------------------------------------------------------
GNGLLLatLonExtract::GNGLLLatLonExtract()
    : GpsLatLonExtractBase() 
    , m_state(stWaitLine)
    , m_numCommas(0)
    , m_pos(0)
{
}

//---------------------------------------------------------------------------------
void GNGLLLatLonExtract::onCharReceived(char ch)
{
    for (size_t idx = 1; idx < 6; idx++)
    {
        m_rmc[idx - 1] = m_rmc[idx];
    }
    m_rmc[5] = ch;

    if (strncmp(m_rmc, "$GNGLL", 6) == 0)
    {
        m_state = stWaitLatLonInfo;
        m_numCommas = 0;
        return;
    }

    if (m_state == stWaitLatLonInfo)
    {
        if (ch == ',')
        {
            // clear tmp buffer
            memset(m_tmpBuf, 0, MAX_TMP_BUFFER);
            m_pos = 0;
            m_numCommas = 0;
            m_state = stGrabLatLonInfo;
        }
        else
        {
            // should never happen
            m_state = stWaitLine;
            return;
        }
    }
    else if (m_state == stWaitStatusChar)
    {
        if (ch == ',')
        {
            m_numCommas++;
            if (m_numCommas == 2)
            {
                m_state = stGrabStatusChar;
                m_pos = 0;
            }
        }
    }
    else if (m_state == stGrabStatusChar)
    {
        // expect active record only
        if (ch == 'A')
        {
            decodeLatLon();
            m_state = stWaitLine;
        }
        else
        {
            // unexpected
            invalidate(); // mark already catched data as no more valid
            m_state = stWaitLine;
            return;
        }
    }
    else if (m_state == stGrabTime)
    {
        // skip all chars just wait comma
        if (ch == ',')
        {
            m_state = stGrabStatusChar;
        }
    }
    else if (m_state == stGrabLatLonInfo)
    {
        // fill m_tmpBuf by incoming chars

        m_tmpBuf[m_pos] = ch;
        m_pos++;
        if (m_pos >= MAX_TMP_BUFFER)
        {
            // should never happen
            m_state = stWaitLine;
            return;
        }

        if (ch == ',')
        {
            m_numCommas++;
        }

        if (m_numCommas == 4)
        {
            m_state = stGrabTime;
            return;
        }
    }
}
