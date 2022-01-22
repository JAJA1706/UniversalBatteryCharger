#pragma once

class Sensors
{
private:
	const int VOLTAGE_SOURCE;
	const double VOLTAGE_REFERENCE;
	const long SENSING_RESISTOR_1;
	const long SENSING_RESISTOR_2;
	const long SENSING_RESISTOR_3;
	const long SENSING_RESISTOR_4;
	const long SENSING_RESISTOR_5;
	const long SENSING_RESISTOR_6;
	const long SENSING_RESISTOR_7;
	const long SENSING_RESISTOR_8;
	const int POWER_RESISTOR;
	const int VOLTAGE_PIN_1;
	const int VOLTAGE_PIN_2;
	const int VOLTAGE_PIN_3;
	const int VOLTAGE_PIN_4;
	const double VOLTAGE_TO_TEMPERATURE_RATIO;
	const int TEMPERATURE_PIN;
	
	constexpr double voltageDividerMultiplier(const double firstRes, const double secondRes) const;
	double measureVoltage(const int analogPin, const double dividerMultiply);
    double measureTemperature(const int analogPin);
public:
	static double current;
	static double batteryVoltage;
	static double dischargeBatteryVoltage;
	static double regulatorVoltageDrop;
	static double transistorVoltageDrop;
	static double powerResistorVoltageDrop;
	static double batteryTemperature;
	

	Sensors();
	void getDataFromSensors();
};


