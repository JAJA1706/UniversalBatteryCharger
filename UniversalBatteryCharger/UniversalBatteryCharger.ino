#include <stdlib.h>
#include <Arduino.h>
#include <Wire.h>
#include "headers/charger.h"
#include "headers/batteryBuilder.h"

Charger charger;

void setup() {
  analogReference(INTERNAL1V1);
  Serial.begin(115200);
  while(!Serial){}
  Serial.println("Witamy w uniwersalnym systemie do ładowania akumulatorów!");

  builder.setLinkedCharger(&charger);
  Wire.onRequest(Charger::onBatteryDataRequest);
}

void loop() 
{
    charger.getSensorData();
    charger.adjustElectricalComponents();

    if( builder.isNewBatteryReady())
    {
      builder.putBatteryInLinkedCharger();
    }

    delay(100);
}