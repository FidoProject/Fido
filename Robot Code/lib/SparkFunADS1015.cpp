/****************************************************************
Core code file for SparkFun ADC Edison Block Support

1 Jun 2015- Mike Hord, SparkFun Electronics
Code developed in Intel's Eclipse IOT-DK

This code requires the Intel mraa library to function; for more
information see https://github.com/intel-iot-devkit/mraa

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.
****************************************************************/

#include "SparkFunADS1015.h"
#include "mraa.hpp"
#include <unistd.h>

// The default constructor.
ads1015::ads1015(mraa::I2c* myPort, unsigned short myI2CAddress):
	_myPort(myPort), _myI2CAddress(myI2CAddress), _scaler(1.0)
{
	_myPort->address(myI2CAddress);
	setRange(_2_048V); // We don't *know* what this value is, since it could
	                   //  have been written on a prior run of this code.
	                   //  Set a known value.
}

// The _scaler variable holds a floating point value representing the number of
//  millivolts per LSB. At its coarsest the ADS1015 is reporting 3mV per bit,
//  and at its finest, 0.125mV (125uV).
float ads1015::getScaler()
{
	return _scaler;
}

// Returns the current reading on a channel, scaled by the current scaler and
//  presented as a floating point number.
float ads1015::getResult(uint8_t channel)
{
  int16_t rawVal = getRawResult(channel);
  return (float)rawVal * _scaler/1000;
}

// Returns the current reading on a differential pair, scaled by the current
//  scaler and presented as a floating point number. Channel options are
//  0 - ch 0 - ch 1
//  1 - ch 0 - ch 3
//  2 - ch 1 - ch 3
//  3 - ch 2 - ch 3
float ads1015::getDiffResult(uint8_t channel)
{
	int16_t rawVal = getRawDiffResult(channel);
	return (float)rawVal * _scaler/1000;
}

// Raw results are signed 12-bit values, i.e., numbers between -2048 and
//  2047. This function reports the differential result.
int16_t ads1015::getRawDiffResult(uint8_t channel)
{
	uint16_t cfgRegVal = getConfigRegister();

	cfgRegVal &= ~CHANNEL_MASK; // clear existing channel settings
	cfgRegVal &= ~SINGLE_ENDED; // clear the SE bit for a diff read
	cfgRegVal |= (channel<<CHANNEL_SHIFT) & CHANNEL_MASK; // put the channel bits in
	cfgRegVal |= START_READ; // set the start read bit

	setConfigRegister(cfgRegVal);

	return readADC();
}

// Single ended raw version. Single-ended results are effectively unsigned 11-bit
//  values, from 0 to 2047.
int16_t ads1015::getRawResult(uint8_t channel)
{
	uint16_t cfgRegVal = getConfigRegister();

	cfgRegVal &= ~CHANNEL_MASK; // clear existing channel settings
	cfgRegVal |= SINGLE_ENDED;  // set the SE bit for a s-e read
	cfgRegVal |= (channel<<CHANNEL_SHIFT) & CHANNEL_MASK; // put the channel bits in
	cfgRegVal |= START_READ;    // set the start read bit

	setConfigRegister(cfgRegVal);

	return readADC();
}

// This handles the actual read of the ADC- starting the conversion and waiting
//  for it to complete. It returns an invalid number (-32768) if the ADC didn't
//  respond in a timely manner (i.e., within 10ms, minimum). The highest
//  sampling rate available is 3300 sps, so we expect to have an answer within
//  300 us or so.
int16_t ads1015::readADC()
{

	uint16_t cfgRegVal = getConfigRegister();
	cfgRegVal |= START_READ; // set the start read bit
	setConfigRegister(cfgRegVal);

	uint8_t result[2];
	int16_t fullValue=0;
	uint8_t busyDelay = 0;

	while ((getConfigRegister() & BUSY_MASK) == 0)
	{
	  usleep(100);
	  if(busyDelay++ > 100) return 0xffff;
	}

	_myPort->readBytesReg(CONVERSION, result, 2);

	fullValue = (result[0]<<8) + result[1];
	return int(fullValue>>4);
}

// Sets the voltage range extent. There are 6 settable ranges:
//  _0_256V - Range is -0.256V to 0.255875V, and step size is 125uV.
//  _0_512V - Range is -0.512V to 0.51175V, and step size is 250uV.
//  _1_024V - Range is -1.024V to 1.0235V, and step size is 500uV.
//  _2_048V - Range is -2.048V to 2.047V, and step size is 1mV.
//  _4_096V - Range is -4.096V to 4.094V, and step size is 2mV.
//  _6_144V - Range is -6.144V to 6.141V, and step size is 3mV.
// The default setting is _2_048V.
// NB!!! Just because FS reading is > 3.3V doesn't mean you can take an
//  input above 3.3V! Keep your input voltages below 3.3V to avoid damage!
void ads1015::setRange(VoltageRange range)
{
  uint16_t cfgRegVal = getConfigRegister();
  cfgRegVal &= ~RANGE_MASK;
  cfgRegVal |= (range << RANGE_SHIFT) & RANGE_MASK;
  setConfigRegister(cfgRegVal);
  switch (range)
  {
  case _6_144V:
	  _scaler = 3.0; // each count represents 3.0 mV
	  break;
  case _4_096V:
	  _scaler = 2.0; // each count represents 2.0 mV
	  break;
  case _2_048V:
  	  _scaler = 1.0; // each count represents 1.0 mV
  	  break;
  case _1_024V:
	  _scaler = 0.5; // each count represents 0.5mV
	  break;
  case _0_512V:
	  _scaler = 0.25; // each count represents 0.25mV
	  break;
  case _0_256V:
	  _scaler = 0.125; // each count represents 0.125mV
	  break;
  default:
	  _scaler = 1.0;  // here be dragons
	  break;
  }
}

// Config register read/write. I'm leaving these public against my better
//  judgement, but be careful! You can really screw stuff up here.

// Write some 16-bit value to the config register. See the datasheet for
//  information about what these bits actually do.
void ads1015::setConfigRegister(uint16_t configValue)
{
	uint8_t data[3];
	data[0] = 0x01;  // address of the config register
	data[1] = configValue>>8;
	data[2] = configValue;
	_myPort->write(data,3);
}

// Get the config register.
uint16_t ads1015::getConfigRegister()
{
	uint16_t cfgRegVal = 0;
	_myPort->readBytesReg(CONFIG, (uint8_t*)&cfgRegVal, 2);
	cfgRegVal = (cfgRegVal>>8) | (cfgRegVal<<8);
	return cfgRegVal;
}
