/*
 *  Author: Claudiu Matei
 */

#ifndef DigitalOutput_h
#define DigitalOutput_h

#include "FlowerPlatformArduinoRuntime.h"
#include <Arduino.h>
#include <stdbool.h>

class DigitalOutput : public Component {
public:

	void setPin(int pin) {
	    this->pin = pin;
	}

	void setInitialState(int initialState) {
	    this->initialState = initialState;
	}

	virtual void setup() {
	    pinMode(pin, OUTPUT);
		digitalWrite(pin, initialState);
		isOn = initialState;
	}


	void setOn() {
		digitalWrite(pin, HIGH);
		isOn = true;
	}

	void setOff() {
		digitalWrite(pin, LOW);
		isOn = false;
	}

	void toggle() {
		if (isOn) {
			setOff();
		} else {
			setOn();
		}
	}

protected:

	int pin;

	int initialState;

	bool isOn;

};

#endif
