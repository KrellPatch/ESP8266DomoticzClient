/*
 * DomoticzClient.h
 *
 *  Created on: 1 jun. 2020
 *      Author: Mark
 */

#ifndef ESP8266DOMOTICZCLIENT_H_
#define ESP8266DOMOTICZCLIENT_H_

#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

#define __DEBUG

#ifdef __DEBUG
	#define DPRINT(...) 	Serial.print(__VA_ARGS__)
	#define DPRINTLN(...)	Serial.println(__VA_ARGS__)
#else
	#define DPRINT(...)
	#define DPRINTLN(...)
#endif

class DomoticzClient {

public:

	// Constructors
	DomoticzClient();
	DomoticzClient(String &);

	/// Destructor
	virtual ~DomoticzClient();

	// Public functions
	void setServerUrl(String &);
	String getServerUrl();
	bool isConnected();
	int getLastHTTPReturnCode();
	String getLastHTTPReturnString();

	bool updateDevice(int, String, String);
	bool updateDeviceBySValue(int, String);
	bool updateDeviceByNValue(int, String);

private:
	WiFiClient _wifiClient;
	HTTPClient _httpClient;
	String _serverUrl;
	bool _isConnected;
	int _lastHTTPReturnCode;

	bool _connect();
	void _disconnect();
	bool _getDomoticzServerStatus(); // Retrieve from json payload: "Status" : "OK"
	bool _deviceIndexExists(int); // Check for existence of device by index

};

#endif /* ESP8266DOMOTICZCLIENT_H_ */
