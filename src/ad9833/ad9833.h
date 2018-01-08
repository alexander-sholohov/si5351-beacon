#pragma once

//
// Developed by Alexander Sholohov <ra9yer@yahoo.com>
//

#include <Arduino.h>

class AD9833
{

public:
    AD9833(uint8_t fsync, uint8_t sclk, uint8_t sdata);

    void initialize();
    void setFrequencyInHZx100(uint32_t freqX100);
    void enableOutput();
    void disableOutput();


private:
    uint8_t _fsync;
    uint8_t _sclk;
    uint8_t _sdata;

    void beginCommunication();
    void endCommunication();
    void writeWord(uint16_t out);

};
