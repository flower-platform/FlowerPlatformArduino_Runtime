#ifndef Input_h
#define Input_h

#include <Arduino.h>
#include <FlowerPlatformArduinoRuntime.h>

class Input : public Component, public EventDispatcher {
protected:
	int lastValue = -1;

	virtual int getEventTypeOffset() {
		return EVENT_TYPE_VALUE_CHANGED;
	}

public:
	static int EVENT_TYPE_VALUE_CHANGED;

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
	}

};

int Input::EVENT_TYPE_VALUE_CHANGED = newEventType();

#endif
