/****************************************************************
Core header file for SparkFun ADC Edison Block Support

1 Jun 2015- Mike Hord, SparkFun Electronics
Code developed in Intel's Eclipse IOT-DK

This code requires the Intel mraa library to function; for more
information see https://github.com/intel-iot-devkit/mraa

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.
****************************************************************/

#ifndef __ads1015_h__
#define __ads1015_h__

#include "mraa.hpp"

// Defines for all the registers and bits. Not that many on this part.

#define CONVERSION 0      // Conversion result register.
#define CONFIG     1      // 16-bit configuration register.
#define THRESHL    2      // Low threshold setting. Not used (yet).
#define THRESHH    3      // High threshold setting. Not used (yet).

// Channel selection and read start stuff- the high nibble of the 16-bit cfg
//  register controls the start of a single conversion, the channel(s) read,
//  and whether they're read single ended or differential.
#define CHANNEL_MASK 0x3000 // There are four channels, and single ended reads
                          //  are specified by a two-bit address at bits 13:12
#define SINGLE_ENDED  0x4000   // Set for single-ended
#define START_READ   0x8000 // To start a read, we set the highest bit of the
						  //  highest nibble.
#define CFG_REG_CHL_MASK 0xf000 // Used to clear the high nibble of the cfg reg
                          //  before we start our read request.
#define BUSY_MASK     0x8000 // When the highest bit in the cfg reg is set, the
                          //  conversion is done.
#define CHANNEL_SHIFT 12  // shift the raw channel # by this

// PGA settings and stuff. These are bits 11:9 of the cfg reg
enum VoltageRange : uint8_t { _6_144V = 0, _4_096V, _2_048V, _1_024V, _0_512V,
	                _0_256V, VOLTAGE_MASK = 0x07};
#define RANGE_SHIFT 9  // how far to shift our prepared data to drop it into the
                       //  right spot in the cfg reg
#define RANGE_MASK 0x0E00 // bits to clear for gain parameter

class ads1015
{
  public:
    ads1015(mraa::I2c* myPort, unsigned short myI2CAddress);
    ~ads1015(){};
    float getResult(uint8_t channel);
    float getDiffResult(uint8_t channel);
    int16_t getRawResult(uint8_t channel);
    int16_t getRawDiffResult(uint8_t channel);
    void setRange(VoltageRange range);
    float getScaler();
    void setConfigRegister(uint16_t configValue);
    uint16_t getConfigRegister();
  private:
    int16_t readADC();
    mraa::I2c* _myPort;
    uint8_t _myI2CAddress;
    float _scaler;
};
#endif
