#include <stdlib.h>
#include <Arduino.h>
#include "headers/charger.h"
#include "headers/sensors.h"

Sensors sensors;
Charger charger;
ChargingProfile profiles[2] = {ChargingProfile(ChargingMethod::constantCurrent, 6000, 1500, 50, 60000, 1500, 100, 0, 0, 0, 0, 0, 0), 
                               ChargingProfile(ChargingMethod::constantCurrent, 6000, 1500, 50, 3600000, 1500, 200, 0, 0, 0, 0, 0, 0)};
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