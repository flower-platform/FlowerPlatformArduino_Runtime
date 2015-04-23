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

	Output(uint8_t pin) : EventDispatcher(0, 0) {
		this->pin = pin;
	    pinMode(pin, OUTPUT);
	}

	void setHigh() {
		digitalWrite(pin, HIGH);
		isOn = true;
	}

	void setLow() {
		digitalWrite(pin, LOW);
		isOn = false;
	}

	void toggleHighLow() {
		if (isOn) {
			setLow();
		} else {
			setHigh();
		}
	}

};

#endif
