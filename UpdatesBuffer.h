/*
 *  Author: Claudiu Matei
 */

#ifndef UpdatesBuffer_h
#define UpdatesBuffer_h

#include <FlowerPlatformArduinoRuntime.h>
#include <WString.h>


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

	UpdatesBuffer(int maxAttributeCount) {
		this->maxAttributeCount = maxAttributeCount;
		this->attributeEntries = new AttributeEntry*[maxAttributeCount]();
		this->attributeCount = 0;
		 json = "{}";
	}

	void updateEntry(String key, int value) {
		updateEntry(key, String(value));
	}

	void updateEntry(String key, String value) {
		int i = 0;
		while (attributeEntries[i] && !(attributeEntries[i]->key.equals(key))) {
			i++;
		}

		if (i >= maxAttributeCount) {
			// Buffer size limit exceeded
			return;
		}

		if (!attributeEntries[i]) {
			attributeEntries[attributeCount++] = new AttributeEntry(key, value);
		} else {
			attributeEntries[i]->value = value;
		}

		makeJson();
	}

	void makeJson() {
		String json = "{";
		for (int i = 0; i < attributeCount; i++) {
			String key = attributeEntries[i]->key;
			String value = attributeEntries[i]->value;
			json += "\"";
			json += key;
			json += "\": ";
			json += value;
			if (i != attributeCount - 1 ) {
				json += ", ";
			}
		}
		json += "}";
		this->json = json;
	}

	String getJson() {
		return json;
	}

protected:

	AttributeEntry** attributeEntries;

	int maxAttributeCount;

	int attributeCount;

	String json;

};

#endif
