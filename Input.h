#ifndef Input_h
#define Input_h

#include <Arduino.h>
#include <FlowerPlatformArduinoRuntime.h>

class Input : public Component, public EventDispatcher {
protected:
	int lastValue = -1;

	virtual int getEventTypeOffset() {
		return VALUE_CHANGED_EVENT;
	}

public:
	static int VALUE_CHANGED_EVENT;

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
		event.type = VALUE_CHANGED_EVENT;
		dispatchEvent(&event);

		lastValue = value;
	}

};

int Input::VALUE_CHANGED_EVENT = newEventType();

#endif
