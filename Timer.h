#ifndef Timer_h
#define Timer_h

#include <Arduino.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <stdbool.h>

class Timer;

class TimerEvent : public Event {
public:

	Timer* timer;

};


class Timer : public Component, public EventDispatcher {
protected:
	unsigned long lastTimestamp;

public:
	static int TIMER_EVENT;
	static int TIMER_COMPLETE_EVENT;

	Timer() : EventDispatcher(2, TIMER_EVENT) {
		repeatCount = 0;
		running = false;
		lastTimestamp = 0;
		currentCount = 0;
	}

	unsigned int currentCount;

	unsigned long delay;

	unsigned int repeatCount;

	bool running;

	virtual void setup() {
	}

	virtual void loop() {
		if (!running) {
			return;
		}
		if (repeatCount > 0 && currentCount > repeatCount) {
			running = false;
			return;
		}
		if (millis() > lastTimestamp + delay) {
			currentCount++;

			TimerEvent event;
			event.type = TIMER_EVENT;
			dispatchEvent(&event);

			if (repeatCount > 0 && currentCount == repeatCount) {
				event.type = TIMER_COMPLETE_EVENT;
				dispatchEvent(&event);

				running = false;
			}

			lastTimestamp = millis();

		}

	}

	void reset() {
		running = false;
		currentCount = 0;
	}

	void start() {
		lastTimestamp = millis();
		running = true;
	}

	void stop() {
		running = false;
	}

};

int Timer::TIMER_EVENT = newEventType();
int Timer::TIMER_COMPLETE_EVENT = newEventType();

#endif
