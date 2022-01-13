#include <stdlib.h>
#include <Arduino.h>
#include "headers/charger.h"
#include "headers/sensors.h"

Sensors sensors;
Charger charger;
ChargingProfile profiles[2] = {ChargingProfile(ChargingMethod::constantCurrent, 2000, 1500, 35, 1800000, 1680, 500, 60000, 20, 60000, 3, 5, 1), 
                               ChargingProfile(ChargingMethod::constantVoltage, 2000, 1500, 35, 300000, 1680, 100, 0, 0, 0, 0, 0, 0)};
                              //{ChargingProfile(ChargingMethod::constantCurrent, 4300, 1500, 40, 1800000, 4200, 700, 60000, 20, 60000, 3, 5, 1), 
                              //ChargingProfile(ChargingMethod::constantVoltage, 4300, 1500, 40, 300000, 4200, 100, 0, 0, 0, 0, 0, 0)};
Battery battery(2, 1, 2000, profiles);

void setup() {
  analogReference(INTERNAL1V1);
  Serial.begin(115200);
  while(!Serial){}
  Serial.println("Jazdunia!!!");
  charger.addBattery(0,battery);
  charger.charge(0);
}

void loop() 
{
    sensors.getDataFromSensors();
    charger.adjustElectricalComponents();
    
    delay(100);
}