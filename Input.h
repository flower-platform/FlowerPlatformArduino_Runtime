#ifndef Input_h
#define Input_h

#include <Arduino.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <stdbool.h>
#include <stdint.h>

class Input : public Component, public EventDispatcher {
protected:
	int lastValue = -1;

public:
	static int EVENT_TYPE_VALUE_CHANGED;
	static int EVENT_TYPE_VALUE_ON;
	static int EVENT_TYPE_VALUE_OFF;

	Input() : EventDispatcher(3, EVENT_TYPE_VALUE_CHANGED) { }

	uint8_t pin;
	bool isAnalog = false;

	virtual void setup() {
		pinMode(pin, INPUT);
		if (!isAnalog) {
			digitalWrite(pin, HIGH);
		}
	}

	virtual void loop() {
		int value;
		if (isAnalog) {
			value = analogRead(pin);
		} else {
			value = digitalRead(pin);
		}
		if (value == lastValue) {
	    	return;
	    }

		ValueChangedEvent event;
		event.target = this;
		event.previousValue = lastValue;
		event.currentValue = value;
		event.type = EVENT_TYPE_VALUE_CHANGED;
		dispatchEvent(&event);

		lastValue = value;

		if (isAnalog) {
			return;
		}

		if (value == HIGH) {
			event.type = EVENT_TYPE_VALUE_ON;
		} else {
			event.type = EVENT_TYPE_VALUE_OFF;
		}
		dispatchEvent(&event);


	}

};

int Input::EVENT_TYPE_VALUE_CHANGED = newEventType();
int Input::EVENT_TYPE_VALUE_ON = newEventType();
int Input::EVENT_TYPE_VALUE_OFF = newEventType();

#endif
