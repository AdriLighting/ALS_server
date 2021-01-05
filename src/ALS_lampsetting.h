#ifndef ALS_LAMPSETTING_H
	#define ALS_LAMPSETTING_H
	#include <Arduino.h>
	#include <ArduinoJson.h>
	#include <adri_rgb_patterns.h>
	#include <ALS_leds_effectsLib.h>
	#include <adri_wifiConnect.h>
	#include <adri_timer.h>
	#include <ALS_espwebserver.h>

	#include "ALS_webserver.h"
	#include "ALS_eventclock.h"

	#include <adri_oled1306.h>
	#include <adri_button.h>



	ALS_udpServer * ALS_udpServer_get();
	extern ALS_udpServer * ALS_udpServerPtr;

	struct ALS_lampsettingReponse {
		const char * name;
	};	
	struct programmSettingList {
		const char * name;
		void (*getValue)(String & ret);
		void (*setValue)(String value);
		
	};	
	class programmSetting 
	{

	public:
		programme_loop 	* _programme;

		programmSetting();
		// ~programmSetting(){};

		void patternLoopKey_jsonOject 	(JsonObject & root);
		void patternLoopkey_json 		(String & root);


		void patternLoopValue_jsonOject 		(JsonObject & root);
		void patternLoopValue_json 				(String & root);
		void patternLoopNameValue_jsonOject 	(JsonObject & root);
		void patternLoopNameValue_json 			(String & out);
		void patternLoopValue_print 			();
		void patternLoopValue_loadFromJson 		();
		void patternLoopValue_cmd 				(int key, String value);

		void  	patternList_jsonObject 		(JsonObject & root);
		boolean patternList_changeByName 	(String search, int & result);
		
		void test();
	};

	class ALS_lampsetting
		{
		    struct savesRequiered {
				boolean effect = false;
				// boolean effect = false;
		    };		
			adri_timer 			* saveRequiered_timer;
			adri_timer 			* saveRequieredLogCmd_timer;
			uint32_t 			saveRequiered_timerDelay 		= 1000;
			uint32_t 			saveRequieredLogCmd_timerDelay 	= 1000;
			uint8_t 			saveRequiered_mod 			= 0;
		public:
			programmSetting 	* _prog;
			boolean 			_lamp_on 					= true;
			uint8_t 			_lamp_bri					= 255;
			boolean 			_effect_play 				= true;	

			ALS_lampsetting();
			~ALS_lampsetting(){};

			void savRequiered_pattern 	() ;
			void savRequiered_effect 	() ;
			void savRequiered_logCmd 	() ;
			void loop 					() ;

			void 	currentSetting_jsonFile 	();	
			void 	currentSetting_jsonFile 	(String & out);	

			void 	patternLoopValue_print 		();	
			void 	patternLoop_json 			(String & out);	

			void 	patternList_print 			();	
			void 	patternList_jsonFile 		();	
			boolean patternList_changeByName 	(String value, int & result) ;	
			
			void patterns_loop();

			void playList_jsonFile();	

			void effect_changeByName 						(String s);	
			void effect_print 								();
			void effect_getValueByKey 						(String key, String & value);
			void effect_getValueByKey 						(const char * key, String & value);
			void effect_returnSimpleValueByKey_jsonOject	(JsonObject & root, String key, String cmd);
			void effect_returnSimpleValueByKey_json 		(String & out, String key, String cmd);
			void effect_returnSimpleValueByKey 				(String & out, String key, String cmd);
			void effect_setvalueFromKey						(String key, String value);		
			void effect_json 								(String & out);
			void effect_color 								();
			void effect_returnSingleValueByKey_jsonOject 	(JsonObject & root, String key, String cmd);
			void effect_jsonOject 							(JsonObject & root);


		};	
		ALS_lampsetting * ALS_lampsettingPtr_get();

	class ALS_lampUpdateClient
	{
		ALS_lampsetting 	* _lampSetting;
	public:
		ALS_lampUpdateClient();
		~ALS_lampUpdateClient(){};

		void andoid_effectSetting 		 		(effectSettingReturn_mod mod, String cmdJson, String cmd, String & result);
		void android_generalSetting 			(String & out, String txt = "");
		void android_generalSetting_jsonObject 	(JsonObject & root);
		void android_chat 						(String txt = "");

		void httpPage_wifi						();
		void httpPage_json						();

		void patterns_loop 						(String txt = "effect suivant");

		void lampStart							();
		void lampStartLed						();


		// #ifdef MRLCALENDAR
		void android_eventSelect(int pos, String & out);
		// #endif		
	};
	ALS_lampUpdateClient * ALS_lampUpdateClientPtr_get();

	void parse(String op, String value);
	void parse(String msg);
	void parse_multi(String msg);	
	void webserver_reponse_setup();


	class lampPeripherals
	{
			int _mod 	= 1; 
			
			int _oled 	= 0; // 1 : ssd1306 128x64 interne
			int _button = 0; // 1 : lc.sc tw + lc.sc hue // 2 : lc.sc tw + lc.sc hue  + sc on/off
			int _leds	= 0; // 0 : none

			int _btTw 		= -1;
			int _btHue 		= -1;
			int _btOnOff	= -1;

			adriOled1306_driver 	* _my_screen 	= nullptr;


	public:
			adri_timer * 	_timer_potar 		= nullptr;
			adri_timer * 	_timer_clear		= nullptr;
			unsigned long 	_timer_clear_delay	= 0;
			unsigned long 	_timer_potar_delay	= 8000;
			uint8_t 		_loop_potar 		= 0;	

		lampPeripherals(int mod);
		void loop();
		
		void button_tw_create(
			int 	pin,
			boolean pullup, 
			int 	input, 
			int 	delay
		);	
		void button_hue_create(
			int 	pin,
			boolean pullup, 
			int 	input, 
			int 	delay
		);	
		void button_onOff_create(
			int 	pin,
			boolean pullup, 
			int 	input 
		);						
		void button_setFunc(int bt, click_func f1, click_func f2) ;	

		void wificonnect(String value);
		
		void wifiPrint();
		void colorPrintLbl(String v);
		void colorPrintValue();
		void oled_clear();

		void Tft9341_ceate();

		~lampPeripherals();
		
	};
	adriOled1306_driver * myOledInstance();
	lampPeripherals 	* lampPeripheralsInstance();

class ALS_lampsettingLog
{

public:
	String 	_op 	= "";
	int 	_cnt 	= 1;	

	ALS_lampsettingLog(String op);
	ALS_lampsettingLog(String op, int cnt);
	~ALS_lampsettingLog(){};
};
void ALS_lampsettingLog_print();
void ALS_lampsettingLog_restore();
#endif