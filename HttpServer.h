/*
 *  Author: Claudiu Matei
 */

#ifndef HttpServer_h
#define HttpServer_h

#include <Ethernet.h>
#include <HardwareSerial.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <WebServer.h>
#include <WString.h>

#include "FlowerPlatformArduinoRuntime.h"

#define URL_MAP_SIZE 8

class HttpCommandEvent : public Event {
public:

	String command;

};

class HttpServer;


class HttpServer : public Component {
public:

	static HttpServer* INSTANCE;

	HttpServer() {
//		if (HTTP_SERVER_INSTANCE) {
//			throw 1;
//		}
		INSTANCE = this;
	}

	void setPort(int port) {
		this->port = port;
	}

	void setUrlPrefix(char* urlPrefix) {
		this->urlPrefix = urlPrefix;
	}

	virtual void setup() {

		static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
		static uint8_t ip[] = { 192, 168, 100, 137 };

		webServer = new WebServer(urlPrefix, port);
		Ethernet.begin(mac, ip);
		webServer->begin();
		webServer->setFailureCommand(&webServerRequestReceived);
		webServer->setDefaultCommand(&webServerRequestReceived);
	}

	virtual void loop() {
//		Serial.println("process connection");
		webServer->processConnection();
	}

	void addUrlHandler(String url, Listener* listener) {
	    urlMappings[INSTANCE->urlCount].url = url;
	    urlMappings[INSTANCE->urlCount++].listener = listener;
	}

	static void webServerRequestReceived(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
		Serial.print(type); Serial.print(" "); Serial.print(url_tail); Serial.print(" "); Serial.print(tail_complete); Serial.println();
		UrlMapping mapping;
		String mappingUrl, requestedUrl = String(url_tail);

		HttpCommandEvent event;
		event.command = requestedUrl.substring(1);

		for (int i = 0; i < INSTANCE->urlCount; i++) {
			mapping = INSTANCE->urlMappings[i];
			mappingUrl = "/" + mapping.url;
			if (mappingUrl.equals(requestedUrl) || mappingUrl == "*") {
				mapping.listener->handleEvent(&event);
			}
		}
	}


protected:

	int port = 80;

	char* urlPrefix = "";

	int urlCount = 0;

	WebServer* webServer = NULL;

	struct UrlMapping {
		String url;
		Listener* listener;
	} urlMappings[URL_MAP_SIZE];

};

HttpServer* HttpServer::INSTANCE = NULL;

#endif
