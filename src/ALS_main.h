#ifndef ALS_MAIN_H 
	#define ALS_MAIN_H 

	#include <adri_tools.h>
	#include <adri_timer.h>
	#include <adri_rgb_patterns.h>
	#include <adri_udp.h>
	#include <ALS_fnc.h>

	#include "ALS_lampsetting.h"
	#include "ALS_webserver.h"
	#include "ALS_wifi.h"
	#include "ALS_eventclock.h"


	#ifdef ALS_USE_ESPWEBSERVER
		#include <adri_espwebserver.h>
		adri_webserver	clientServer(80); 
		#ifdef ALS_USE_SOCKETSERVER
		adri_socket		socketServer(81); 
		#endif	
		#include <ALS_espwebserver.h>
	#endif

	adri_udp 		* ALS_udp		= nullptr;
	adri_udpMulti 	* ALS_udpMulti 	= nullptr;

	// void ALS_lamp_show(uint8_t v) {
	// 	leds_settingPtr_get()->lamp_show();
	// }
	void ALS_lamp_clear(uint8_t v) {
		leds_settingPtr_get()->lamp_clear();
	}
	void ALS_lamp_bri(uint8_t v) {
		leds_settingPtr_get()->lamp_bri(v);
	}
	void ALS_registerFuncExt() {
		ALS_fnc_add(ALS_lamp_clear);	
		ALS_fnc_add(ALS_lamp_bri);	
	}
	
	class ALS_main
	{
		adri_tools 				* _adri_tools;
		adriTools_serialRead    * _serial;
		ALS_wifi				* _ALS_wifi;

		boolean 				_iCanEvents				= false;
		int						_timerEventSetupMod 	= -1;
		boolean					_webserverLoopEnabled 	= false;
		adri_timer				* _timerEventSetup;
		
		effect_manager 			* _ledsEffect;
		leds_setting			* _ledsSetting;

		programme_loop          * _programme;
		playlist_management     * _playlist;

		ALS_espwebserver		* _webserver;

		ALS_udpServer			* _udpServer;

		ALS_lampsetting 		* _lampSetting;

		ALS_lampUpdateClient	* _lampUpdateClient;
	public:


		ALS_main(){};
		~ALS_main(){};

		void loop(){

			_ALS_wifi->_loop();

			if (_ALS_wifi->_connectMod == 1) {

				_ALS_wifi->_connectMod = 0;

				// initialize_webserver();
				_webserver->serverInitialize();
				// begin_webserver();
				_webserver->serverBegin();
				_udpServer->udp_setup(9100);
				_udpServer->udpMulti_setup(9300);

				fsprintf("\n[free heap][%d]\n", ESP.getFreeHeap());

				// Serial.flush();

			   	// delay(1000);	
				// pInitHeap = ESP.getFreeHeap();
				
				ALS_lampUpdateClientPtr_get()->lampStart();

				if (wifi_connect_result("")) { 													// STA ...
					_timerEventSetupMod = 0;
					_timerEventSetup->activate();
					ALS_lampsettingLog_restore();
					fsprintf("\n[0][_timerEventSetup] : %d\n", _timerEventSetupMod);
				} else { 																		// AP 
					fsprintf("\n[1][_timerEventSetup] : %d\n", _timerEventSetupMod);
					_timerEventSetupMod 	= -1;
					_webserverLoopEnabled 	= true;
				}
				
				_ALS_wifi->_isConnect = true;		
			}
			if (_timerEventSetup->isActivate()) {
				if (_timerEventSetup->loop_stop()) { 		

					/*
						5 launch webserverloop
					*/					
					if (_timerEventSetupMod == 1) { 											
						_timerEventSetup->set_duration_max(5000);
						_timerEventSetupMod 	= -1;
						_webserverLoopEnabled 	= true;
						fsprintf("\n[1][_webserverLoopEnabled] : %d\n", _webserverLoopEnabled);	
					}

					/*
						1 _ALS_wifi->_isConnect = true;	
						2 Sta is connected;	
						3 launch sta event;	
					*/
					if (_timerEventSetupMod == 0) {				
						_timerEventSetupMod = -1;
						_iCanEvents 		= true;
						fsprintf("\n[0][_webserverLoopEnabled] : %d\n", _webserverLoopEnabled);
					}
				}
			} 
			if (_ALS_wifi->_isConnect) {
				if ((_iCanEvents) && (wifi_connect_result(""))) {

					/*
						3 launch sta event;	
						4 set timer for webserverloop
					*/			
					if (_ALS_wifi->ntpTime_isSet()) {
						if (_ALS_wifi->_eventModStart == 0) {
							_ALS_wifi->_eventModStart 	= 1; 
							_timerEventSetupMod 	= 1;
							_timerEventSetup->set_duration_max(2000);
							_timerEventSetup->activate();

							fsprintf("[envent_setup]\n");
							envent_setup();

							fsprintf("[checkConnected_start]");
							_ALS_wifi->_checkConnected_start();

						   	String timeStr;
						   	_ALS_wifi->ntpTime_getTime(timeStr);
						   	int wD, sMon, sYear;
						   	adri_timeNtp_instance()->dateGet(wD, sMon, sYear);
						   	String timeStamp = String(wD)+"/"+String(sMon)+"/"+String(sYear)+"_"+timeStr;

							String logStr = "";
							adri_toolsPtr_get()->log_read 	(logStr, false);
							adri_toolsPtr_get()->log_write 	(logStr, timeStamp);

							delay(1000);
						}
						if (_ALS_wifi->_eventModStart == 1) {
							event_loop();						
						}

					} else {
						if (_ALS_wifi->_eventModStart == 0) {
							_ALS_wifi->_eventModStart 	= -1; 
							_timerEventSetupMod 	= 1;
							_timerEventSetup->set_duration_max(2000);
							_timerEventSetup->activate();		
							fsprintf("[checkConnected_start]");
							_ALS_wifi->_checkConnected_start();
						   	String timeStr;
							_ALS_wifi->ntpTime_getTime(timeStr);	
							String logStr = "";
							adri_toolsPtr_get()->log_read 	(logStr, false);
							adri_toolsPtr_get()->log_write 	(logStr, timeStr);
							adri_toolsPtr_get()->log_read 	(logStr, true);							
						}				
					}
				}
			
				_ALS_wifi->ntpTime_loop();
				_ALS_wifi->_checkConnected_loop();	

				if(!_ALS_wifi->_otaEnabled) 	_ALS_wifi->MDSN_loop();
				else 							arduinoOTA_loop();	

				if (_webserverLoopEnabled) {
					// webserver_loop();
					_webserver->serverLoop();
					_udpServer->udp_loop();
					_udpServer->udpMulti_loop();					
				}					
			}	

			ALS_lampsettingPtr_get()->loop();

			if(ALS_lampsettingPtr_get()->_lamp_on) {
				
				if (_ALS_wifi->_isConnect && (ALS_lampsettingPtr_get()->_effect_play) && (programmeLoop_instance()->patternLoop_isPlaying())) ALS_lampsettingPtr_get()->patterns_loop();

				if(ALS_lampsettingPtr_get()->_effect_play) {
					effect_manager_instance()->loop();
				}
			} else delay(1);

		}
		void setup(const char * value, ledsBase * leds){

		    WiFi.persistent(false);
		    WiFi.disconnect(true);
		    WiFi.setPhyMode(WIFI_PHY_MODE_11B);
		    wifi_set_sleep_type(NONE_SLEEP_T);

		    boolean _fsok = LittleFS.begin();

			_timerEventSetup 	= new adri_timer(5000, "", false);

			_adri_tools 		= new adri_tools();

			_serial 			= new adriTools_serialRead();

			_ALS_wifi			= new ALS_wifi(value);

			_ledsSetting 		= new leds_setting(leds);
			leds_setting_ptr_set(_ledsSetting);
			_ledsSetting->lamp_begin();

			ALS_registerFuncExt();

			#ifdef ALS_USE_LEDSADAFRUITNEO
				fsprintf("[chooseEffectRegister_initialize ALS_USE_LEDSADAFRUITNEO]\n");
		 		chooseEffectRegister_initialize(ChooseEffectRegisterAdaNeo, ChooseEffectRegisterAdaNeoCount);
			#endif
			#ifdef ALS_USE_LEDSFASTLED
		 		fsprintf("[chooseEffectRegister_initialize ALS_USE_LEDSFASTLED]\n");
		 		chooseEffectRegister_initialize(ChooseEffectRegisterFastled, ChooseEffectRegisterFastledCount);
			#endif		
			#ifdef ALS_USE_LEDSFASTLEDRGBW
				fsprintf("[chooseEffectRegister_initialize ALS_USE_LEDSFASTLEDRGBW]\n");
		 		chooseEffectRegister_initialize(ChooseEffectRegisterFastledRGBW, ChooseEffectRegisterFastledRGWCount);
			#endif	

			_playlist       = new playlist_management(); 
			_programme      = new programme_loop(); 
			_programme->patternList_initialize(chooseEffectList_cnt);
			for (int j = 0; j < chooseEffectList_cnt; ++j) {
			    _programme->patternList_item_add(ch_toString(chooseEffectList_ptr[j].name));
			}         

			RGBeffecPalette_ptr_get()->palette_init();

			_ledsEffect 		= new effect_manager();

			_lampSetting 		= new ALS_lampsetting();
			_lampUpdateClient 	= new ALS_lampUpdateClient();

			leds_settingPtr_get()->lamp_begin();
			leds_settingPtr_get()->lamp_clear();
			ALS_lampUpdateClientPtr_get()->lampStartLed();
			leds_settingPtr_get()->lamp_bri(ALS_lampsettingPtr_get()->_lamp_bri);
		
			webserver_reponse_setup();

			_webserver = new ALS_espwebserver();
			_webserver->serverFS(_fsok);

			_udpServer 			= new ALS_udpServer();
			ALS_udpServerPtr 	= _udpServer;
			#ifdef ALS_USE_UDP	
				ALS_udp			= new adri_udp();
			#endif	
			#ifdef ALS_USE_UDPMULTI	
				ALS_udpMulti	= new adri_udpMulti();
			#endif	

			
		}


	};

		
#endif

