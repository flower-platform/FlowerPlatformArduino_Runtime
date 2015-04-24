/*
 *  Author: Claudiu Matei
 */

#ifndef FlowerPlatformArduinoRuntime_h
#define FlowerPlatformArduinoRuntime_h

#define MAX_LISTENERS_PER_EVENT_TYPE 4

#define DEBUG_FP 0

#include <HardwareSerial.h>

class Component {
public:

	virtual ~Component() { }

	virtual void setup() { }

	virtual void loop() { }

};

class ApplicationBase {
public:

	ApplicationBase(int maxComponentCount) {
		components = new Component*[maxComponentCount]();
		componentCount = 0;
	}

	virtual ~ApplicationBase() {
		delete components;
	}

	virtual void loop() {
		for (int i = 0; i < componentCount; i++) {
			components[i]->loop();
		}
	}

protected:
	int componentCount;
	Component** components;

	void addComponent(Component* component) {
		components[componentCount++] = component;
		component->setup();
	}

};

// forward declaration
class EventDispatcher;
class Event {
public:

	EventDispatcher* target;

	int type;

};

class Listener {
public:

	virtual ~Listener() { }

	virtual void handleEvent(Event* event) { }

};

template <class T> class DelegatingListener : public Listener {
protected:
	T* instance;
	void (T::*functionPointer)(Event* event);
public:
	DelegatingListener(T* _instance, void (T::*_functionPointer)(Event* event)) {
		instance = _instance;
		functionPointer = _functionPointer;
	}
	virtual void handleEvent(Event* event) {
		(*instance.*functionPointer)(event);
	}
};

class EventDispatcher {
public:

	EventDispatcher(int eventCount, int eventTypeOffset) {
		this->eventCount = eventCount;
		this->eventTypeOffset = eventTypeOffset;
		listeners = new Listener*[eventCount * MAX_LISTENERS_PER_EVENT_TYPE]();
	}

	virtual ~EventDispatcher() {
		delete listeners;
	}

	void addEventListener(int eventType, Listener* listener) {
		int eventIndex = eventType - eventTypeOffset;
		for (int i = eventCount * eventIndex; i < eventCount * eventIndex + MAX_LISTENERS_PER_EVENT_TYPE; i++) {
			if (!listeners[i]) {
				listeners[i] = listener;
				break;
			}
		}
	}

	//void removeEventListener(int eventType, Listener* listener);

protected:

	static int nextEventType;

	static int newEventType() {
		return nextEventType++;
	}

	int eventCount;

	int eventTypeOffset;

	Listener** listeners;

	void dispatchEvent(Event* event) {
		#if DEBUG_FP > 0
		Serial.print("Component::dispatchEvent type="); Serial.println(event->type);
	//	Serial.print("listeners size: "); Serial.println(listeners->size());
		#endif

		int eventIndex = event->type - eventTypeOffset;

		for (int i = eventCount * eventIndex; i < eventCount * eventIndex + MAX_LISTENERS_PER_EVENT_TYPE; i++) {
			if (!listeners[i]) {
				break;
			}
			listeners[i]->handleEvent(event);
		}
	}

};

class ValueChangedEvent : public Event {
public:

	int previousValue;

	int currentValue;

};

/*
class DataChangedEvent : public Event {
	public:
		String attribute;
		String previousValue;
		String currentValue;
};

class DataSetChangedEvent : public Event {
	public:
		std::vector<DataSetEntry> dataSet;
};

class DataSetEntry {
	public:
		DataSetEntry(String attribute, String value);
		String attribute;
		String value;
};
*/

int EventDispatcher::nextEventType = 0;

#endif
