#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include <FatApiConstants.h>
#include <FatFile.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <HardwareSerial.h>
#include <pins_arduino.h>
#include <stdint.h>
#include <SdFat.h>
#include <WString.h>


class Logger : public Component {
protected:
	unsigned long lastTimestamp;

public:

	String channelName;

	SdFat sd;

	SdFile file;

	char* fileName;

	unsigned long timeInterval;

	Logger(uint8_t chipSelectPin) {
		lastTimestamp = 0;
		pinMode(SS, OUTPUT);
		digitalWrite(SS, HIGH);
		pinMode(chipSelectPin, OUTPUT);
		pinMode(chipSelectPin, HIGH);
		sd.begin(chipSelectPin);

	}

	void log(int value) {
		unsigned long t = millis();
		if (!fileName) {
			fileName = new char[9];
			int cnLength = channelName.length();
			String fn;
			if (channelName.length() <= 8) {
				fn = fileName;
				channelName.toCharArray(fileName, cnLength, 0);
			} else {
				String fn = channelName.substring(0, 4) + "~";
				fn += channelName.substring(cnLength - 3);
				fn.toCharArray(fileName, cnLength, 0);
			}
			Serial.println(fileName);
		}


		if (file.open(fileName, O_CREAT | O_WRITE)) {
			file.print(t);
			file.print(" ");
			file.println(value);
			file.close();
		}
		Serial.print("LOG ");
		Serial.print(t);
		Serial.print(" ");
		Serial.println(value);
	}

/*
	void readLog() {
		if (!fileName) {
			fileName = new char[9];
			int cnLength = channelName.length();
			String fn;
			if (channelName.length() <= 8) {
				fn = fileName;
				channelName.toCharArray(fileName, cnLength, 0);
			} else {
				String fn = channelName.substring(0, 4) + "~";
				fn += channelName.substring(cnLength - 3);
				fn.toCharArray(fileName, cnLength, 0);
			}
			Serial.println(fileName);
		}

		ArduinoOutStream cout(Serial);

		if (file.open(fileName, O_READ)) {
			while (file.dirSize())
			file.read()
			file.close();
		}
	}
*/

};

#endif
