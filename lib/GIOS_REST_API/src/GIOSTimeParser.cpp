/*
    GIOSTimeParser.h - Library for Parsing GMT time. Returned format is gios_time.
    Author - Tomasz Babiuk
*/

#include "GIOSTimeParser.h"

unsigned short GIOSTimeParser::asciiPairToNumber(char chFirst, char chSecond) {
    return (chFirst - 48)*10 + (chSecond - 48);
}

unsigned short GIOSTimeParser::extractHoursFromGMTString(const char* s, unsigned short length) {
    return asciiPairToNumber(s[length - 12], s[length - 11]);
}

unsigned short GIOSTimeParser::extractMinutesFromGMTString(const char* s, unsigned short length) {
    return asciiPairToNumber(s[length - 9], s[length - 8]);
}

unsigned short GIOSTimeParser::extractSecondsFromGMTString(const char* s, unsigned short length) {
    return asciiPairToNumber(s[length - 6], s[length - 5]);
}

gios_time GIOSTimeParser::fromGMTString(const char* s, unsigned short length) {
    gios_time time;
    if (length > 28) {
        time.h = extractHoursFromGMTString(s, length);
        time.m = extractMinutesFromGMTString(s, length);
        time.s = extractMinutesFromGMTString(s, length);
    } else {
        time.invalidFormat = true;
    }
    return time;
}