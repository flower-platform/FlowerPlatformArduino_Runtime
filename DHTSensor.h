/*
 *  Author: Claudiu Matei
 */

#ifndef DHTSensor_h
#define DHTSensor_h

#include <Arduino.h>
#include <dht.h>

#include "FlowerPlatformArduinoRuntime.h"

class DHTSensor : public Component, public EventDispatcher  {
public:

	static int EVENT_TYPE_TEMPERATURE_CHANGED;
	static int EVENT_TYPE_HUMIDITY_CHANGED;

	virtual ~DHTSensor() {
		delete dhtSensorInstance;
	}

	void setPin(int pin) {
	    this->pin = pin;
	}

	void setPollInterval(unsigned long pollInterval) {
		this->pollInterval = (pollInterval > 0) ? pollInterval : 0;
	}

	virtual void setup() {
		this->lastTime = 0;
		this->dhtSensorInstance = new dht();
	}

	virtual void loop() {

		if ((unsigned long int)(millis() - lastTime) < pollInterval) {
			return;
		}

		lastTime = millis();

		check(pin);

		if (currentTemperature != previousTemperature) {
			ValueChangedEvent event;
			event.type = EVENT_TYPE_TEMPERATURE_CHANGED;
			event.target = this;
			event.currentValue = currentTemperature;
			event.previousValue = previousTemperature;
			previousTemperature = currentTemperature;
			dispatchEvent(&event);
		}

		if (currentHumidity != previousHumidity) {
			ValueChangedEvent event;
			event.type = EVENT_TYPE_HUMIDITY_CHANGED;
			event.target = this;
			event.currentValue = currentHumidity;
			event.previousValue = previousHumidity;
			previousHumidity = currentHumidity;
			dispatchEvent(&event);
		}
	}

	void check(int pin) {
		dhtSensorInstance->read11(pin);
		currentTemperature =  dhtSensorInstance->temperature;
		currentHumidity = dhtSensorInstance->humidity;
	}

protected:
	int pin;
	dht* dhtSensorInstance;
	unsigned long pollInterval;
	unsigned long lastTime;
	double currentTemperature = 0;
	double currentHumidity = 0;
	double previousTemperature = -1000;
	double previousHumidity = -1000;
};

/*
class AnalogTemperatureSensor : public Input {
	public:
        AnalogTemperatureSensor(int pin, bool isDigital, unsigned long int pollInterval, int numberOfAttributes);
		void refresh();
		double calculateAnalogTemperature(int value);
    protected:
		unsigned long int pollInterval;
		unsigned long lastTime;
		double currentTemperature;
		double previousTemperature = -1000;
		int currentValue;
};

const int EVENT_SUBTYPE_DHT_SENSOR = 2;

class DHTSensorEvent : public Event {
	public:
		Input* input;
		double digitalTemperature = 0.00;
		double digitalHumidity = 0.00;
};

const int EVENT_SUBTYPE_ANALOG_SENSOR = 3;
class AnalogTemperatureSensorEvent : public Event {
	public:
		Input* input;
		double analogTemperature;
};
 */

int DHTSensor::EVENT_TYPE_TEMPERATURE_CHANGED = EventDispatcher::newEventType();
int DHTSensor::EVENT_TYPE_HUMIDITY_CHANGED = EventDispatcher::newEventType();

#endif
