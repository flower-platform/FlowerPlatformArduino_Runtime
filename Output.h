/*
 *  Author: Claudiu Matei
 */

#ifndef Output_h
#define Output_h

#include <Arduino.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <HardwareSerial.h>
#include <stdbool.h>
#include <stdint.h>

class Output : public EventDispatcher {
protected:
	uint8_t pin;
	int lastValue = 0;

public:

	static int VALUE_CHANGED_EVENT;

	bool isPwm = false;

	Output(uint8_t pin) : EventDispatcher(1, VALUE_CHANGED_EVENT) {
		this->pin = pin;
	    pinMode(pin, OUTPUT);
	}

	void setHigh() {
<<<<<<< HEAD
//		if (isOn) {
//			return;
//		}
		digitalWrite(pin, HIGH);
		isOn = true;
		dispatchEvents();
=======
		if (lastValue) {
			return;
		}
		setValue(HIGH);
>>>>>>> branch 'master' of https://github.com/flower-platform/FlowerPlatformArduino_Runtime.git
	}

	void setLow() {
<<<<<<< HEAD
//		if (!isOn) {
//			return;
//		}
		digitalWrite(pin, LOW);
		isOn = false;
		dispatchEvents();
=======
		if (!lastValue) {
			return;
		}
		setValue(LOW);
	}

	void setValue(int value) {
		if (isPwm) {
			analogWrite(pin, value);
		} else {
			digitalWrite(pin, value);
		}
		dispatchEvents(value);
>>>>>>> branch 'master' of https://github.com/flower-platform/FlowerPlatformArduino_Runtime.git
	}

	void toggleHighLow() {
		if (lastValue) {
			setValue(LOW);
		} else {
			setValue(HIGH);
		}
	}

	void dispatchEvents(int value) {
		ValueChangedEvent event;
		event.target = this;
		event.previousValue = lastValue;
		event.currentValue = value;
		event.type = VALUE_CHANGED_EVENT;
		dispatchEvent(&event);

		lastValue = value;
	}

};

int Output::VALUE_CHANGED_EVENT = EventDispatcher::newEventType();


#endif
