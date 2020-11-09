/*
  GIOSRestApi.h - Library for getting sensor values from GIOS REST API. See details here: http://powietrze.gios.gov.pl/pjp/content/api
  Author - Tomasz Babiuk
*/
#ifndef GIOSRestApi_h
#define GIOSRestApi_h

#include <Arduino.h>

#define GIOS_ERROR_CANNOT_CONNECT    -1.00
#define GIOS_ERROR_NON_200_HTTP_CODE -2.00
#define GIOS_ERROR_PARSING_EXCEPTION -3.00

class GIOSRestApi
{
  public:
    static double getSensorData(int sensorId);

  private:
};

#endif