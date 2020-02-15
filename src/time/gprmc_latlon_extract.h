#pragma once
//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

#include "gps_latlon_extract_base.h"


class GPRMCLatLonExtract : public GpsLatLonExtractBase
{
public:
    GPRMCLatLonExtract();

    void onCharReceived(char ch) override;
    // void onCharReceived(char ch);

private:
    enum State {
        stWaitLine,
        stWaitStatusChar,
        stGrabStatusChar,
        stWaitLatLonInfo,
        stGrabLatLonInfo,
        stGrabTime,
    };

    State m_state;
    int m_numCommas;
    int m_pos;
    char m_rmc[6];
};
