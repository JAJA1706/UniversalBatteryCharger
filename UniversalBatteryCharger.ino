#include <stdlib.h>
#include <Arduino.h>
#include "headers/charger.h"
#include "headers/sensors.h"

// const int pwmPin = 4;
// const int pwmPin2 = 13;
// int outputValue = 86; //tranzystor
// int outputValue2 = 50; //regulator
Sensors sensors;
Charger charger;
Battery balz;

void setup() {
  analogReference(INTERNAL1V1);
  Serial.begin(115200);
  while(!Serial){}
}

void loop() 
{
    //analogWrite(pwmPin, outputValue);
    //analogWrite(pwmPin2, outputValue2);
    sensors.getDataFromSensors();
    charger.adjustElectricalComponents();
    // Serial.println(Sensors::voltage1);
    // Serial.println(Sensors::voltage2);
    // Serial.println(Sensors::voltage3);
    
    //delay(5000);
}

//constexpr unsigned long nice(const unsigned startTime)
//{
//    unsigned long currentTime = millis();
//    if(currentTime > startTime) //timer overflow protection
//        return currentTime - startTime;
//    else
//       return currentTime + UINT32_MAX - startTime;
//}
