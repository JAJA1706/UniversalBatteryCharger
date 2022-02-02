#include "headers/charger.h"
#include "headers/sensors.h"
#include <Wire.h>
#include <Arduino.h>

constexpr int Charger::NUMBER_OF_CANALS;
double Charger::completePercentageToSend = 0;
int Charger::currentCanalChargingToSend = -1;
double Charger::batteryVoltageToSend = 0;
double Charger::currentToSend = 0;

Charger::Charger() :  manual{ManualControl(29,31,52), ManualControl(30,32,33)}, chargingRelays{50,25}, dischargingRelays{51,53}
{
    for(int i = 0; i < NUMBER_OF_CANALS; ++i)
    {
        batteries[i] = Battery();
        dischargeQueue[i] = -1;
        chargeQueue[i] = -1;
        pinMode(chargingRelays[i], OUTPUT);
        digitalWrite(chargingRelays[i], HIGH);
        pinMode(dischargingRelays[i], OUTPUT);
        digitalWrite(dischargingRelays[i], HIGH);
        manual[i].linkBattery(&batteries[i]);
    }
    
    regulator.setSensors(&sensors);
    monitor.setSensors(&sensors);
}

void Charger::getSensorData()
{
    sensors.getDataFromSensors();
    batteryVoltageToSend = sensors.batteryVoltage;
    currentToSend = sensors.current;
}

void Charger::adjustElectricalComponents()
{
    handleManualInputs();
    adjustRelays();
    checkChargeQueue();
    checkDischargeQueue();
}

void Charger::handleManualInputs()
{
    for(int i = 0; i < NUMBER_OF_CANALS; ++i)
    {
        BatteryMode mode = manual[i].checkForNewBatteryState();
        if( mode != batteries[i].getMode() )
        {
            setBatteryMode(i, mode);
        }
    }
}

void Charger::adjustRelays()
{
    for(int i = 0; i < NUMBER_OF_CANALS; ++i)
    {
        BatteryMode mode = batteries[i].getMode();
        switch(mode)
        {
            case BatteryMode::Charge:
                digitalWrite(chargingRelays[i], LOW);
                digitalWrite(dischargingRelays[i], HIGH);
                break;
            case BatteryMode::Discharge:
                digitalWrite(chargingRelays[i], HIGH);
                digitalWrite(dischargingRelays[i], LOW);
                break;
           default:
                digitalWrite(chargingRelays[i], HIGH);
                digitalWrite(dischargingRelays[i], HIGH);
                break;
        }
    }
}

void Charger::checkChargeQueue()
{
    if(chargeQueue[0] == -1)
        return;

    ChargingProfile& profile = batteries[chargeQueue[0]].getOngoingChargingProfile();

    regulator.applyProfile( profile );
    int result = monitor.checkForEndOfTheCharge( profile );
    completePercentageToSend = monitor.getCompletePercentage( batteries[chargeQueue[0]].getCapacity() );
    if( result == 1 )
    {
        monitor.profileChargingEnded();
        batteries[chargeQueue[0]].moveToNextStep();
        if( batteries[chargeQueue[0]].isChargingCompleted() )
        {
            setBatteryMode(chargeQueue[0], BatteryMode::Wait);
        }
    }
    else if( result == -1 )
    {
        setBatteryMode(chargeQueue[0], BatteryMode::Wait);
    }
}

void Charger::checkDischargeQueue()
{
    if( dischargeQueue[0] == -1 )
        return;

    double cutOffVoltage = batteries[dischargeQueue[0]].getMinVoltage();
    if( cutOffVoltage <= sensors.dischargeBatteryVoltage)
    {
        setBatteryMode(dischargeQueue[0], BatteryMode::Wait);
    }

}

bool Charger::addBattery(const int canal, Battery newBattery)
{
    if( !canalExist(canal) )
        return false;

    newBattery.setMode(batteries[canal].getMode());
    batteries[canal] = newBattery;
    return true;
}

void Charger::charge(const int canal)
{
    if( !canalExist(canal) )
        return;

    setBatteryMode(canal, BatteryMode::Charge);
}

void Charger::discharge(const int canal)
{
    if( !canalExist(canal) )
        return;

    setBatteryMode(canal, BatteryMode::Discharge);
}

void Charger::setBatteryMode(const int canal, const BatteryMode newMode)
{
    if( batteries[canal].getMode() == newMode){
        return;
    }

    if( batteries[canal].isCharged() )
    {
        if( canal == 0 )
        {
            batteryChargingEnded();
        }
        removeFromQueue(canal, chargeQueue);
        if( canal == 0 && chargeQueue[0] != -1)
        {
            batteryChargingStarted();
        }
    }

    if( batteries[canal].isDischarged() )
    {
        removeFromQueue(canal, dischargeQueue);
    }
    switch (newMode)
    {
    case BatteryMode::Charge:
        batteries[canal].setMode(BatteryMode::Charge);
        if( !pushBackToQueue(canal, chargeQueue) ) //when pushed in front of the queue
        {
            batteryChargingStarted();
        }
        break;
    case BatteryMode::Discharge:
        batteries[canal].setMode(BatteryMode::Discharge);
        pushBackToQueue(canal, dischargeQueue);
        break;
    case BatteryMode::Wait:
        batteries[canal].setMode(BatteryMode::Wait);
        break;
    }
}

void Charger::batteryChargingStarted()
{
    regulator.batteryChargingStarted(batteries[chargeQueue[0]].getOngoingChargingProfile().desiredVoltage);
    monitor.batteryChargingStarted();
    currentCanalChargingToSend = chargeQueue[0];
    completePercentageToSend = 0;
}
void Charger::batteryChargingEnded()
{
    regulator.batteryChargingEnded();
    monitor.batteryChargingEnded();
    currentCanalChargingToSend = -1;
}

int Charger::pushBackToQueue(const int canal, int* queue)
{
    for(int i = 0; i < NUMBER_OF_CANALS; ++i)
    {
        if(queue[i] == -1)
        {
            queue[i] = canal;
            return i;
        }
    }
    return -1;
}

void Charger::removeFromQueue(const int canal, int* queue)
{
    int positionInQueue = NUMBER_OF_CANALS;
    for(int i = 0; i < NUMBER_OF_CANALS; ++i)
    {
        if(queue[i] == canal)
            positionInQueue = i;
    }

    if(positionInQueue == NUMBER_OF_CANALS)
        return;

    for(int i = positionInQueue; i < NUMBER_OF_CANALS-1; ++i)
    {
        queue[i] = queue[i+1];
    }
    queue[NUMBER_OF_CANALS-1] = -1;
}

constexpr bool Charger::canalExist(const int canal) const
{
    return canal >= 0 && canal < NUMBER_OF_CANALS;
}

void Charger::onBatteryDataRequest(){
    String response;
    response = String(currentCanalChargingToSend) + '\n';
    response += String(completePercentageToSend, 2) + '\n';
    response += String(batteryVoltageToSend, 2) + '\n';
    response += String(currentToSend, 2) + '\n';
    Wire.write( response.c_str(), response.length() );
}