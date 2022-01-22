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

void BatteryBuilder::putBatteryInCharger(const Battery& newBattery){
    profilesIterator = 0;
    buildingProfiles = true;
    charger->addBattery(batteryCanal, newBattery);
}

void BatteryBuilder::buildBattery(const String& batteryJSON){
    deserializeJson(doc, batteryJSON);
    BatteryMode mode;
    double capacity;
    double minVoltage;

    String batteryMode = doc["BatteryMode"];
    if(batteryMode == "laduj")
        mode = BatteryMode::Charge;
    else if(batteryMode == "rozladuj")
        mode = BatteryMode::Discharge;
    else
        mode = BatteryMode::Wait;
    

    capacity = doc["Capacity"];
    minVoltage = doc["MinVoltage"];
    Battery newBattery(profilesIterator,minVoltage,capacity,buildedBatteryProfiles);
    putBatteryInCharger(newBattery);

    if(mode == BatteryMode::Charge)
        charger->charge(batteryCanal);
    else if(mode == BatteryMode::Discharge)
        charger->discharge(batteryCanal);
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