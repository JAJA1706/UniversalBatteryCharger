#include "headers/charger.h"
#include "headers/sensors.h"

constexpr int Charger::NUMBER_OF_CANALS;

Charger::Charger()
{
    for(int i = 0; i < NUMBER_OF_CANALS; ++i)
    {
        batteries[i] = Battery();
        dischargeQueue[i] = -1;
        chargeQueue[i] = -1;
    }
}

void Charger::adjustElectricalComponents()
{
    adjustRelays();
    checkChargeQueue();
    checkDischargeQueue();
}

void Charger::adjustRelays()
{
    /*for(int i = 0; i < NUMBER_OF_CANALS; ++i)
    {
        //disconect relays of waiting batteries
    }
    for(int i = 1; i < NUMBER_OF_CANALS; ++i)
    {
        //disconnect relays of batteries in queues
    }
    connect charged relay
    connect discharge relay
    }*/
}

void Charger::checkChargeQueue()
{
    if(chargeQueue[0] == -1)
        return;

    ChargingProfile& profile = batteries[chargeQueue[0]].getOngoingChargingProfile();

    regulator.applyProfile( profile );
    int result = monitor.checkForEndOfTheCharge( profile );

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
    if( cutOffVoltage <= Sensors::dischargeBatteryVoltage)
    {
        setBatteryMode(dischargeQueue[0], BatteryMode::Wait);
    }

}

bool Charger::addBattery(const int canal, const Battery newBattery)
{
    if( !canalExist(canal) )
        return false;

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
    if( batteries[canal].getMode() == newMode)
        return;

    if( batteries[canal].isCharged() )
    {
        if( canal == 0 )
        {
            batteryChargingEnded();
        }
        removeFromQueue(canal, chargeQueue);
        if( chargeQueue[0] != -1)
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
}
void Charger::batteryChargingEnded()
{
    regulator.batteryChargingEnded();
    monitor.batteryChargingEnded();
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