/*
 * Author: Claudiu Matei
 */

#ifndef DigitalInput_h
#define DigitalInput_h

#include <Arduino.h>
#include <FlowerPlatformArduinoRuntime.h>

class DigitalInput : public Component, public EventDispatcher {
public:

	static int EVENT_TYPE_VALUE_CHANGED;
	static int EVENT_TYPE_VALUE_ON;
	static int EVENT_TYPE_VALUE_OFF;

	void setPin(int pin) {
	    this->pin = pin;
	}

	virtual void setup() {
		pinMode(pin, INPUT);
		digitalWrite(pin, HIGH);
	}

	virtual void loop() {
		int value = readValue();
		if (value == lastValue) {
	    	return;
	    }

		ValueChangedEvent event;
		event.component = this;
		event.previousValue = lastValue;
		event.currentValue = value;

		event.type = EVENT_TYPE_VALUE_CHANGED;
		dispatchEvent(&event);

		if (value == HIGH) {
			event.type = EVENT_TYPE_VALUE_ON;
		} else {
			event.type = EVENT_TYPE_VALUE_OFF;
		}
		dispatchEvent(&event);

		lastValue = value;
	}

protected:

	virtual int getEventTypeOffset() {
		return EVENT_TYPE_VALUE_CHANGED;
	}

	int pin;

	int lastValue = -1;

	int readValue() {
		return digitalRead(pin);
	}

};

int DigitalInput::EVENT_TYPE_VALUE_CHANGED = EventDispatcher::newEventType();
int DigitalInput::EVENT_TYPE_VALUE_ON = newEventType();
int DigitalInput::EVENT_TYPE_VALUE_OFF = newEventType();

#endif