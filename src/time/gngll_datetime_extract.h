#pragma once
//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "gps_datetime_extract_base.h"

class GNGLLDatetimeExtract : public GpsDatetimeExtractBase
{
public:
    GNGLLDatetimeExtract();
    void onCharReceived(char ch) override;

private:
    enum State {
        stWaitLine,
        stWaitTime,
        stGrabTime,
    };

    State m_state;
    int m_numCommas;
    int m_pos;
    char m_rmc[6];
    char m_tmpDatetime[6 + 6];
};
