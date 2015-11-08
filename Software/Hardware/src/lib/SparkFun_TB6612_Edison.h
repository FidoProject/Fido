/****************************************************************************
 * SparkFun_TB6612_Edison.h
 * Header file for TB6612 motor driver library
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

#ifndef __sparkfun_tb6612_edison_h_
#define __sparkfun_tb6612_edison_h_

#include "mraa.hpp"

class tb6612
{
  public:
    // No parameters need to be passed to the constructor.
    tb6612();

    // The destructor is empty.
    ~tb6612(){};  

    // General purpose drive function. Values for dcA and dcB should be
    //  between -1.0 and 1.0 inclusive.
    void diffDrive(float dcA, float dcB);

    // standby differs from brake in that the outputs are hi-z instead of
    //  shorted together. After the constructor is called, the motors are in
    //  standby.
    void standby(bool disableMotors);

    // shortBrake shorts the motor leads together to drag the motor to a halt
    //  as quickly as possible.
    void shortBrake(bool brakeA, bool brakeB);

    // checks the standby gpio pin and returns true or false depending on that
    //  pin's current state.
    bool getStandby();

    // getDiffDrive relies on the recorded value of the drive values; it does
    //  not retrieve them from the OS. Thus, if an external process has
    //  modified them, these values may be wrong.
    void getDiffDrive(float* dcA, float* dcB);

    // getShortBrake checks the pin values, and so provides an accurate
    //  accounting of the current status.
    void getShortBrake(bool* brakeA, bool* brakeB);

  private:
    // the fwd/rev private members set the pins controlling the TB6612 to the
    //  appropriate state for forward or reverse drive.
    void fwdA();
    void revA();
    void fwdB();
    void revB();

    // We record the values for drive output for later reporting.
    float _dcA, _dcB;

    // These structs hold the information regarding the particular gpio or pwm
    //  output we're interfacing with.
    mraa_pwm_context _pwmA;
    mraa_pwm_context _pwmB;
    mraa_gpio_context _A1;
    mraa_gpio_context _A2;
    mraa_gpio_context _B1;
    mraa_gpio_context _B2;
    mraa_gpio_context _standbyPin;
};

#endif

