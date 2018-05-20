#ifndef __CONFIG_H__
#define __CONFIG_H__

// Enable debug prints
// #define MY_DEBUG

#define MY_RADIO_NRF24

#define CHILD_ID_HUM 0
#define CHILD_ID_AIRTEMP 1
#define CHILD_ID_WATERTEMP 2
#define CHILD_ID_WATERLEVEL 3
#define CHILD_ID_WATERLEVEL_PERCENT 4
#define CHILD_ID_LUX 5
#define CHILD_ID_HEATSINK_TEMP 6

#define SKETCH_NAME "Hydroponics"
#define SKETCH_VERSION "1.2.1"

#define SONAR_TRIGGER_PIN 11
#define SONAR_ECHO_PIN 12
#define SONAR_MAX_DISTANCE 200 // 14

#define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 3 // Pin where dallase sensor is connected 

#endif