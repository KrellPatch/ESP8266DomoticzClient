/*
 * ESP8266DomoticzClient.h
 *
 *  Created on: 1 jun. 2020
 *      Author: Mark
 */

#ifndef ESP8266DOMOTICZCLIENT_H_
#define ESP8266DOMOTICZCLIENT_H_

#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define VERSION_ 1.0

#define __DEBUG

#ifdef __DEBUG
	#define DPRINT(...) 	Serial.print(__VA_ARGS__)
	#define DPRINTLN(...)	Serial.println(__VA_ARGS__)
#else
	#define DPRINT(...)	// Nothing
	#define DPRINTLN(...)  //Nothing
#endif

class DomoticzClient {

public:

	// Constructors
	DomoticzClient();
	DomoticzClient(String &);

	/// Destructor
	virtual ~DomoticzClient();

	// Public functions
	void 		setServerUrl(String &);
	String 	getServerUrl();
	bool 		isConnected();
	int 		getLastHTTPReturnCode();
	String 	getLastHTTPReturnString();
	bool 		addLogMessage(String);

	bool 		updateDevice(int, String, String);
	bool 		updateDeviceBySValue(int, String);
	bool 		updateDeviceByNValue(int, String);

private:
	WiFiClient 	_wifiClient;
	HTTPClient 	_httpClient;
	String 			_serverUrl;
	bool 				_isConnected;
	int 				_lastHTTPReturnCode;

	bool 		_connect();
	void 		_disconnect();
	bool 		_getDomoticzServerStatus(); // ToDo Retrieve from json payload: "Status" : "OK"
	bool 		_deviceIndexExists(int);
	String 	_urlEncode(String&);

};

#endif /* ESP8266DOMOTICZCLIENT_H_ */
