#include "headers/sensors.h"
#include <Arduino.h>

double Sensors::current = 0;
double Sensors::batteryVoltage = 0;
double Sensors::dischargeBatteryVoltage = 0;
double Sensors::regulatorVoltageDrop = 0;
double Sensors::transistorVoltageDrop = 0;
double Sensors::powerResistorVoltageDrop = 0;
double Sensors::batteryTemperature = 0;


Sensors::Sensors() :
    VOLTAGE_SOURCE(12),
	VOLTAGE_REFERENCE(1.0765),
	SENSING_RESISTOR_1(96400), 
	SENSING_RESISTOR_2(10366), 
	SENSING_RESISTOR_3(97300),
	SENSING_RESISTOR_4(10404), 
	SENSING_RESISTOR_5(98300), 
	SENSING_RESISTOR_6(11998),
    SENSING_RESISTOR_7(0),
	SENSING_RESISTOR_8(0),
	POWER_RESISTOR(2),
	VOLTAGE_PIN_1(A0), 
	VOLTAGE_PIN_2(A1), 
	VOLTAGE_PIN_3(A2), 
    VOLTAGE_PIN_4(A3),
    VOLTAGE_TO_TEMPERATURE_RATIO(0.01),
    TEMPERATURE_PIN(A4)
{
    pinMode(VOLTAGE_PIN_1, INPUT);
    pinMode(VOLTAGE_PIN_2, INPUT);
    pinMode(VOLTAGE_PIN_3, INPUT);
    pinMode(VOLTAGE_PIN_4, INPUT);
    pinMode(TEMPERATURE_PIN, INPUT);
}


void Sensors::getDataFromSensors()
{
    double voltage1 = measureVoltage(VOLTAGE_PIN_1, voltageDividerMultiplier(SENSING_RESISTOR_1, SENSING_RESISTOR_2));
    double voltage2 = measureVoltage(VOLTAGE_PIN_2, voltageDividerMultiplier(SENSING_RESISTOR_3, SENSING_RESISTOR_4));
    double voltage3 = measureVoltage(VOLTAGE_PIN_3, voltageDividerMultiplier(SENSING_RESISTOR_5, SENSING_RESISTOR_6));
    double voltage4 = measureVoltage(VOLTAGE_PIN_3, voltageDividerMultiplier(SENSING_RESISTOR_7, SENSING_RESISTOR_8));
    regulatorVoltageDrop = VOLTAGE_SOURCE - voltage1;
    transistorVoltageDrop = voltage3;
    powerResistorVoltageDrop = voltage1 - voltage2;
    current = powerResistorVoltageDrop / POWER_RESISTOR;
    batteryVoltage = voltage2 - voltage3;
    dischargeBatteryVoltage = voltage4;

    batteryTemperature = measureTemperature(TEMPERATURE_PIN);

    Serial.print("voltage: ");
    Serial.print(batteryVoltage);
    Serial.print("   current: ");
    Serial.println(current);
    Serial.print("temperature: ");
    Serial.println(batteryTemperature);
}

double Sensors::measureVoltage(const int analogPin, const double dividerMultiply)
{
	double splitedMean = 0;
    int badSamples = 0;
	const int NUMBER_OF_SAMPLES = 60;
    for(int i = 0; i < NUMBER_OF_SAMPLES; ++i)
    {
        int readValue = analogRead(analogPin);
        double result = readValue * 0.97751710; //1000/1023
        result = result * VOLTAGE_REFERENCE * dividerMultiply;
        
        double currentMean = splitedMean * NUMBER_OF_SAMPLES / i;
        if( i > 4 && (result > 1.15*currentMean || result < 0.85*currentMean)) //removing bad samples
        {
             result = currentMean;
             ++badSamples;
             if(badSamples > NUMBER_OF_SAMPLES / 10)
             {
                return 0;
             }
        }
        splitedMean += result/NUMBER_OF_SAMPLES;
        
        delay(2);
    }
    return splitedMean;
}

double Sensors::measureTemperature(const int analogPin)
{
    const int ANALOG_READ_RESOLUTION = 1023;
    double result = analogRead(analogPin);  
    result *= VOLTAGE_REFERENCE;
    result /= ANALOG_READ_RESOLUTION;
    result /= VOLTAGE_TO_TEMPERATURE_RATIO;
    return result;
}

constexpr double Sensors::voltageDividerMultiplier(const double firstRes, const double secondRes) const
{
    return (firstRes + secondRes) / secondRes;
}
