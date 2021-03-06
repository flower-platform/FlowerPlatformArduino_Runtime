/*
 *  Author: Claudiu Matei
 */

#ifndef UpdatesBuffer_h
#define UpdatesBuffer_h

#include <FlowerPlatformArduinoRuntime.h>
#include <Print.h>
#include <stdlib.h>
#include <string.h>


class AttributeEntry {
public:

	AttributeEntry(const char* key, const char* value) {
		this->key = new char[strlen(key) + 1];
		strcpy(this->key, key);
		strcpy(this->value, value);
	}

	char* key;

	char value[4];

};

class UpdatesBuffer {
public:

	UpdatesBuffer(int maxAttributeCount) {
		this->maxAttributeCount = maxAttributeCount;
		this->attributeEntries = new AttributeEntry*[maxAttributeCount]();
		this->attributeCount = 0;
	}

	void updateEntry(const char* key, int value) {
		char valueStr[4];
		itoa(value, valueStr, 10);
		updateEntry(key, valueStr);
	}

	void updateEntry(const char* key, const char* value) {
		int i;
		for (i = 0; i < attributeCount; i++) {
//			Serial.print(attributeEntries[i]->key); Serial.print(F(" ")); Serial.println(key);
			if (strcmp(attributeEntries[i]->key, key) == 0) {
				break;
			}
		}

		if (i >= attributeCount) {
			// i.e. was not found
			attributeEntries[attributeCount++] = new AttributeEntry(key, value);
		} else {
			strcpy(attributeEntries[i]->value, value);
		}
	}

	void printEntriesAsJson(Print* print) {
		print->print(F("{"));
		for (int i = 0; i < attributeCount; i++) {
			const char* key = attributeEntries[i]->key;
			const char* value = attributeEntries[i]->value;
			print->print(F("\""));
			print->print(key);
			print->print(F("\": "));
			print->print(value);
			print->print(F(", "));
		}
		print->print(F("\"FREE_MEM\": "));
		print->print(freeRam());
		print->print(F("}"));
	}

protected:

	AttributeEntry** attributeEntries;

	int maxAttributeCount;

	int attributeCount;

	//int freeRam() {
	//	extern int __heap_start, *__brkval;
	//	int v;
	//	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
	//}

	int freeRam() {
		 int size = 2048; // Use 2048 with ATmega328
		  byte *buf;

		  while ((buf = (byte *) malloc(--size)) == NULL) ;

		  free(buf);

		  return size;
	}

};

#endif
