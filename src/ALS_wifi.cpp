#include "ALS_wifi.h"
#include <ESP8266HTTPClient.h>
#include "ALS_lampsetting.h"


ALS_wifi * ALS_wifi_ptr;
ALS_wifi * ALS_wifiPtr_get(){
    return ALS_wifi_ptr;
}
ALS_wifi::ALS_wifi(const char * value) : wifiConnect(){
	ALS_wifi_ptr = this;
	_checkConnected_timer 	= new adri_timer(180000, 	"", false);
	_connectModTimer 		= new adri_timer(2000, 		"", false);
	_hostname 				= value;
	new wifi_credential_ap("");
	_ntpTime 				= new adri_timeNtp();
	
}




mod_wifiConnect_error ALS_wifi::error_get() {
	return _error_setup;
}

int 	ALS_wifi::_connectMod_get	() 				{ return _connectMod; }

void 	ALS_wifi::_connectMod_set	(int value) 	{ _connectMod = value; }

void 	ALS_wifi::_hostname_get		(String & ret) 	{ ret = ch_toString(_hostname);}	

void 	ALS_wifi::_otaEnabled_set 	(boolean ret)	{ _otaEnabled = ret;}

boolean ALS_wifi::_setup(WIFICONNECT_MOD cMod, WIFICONNECTSSID_MOD sMod) {

	if (!wifi_credential_sta_fromSPIFF()) return false;
	// wifi_credential_sta_print();

	load_fromSpiif 				();
	credential_sta_pos_set 		(0);
	connect_set 				(cMod);
	connectSSID_set 			(sMod);
	station_set 				(WIFI_STA);
	hostName_set 				(_hostname);
	setup_id					();
	// print_cfg 					();

	// if (!_ap->load_fromSpiif()) {
	wifi_credentialAp_ptr_get()->hostname_set(ch_toString(_hostname));
	wifi_credentialAp_ptr_get()->psk_set("rgbw1234");
	if (_credential_sta->ip_get() == "") 	{wifi_credentialAp_ptr_get()->ip_set("192.168.4.1");}
	else 									{wifi_credentialAp_ptr_get()->ip_set(_credential_sta->ip_get());}
	// }
	// wifi_credentialAp_ptr_get()->print();
	return true;
}

boolean ALS_wifi::_connect(WIFICONNECT_MOD cMod, WIFICONNECTSSID_MOD sMod) {

	lampPeripheralsInstance()->oled_clear();
	lampPeripheralsInstance()->wificonnect("Connect ALS_wifi en cour");

	if (cMod == AWC_SETUP) {

		setup();

		lampPeripheralsInstance()->oled_clear();
		lampPeripheralsInstance()->wifiPrint();

		if(!_otaEnabled)	MDSN_begin			();
		else 				arduinoOTA_setup	(_hostname);

		ntpTime_setup();

		_connectModTimer->set_duration_max(2000);
		_connectModTimer->activate();
		_connectMod 		= 0;
		_connectMod_setTo 	= 1;

	}

	if (cMod == AWC_LOOP) {
		_connectModTimer->set_duration_max(2000);
		_connectModTimer->activate();
		_connectMod 		= 0;
		_connectMod_setTo 	= 2;
	}
}
	
void ALS_wifi::_loop() {
	if ( _connectModTimer->isActivate() ) {
		if ( _connectModTimer->loop_stop() ) {

			_connectMod = _connectMod_setTo;
			switch(_connectMod) {
				case 1:
				break;
				case 2:
					setupLoop();
				break;
			}
			
		}	
	}	

	wifi_loop();

	if ((!_isConnect) && (wifi_loop_statu()==wifi_statu_sta_isconnected) && (_connectMod == 2)) {
		_connectMod = 3;

		// fsprintf("\n[wifi_loop_statu : wifi_statu_sta_isconnected]\n")
		lampPeripheralsInstance()->oled_clear();
		lampPeripheralsInstance()->wifiPrint();

		if(!_otaEnabled)	MDSN_begin			();
		else 				arduinoOTA_setup	(_hostname);

		ntpTime_setup();

		_connectModTimer->set_duration_max(1500);
		_connectModTimer->activate();
		_connectMod_setTo 	= 1;		

	}
}	

void ALS_wifi::_checkConnected_start(){
	WiFiMode_t wifiMod = WiFi.getMode();
	if (wifiMod == WIFI_AP) return;
	_checkConnected_timer->activate();	
}
boolean ALS_wifi::_checkConnected(int & result){
    WiFiClient client;
    HTTPClient http;
	if (http.begin(client, "http://jigsaw.w3.org/HTTP/connection.html")) {  // HTTP
		result = http.GET();
		http.end();
		return true;
	} else {
		result = -2;
		return false;
	}
}
void ALS_wifi::_checkConnected_loop(){
	if (!_checkConnected_timer->isActivate()) return;
	if (_checkConnected_timer->loop()) {
		int result = -1;
		boolean check = _checkConnected(result);
		if ((!check) || (result <= 0)) {

			char buffer[200];
		   	
		   	String timeStr;
		   	ntpTime_getTime(timeStr);

			sprintf(buffer, "[_checkConnected_loop] begin: %d - result: %d", check, result);

			fsprintf("\n%s\n", buffer);

			String logStr = "";
			adri_toolsPtr_get()->log_read(logStr, false);
			adri_toolsPtr_get()->log_write(logStr, timeStr, String(buffer));		
					
			ESP.restart();
					
		}
	}
}	


void ALS_wifi::ntpTime_setup() {
	boolean sWifi = wifi_connect_result("");
	_ntpTime->setup(sWifi);
}
void ALS_wifi::ntpTime_loop() {
	if (!ntpTime_isSet()) return;
	_ntpTime->loop();
}			
void ALS_wifi::ntpTime_getTime(String & ret) {
	time_t 	time;
   	if (ntpTime_isSet()){
		time	= adri_timeNtp_instance()->timeget();
		ret 	= ntpTime_toString(time);	   		
   	} else {
   		ret = on_time();					 
   	}
}
String ALS_wifi::ntpTime_toString(time_t t) {
    char tmpStr[100];
    sprintf(tmpStr, "%02d:%02d:%02d", hour(t), minute(t), second(t));
    return String(tmpStr);
}
boolean ALS_wifi::ntpTime_isSet(){
	boolean ret = false;
	if(	_ntpTime->time_isSet ) {
		ret = true;
	} else {
		ret = false;
	}
	return ret;	
}