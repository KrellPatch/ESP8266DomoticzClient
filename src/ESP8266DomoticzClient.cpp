/*
 * ESP8266DomoticzClient.cpp
 *
 *  Created on: 1 jun. 2020
 *      Author: Mark
 */

#include "ESP8266DomoticzClient.h"

DomoticzClient::DomoticzClient (const String & serverUrl) :
		_serverUrl {serverUrl},
	_lastHTTPReturnCode {0}
{
	_isConnected = _connect();
}

DomoticzClient::DomoticzClient() :
	DomoticzClient((String&) "")
{}

DomoticzClient::~DomoticzClient() {

}

// Sets the server URL passed and attempts to connect
// Returns connection success
void DomoticzClient::setServerUrl(const String & serverUrl) {
	// Disconnect previous URL
	if(_serverUrl) {
		_disconnect();
	}
	_serverUrl = serverUrl;
	_isConnected = _connect();
}

String DomoticzClient::getServerUrl () {
	return _serverUrl;
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
	if(!this->_deviceIndexExists(index)) return false;
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
	if(!this->_deviceIndexExists(index)) return false;
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
	if(!this->_deviceIndexExists(index)) return false;
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
	return false;
}

bool DomoticzClient::addLogMessage(String message) {
	if(_serverUrl == "") return false;
	String httpString = _serverUrl;
	httpString += "/json.htm?type=command&param=addlogmessage&message=";
	httpString += this->_urlEncode(message);

	DPRINTLN(httpString);

	_httpClient.setURL(httpString);
	_lastHTTPReturnCode = _httpClient.GET();
	if(_lastHTTPReturnCode == HTTP_CODE_OK) {
			return true;
	}
	DPRINTLN(this->getLastHTTPReturnString());
	return false;
} // addLogMessage()


/*
 * Private (helper) functions
 */
// Connect to domoticz instance
bool DomoticzClient::_connect () {
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

// Disconnect from domoticz instance and destroy tcp link
void DomoticzClient::_disconnect () {
	_httpClient.end();
	_serverUrl = "";
	_isConnected = false;
}

// Check if device by index exists
bool DomoticzClient::_deviceIndexExists(int index) {
	if(_serverUrl == "") return false;
	if(index <= 0) return false;

	String httpString = _serverUrl;
	httpString += "/json.htm?type=devices&rid=";
	httpString += index;

	_httpClient.setURL(httpString);
	_lastHTTPReturnCode = _httpClient.GET();
	if(_lastHTTPReturnCode == HTTP_CODE_OK) {
		String httpPayload = _httpClient.getString();
		const size_t capacity = 2*JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(34) + 660;
		DynamicJsonDocument doc(capacity);
		DeserializationError err = deserializeJson(doc, httpPayload);
		if(!err) {
			// If parsed payload contains a "result" object, a device with the corresponding index was found
			if(doc["result"][0]) return true;
		} else {
			DPRINTLN(err.c_str());
			return false;
		}
	}
	DPRINTLN(this->getLastHTTPReturnString());
	return false;
} // _deviceIndexExists()

String DomoticzClient::_urlEncode(String& oldString) {
	String newString;
	unsigned int index = 0;
	char c;
	const String hex = "0123456789abcdef";

	/*
	 * TODO check if all allowed character are checked (~ etc)
	 */
	while( (c = oldString.charAt(index)) != 0 ){
		if( ('a' <= c && c <= 'z')
		|| ('A' <= c && c <= 'Z')
		|| ('0' <= c && c <= '9') ){
			newString += c;
		} else {
			newString += "%";
			newString += hex.charAt(c >> 4);
			newString += hex.charAt(c & 15);
		}
		++index;
	}
	return newString;
} // _urlEncode()
