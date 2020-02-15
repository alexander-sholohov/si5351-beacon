#pragma once

//
// Developed by Alexander Sholohov <ra9yer@yahoo.com>
//

class CTimeRange
{
public:
    CTimeRange();
    long millisecondsElapsed() const;
    long secondsElapsed() const;
    void resetToNow();
private:
    unsigned long _stamp;

};
