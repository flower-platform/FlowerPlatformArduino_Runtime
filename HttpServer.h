/*
 *  Author: Claudiu Matei
 */

#ifndef HttpServer_h
#define HttpServer_h

#include <Ethernet.h>
#include <FlowerPlatformArduinoRuntime.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <WebServer.h>
#include <WString.h>

#define URL_MAP_SIZE 8

class HttpServer;

class HttpCommandEvent : public Event {
public:

	String command;

	HttpServer* server;

};


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
		static uint8_t ip[] = { 192, 168, 100, 253 };

		Ethernet.begin(mac, ip);
		webServer = new WebServer(urlPrefix, port);
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

	void httpSuccess(const char *contentType = "text/html; charset=utf-8", const char *extraHeaders = NULL) {
		webServer->httpSuccess(contentType, extraHeaders);
	}

	void print(const char* s) {
		webServer->print(s);
	}

	void print(String s) {
		int len = s.length() + 1;
		char sChar[len];
		s.toCharArray(sChar, len, 0);
		webServer->print(sChar);
	}

	static void webServerRequestReceived(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete) {
//		Serial.print(type); Serial.print(" * "); Serial.print(url_tail); Serial.print(" * "); Serial.print(tail_complete); Serial.println();
		UrlMapping mapping;
		String mappingUrl, requestedUrl = String(url_tail);

		HttpCommandEvent event;
		event.command = requestedUrl.substring(1);
		event.server = INSTANCE;

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
