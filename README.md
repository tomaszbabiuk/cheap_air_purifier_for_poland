# Cheap air purifier solution for Poland
This repository contains everything you need to build a cheap, effective and powerful air purifier. The project targets Poland (as the power of the purifier is controlled by online data from government air quality sensors).

# Getting started
To build the solution follow these steps:
1. Download Visual Studio Code
2. Install PlatformIO extension
3. Open platformio.ini file from this repository
5. Find your sensor id (detailed instruction are in the next paragraph) 
6. Edit main.cpp file, fill the constants for WIFI_SSID, WIFI_PASSWORD and GIOS_SENSOR_ID
7. Rebuild the solution
8. Upload to the NodeMCU Shield
9. Connect the Vent to MotorA connectors of NodeMCU
10. Connect the power (12V recommended) 
11. Enjoy

# Example of finding 'sensor id':
The sensor on the map I'm interested in is: "Stacja osiedle piastów"
After searching through the GIOŚ API sensors list (http://api.gios.gov.pl/pjp-api/rest/station/findAll) I found that the station id is 10139.

To get pollution data from that sensor I need the sensor id (not station id). I can get if from here: http://api.gios.gov.pl/pjp-api/rest/station/sensors/10139.
In my case, the sensor id I'm interested in is 16784.

I can check the sensor data here: http://api.gios.gov.pl/pjp-api/rest/data/getData/16784