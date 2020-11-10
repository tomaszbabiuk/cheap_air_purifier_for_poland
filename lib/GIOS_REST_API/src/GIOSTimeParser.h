/*
    GIOSTimeParser.h - Library for Parsing GMT time. Returned format is gios_time.
    Author - Tomasz Babiuk
*/

#ifndef GIOSTimeParser_h
#define GIOSTimeParser_h

struct gios_time {
    short h = 0;
    short m = 0;
    short s = 0;
    bool invalidFormat = false;

    int inSeconds() {
      return h* 3600 + m * 60 + s;
    }
};

class GIOSTimeParser
{
  public:
    static gios_time fromGMTString(const char* s, unsigned short length);

  private:
    static unsigned short asciiPairToNumber(char chFirst, char chSecond);
    static unsigned short extractHoursFromGMTString(const char* s, unsigned short length);
    static unsigned short extractMinutesFromGMTString(const char* s, unsigned short length);
    static unsigned short extractSecondsFromGMTString(const char* s, unsigned short length);
};

#endif