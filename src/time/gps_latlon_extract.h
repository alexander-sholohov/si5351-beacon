#pragma once
//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//


class Stream;

class GpsLatLonExtract
{
    enum State {
        stWaitLine,
        stWaitStatusChar,
        stGrabStatusChar,
        stWaitLatLonInfo,
        stGrabLatLonInfo,
        stGrabTime,
    };

public:
    GpsLatLonExtract();
    bool isValidLatLonPresent() const { return m_validLatLonPresent; }
    const char *calcMaidenheadLocator(unsigned numChars);
    void onCharReceived(char ch);

private:
    State m_state;
    int m_numCommas;
    int m_pos;
    bool m_validLatLonPresent;
    char m_rmc[6];
    enum {
        MAX_TMP_BUFFER = 30,
        MAX_FRAC_CHARS = 5,
    };
    char m_tmpBuf[MAX_TMP_BUFFER]; // store string like "4916.45000,N,12311.12000,W,"

    // ready to use data
    char m_latDegree[2]; // for latitude 2 chars +-90
    char m_latMinutesInt[2]; // 00-59
    char m_latMinutesFrac[MAX_FRAC_CHARS]; // 00000-99999
    char m_northORSouth; // N/S

    char m_lonDegree[3]; // for longitude 3 chars +-180
    char m_lonMinutesInt[2]; // 00-59
    char m_lonMinutesFrac[MAX_FRAC_CHARS]; // 00000-99999
    char m_westOREast; // W/E

    char m_maidenheadLocator[11]; // AA00aa00aa + \0

    void decodeLatLon();


};
