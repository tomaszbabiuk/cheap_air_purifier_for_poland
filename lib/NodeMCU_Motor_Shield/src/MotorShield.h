/*
  MotorShield.h - Library for controlling NodeMCU MotorShield
  Author - Tomasz Babiuk
*/
#ifndef MotorShield_h
#define MotorShield_h

#include <Arduino.h>

class MotorShield
{
  public:
    static void init();
    static void driveMotorA(bool clockwise, int power);
    static void driveMotorB(bool clockwise, int power);

  private:
};

#endif