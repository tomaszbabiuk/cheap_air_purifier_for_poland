/*
  MotorShield.cpp - Library for controlling NodeMCU MotorShield
  Author - Tomasz Babiuk
*/

#include <Arduino.h>
#include "MotorShield.h"

int MOTOR_A_DIRECTION_PIN = 0;
int MOTOR_A_CONTROL_PIN = 5;
int MOTOR_B_DIRECTION_PIN = 2;
int MOTOR_B_CONTROL_PIN = 4;

void MotorShield::init() {
    pinMode(MOTOR_A_DIRECTION_PIN, OUTPUT);
    pinMode(MOTOR_A_CONTROL_PIN, OUTPUT);
    pinMode(MOTOR_B_DIRECTION_PIN, OUTPUT);
    pinMode(MOTOR_B_CONTROL_PIN, OUTPUT);
    analogWriteFreq(32000);
}

void MotorShield::driveMotorA(bool clockwise, int power) {
    Serial.printf("Driving motor A: %d.%d\n", clockwise, power);
    digitalWrite(MOTOR_A_DIRECTION_PIN, !clockwise);
    analogWrite(MOTOR_A_CONTROL_PIN, power);
}

void MotorShield::driveMotorB(bool clockwise, int power) {
    digitalWrite(MOTOR_B_DIRECTION_PIN, !clockwise);
    analogWrite(MOTOR_B_CONTROL_PIN, power);
}