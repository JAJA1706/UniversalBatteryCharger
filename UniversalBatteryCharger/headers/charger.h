#pragma once
#include "battery.h"
#include "currentRegulator.h"
#include "chargingMonitor.h"

class Charger
{
private:
    static constexpr int NUMBER_OF_CANALS = 2;
    Battery batteries[NUMBER_OF_CANALS];
    int chargeQueue[NUMBER_OF_CANALS];
    int dischargeQueue[NUMBER_OF_CANALS];
    CurrentRegulator regulator;
    ChargingMonitor monitor;

    void setBatteryMode(const int canal, const BatteryMode newMode);
    void adjustRelays();
    void checkChargeQueue();
    void checkDischargeQueue();
    void batteryChargingStarted();
    void batteryChargingEnded();
    void removeFromQueue(const int canal, int* queue);
    int pushBackToQueue(const int canal, int* queue);
    constexpr bool canalExist(const int canal) const;
public:
    Charger();
    void adjustElectricalComponents();
    bool addBattery(const int canal, const Battery newBattery);
    void charge(const int canal);
    void discharge(const int canal);
};
