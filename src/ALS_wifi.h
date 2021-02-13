#ifndef ALS_wifi_h
	#define ALS_wifi_h 
	
	#ifdef WIFI_TEST
		void wifitest();
	#endif	



	#include <adri_wifiConnect.h>
	#include <adri_tools.h>
	#include <adri_espwebserver_tools.h>


		#include <adri_timeNtp.h>
		#include <adri_timer.h>

	enum mod_wifiConnect_error
	{
		mwce_none, mwce_setup
	};

	class ALS_wifi : public wifiConnect
	{
		const char 				* _hostname 			= "HELLOWORLD";

		mod_wifiConnect_error 	_error_setup 			= mwce_none;

		adri_timer 				* _checkConnected_timer = nullptr;

		int 					_connectMod_setTo		= 0;
		adri_timer 				* _connectModTimer		= nullptr;

		adri_timeNtp			* _ntpTime;

	public:

		boolean 				_isConnect 		= false;
		int 					_connectMod		= 0;
		int						_eventModStart	= 0;
		boolean					_otaEnabled 	= true;

		ALS_wifi(const char * value);
		~ALS_wifi(){fsprintf("\n[ALS_wifi delete();]\n");};

		mod_wifiConnect_error error_get();
		void 	_connectMod_set 	(int value);
		int 	_connectMod_get 	();	
		void 	_hostname_get 		(String & ret);	
		void 	_otaEnabled_set 	(boolean ret);			
		boolean _setupFromSpiff		();
		boolean _setup 				(WIFICONNECT_MOD cMod = AWC_SETUP, WIFICONNECTSSID_MOD sMod = AWCS_NORMAL);
		boolean _setupAp			(WIFICONNECT_MOD cMod = AWC_SETUP, WIFICONNECTSSID_MOD sMod = AWCS_NORMAL);
		boolean _connect 			(WIFICONNECT_MOD cMod);
		boolean _connectFromSpiff	();
		void 	_loop 				();

		void 	_checkConnected_start 	();
		boolean _checkConnected 		(int & result);
		void 	_checkConnected_loop 	();

		void 	ntpTime_setup 		();
		void 	ntpTime_loop 		();	
		void 	ntpTime_getTime 	(String & ret);
		String 	ntpTime_toString 	(time_t t);
		boolean ntpTime_isSet 		();		

	};

	ALS_wifi * ALS_wifiPtr_get();


#endif