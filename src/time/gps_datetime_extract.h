#pragma once
//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

class Stream;

class GpsDataExtract
{
    enum State {
        stWaitLine,
        stWaitTime,
        stGrabTime,
        stWaitDate,
        stGrabDate,
    };

public:
    GpsDataExtract();
    bool isDateTimePresent() const;
    void retrieveDateTime(char* buf);
    void onCharReceived(char ch);

private:
    State m_state;
    int m_numCommas;
    int m_pos;
    bool m_isDateTimePresent;
    char m_rmc[6];
    char m_datetime[6+6];

};
