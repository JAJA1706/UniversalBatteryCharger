#include "headers/manualControl.h"
#include "Arduino.h"

ManualControl::ManualControl(const int ledpin, const int ledpin2, const int buttonpin)
 : LED_PIN{ledpin,ledpin2}, BUTTON_PIN{buttonpin}
{
    pinMode(LED_PIN[0], OUTPUT);
    pinMode(LED_PIN[1], OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void ManualControl::linkBattery(Battery* _newBatteryPtr)
{
    linkedBattery = _newBatteryPtr;
}

BatteryMode ManualControl::checkForNewBatteryState()
{
    if(linkedBattery == nullptr)
    {
        return BatteryMode::Wait;
    }

    const BatteryMode linkedBatteryMode = linkedBattery->getMode();
    int buttonState = digitalRead(BUTTON_PIN);
    switch(linkedBatteryMode)
    {
        case BatteryMode::Charge:
            digitalWrite(LED_PIN[0], HIGH);
            digitalWrite(LED_PIN[1], HIGH);
            break;
        case BatteryMode::Discharge:
            digitalWrite(LED_PIN[0], LOW);
            digitalWrite(LED_PIN[1], HIGH);
            break;
        case BatteryMode::Wait:
            digitalWrite(LED_PIN[0], LOW);
            digitalWrite(LED_PIN[1], LOW);
            break;
    }

    if(buttonState == LOW)
    { 
        switch(linkedBatteryMode)
        {
            case BatteryMode::Charge:
                return BatteryMode::Discharge;
                break;
            case BatteryMode::Discharge:
                return BatteryMode::Wait;
                break;
            case BatteryMode::Wait:
                return BatteryMode::Charge;
                break;
        }
    }
    else
    {
        return linkedBatteryMode;
    }
}