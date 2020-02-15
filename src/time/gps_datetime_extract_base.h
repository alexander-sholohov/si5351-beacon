#pragma once
//
// Author: Alexander Sholohov <ra9yer@yahoo.com>
//
// License: MIT
//

class GpsDatetimeExtractBase
{
public:
    GpsDatetimeExtractBase();
    bool isDateTimePresent() const;
    const char* getDateTimeAsShortString() const;

    virtual void onCharReceived(char ch) = 0;

    bool isEventPresent() const;
    void resetEvent();

protected:
    void invalidate();
    void putDateTime(char * datetime);

private:
    bool m_isDateTimePresent;
    bool m_isEventPresent;
    char m_datetime[6 + 6 + 1];

};
