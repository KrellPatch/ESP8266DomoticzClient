/*
 * DomoticzClient.cpp
 *
 *  Created on: 1 jun. 2020
 *      Author: Mark
 */

#include "ESP8266DomoticzClient.h"

DomoticzClient::DomoticzClient () {
	_serverUrl = "";
	_isConnected = false;
	_lastHTTPReturnCode = 0;
}

DomoticzClient::DomoticzClient (String & serverUrl) {
	if(serverUrl != "") {
		_serverUrl = serverUrl;
		_isConnected = this->_connect();
	}
}

DomoticzClient::~DomoticzClient() {
	// TODO Auto-generated destructor stub
}
// Sets the server URL passed and attempts to connect
// Returns connection success
void DomoticzClient::setServerUrl(String & serverUrl) {
	_serverUrl = serverUrl;
	_isConnected = this->_connect();
}

String DomoticzClient::getServerUrl () {
	return _serverUrl;
}

bool DomoticzClient::_connect () { // @suppress("No return")
	if(_serverUrl == "") return false;

	String httpString = _serverUrl;
	httpString += "/json.htm?type=command&param=getversion";
	DPRINTLN(httpString);

	if(!_httpClient.begin(_wifiClient, httpString)) {
		DPRINTLN("Unable to initialize HTTP client");
		return false;
	}
	_lastHTTPReturnCode = _httpClient.GET();
	if(_lastHTTPReturnCode == HTTP_CODE_OK) {
			return true;
	}
	DPRINTLN(this->getLastHTTPReturnString());
	return false;
}

void DomoticzClient::_disconnect () {
	_serverUrl = "";
	_isConnected = false;
}

int DomoticzClient::getLastHTTPReturnCode() {
	return _lastHTTPReturnCode;
}

String DomoticzClient::getLastHTTPReturnString() {
	return _httpClient.errorToString(_lastHTTPReturnCode);
}

bool DomoticzClient::isConnected () {
	return _isConnected;
}

bool DomoticzClient::updateDevice(int index, String nValue, String sValue) {
	if(_serverUrl == "") return false;
	if(index <= 0) return false;
	if(!nValue) return false;
	if(!sValue) return false;

	String httpString = _serverUrl;
	httpString += "/json.htm?type=command&param=udevice&idx=";
	httpString += index;
	httpString += "&nvalue=";
	httpString += nValue;
	httpString += "&svalue=";
	httpString += sValue;

	DPRINTLN(httpString);

	_httpClient.setURL(httpString);
	_lastHTTPReturnCode = _httpClient.GET();
	if(_lastHTTPReturnCode == HTTP_CODE_OK) {
			return true;
	}
	DPRINTLN(this->getLastHTTPReturnString());
	return false;
}

bool DomoticzClient::updateDeviceBySValue(int index, String sValue) {
	if(_serverUrl == "") return false;
	if(index <= 0) return false;
	if(!sValue) return false;

	String httpString = _serverUrl;
	httpString += "/json.htm?type=command&param=udevice&idx=";
	httpString += index;
	httpString += "&svalue=";
	httpString += sValue;

	DPRINTLN(httpString);

	_httpClient.setURL(httpString);
	_lastHTTPReturnCode = _httpClient.GET();
	if(_lastHTTPReturnCode == HTTP_CODE_OK) {
			return true;
	}
	DPRINTLN(this->getLastHTTPReturnString());
	return false;}

bool DomoticzClient::updateDeviceByNValue(int index, String nValue) {
	if(_serverUrl == "") return false;
	if(index <= 0) return false;
	if(!nValue) return false;

	String httpString = _serverUrl;
	httpString += "/json.htm?type=command&param=udevice&idx=";
	httpString += index;
	httpString += "&nvalue=";
	httpString += nValue;

	DPRINTLN(httpString);

	_httpClient.setURL(httpString);
	_lastHTTPReturnCode = _httpClient.GET();
	if(_lastHTTPReturnCode == HTTP_CODE_OK) {
			return true;
	}
	DPRINTLN(this->getLastHTTPReturnString());
	return false;}
