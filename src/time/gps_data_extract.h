#pragma once

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
    GpsDataExtract(Stream&);
    void doWork();
    bool isDateTimePresent() const;
    void retrieveDateTime(char* buf);
private:
    Stream& m_uart;
    State m_state;
    int m_numCommas;
    int m_pos;
    bool m_isDateTimePresent;
    char m_rmc[6];
    char m_datetime[6+6];

    void onCharReceived(char ch);
};
