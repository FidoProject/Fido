/****************************************************************************
 * SparkFun_TB6612_Edison.cpp
 * Implementation of class for TB6612 H-bridge chip
 * Mike Hord @ SparkFun Electronics
 * 10 July 2015
 * https://github.com/sparkfun/SparkFun_H-Bridge_Block_for_Edison_CPP_Library
 *
 * This library defines a simple class for driving the TB6612 H-Bridge chip
 *  used on the H-Bridge Block for Edison from SparkFun Electronics. It 
 *  provides a front end to the MRAA functions from Intel, encapsulating the
 *  various GPIO settings needed to control the TB6612.
 *
 * Development environment specifics:
 *  Code developed in Intel's Eclipse IOT-DK
 *  This code requires the Intel mraa library to function; for more
 *  information see https://github.com/intel-iot-devkit/mraa
 *
 * This code is beerware; if you see me (or any other SparkFun employee) at the
 * local, and you've found our code helpful, please buy us a round!
 * ****************************************************************************/ 

#include "mraa.hpp"
#include <unistd.h>

#include "SparkFun_TB6612_Edison.h"

//using namespace mraa;

tb6612::tb6612():
  _dcA(0.0), _dcB(0.0)
{
  
  // _pwmA is pwm channel 0, on pin 20 in mraa
  _pwmA = mraa_pwm_init(20);
  mraa_pwm_period_us(_pwmA, 1000);
  mraa_pwm_enable(_pwmA, 1);
 
  // _pwmB is pwm channel 1, on pin 14 in mraa
  _pwmB = mraa_pwm_init(14);
  mraa_pwm_period_us(_pwmB, 1000);
  mraa_pwm_enable(_pwmB, 1);

  mraa_pwm_write(_pwmA, 0.01);
  mraa_pwm_write(_pwmB, 0.01);
  mraa_pwm_write(_pwmA, 0.0);
  mraa_pwm_write(_pwmB, 0.0);

  // _A1 and _A2 are on GPIO48 and GPIO47, respectively, which are pins 33 and
  //  46 in mraa, respectively.
  _A1 = mraa_gpio_init(33);
  _A2 = mraa_gpio_init(46);
  mraa_gpio_dir(_A1, MRAA_GPIO_OUT);
  mraa_gpio_mode(_A1, MRAA_GPIO_STRONG);
  mraa_gpio_write(_A1, 1);
  mraa_gpio_dir(_A2, MRAA_GPIO_OUT);
  mraa_gpio_mode(_A2, MRAA_GPIO_STRONG);
  mraa_gpio_write(_A2, 1);

  // _B1 and _B2 are on GPIO15 and GPIO14, respectively, which are pins 48 and
  //  36, respectively
  _B1 = mraa_gpio_init(48);
  _B2 = mraa_gpio_init(36);
  mraa_gpio_dir(_B1, MRAA_GPIO_OUT);
  mraa_gpio_mode(_B1, MRAA_GPIO_STRONG);
  mraa_gpio_write(_B1, 1);
  mraa_gpio_dir(_B2, MRAA_GPIO_OUT);
  mraa_gpio_mode(_B2, MRAA_GPIO_STRONG);
  mraa_gpio_write(_B2, 1);

  // _standbyPin is on GPIO49, which is pin 47 in mraa
  _standbyPin = mraa_gpio_init(47);
  mraa_gpio_dir(_standbyPin, MRAA_GPIO_OUT);
  mraa_gpio_mode(_standbyPin, MRAA_GPIO_STRONG);
  mraa_gpio_write(_standbyPin, 1);
}

void tb6612::diffDrive(float dcA, float dcB)
{
  _dcA=dcA;
  _dcB=dcB;
  if (dcA < 0)
  {
    revA();
    dcA *= -1;
  }
  else
  {
    fwdA();
  }
  if (dcB < 0)
  {
    revB();
    dcB *= -1;
  }
  else
  {
    fwdB();
  }
  mraa_pwm_write(_pwmA, dcA);
  mraa_pwm_write(_pwmB, dcB);
}

void tb6612::standby(bool disableMotors)
{
  if (disableMotors)
  {
    mraa_gpio_write(_standbyPin, 0);
  }
  else
  {
    mraa_gpio_write(_standbyPin, 1);
  }
}

void tb6612::shortBrake(bool brakeA, bool brakeB)
{
  if (brakeA)
  {
    mraa_gpio_write(_A1, 1);
    mraa_gpio_write(_A2, 1);
  }
  if (brakeB)
  {
    mraa_gpio_write(_B1, 1);
    mraa_gpio_write(_B2, 1);
  }
}

bool tb6612::getStandby()
{
  if (mraa_gpio_read(_standbyPin) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void tb6612::getDiffDrive(float* dcA, float* dcB)
{
  *dcA = _dcA;
  *dcB = _dcB;
}

void tb6612::getShortBrake(bool* brakeA, bool* brakeB)
{
  if ( (mraa_gpio_read(_A1) == 1) && (mraa_gpio_read(_A2) == 1) )
  {
    *brakeA = true;
  }
  else
  {
    *brakeA = false;
  }
  if ( (mraa_gpio_read(_B1) == 1) && (mraa_gpio_read(_B2) == 1) )
  {
    *brakeB = true;
  }
  else
  {
    *brakeB = false;
  }
}

void tb6612::fwdA()
{
  mraa_gpio_write(_A1, 0);
  mraa_gpio_write(_A2, 1);
}

void tb6612::revA()
{
  mraa_gpio_write(_A1, 1);
  mraa_gpio_write(_A2, 0);
}

void tb6612::fwdB()
{
  mraa_gpio_write(_B1, 0);
  mraa_gpio_write(_B2, 1);
}

void tb6612::revB()
{
  mraa_gpio_write(_B1, 1);
  mraa_gpio_write(_B2, 0);
}


