
//
// Developed by Alexander Sholohov <ra9yer@yahoo.com>
//

#include "ad9833.h"

#define AD9833_REF_XTAL_IN_HZ (25000000LL)

#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

#define MYNOP() _NOP()


//------------------------------------------------------------------------------------------
AD9833::AD9833(uint8_t fsync, uint8_t sclk, uint8_t sdata)
    : _fsync(fsync)
    , _sclk(sclk)
    , _sdata(sdata)
{
}


//------------------------------------------------------------------------------------------
void AD9833::initialize()
{
    beginCommunication();
    writeWord(0x2100); // start with reset
    writeWord(0xc000); // set phase to 0
    endCommunication();
}


//------------------------------------------------------------------------------------------
void AD9833::beginCommunication()
{
    digitalWrite(_sclk, HIGH);
    digitalWrite(_fsync, LOW);
}

//------------------------------------------------------------------------------------------
void AD9833::endCommunication()
{
    digitalWrite(_sclk, HIGH);
    digitalWrite(_fsync, HIGH);
}

//------------------------------------------------------------------------------------------
void AD9833::writeWord(uint16_t out)
{
    for(size_t i=0; i<16; i++ )
    {
        if(out&0x8000)
        {
            digitalWrite(_sdata, HIGH);
        }
        else
        {
            digitalWrite(_sdata, LOW);
        }

        digitalWrite(_sclk, LOW);
        MYNOP();
        digitalWrite(_sclk, HIGH);
        out <<= 1;
    }
}

//------------------------------------------------------------------------------------------
void AD9833::setFrequencyInHZx100(uint32_t freqX100)
{
    uint64_t ref = AD9833_REF_XTAL_IN_HZ * 100;
    uint64_t twoPow28 = 0x10000000LL;
    uint64_t freq = (uint64_t)freqX100 * twoPow28 / ref;

    uint32_t lsb14b = freq & 0x3fff;
    uint32_t msb14b = (freq >> 14) & 0x3fff;

    // set bits 15,14 to 01
    lsb14b |= 0x4000;
    msb14b |= 0x4000;

    beginCommunication();
    writeWord(0x2000); // start. enable output if it was disabled
    writeWord(lsb14b);
    writeWord(msb14b);
    endCommunication();

}

//------------------------------------------------------------------------------------------
void AD9833::enableOutput()
{
    beginCommunication();
    writeWord(0x2000); // turn reset OFF
    endCommunication();
}

//------------------------------------------------------------------------------------------
void AD9833::disableOutput()
{
    beginCommunication();
    writeWord(0x2100); // turn reset ON
    endCommunication();
}
