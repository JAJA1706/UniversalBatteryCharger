#include <stdlib.h>
#include <Arduino.h>
#include <Wire.h>
#include "headers/charger.h"
#include "headers/sensors.h"
#include "headers/batteryBuilder.h"

Sensors sensors;
Charger charger;

void setup() {
  analogReference(INTERNAL1V1);
  Serial.begin(115200);
  while(!Serial){}
  Serial.println("Jazdunia!!!");
  builder.setLinkedCharger(&charger);
  Wire.onRequest(Charger::onBatteryDataRequest);
}

void loop() 
{
    sensors.getDataFromSensors();
    charger.adjustElectricalComponents();
    delay(100);
}