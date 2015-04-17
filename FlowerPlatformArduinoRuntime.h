/*
 *  Author: Claudiu Matei
 */

#ifndef FlowerPlatformArduinoRuntime_h
#define FlowerPlatformArduinoRuntime_h

#include <HardwareSerial.h>
#include <vector>

class Component {
public:

	virtual ~Component() { }

	virtual void setup() { }

	virtual void loop() { }

};

class ApplicationBase {
public:

	ApplicationBase() {
		components = new std::vector<Component*>();
	}

	virtual ~ApplicationBase() {
		delete components;
	}

	virtual void loop() {
		for (int i = 0; i < components->size(); i++) {
			components->at(i)->loop();
		}
	}

protected:
	std::vector<Component*>* components;

	void addComponent(Component* component) {
		components->push_back(component);
		component->setup();
	}

};

class Event {
public:

	Component* component;

	int type;

};

class Listener {
public:

	virtual ~Listener() { }

	virtual void handleEvent(Event* event) { }

};

class EventDispatcher {
public:

	EventDispatcher() {
		listeners = new std::vector<std::vector<Listener*>*>();
	}

	virtual ~EventDispatcher() {
		listeners->clear();
		delete listeners;
	}

	void addEventListener(int eventType, Listener* listener) {
		int vectorIndex = eventType - getEventTypeOffset();
	//	Serial.print("Vector index: "); Serial.println(vectorIndex);
		std::vector<Listener*>* eventListeners;
		if (vectorIndex + 1 > listeners->size()) {
			eventListeners = new std::vector<Listener*>();
			listeners->resize(vectorIndex + 1);
		}
		eventListeners = listeners->at(vectorIndex);
		if (!eventListeners) {
			eventListeners = new std::vector<Listener*>();
			listeners->at(vectorIndex) = eventListeners;
		}
		eventListeners->push_back(listener);
	}

	//void removeEventListener(int eventType, Listener* listener);

protected:

	static int nextEventType;

	static int newEventType() {
		return nextEventType++;
	}

	std::vector<std::vector<Listener*>*>* listeners;

	virtual int getEventTypeOffset() {
		return -1;
	}

	void dispatchEvent(Event* event) {
		Serial.print("Component::dispatchEvent type="); Serial.println(event->type);
	//	Serial.print("listeners size: "); Serial.println(listeners->size());

		int vectorIndex = event->type - getEventTypeOffset();
		if (vectorIndex >= listeners->size()) {
			return;
		}
	//	Serial.print("vectorIndex: "); Serial.println(vectorIndex);
		std::vector<Listener*>* eventListeners = listeners->at(vectorIndex);
		if (!eventListeners) {
			return;
		}
	//	Serial.print("eventListeners size: "); Serial.println(eventListeners->size());

		for (unsigned int i = 0; i < eventListeners->size(); i++) {
	//		Serial.print("handle: "); Serial.println(i);
			eventListeners->at(i)->handleEvent(event);
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
