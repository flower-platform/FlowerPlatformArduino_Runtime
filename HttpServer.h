/*
 *  Author: Claudiu Matei
 */

#ifndef HttpServer_h
#define HttpServer_h

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetServer.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <HardwareSerial.h>
#include <stdint.h>
#include <WString.h>

#define DEBUG_HTTP_SERVER 0

#define URL_MAP_SIZE 16
#define LINE_BUFFER_SIZE 128

class HttpServer;

class HttpCommandEvent : public Event {
public:

	String command;

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
			char currentLine[256];
			int currentLineSize = 0;
			activeClient = &client;
			String requestMethod;
			String requestUrl;

			while (client.connected()) {
				if (client.available()) {
					char c = client.read();

					if (c == '\n') {
						currentLine[currentLineSize] = '\0';

						#if DEBUG_HTTP_SERVER > 0
						Serial.print(">> "); Serial.println(currentLine);
						#endif

						if (currentLineSize == 0) {
							dispatchEvent(requestMethod, requestUrl, activeClient);
							break;
						}

						String line(currentLine);
						if (line.startsWith("GET") || line.startsWith("POST")) {
							requestMethod = line.substring(0, line.indexOf(' '));
							requestUrl = line.substring(requestMethod.length() + 2, line.indexOf(' ', requestMethod.length() + 2));
						}
						currentLineSize = 0;
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

	void addUrlHandler(String url, Listener* listener) {
	    if (urlCount >= URL_MAP_SIZE) {

	    	#if DEBUG_HTTP_SERVER > 0
	    	Serial.print("HttpServer.addUrlHandler: "); Serial.println("URL map size limit exceeded");
			#endif

	    	return;
	    }
		urlMappings[urlCount].url = url;
	    urlMappings[urlCount++].listener = listener;
	}

	void httpSuccess(const char *contentType = "text/html; charset=utf-8") {
		activeClient->println("HTTP/1.1 200 OK");
		activeClient->print("Content-Type: "); activeClient->println(contentType);
		activeClient->println("Connection: close");  // the connection will be closed after completion of the response
		activeClient->println();
	}

	void dispatchEvent(String requestMethod, String requestUrl, EthernetClient* client) {

		#if DEBUG_HTTP_SERVER > 0
		Serial.print("HttpServer.dispatchEvent: "); Serial.print(requestMethod); Serial.print(" * "); Serial.print(requestUrl); Serial.println();
		#endif

		HttpCommandEvent event;
		event.command = requestUrl;
		event.server = this;
		event.client = client;

		UrlMapping mapping;
		for (int i = 0; i < urlCount; i++) {
			mapping = urlMappings[i];
			if (mapping.url.equals(requestUrl) || mapping.url == "*") {
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
		String url;
		Listener* listener;
	} urlMappings[URL_MAP_SIZE];

};

#endif
