/*
 *  Author: Claudiu Matei
 */

#ifndef Output_h
#define Output_h

#include <Arduino.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <stdbool.h>
#include <stdint.h>

class Output : public EventDispatcher {
protected:
	uint8_t pin;
	bool isOn = false;

public:

	static int EVENT_TYPE_VALUE_CHANGED;
	static int EVENT_TYPE_VALUE_ON;
	static int EVENT_TYPE_VALUE_OFF;

	Output(uint8_t pin) : EventDispatcher(3, EVENT_TYPE_VALUE_CHANGED) {
		this->pin = pin;
	    pinMode(pin, OUTPUT);
	}

	void setHigh() {
		if (isOn) {
			return;
		}
		digitalWrite(pin, HIGH);
		isOn = true;
		dispatchEvents();
	}

	void setLow() {
		if (!isOn) {
			return;
		}
		digitalWrite(pin, LOW);
		isOn = false;
		dispatchEvents();
	}

	void toggleHighLow() {
		if (isOn) {
			setLow();
		} else {
			setHigh();
		}
		dispatchEvents();
	}

	void dispatchEvents() {
		ValueChangedEvent event;
		event.target = this;
		event.previousValue = isOn ? 0 : 1;
		event.currentValue = isOn ? 1 : 0;

		event.type = EVENT_TYPE_VALUE_CHANGED;
		dispatchEvent(&event);

		if (isOn) {
			event.type = EVENT_TYPE_VALUE_ON;
		} else {
			event.type = EVENT_TYPE_VALUE_OFF;
		}
		dispatchEvent(&event);

	}

};

int Output::EVENT_TYPE_VALUE_CHANGED = EventDispatcher::newEventType();
int Output::EVENT_TYPE_VALUE_ON = EventDispatcher::newEventType();
int Output::EVENT_TYPE_VALUE_OFF = EventDispatcher::newEventType();


#endif
