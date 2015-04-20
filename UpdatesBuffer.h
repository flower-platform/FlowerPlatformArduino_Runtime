/*
 *  Author: Claudiu Matei
 */

#ifndef UpdatesBuffer_h
#define UpdatesBuffer_h

#include <Arduino.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <HardwareSerial.h>
#include <stdbool.h>
#include <WString.h>
#include <vector>

class AttributeEntry {
public:

	AttributeEntry(String key, String value) {
		this->key = key;
		this->value = value;
	}

	String key;

	String value;

};

class UpdatesBuffer : public Component {
public:

	UpdatesBuffer() {
		this->attributeEntries = new std::vector<AttributeEntry*>();
		 json = "{}";
	}

	void updateEntry(String key, int value) {
		updateEntry(key, String(value));
	}

	void updateEntry(String key, String value) {
		boolean found = false;
		for (unsigned int i = 0; i < attributeEntries->size(); i++) {
			if (attributeEntries->at(i)->key.equals(key)) {
				found = true;
				attributeEntries->at(i)->value = value;
				break;
			}
		}
		if (!found) {
			attributeEntries->push_back(new AttributeEntry(key, value));
		}
		makeJson();
	}

	void makeJson() {
		json = "{";
		for (unsigned int i = 0; i < attributeEntries->size(); i++) {
			json += "\"";
			json += attributeEntries->at(i)->key;
			json += "\": \"";
			json += attributeEntries->at(i)->value;
			json += "\"";
			if (i != attributeEntries->size() - 1 ) {
				json += ", ";
			}
		}
		json += "}";
	}

	String getJson() {
		return json;
	}

protected:

	std::vector<AttributeEntry*>* attributeEntries;

	String json;

};

#endif
