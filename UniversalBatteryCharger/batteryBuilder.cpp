#include "headers/batteryBuilder.h"
#include <ArduinoJson.h>
#include <Wire.h>

String BatteryBuilder::I2Cbuffer;
String BatteryBuilder::I2CString;
BatteryBuilder builder;
StaticJsonDocument<1024> doc;

BatteryBuilder::BatteryBuilder() : profilesIterator(0), buildingProfiles(true), batteryCanal(0)
{
  I2Cbuffer.reserve(1024);
  I2CString.reserve(1024);
  I2Cbuffer = "";
  I2CString = "";
  Wire.begin(9);
  Wire.onReceive( BatteryBuilder::onReceiveString );
}

void BatteryBuilder::setLinkedCharger(Charger* chargerPtr){
    charger = chargerPtr;
}

void BatteryBuilder::putBatteryInLinkedCharger(){
    charger->addBattery(batteryCanal, newBattery);

    if(newMode == BatteryMode::Charge)
        charger->charge(batteryCanal);
    else if(newMode == BatteryMode::Discharge)
        charger->discharge(batteryCanal);

    newBatteryReady = false;
}

void BatteryBuilder::buildBattery(const String& batteryJSON){
    deserializeJson(doc, batteryJSON);
    double capacity;
    double minVoltage;

    String batteryMode = doc["BatteryMode"];
    if(batteryMode == "laduj")
        newMode = BatteryMode::Charge;
    else if(batteryMode == "rozladuj")
        newMode = BatteryMode::Discharge;
    else
        newMode = BatteryMode::Wait;
    

    capacity = doc["Capacity"];
    minVoltage = doc["MinVoltage"];
    Battery battery(profilesIterator,minVoltage,capacity,buildedBatteryProfiles);
    newBattery = battery;
    newBatteryReady = true;

    profilesIterator = 0;
    buildingProfiles = true;
}

void BatteryBuilder::buildBatteryProfile(const String& profileJSON){
    ChargingProfile profile;
    deserializeJson(doc, profileJSON);
    String method = doc["method"];
    if(method == "stale napiecie")
        profile.method = ChargingMethod::constantVoltage;
    else
        profile.method = ChargingMethod::constantCurrent;
    
    profile.maxVoltage = doc["maxVoltage"];
    profile.maxCurrent = doc["maxCurrent"];
    profile.maxTemperature = doc["maxTemperature"];
    profile.maxTime = doc["maxTime"];
    profile.desiredVoltage = doc["desiredVoltage"];
    profile.desiredCurrent = doc["desiredCurrent"];
    profile.voltageDeltaInterval = doc["voltageDeltaInverval"];
    profile.minVoltageDelta = doc["minVoltageDelta"];
    profile.temperatureDeltaInterval = doc["temperatureDeltaInterval"];
    profile.maxTemperatureDelta = doc["maxTemperatureDelta"];
    profile.endingVoltageDrop = doc["endingVoltageDrop"];
    String lookFor = doc["lookForEndingVoltageDrop"];
    if( lookFor == "tak" )
        profile.lookForEndingVoltageDrop = true;
    else
        profile.lookForEndingVoltageDrop = false;

    buildedBatteryProfiles[profilesIterator] = profile;
    ++profilesIterator;
}

void BatteryBuilder::onReceiveString(int bytes){
    while(Wire.available())
    {
        char c = Wire.read();
        I2Cbuffer += c;
        if( c == '\0' )
        {
            I2CString = I2Cbuffer;
            I2Cbuffer = "";
        }
    }
    if(I2CString == "")
    {
        return;
    }
    else if( I2CString.length() < 10)
    {
        builder.batteryCanal = I2CString.toInt();
        builder.buildingProfiles = false;
    }
    else if(builder.buildingProfiles)
    {
        builder.buildBatteryProfile(I2CString);
    }
    else
    {
        builder.buildBattery(I2CString);
    }
    I2CString = "";
}

bool BatteryBuilder::isNewBatteryReady(){
    return newBatteryReady;
}