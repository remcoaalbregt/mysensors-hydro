#define MY_NODE_ID 97

#include <Arduino.h>
#include <config.h>
#include <MySensors.h>
#include <Wire.h>
#include <HTU21D.h>
#include <BH1750.h>
#include <SPI.h>
#include <DallasTemperature.h>
#include <OneWire.h>
// #include <NewPing.h>

unsigned long SLEEP_TIME = 20000; // 120000

HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);
BH1750 lightSensor;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dsSensors(&oneWire);

uint16_t lastlux = -1;
float lastAirTemp = -1;
float lastWaterTemp = -1;
float lastHeatsinkTemp = -1;
float lastHum = -1;

static const uint8_t FORCE_UPDATE_N_READS = 4;
uint8_t nNoUpdatesAirTemp = 0;
uint8_t nNoUpdatesWaterTemp = 0;
uint8_t nNoUpdatesHeatsinkTemp = 0;
uint8_t nNoUpdatesHum = 0;
uint8_t nNoUpdatesLux = 0;

MyMessage msgAirHum(CHILD_ID_HUM, V_HUM);
MyMessage msgAirTemp(CHILD_ID_AIRTEMP, V_TEMP);
MyMessage msgWaterTemp(CHILD_ID_WATERTEMP, V_TEMP);
MyMessage msgLight(CHILD_ID_LUX, V_LEVEL );
MyMessage msgHeatsinkTemp(CHILD_ID_HEATSINK_TEMP, V_TEMP );
// MyMessage msgWaterVolume(CHILD_ID_WATERLEVEL, V_LEVEL);
// MyMessage msgWaterVolumePercent(CHILD_ID_WATERLEVEL_PERCENT, V_LIGHT_LEVEL);

void presentation() { 
  sendSketchInfo(SKETCH_NAME, SKETCH_VERSION);
  wait(50);

  present(CHILD_ID_HUM, S_HUM, "Humidity");
  wait(50);
  present(CHILD_ID_AIRTEMP, S_TEMP, "Air temperature");
  wait(50);
  present(CHILD_ID_WATERTEMP, S_TEMP, "Water temperature");
  wait(50);
  present(CHILD_ID_LUX, S_LIGHT_LEVEL, "Light level");
  wait(50);
  present(CHILD_ID_HEATSINK_TEMP, S_TEMP, "Heatsink temperature");
  wait(50);
  // present(CHILD_ID_WATERLEVEL, V_LEVEL);
  // present(CHILD_ID_WATERLEVEL_PERCENT, V_LEVEL);
}

void before() {
  dsSensors.begin();
}

void setup() { 
//  wait(500); // Allow time for radio if power used as reset

 //lightSensor.begin();
 dsSensors.setWaitForConversion(false);

  //while (myHTU21D.begin() != true) {
  //  Serial.println(F("HTU21D, SHT21 sensor is failed or not connected"));
  //  wait(2000);
  //}
}

void loop() {  
  wait(500); // Allow time for radio if power used as reset
  
  //sendAirTemperature();
  //wait(50);
  //sendHumidity();
  //wait(50);
  sendWaterTemperature();
  wait(50);
  sendHeatsinkTemperature();
  wait(50);
  //// sendWaterLevel();
  //// wait(10);
  //sendLuxLevel();
  //wait(50);

  #ifdef MY_DEBUG
    Serial.println();
  #endif

  sleep(SLEEP_TIME);
}

bool isValidReading(float value) {
  return value != 255.00;
}

bool isValidTemperatureReading(float temperature) {
  return isValidReading(temperature) && temperature != -127.00 && temperature != 85.00;
}

void sendAirTemperature() {
  nNoUpdatesAirTemp++;

  float temperature = myHTU21D.readTemperature();

  #ifdef MY_DEBUG
    Serial.print("Air temp: ");
    Serial.println(temperature);
  #endif

  if (temperature != lastAirTemp || nNoUpdatesAirTemp == FORCE_UPDATE_N_READS) {
    if(isValidTemperatureReading(temperature)) {
      nNoUpdatesAirTemp = 0;
      lastAirTemp = temperature;
      
      send(msgAirTemp.set(temperature, 1));
    }
  }
}

void sendHumidity() {
  nNoUpdatesHum++;

  float humidity = myHTU21D.readCompensatedHumidity();

  #ifdef MY_DEBUG
    Serial.print("Humidity: ");
    Serial.println(humidity);
  #endif

  if (humidity != lastHum || nNoUpdatesHum == FORCE_UPDATE_N_READS) {
    if(isValidReading(humidity)) {
      nNoUpdatesHum = 0;
      lastHum = humidity;
      
      send(msgAirHum.set(humidity, 1));
    }
  }
}

void sendWaterTemperature() {
  nNoUpdatesWaterTemp++;

  dsSensors.requestTemperatures();
  float temperature = dsSensors.getTempCByIndex(1);

  #ifdef MY_DEBUG
    Serial.print("Water temp: ");
    Serial.println(temperature);
  #endif

  if (temperature != lastWaterTemp || nNoUpdatesWaterTemp == FORCE_UPDATE_N_READS) {
    if(isValidTemperatureReading(temperature)) {
      nNoUpdatesWaterTemp = 0;
      lastWaterTemp = temperature;

      send(msgAirTemp.set(temperature, 1));
    }
  }
}

void sendHeatsinkTemperature() {
  nNoUpdatesHeatsinkTemp++;

  dsSensors.requestTemperatures();
  float temperature = dsSensors.getTempCByIndex(0);

  #ifdef MY_DEBUG
    Serial.print("Heatsink temp: ");
    Serial.println(temperature);
  #endif

  if (temperature != lastHeatsinkTemp || nNoUpdatesHeatsinkTemp == FORCE_UPDATE_N_READS) {
    if(isValidTemperatureReading(temperature)) {
      nNoUpdatesHeatsinkTemp = 0;
      lastHeatsinkTemp = temperature;

      send(msgHeatsinkTemp.set(temperature, 1));
    }
  }
}

// void sendWaterLevel() {
//   long pingHeight = getPingHeight();
// }

void sendLuxLevel() {
  nNoUpdatesLux++;

  uint16_t lux = lightSensor.readLightLevel();
  
  #ifdef MY_DEBUG
    Serial.print("Lux: ");
    Serial.println(lux);
  #endif

  if (lux != lastlux || nNoUpdatesLux == FORCE_UPDATE_N_READS) {
    nNoUpdatesLux = 0;
    lastlux = lux;

    send(msgLight.set(lux));
  }
}

// int getWaterVolume() {

// }

// byte getWaterLevel() {
  
// }

// long getPingHeight() {
//   long pingHeight;
//   pingHeight = sonar.ping_cm();             //- Do multiple (5) pings and return median

//   #ifdef MY_DEBUG
//     Serial.print("Ping Height raw in cm: ");
//     Serial.println(pingHeight);
//   #endif

//   return pingHeight;
// }