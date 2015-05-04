/*
 *  Author: Claudiu Matei
 */

#ifndef HttpServer_h
#define HttpServer_h

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetServer.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <stdint.h>
#include <string.h>
#include <WString.h>

#define DEBUG_HTTP_SERVER 0

#define URL_MAP_SIZE 16
#define LINE_BUFFER_SIZE 128

class HttpServer;

class HttpCommandEvent : public Event {
public:

	const char* command;

	HttpServer* server;

	EthernetClient* client;

};

class HttpServer : public Component {
public:

	void setPort(int port) {
		this->port = port;
	}

	void setUrlPrefix(char* urlPrefix) {
		this->urlPrefix = urlPrefix;
	}

	virtual void setup() {

		static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
		static uint8_t ip[] = { 192, 168, 100, 253 };

		Ethernet.begin(mac, ip);
		this->server = new EthernetServer(this->port);
	}

	virtual void loop() {
		// listen for incoming clients
		EthernetClient client = server->available();

		if (client) {
			char currentLine[64];
			int currentLineSize = 0;
			activeClient = &client;

			while (client.connected()) {
				if (client.available()) {
					char c = client.read();

					if (c == '\n') {
						currentLine[currentLineSize] = '\0';

						#if DEBUG_HTTP_SERVER > 0
						Serial.print(F(">> ")); Serial.println(currentLine);
						#endif

						if (currentLineSize == 0) {
							break;
						}

						if (strncmp(currentLine, "GET", 3) == 0 || strncmp(currentLine, "POST", 4 == 0)) {
							char* k = strchr(currentLine, ' ');
							k[0] = '\0'; // break string
							k+=2; // skip space and leading url slash
							char* requestUrl = k;
							k = strchr(requestUrl, ' ');
							k[0] = 0;
							dispatchEvent(currentLine, requestUrl, activeClient);
							break;
						}
					}
					else if (c != '\r' && currentLineSize < LINE_BUFFER_SIZE - 1) {
						currentLine[currentLineSize++] = c;
					}
				}
			}

			// give the web browser time to receive the data
			delay(1);
			client.stop();
		}
	}

	void addUrlHandler(const char* url, Listener* listener) {
	    if (urlCount >= URL_MAP_SIZE) {

	    	#if DEBUG_HTTP_SERVER > 0
	    	Serial.print(F("HttpServer.addUrlHandler: URL map size limit exceeded"));
			#endif

	    	return;
	    }
		urlMappings[urlCount].url = url;
	    urlMappings[urlCount++].listener = listener;
	}

	void httpSuccess(const char* contentType = "text/html; charset=utf-8") {
		activeClient->println(F("HTTP/1.1 200 OK"));
		activeClient->print(F("Content-Type: ")); activeClient->println(contentType);
		activeClient->println(F("Access-Control-Allow-Origin: *"));
		activeClient->println(F("Connection: close"));  // the connection will be closed after completion of the response
		activeClient->println();
	}

	void dispatchEvent(const char* requestMethod, const char* requestUrl, EthernetClient* client) {

		#if DEBUG_HTTP_SERVER > 0
		Serial.print(F("HttpServer.dispatchEvent: ")); Serial.print(requestMethod); Serial.print(F(" * ")); Serial.print(requestUrl); Serial.println();
		#endif

		HttpCommandEvent event;
		event.command = requestUrl;
		event.server = this;
		event.client = client;

		UrlMapping mapping;
		for (int i = 0; i < urlCount; i++) {
			mapping = urlMappings[i];
			if (strcmp(mapping.url, requestUrl) == 0 || mapping.url[0] == '*') {
				mapping.listener->handleEvent(&event);
			}
		}

	}


protected:

	int port = 80;

	char* urlPrefix = "";

	int urlCount = 0;

	EthernetServer* server;

	EthernetClient* activeClient;

	struct UrlMapping {
		const char* url;
		Listener* listener;
	} urlMappings[URL_MAP_SIZE];

};

#endif
