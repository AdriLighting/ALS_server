#include "ALS_lampsetting.h"
#include <adri_tools.h>
#include <ALS_fnc.h>
#include <adri_webserver_reponselist.h>
#include <adri_webserver_reponse.h>
#include <adri_webserver_parsecommand.h>
#include "ALS_wifi.h"


// #define DEBUG

namespace {
	uint16_t potvale = 0;
	const size_t serializeSize = 512 * 20;



}
	// ALS_main * ALS_mainPtr;
	// void ALS_register(ALS_main*ptr){ALS_mainPtr=ptr;}

	ALS_udpServer * ALS_udpServerPtr;
	ALS_udpServer * ALS_udpServer_get(){return ALS_udpServerPtr;}



	const char PROGMEM settingPatternLoop_pos			[] = "pos";
	const char PROGMEM settingPatternLoop_posMin		[] = "posMin";
	const char PROGMEM settingPatternLoop_posMax		[] = "posMax";
	const char PROGMEM settingPatternLoop_delay			[] = "delay";
	const char PROGMEM settingPatternLoop_delayCurrent 	[] = "delayCurrent";
	const char PROGMEM settingPatternLoop_delayManual	[] = "delayManual";
	const char PROGMEM settingPatternLoop_play			[] = "play";
	const char PROGMEM settingPatternLoop_playRnd		[] = "random";
	const char PROGMEM settingPatternLoop_currentName 	[] = "currentName";

void setting_set (String value){;}

void setting_setEffect (String value){
	int p_pos = -1;
	if (ALS_lampsettingPtr_get()->patternList_changeByName(value, p_pos)){}
}

void setting_patternLoop_pos_set 			(String value){programmeLoop_instance()->patternLoop_pos_set(value.toInt());}
void setting_patternLoop_posMin_set 		(String value){}
void setting_patternLoop_posMax_set 		(String value){}
void setting_patternLoop_delay_set 			(String value){programmeLoop_instance()->patternLoop_delay_set(value.toInt());}
void setting_patternLoop_delayCurrent_set	(String value){programmeLoop_instance()->patternLoop_delayCurrent_set(map(value.toInt(),0,255,5,255));}
void setting_patternLoop_delayManual_set 	(String value){programmeLoop_instance()->patternLoop_delayManual_set(stringToBool(value.toInt()));}
void setting_patternLoop_isPlaying_set 		(String value){programmeLoop_instance()->patternLoop_play_set(stringToBool(value.toInt()));}
void setting_patternLoop_isRndPlaying_set 	(String value){programmeLoop_instance()->patternLoop_playRnd_set(stringToBool(value.toInt()));}

void setting_patternLoop_pos_get 			(String & ret){uint8_t 	value;programmeLoop_instance()->patternLoop_pos_get(value);			ret = String(value);}
void setting_patternLoop_posMin_get 		(String & ret){uint8_t 	value;programmeLoop_instance()->patternLoop_posMin_get(value);		ret = String(value);}
void setting_patternLoop_posMax_get 		(String & ret){uint8_t	value;programmeLoop_instance()->patternLoop_posMax_get(value);		ret = String(value);}
void setting_patternLoop_delay_get 			(String & ret){uint32_t value;programmeLoop_instance()->patternLoop_delay_get(value);			ret = String(value);}
void setting_patternLoop_delayCurrent_get 	(String & ret){uint32_t value;programmeLoop_instance()->patternLoop_delayCurrent_get(value);	ret = String(value);}
void setting_patternLoop_delayManual_get 	(String & ret){boolean 	value;programmeLoop_instance()->patternLoop_delayManual_get(value);	ret = String(value);}
void setting_patternLoop_isPlaying 			(String & ret){boolean 	value = programmeLoop_instance()->patternLoop_isPlaying();			ret = String(value);}
void setting_patternLoop_isRndPlaying 		(String & ret){boolean 	value = programmeLoop_instance()->patternLoop_isRndPlaying();			ret = String(value);}
void setting_patternLoop_currentName 		(String & ret){
	uint8_t value;
	programmeLoop_instance()->patternLoop_pos_get(value);
	programmeLoop_instance()->patternList_item_getName(ret, value);}

PROGMEM programmSettingList  patternloopSettingList [] = { 
	{settingPatternLoop_pos, 			setting_patternLoop_pos_get, 			setting_patternLoop_pos_set},
	{settingPatternLoop_posMin, 		setting_patternLoop_posMin_get, 		setting_patternLoop_posMin_set},
	{settingPatternLoop_posMax, 		setting_patternLoop_posMax_get, 		setting_patternLoop_posMax_set},
	{settingPatternLoop_delay, 			setting_patternLoop_delay_get, 			setting_patternLoop_delay_set},
	{settingPatternLoop_delayCurrent, 	setting_patternLoop_delayCurrent_get, 	setting_patternLoop_delayCurrent_set},
	{settingPatternLoop_delayManual, 	setting_patternLoop_delayManual_get, 	setting_patternLoop_delayManual_set},
	{settingPatternLoop_play, 			setting_patternLoop_isPlaying, 			setting_patternLoop_isPlaying_set},
	{settingPatternLoop_playRnd,		setting_patternLoop_isRndPlaying,		setting_patternLoop_isRndPlaying_set},
	{settingPatternLoop_currentName,	setting_patternLoop_currentName,		setting_setEffect},
};
uint8_t patternloopSettingListCount = ARRAY_SIZE(patternloopSettingList);

PROGMEM programmSettingList  generalSettingList [] = { 
	{settingPatternLoop_pos, 			setting_patternLoop_pos_get, 			setting_patternLoop_pos_set},
	{settingPatternLoop_posMin, 		setting_patternLoop_posMin_get, 		setting_patternLoop_posMin_set},
};
uint8_t generalSettingListCount = ARRAY_SIZE(generalSettingList);



programmSetting::programmSetting() {
	_programme = programmeLoop_instance();

}

// programmSetting::Test tt[] = 
// {
//     { settingPatternLoop_pos, 		&programmSetting::f1},
//     { settingPatternLoop_posMin, 	&programmSetting::f2},
//     { "Function f3", &programmSetting::f3},
//     {0, 0}
// };

void programmSetting::patternLoopKey_jsonOject(JsonObject & root){
    for (int i = 0; i < patternloopSettingListCount; ++i) {
    	root[ch_toString(patternloopSettingList[i].name)] = String(i);
    }
}
void programmSetting::patternLoopkey_json(String & out) {
	DynamicJsonDocument json(serializeSize);
	JsonObject root = json.to<JsonObject>();

    JsonObject object = root.createNestedObject(F("patlokey"));
	patternLoopKey_jsonOject(object);

	serializeJson(json, out);
}

void programmSetting::patternLoopValue_jsonOject(JsonObject & root){
    JsonObject patternLoop = root.createNestedObject(F("patlo"));
    for (int i = 0; i < patternloopSettingListCount; ++i) {
    	String value = "";
    	patternloopSettingList[i].getValue(value);
    	patternLoop[String(i)] = value;
    }
}
void programmSetting::patternLoopValue_json(String & out) {
	DynamicJsonDocument json(serializeSize);
	JsonObject root = json.to<JsonObject>();
	patternLoopValue_jsonOject(root);
	serializeJson(json, out);
}

void programmSetting::patternLoopNameValue_jsonOject(JsonObject & root){
    JsonObject patternLoop = root.createNestedObject(F("patlo"));
    for (int i = 0; i < patternloopSettingListCount; ++i) {
    	String value = "";
    	patternloopSettingList[i].getValue(value);
    	patternLoop[ch_toString(patternloopSettingList[i].name)] = value;
    }
}
void programmSetting::patternLoopNameValue_json(String & out) {
	DynamicJsonDocument json(serializeSize);
	JsonObject root = json.to<JsonObject>();
	patternLoopNameValue_jsonOject(root);
	serializeJson(json, out);
}

void programmSetting::patternLoopValue_print(){

	fsprintf("\n[programmSetting::patternLoopValue_print]\n");

    for (int i = 0; i < patternloopSettingListCount; ++i)
    {
    	String value = "";
    	patternloopSettingList[i].getValue(value);
    	fsprintf("[%5d][%-20s][%-10s]\n", i, patternloopSettingList[i].name, value.c_str());
    }

}
void programmSetting::patternLoopValue_cmd(int key, String value){

	fsprintf("\n[programmSetting::patternLoopValue_cmd]\n\tkey : %d - value : %s\n", key, value.c_str());

    for (int i = 0; i < patternloopSettingListCount; ++i)
    {
    	if (i == key) {patternloopSettingList[i].setValue(value);break;}
    }

}
void programmSetting::patternLoopValue_loadFromJson(){

	#ifdef DEBUG
		fsprintf("\n[programmSetting::patternLoopValue_load]\n");
	#endif

        File file = SPIFFS.open("/json/currentSetting.json", "r");
        if (file) {
            DynamicJsonDocument buff(serializeSize);
            DeserializationError err = deserializeJson(buff, file);
            if (err) {
                Serial.print(F("[programmSetting::patternLoopValue_load] FLASHFS Error parsing settings json file: "));
                Serial.println(err.c_str());
                file.close(); 
                return;
            }
            // fsprintf("\n[effectName : %s]\n", name.c_str());
            JsonObject oject = buff["patterns"]["patlo"];
            String key;
            String value;
		    for (int i = 0; i < patternloopSettingListCount; ++i)
		    {
	            key = ch_toString(patternloopSettingList[i].name);
	            if (oject.containsKey(key)) {
	                value = oject[key].as<String>();
	                patternloopSettingList[i].setValue(value);
	                #ifdef DEBUG
	                	fsprintf("key: %15s value: %10s\n", key.c_str(),value.c_str());
	                #endif
	            }
		    }

            file.close(); 
        }

#ifdef DEBUG
    patternLoopValue_print();
#endif
}


boolean programmSetting::patternList_changeByName(String search, int & result){
	_programme->pattern_list_changeEffect(search, result);
}

void programmSetting::patternList_jsonObject(JsonObject & root){
	_programme->patternList_jsonObject(root);
}

void programmSetting::test(){
	// fsprintf("\ntest\n");
}


ALS_lampsetting * ALS_lampsettingPtr;
ALS_lampsetting * ALS_lampsettingPtr_get() {return ALS_lampsettingPtr;}
ALS_lampsetting::ALS_lampsetting(){
	ALS_lampsettingPtr = this;
	_prog = new programmSetting();
	saveRequiered_timer = new adri_timer(saveRequiered_timerDelay, "", false);
}

void ALS_lampsetting::loop() {
	if (saveRequiered_timer->isActivate()) {

		if (saveRequiered_timer->loop(saveRequiered_timerDelay)) {

			if (saveRequiered_mod == 0) {
				// currentSetting_jsonFile();

				// String name;
				// effect_manager_instance()->getEffectName(name);	
				// fsprintf("\n[saveRequiered_timer MOD 0] : %s\n",name.c_str());	
				// fsprintf("\n[lampSetting::loop] effect_manager_sav\n");
				uint8_t pos = effect_manager_instance()->activeEffectIndex();
				effect_manager_sav(pos);
		
			}
			if (saveRequiered_mod == 1) {
				// fsprintf("\n[lampSetting::loop] currentSetting_jsonFile\n");
				currentSetting_jsonFile();
				// String name;
				// effect_manager_instance()->getEffectName(name);	
				// fsprintf("\n[saveRequiered_timer MOD 1] : %s\n",name.c_str());				
			}

			// lampUpdateClientInctance()->android_chat(name + " vient d'étre sauvegardé");	

			saveRequiered_mod = 0;
			saveRequiered_timer->activate(false);
		}
	}

}
void ALS_lampsetting::savRequiered_pattern() {

	saveRequiered_mod = 1;

	if (saveRequiered_timer->isActivate()) {
		saveRequiered_timerDelay+=150;
		if (saveRequiered_timerDelay>30000){
			saveRequiered_timerDelay=3000;
			#ifdef DEBUG
				fsprintf("\n[lampSetting::savRequiered_pattern] RESETTIMER\n");
			#endif
		}
	} else {
		saveRequiered_timerDelay=3000;
		saveRequiered_timer->activate();
	}
}
void ALS_lampsetting::savRequiered_effect() {
	
	saveRequiered_mod = 0;

	if (saveRequiered_timer->isActivate()) {
		saveRequiered_timerDelay+=150;
		if (saveRequiered_timerDelay>30000){
			saveRequiered_timerDelay=3000;
			#ifdef DEBUG
				fsprintf("\n[lampSetting::savRequiered_effect] RESETTIMER\n");
			#endif
		}
	} else {
		saveRequiered_timerDelay=3000;
		saveRequiered_timer->activate();
	}
}

void patternLoopNameValue_jsonOject(JsonObject & root){
    JsonObject patternLoop = root.createNestedObject(F("patlo"));
    for (int i = 0; i < patternloopSettingListCount; ++i) {
    	String value = "";
    	patternloopSettingList[i].getValue(value);
    	patternLoop[ch_toString(patternloopSettingList[i].name)] = value;
    }
}
void ALS_lampsetting::currentSetting_jsonFile(){
	#ifdef DEBUG
		fsprintf("\n[lampSetting::currentSetting_jsonFile To SPIFF]\n");
	#endif
    File file = SPIFFS.open("/json/currentSetting.json", "w");
    if (file) {

		DynamicJsonDocument json(2000);
		JsonObject root = json.to<JsonObject>();

		JsonObject object = root.createNestedObject("patterns");
		patternLoopNameValue_jsonOject(object);

		effectSetting_toJson_keyInNumber = false;
		object = root.createNestedObject("effect");		
		effect_manager_instance()->settingToJson(object);
		effectSetting_toJson_keyInNumber = true;

		object = root.createNestedObject("playlist");		
		playlist_instance()->json_objectName(object);

		object = root.createNestedObject("general");		
		object[F("lp")] 		= String(_lamp_on);
		object[F("bri")] 		= String(_lamp_bri);

		serializeJson(json, file);
		#ifdef DEBUG
			serializeJson(json, Serial);
		#endif
		file.close();	
    }
}
void ALS_lampsetting::currentSetting_jsonFile(String & out){
		#ifdef DEBUG
			fsprintf("\n[lampSetting::currentSetting_jsonFile]\n");
		#endif

		DynamicJsonDocument json(2000);
		JsonObject root = json.to<JsonObject>();

		JsonObject object = root.createNestedObject("patterns");
		patternLoopNameValue_jsonOject(object);

		effectSetting_toJson_keyInNumber = false;
		object = root.createNestedObject("effect");		
		effect_manager_instance()->settingToJson(object);
		effectSetting_toJson_keyInNumber = true;

		object = root.createNestedObject("playlist");		
		playlist_instance()->json_objectName(object);

		object = root.createNestedObject("general");		
		object[F("lp")] 		= String(_lamp_on);
		object[F("bri")] 		= String(_lamp_bri);

		serializeJson(json, out);
		#ifdef DEBUG
			serializeJson(json, Serial);
		#endif	
}


void ALS_lampsetting::patternLoopValue_print() 	{_prog->patternLoopValue_print();}

void ALS_lampsetting::patternLoop_json(String & ret) {
	#ifdef DEBUG
		fsprintf("\n[lampSetting::patternLoop_json]\n");
	#endif	

	DynamicJsonDocument json(serializeSize);
	JsonObject root = json.to<JsonObject>();

	_prog->patternLoopValue_jsonOject(root);

	serializeJson(json, ret);
	#ifdef DEBUG
		serializeJson(json, Serial);
	#endif	
}


void ALS_lampsetting::patternList_print() {programmeLoop_instance()->patternList_print();}

void ALS_lampsetting::patternList_jsonFile() {
	#ifdef DEBUG
		fsprintf("\n[lampSetting::patternList_jsonFile]\n");
	#endif
    File file = SPIFFS.open("/json/patternList.json", "w");
    if (file) {
		DynamicJsonDocument json(10000);
		JsonObject root = json.to<JsonObject>();

		JsonObject object = root.createNestedObject(F("list"));
		_prog->patternList_jsonObject(object);

		serializeJson(json, file);
		#ifdef DEBUG
			serializeJson(json, Serial);
		#endif			
		file.close();
    }
}

boolean ALS_lampsetting::patternList_changeByName(String value, int & result) {
	if (!programmeLoop_instance()->pattern_list_changeEffect(value, result)) return false;
	_effect_play = true;
	effect_manager_instance()->changeEffectByName(value);
	return true;
}


void ALS_lampsetting::playList_jsonFile() {
	#ifdef DEBUG
		fsprintf("\n[lampSetting::playList_jsonFile]\n");
	#endif
    File file = SPIFFS.open("/json/playList.json", "w");
    if (file) {
		DynamicJsonDocument json(serializeSize);
		JsonObject root = json.to<JsonObject>();

		JsonObject object = root.createNestedObject("list");
		playlist_instance()->item_jsonObject(object);

		serializeJson(json, file);
		#ifdef DEBUG
			serializeJson(json, Serial);
		#endif		
		file.close();
    }
}

void ALS_lampsetting::effect_changeByName(String search) 	{effect_manager_instance()->changeEffectByName(search);}

void ALS_lampsetting::effect_print() 						{effect_manager_instance()->settingToSerial();}

void ALS_lampsetting::effect_getValueByKey(String key, String & value)			{effect_manager_instance()->settingGetValueByKey(key, value);}
void ALS_lampsetting::effect_getValueByKey(const char * key, String & value)	{effect_manager_instance()->settingGetValueByKey(key, value);}
void ALS_lampsetting::effect_returnSimpleValueByKey_jsonOject(JsonObject & root, String key, String cmd) {
		String result = "";
		effect_getValueByKey(key, result);
		JsonObject effect = root.createNestedObject(F("rd"));
		effect[cmd] = result;
}
void ALS_lampsetting::effect_returnSingleValueByKey_jsonOject(JsonObject & root, String key, String cmd) {
        String result = "";
        effect_getValueByKey(key, result);
        root[cmd] = result;
}
void ALS_lampsetting::effect_returnSimpleValueByKey_json(String & out, String key, String cmd) {
    #ifdef DEBUG
        fsprintf("\n[effectSetting::effect_returnSimpleValueByKey_json]\n");
    #endif       
    const size_t capacity = 4*JSON_OBJECT_SIZE(1);    
	DynamicJsonDocument json(capacity);
	JsonObject root = json.to<JsonObject>();
	effect_returnSimpleValueByKey_jsonOject(root, key, cmd);
    serializeJson(json, out);
    #ifdef DEBUG
        serializeJson(json, Serial);
    #endif        
}

void ALS_lampsetting::effect_returnSimpleValueByKey(String & out, String key, String cmd) {
	effect_returnSimpleValueByKey_json(out, key, cmd);}

void ALS_lampsetting::effect_setvalueFromKey(String key, String value) {
	_effect_play = true;
	effect_manager_instance()->settingFromSerial(key, value, true);}

void ALS_lampsetting::effect_json(String & out) {
    #ifdef DEBUG
        fsprintf("\n[effectSetting::effect_json]\n");
    #endif    
    const size_t capacity = 512 * 4;
	DynamicJsonDocument json(capacity);
	JsonObject root = json.to<JsonObject>();
    effect_manager_instance()->settingToJson(root);
    serializeJson(json, out);
    #ifdef DEBUG
        serializeJson(json, Serial);
    #endif      
}	

void ALS_lampsetting::effect_color() {
    String name = "";
    effect_manager_instance()->getEffectName(name);
    if (name != ch_toString(chooseEffect_color)){
        int p_pos = -1;
       	patternList_changeByName(ch_toString(chooseEffect_color), p_pos);
        uint8_t pos = effect_manager_instance()->activeEffectIndex();
        effect_manager_sav(pos);        
    }   
}

void ALS_lampsetting::effect_jsonOject(JsonObject & root) {
	effect_manager_instance()->settingToJson(root);}



ALS_lampUpdateClient * ALS_lampUpdateClientPtr;
ALS_lampUpdateClient * ALS_lampUpdateClientPtr_get() {return ALS_lampUpdateClientPtr;}
ALS_lampUpdateClient::ALS_lampUpdateClient(){
	ALS_lampUpdateClientPtr= this;
	_lampSetting = ALS_lampsettingPtr_get();
}

void ALS_lampUpdateClient::andoid_effectSetting(effectSettingReturn_mod mod, String cmdJson, String cmd, String & result){
	
	#ifdef DEBUG
		fsprintf("\n[lampUpdateClient::andoid_effectSetting]\n");
	#endif

	effectSettingString_reponse * ptr;
	int ptrCnt;
	String key;
	String getCmd;

	if (mod == esrm_multiple){

		DynamicJsonDocument json(255);
		JsonObject root = json.to<JsonObject>();
		JsonObject effect = root.createNestedObject(F("rd"));		
			// fsprintf("\t[esrm_multiple]cmdJson: %s, cmd: %s\n", cmdJson.c_str(), cmd.c_str());
		for (int i = 0; i < effectSettingString_fullListCount; ++i) {
			if (String(i) == cmd) {
				ptr 	= effectSettingString_fullList[i].list;

				// uint8_t ptr_count = ARRAY_SIZE(ptr);
				// fsprintf("\t[ptr]listcnt: %d\n", ptr_count);

				ptrCnt	= effectSettingString_fullList[i].cnt;
				// fsprintf("\t[ptrCnt]listcnt: %d\n", ptrCnt);

				for (int k = 0; k < ptrCnt; ++k) {

					key = ch_toString(ptr[k].id);

					// fsprintf("\t[%d] %s\n", k, key.c_str());

					effectSettingStringList_keyToJson(key, getCmd);

					// fsprintf("\t\t cmd: %s\n",  getCmd.c_str());

					_lampSetting->effect_returnSingleValueByKey_jsonOject(effect, key, getCmd);
				}

				serializeJson(json, result);

				#ifdef DEBUG
					serializeJson(json, Serial);
				#endif	

				break;						
			}
		}
		return;	
	}


	switch(mod) {
		case esrm_value:
			_lampSetting->effect_returnSimpleValueByKey(result, cmdJson , cmd);
			#ifdef DEBUG
				fsprintf("\t%s\n", result.c_str());	
			#endif
		break;
		case esrm_full: 
			_lampSetting->effect_json(result);
		break;
	}

}

void ALS_lampUpdateClient::android_generalSetting(String & out, String txt) {
	#ifdef DEBUG
		fsprintf("\n[lampUpdateClient::android_generalSetting]\n");
	#endif

	DynamicJsonDocument json(serializeSize);
	JsonObject root = json.to<JsonObject>();

	root[F("txt")]	= txt;

	JsonObject object = root.createNestedObject("patterns");
	_lampSetting->_prog->patternLoopValue_jsonOject(object);

	object = root.createNestedObject("effect");		
	_lampSetting->effect_jsonOject(object);

	object = root.createNestedObject("playlist");		
	playlist_instance()->json_object(object);

	object = root.createNestedObject("general");		
	object[F("lp")] 		= String(_lampSetting->_lamp_on);
	object[F("bri")] 		= String(_lampSetting->_lamp_bri);

	serializeJson(json, out);	
	#ifdef DEBUG
		serializeJson(json, Serial);
	#endif	
}
void ALS_lampUpdateClient::android_generalSetting_jsonObject(JsonObject & root) {

	JsonObject object = root.createNestedObject("patterns");
	_lampSetting->_prog->patternLoopValue_jsonOject(object);

	object = root.createNestedObject("effect");		
	_lampSetting->effect_jsonOject(object);

	object = root.createNestedObject("playlist");		
	playlist_instance()->json_object(object);

	object = root.createNestedObject("general");		
	object[F("lp")] 		= String(_lampSetting->_lamp_on);
	object[F("bri")] 		= String(_lampSetting->_lamp_bri);
}
// #ifdef MRLCALENDAR
void ALS_lampUpdateClient::android_eventSelect(int pos, String & out) {
	#ifdef DEBUG
		fsprintf("\n[lampUpdateClient::android_eventSelect]\n");
	#endif

	DynamicJsonDocument json(serializeSize);
	JsonObject root = json.to<JsonObject>();

	JsonObject object = root.createNestedObject("eventSelect");
	eventsDetails_jsonObject(pos, object) ;

	serializeJson(json, out);	
	#ifdef DEBUG
		serializeJson(json, Serial);
	#endif
}
// #endif
void ALS_lampUpdateClient::android_chat(String txt){
	#ifdef DEBUG
		fsprintf("\n[lampUpdateClient::android_chat]\n");
	#endif

	String rep = "";

	DynamicJsonDocument json(serializeSize);
	JsonObject root = json.to<JsonObject>();
	root[F("chat")]	= txt;
	serializeJson(json, rep);	
	ALS_udpServer_get()->udp_send(rep);
	// lampServer_instance()->udpmultiserver_instance()->send(rep);
	#ifdef DEBUG
		serializeJson(json, Serial);
	#endif	
}

void ALS_lampUpdateClient::patterns_loop(String txt){
	#ifdef DEBUG
		fsprintf("\n[lampUpdateClient::patterns_loop]\n");
	#endif

	String rep = "";

	DynamicJsonDocument json(serializeSize);
	JsonObject root = json.to<JsonObject>();
	root[F("txt")]	= txt;
	android_generalSetting_jsonObject(root);
	serializeJson(json, rep);	
	ALS_udpServer_get()->udp_send(rep);
	ALS_udpServer_get()->udpMulti_send(rep);
	#ifdef DEBUG
		serializeJson(json, Serial);
	#endif	
}

void ALS_lampUpdateClient::httpPage_wifi(){
	#ifdef DEBUG
		fsprintf("\n[lampUpdateClient::httpPage_wifi]\n");
	#endif

	String rep = "";
	DynamicJsonDocument json(serializeSize);
	JsonObject root = json.to<JsonObject>();
	JsonObject object = root.createNestedObject("wifiid");
	object[F("c_ssid")]	= wifiConnect_instance()->currentSSID_get();
	object[F("c_ip")]	= wifiConnect_instance()->currentIp_get();
	for (int i = 0; i < wifiConnect_getValues_count; ++i)
	{
		object[ch_toString(wifiConnect_getValues[i].name)] = wifiConnect_getValues[i].getValue(wifiConnect_instance()); 
	}
	serializeJson(json, rep);	
	#ifdef DEBUG
		serializeJson(json, Serial);
	#endif
		
	ALS_espwebserverPtr_get()->http_sendJson(rep);
}
void ALS_lampUpdateClient::httpPage_json(){

	String rep = "";
	// DynamicJsonDocument json(serializeSize);
	// JsonObject root 	= json.to<JsonObject>();
	// android_generalSetting_jsonObject(root);
	// serializeJson(json, rep);	
	ALS_lampsettingPtr_get()->currentSetting_jsonFile(rep);
	// fsprintf("\n[lampUpdateClient::httpPage_json]\n\t%s\n", rep.c_str());

	ALS_espwebserverPtr_get()->http_sendJson(rep);

}
void ALS_lampUpdateClient::lampStart(){
	// _lampSetting->patternLoop_jsonInfo_jsonFile();
	#ifdef DEBUG
		fsprintf("\n[lampUpdateClient::lampStart]\n");
		fsprintf("\n[lampUpdateClient::currentSetting_jsonFile]\n");
	#endif
	ALS_lampsettingPtr_get()->currentSetting_jsonFile();
	#ifdef DEBUG
		fsprintf("\n[lampUpdateClient::playList_jsonFile]\n");
	#endif
	ALS_lampsettingPtr_get()->playList_jsonFile();
	#ifdef DEBUG
		fsprintf("\n[lampUpdateClient::patternList_jsonFile]\n");
	#endif
	ALS_lampsettingPtr_get()->patternList_jsonFile();

	// _lampSetting->effect_allEffectSavValue_jsonFile();
	// _lampSetting->effect_allEffectValue_jsonFile();
	// _lampSetting->effect_jsonInfo_jsonFile();
}

void ALS_lampUpdateClient::lampStartLed(){
    String p_name;
	uint8_t pos = 0;



    File file = SPIFFS.open("/json/currentSetting.json", "r");
    if (file) {
    	#ifdef DEBUG
			fsprintf("\n[lampUpdateClient::lampStartLed] file open\n");
		#endif
        DynamicJsonDocument buff(serializeSize);
        DeserializationError err = deserializeJson(buff, file);
        if (err) {
        	#ifdef DEBUG
            	Serial.print(F("FLASHFS Error parsing settings json file: currentSetting.json"));
            #endif
            Serial.println(err.c_str());
        } else {
            JsonObject oject = buff["general"];
			if (oject.containsKey("bri")) 	ALS_lampsettingPtr_get()->_lamp_bri = oject[F("bri")];
			if (oject.containsKey("lp")) 	{
				String value = oject[F("lp")].as<String>();
				ALS_lampsettingPtr_get()->_lamp_on = stringToBool(value);
				// fsprintf("\n[lp val]: %s\n", value.c_str());
				// fsprintf("[lp]: %d\n", ALS_lampsettingPtr_get()->_lamp_on);
			}
			
        }
        file.close(); 

    } else {

	   	programmeLoop_instance()->patternList_item_getName(p_name, 0);
	   	effect_manager_instance()->changeEffectByName(p_name);    
   		#ifdef DEBUG
			fsprintf("\n[lampUpdateClient::lampStartLed] set default tw\n");
		#endif
		ALS_lampsettingPtr_get()->effect_color();
		ALS_lampsettingPtr_get()->effect_setvalueFromKey(ch_toString(settingEffectKey_tw), String(255));
    }
    #ifdef DEBUG
		fsprintf("\n[lampUpdateClient::patternLoopValue_loadFromJson]\n");
	#endif
	ALS_lampsettingPtr_get()->_prog->patternLoopValue_loadFromJson();

   	programmeLoop_instance()->patternLoop_pos_get(pos);
   	programmeLoop_instance()->patternList_item_getName(p_name, pos);
   	effect_manager_instance()->changeEffectByName(p_name);
   	
	if (!ALS_lampsettingPtr_get()->_lamp_on) ALS_fnc_lampClear();

}
void webserver_cmd_patlo(String msg){
	String json;
	ALS_lampsettingPtr_get()->patternLoop_json(json);
	ALS_espwebserverPtr_get()->http_sendJson(json);
}
void parse_effect_setting(String url){
	if (!ALS_lampsettingPtr_get()->_lamp_on) return;

	String cmd 		= "";
	String cmdJson 	= "";
	String value 	= "";	
	String rep		= "";
	effectSettingReturn_mod mod = esrm_value;
	adriTools_serialReadPtr_get()->splitText(url, "&", cmd,  value) ;
	effectSettingStringList_keyFromJson 	(cmd, cmdJson);
	effectSettingStringList_retGrpFromJson 	(cmd, mod);
	ALS_lampsettingPtr_get()->effect_setvalueFromKey(cmdJson, value);
	ALS_lampUpdateClientPtr_get()->andoid_effectSetting(mod, cmdJson, cmd, rep);
	ALS_udpServer_get()->udp_send(rep);
	ALS_udpServer_get()->udpMulti_send(rep);
	ALS_lampsettingPtr_get()->savRequiered_effect();
}

void parse_effect_choose(String url){
	if (!ALS_lampsettingPtr_get()->_lamp_on) return;

	String 	cmd 		= "";
	String 	cmdJson 	= "";
	String 	value 		= "";	
	int 	p_pos 		= -1;
	String 	rep			= "";
	String 	name		= "";

	adriTools_serialReadPtr_get()->splitText(url, "&", cmd,  value) ;
	if (cmd == "change") {
		if (value!="") 	{
			programmeLoop_instance()->pattern_list_changeEffect(true, name);
			effect_manager_instance()->changeEffectByName(name);
		}
		else {
			programmeLoop_instance()->pattern_list_changeEffect(false, name);
			effect_manager_instance()->changeEffectByName(name);
		}
		ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "");
		ALS_udpServer_get()->udp_send(rep);
		ALS_udpServer_get()->udpMulti_send(rep);
		ALS_lampsettingPtr_get()->savRequiered_pattern();
	} else {
		if (ALS_lampsettingPtr_get()->patternList_changeByName(value, p_pos)){
			ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "");
			ALS_udpServer_get()->udp_send(rep);
			ALS_udpServer_get()->udpMulti_send(rep);
			ALS_lampsettingPtr_get()->savRequiered_pattern();
		}		
	}
}

void parse_screen_initialize(String url){
	String cmd 		= "";
	String value 	= "";	
	String rep 		= "";	

	// fsprintf("\n[parse_screen_initialize]\n\t%s\n",url.c_str());
	adriTools_serialReadPtr_get()->splitText(url, "&", cmd,  value) ;
	int pos = value.toInt();
	if (cmd == "colorpicker") {
			ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "");
			ALS_udpServer_get()->udp_send(rep);
			ALS_udpServer_get()->udpMulti_send(rep);
			return;		
	}
	switch(pos) {
		case 0:
			ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "Bienvenue");
			ALS_udpServer_get()->udp_send(rep);
			ALS_udpServer_get()->udpMulti_send(rep);
		break;

		case 1: 
			ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "Bienvenue");
			ALS_udpServer_get()->udp_send(rep);
			ALS_udpServer_get()->udpMulti_send(rep);
		break;
	}
}

const char PROGMEM req_general_lpOn [] = "lp_on";
const char PROGMEM req_general_bri 	[] = "bri";
int command_lampOn(String value){
	ALS_lampsettingPtr_get()->_lamp_on 	= !ALS_lampsettingPtr_get()->_lamp_on;
	// fsprintf("\n[_lamp_on]\n\t%d\n", ALS_lampsettingPtr_get()->_lamp_on);
	if (!ALS_lampsettingPtr_get()->_lamp_on){
		// uint8_t pos = effect_manager_instance()->activeEffectIndex();
		// effect_manager_sav(pos);	
		ALS_fnc_lampClear();
	} else {
		effectIdInstance()->loadSettingFromFile("/effectsSetting/");
		ALS_lampsettingPtr_get()->_effect_play = true;
	}
} 
int command_lampBri(String value){
	if (!ALS_lampsettingPtr_get()->_lamp_on) return 0;
	ALS_lampsettingPtr_get()->_lamp_bri 	= value.toInt();
	ALS_fnc_lampBri(value.toInt());
}
const char PROGMEM req_general_autoplay 	[] = "autoplay";
int command_lampAutoPLay(String value){
	programmeLoop_instance()->patternLoop_autoPlay();
	// ALS_lampsettingPtr_get()->currentSetting_jsonFile();
}
const char PROGMEM req_general_autoplay_rnd	[] = "autoplay_rnd";
int command_lampAutoPLaRnd(String value){
	programmeLoop_instance()->patternLoop_autoPlayRnd();
	// ALS_lampsettingPtr_get()->currentSetting_jsonFile();
}
const char PROGMEM req_general_autoplay_delay 	[] = "autoplay_delay";
int command_lampAutoPLayDelay(String value){
	programmeLoop_instance()->patternLoop_delayCurrent_set(value.toInt());
	// ALS_lampsettingPtr_get()->currentSetting_jsonFile();
}
const char PROGMEM req_general_eff_next 	[] = "eff_next";
int command_lamp_effNext(String value){
	String name;
	programmeLoop_instance()->pattern_list_changeEffect(true, name);
	effect_manager_instance()->changeEffectByName(name);
	// ALS_lampsettingPtr_get()->currentSetting_jsonFile();
}
const char PROGMEM req_general_eff_prev 	[] = "eff_prev";
int command_lamp_effPrev(String value){
	String name;
	programmeLoop_instance()->pattern_list_changeEffect(false, name);
	effect_manager_instance()->changeEffectByName(name);
	// ALS_lampsettingPtr_get()->currentSetting_jsonFile();		
}


const char PROGMEM req_general_wifi 	[] = "wifi";
int command_wifiId(String value){
	ALS_lampUpdateClientPtr_get()->httpPage_wifi();
}
const char PROGMEM req_general_json 	[] = "json";
int command_json(String value){
	ALS_lampUpdateClientPtr_get()->httpPage_json();
}
const char PROGMEM req_general_effectSettingMod 	[] = "effectSettingMod";
int command_general_effectSettingMod(String value){
	
}

const char PROGMEM req_general_ecColor		[] = "ec_color";
const char PROGMEM req_general_ecTw			[] = "ec_tw";
const char PROGMEM req_general_ecRnd		[] = "ec_rnd";
int command_general_ecColor(String value){
	if (!ALS_lampsettingPtr_get()->_lamp_on) ALS_lampsettingPtr_get()->_lamp_on = true;

	ALS_lampsettingPtr_get()->effect_color();
	if (value != "") effectIdInstance()->settingFromSerial(ch_toString(settingEffectKey_isTw), "0", true);	
	
	// String rep = "";	
	// ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "");
	// ALS_udpServer_get()->udp_send(rep);
	// ALS_udpServer_get()->udpMulti_send(rep);
	
	// ALS_lampsettingPtr_get()->currentSetting_jsonFile();
	// uint8_t pos = effect_manager_instance()->activeEffectIndex();
	// effect_manager_sav(pos);
}
int command_general_ecTw(String value){
	if (!ALS_lampsettingPtr_get()->_lamp_on) ALS_lampsettingPtr_get()->_lamp_on = true;

	ALS_lampsettingPtr_get()->effect_color();
	if (value != "") effectIdInstance()->settingFromSerial(ch_toString(settingEffectKey_isTw), "1", true);	
	
	// String rep = "";		 	
	// ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "");
	// ALS_udpServer_get()->udp_send(rep);
	// ALS_udpServer_get()->udpMulti_send(rep);

	// ALS_lampsettingPtr_get()->currentSetting_jsonFile();
	// uint8_t pos = effect_manager_instance()->activeEffectIndex();
	// effect_manager_sav(pos);	
}
int command_general_ecRnd(String value){
	if (!ALS_lampsettingPtr_get()->_lamp_on) ALS_lampsettingPtr_get()->_lamp_on = true;
	String name = "";
   	programmeLoop_instance()->patternList_randomEffect(name);
   	effect_manager_instance()->changeEffectByName(name);

	// String rep = "";	
	// ALS_lampUpdateClientPtr_get()->android_generalSetting(rep, "");
	// ALS_udpServer_get()->udp_send(rep);
	// ALS_udpServer_get()->udpMulti_send(rep);

	// ALS_lampsettingPtr_get()->currentSetting_jsonFile();
	// uint8_t pos = effect_manager_instance()->activeEffectIndex();
	// effect_manager_sav(pos);	
}
requestCommandsLists requestCommandsList [] = {
	{req_general_lpOn,		command_lampOn,			"", NULL, 	0},
	{req_general_bri,		command_lampBri,		"", NULL, 	0},

	{req_general_wifi,		command_wifiId,			"", NULL, 	0},
	{req_general_json,		command_json,				"", NULL, 	0},

	{req_general_ecColor,		command_general_ecColor,	"", NULL, 	0},
	{req_general_ecTw,			command_general_ecTw,		"", NULL, 	0},
	{req_general_ecRnd,			command_general_ecRnd,		"", NULL, 	0},

	{req_general_eff_next,			command_lamp_effNext,			"", NULL, 	0},
	{req_general_eff_prev,			command_lamp_effPrev,			"", NULL, 	0},
	{req_general_autoplay_delay,	command_lampAutoPLayDelay,		"", NULL, 	0},
	{req_general_autoplay,			command_lampAutoPLay,			"", NULL, 	0},
	{req_general_autoplay_rnd,		command_lampAutoPLaRnd,			"", NULL, 	0},

};
uint8_t requestCommandsListCount = ARRAY_SIZE(requestCommandsList);


void parse_general(){
	String cmd 		= "";
	String value 	= "";	
	String rep 		= "";	

	// #ifdef DEBUG
	for (int i = 0; i < 10; ++i)
	{
		// fsprintf("[%5d][cmd: %-15s][value: %-15s]\n",
		// 	i,
		// 	getCommand_returnArray[i][0].c_str(),
		// 	getCommand_returnArray[i][1].c_str()
		// );
		if (getCommand_returnArray[i][0]!="") {
			cmd = getCommand_returnArray[i][0];
			break;
		}
		// for (int j = 0; j < requestCommandsListCount; ++j) {
		// 	if (ch_toString(requestCommandsList[j].name) == getCommand_returnArray[i][0]){
		// 	}
		// }
	}
	// #endif
	// fsprintf("\ncmd: %s\n", cmd.c_str());
	if (cmd == "autoplay_delay") {
		const size_t capacity = 4*JSON_OBJECT_SIZE(1);    
		DynamicJsonDocument json(capacity);
		JsonObject root = json.to<JsonObject>();
		JsonObject effect = root.createNestedObject(F("autoplay_delay"));
		uint32_t delay=0;
		programmeLoop_instance()->patternLoop_delayCurrent_get(delay);
		// fsprintf("\ndc: %d\n", delay);
		// char buf[80];
		// sprintf()
		effect["value"] = delay;
		serializeJson(json, rep);
	} else {
		ALS_lampUpdateClientPtr_get()->android_generalSetting(rep);
	}
	ALS_udpServer_get()->udp_send(rep);
	ALS_udpServer_get()->udpMulti_send(rep);

	ALS_lampsettingPtr_get()->currentSetting_jsonFile();
	// ALS_lampsettingPtr_get()->savRequiered_effect();
	
}

// #ifdef MRLCALENDAR
	void parse_event(){
		String cmd 		= "";
		String value 	= "";	
		String rep 		= "";	

		ALS_lampUpdateClientPtr_get()->android_eventSelect(appi_event_select, rep);
		ALS_udpServer_get()->udp_send(rep);
		
	}
	const char PROGMEM req_eventSelect 								[] = "eventSelect";
	const char PROGMEM req_alarm_select_time           				[] = "alarm_select_time";
	const char PROGMEM req_alarm_select_duration           			[] = "alarm_select_duration";
	const char PROGMEM req_alarm_select_repeat           			[] = "alarm_select_repeat";
	const char PROGMEM req_alarm_select_activate           			[] = "alarm_select_activate";
	const char PROGMEM req_alarm_select_setNextDay           		[] = "alarm_select_setNextDay";
	const char PROGMEM req_alarm_select_action           			[] = "alarm_select_action";
	int command_eventSelect(String v) 					{event_getPosByName(v);}
	int command_alarm_select_time(String value) 		{appi_event_select_set_times(value, 0);}
	int command_alarm_select_duration(String value) 	{appi_event_select_set_times(value, 1);}
	int command_alarm_select_repeat(String value) 		{appi_event_select_set_times(value, 2);}
	int command_alarm_select_activate(String value) 	{appi_event_select_activate();}	
	int command_alarm_select_setNextDay(String value) 	{appi_event_select_setNextDay();}	
	int command_alarm_select_action(String value) 		{appi_event_select_action(value.toInt());}	
	requestCommandsLists requestEventCommandsList [] = {
		{req_eventSelect,				&command_eventSelect,				"ALL", 			NULL, 	0},
		{req_alarm_select_time,			&command_alarm_select_time,			"ALL", 			NULL, 	0},
		{req_alarm_select_duration,		&command_alarm_select_duration,		"ALL", 			NULL, 	0},
		{req_alarm_select_repeat,		&command_alarm_select_repeat,		"ALL", 			NULL, 	0},
		{req_alarm_select_activate,		&command_alarm_select_activate,		"ALL", 			NULL, 	0},
		{req_alarm_select_action,		&command_alarm_select_action,		"ALL", 			NULL, 	0},
	};

uint8_t requestEventCommandsListCount = ARRAY_SIZE(requestEventCommandsList);
// #endif




void parse(String op, String value){

	// Serial.printf("\n[parse]\n\t%s\n\t%s\n", op.c_str(), value.c_str());


	for (int i = 0; i < requestReponse_cnt; ++i)
	{ 

		if (requestReponse_array[i]->_protocol != requestProtocol_udp) continue;


		String reponse;
		String sOp;

		String parseRequest = value;

		String uri = "/" + op; 

		// Serial.printf("\n[uri] : %s\n", uri.c_str());

		// continue;

		int pos = -1;
		requestReponse_search(uri, &pos);

		if (pos < 0) continue;


		if (requestReponse_array[pos]->_mod == requestType_name) {

			uri.remove(0, 1);
			parseRequest = "&" + uri + "=";

			if (requestReponse_array[pos]->_parseMod == requestParseCmd_fromFunc) requestReponse_array[pos]->_func(parseRequest); 
			if (requestReponse_array[pos]->_parseMod == requestParseCmd_fromList) requestReponse_pareseUrl_fromList(pos, parseRequest);			

			if (requestReponse_array[pos]->_reponseMod == requestReponseType_fromFunc) requestReponse_array[pos]->_funcReponse(); 
			
			// yield();

			break;
		}

		if (requestReponse_array[pos]->_mod == requestType_wParam) {

			if (requestReponse_array[pos]->_parseMod == requestParseCmd_fromList) requestReponse_pareseUrl_fromList(pos, parseRequest);
			if (requestReponse_array[pos]->_parseMod == requestParseCmd_fromFunc) requestReponse_array[pos]->_func(parseRequest); 


			if (requestReponse_array[pos]->_reponseMod 	== requestReponseType_fromFunc) {
				requestReponse_array[pos]->_funcReponse(); 
			}
			if (requestReponse_array[pos]->_reponseMod 	== requestReponseType_jsonFromList) {
				requestReponse_reponse_fromList(pos, sOp, &reponse);
				// fsprintf("\nreposne: %d\n%S\n",cnt, reponse.c_str());				
			}	
			
			// yield();

			break;		
		}


	}	
}

void parse(String msg) {

	String op 		= literal_value("op", 	msg);
	String value 	= literal_value("msg", 	msg);

	parse(op, value);
	// #endif	
}

void parse_multi(String msg) {

	    String hostname;
	    ALS_wifiPtr_get()->_hostname_get(hostname);

		String host		= literal_value("host",	msg);
		String from		= literal_value("from",	msg);

		// Serial.printf("\n[parse_multi]\n\tme: %s\n\thost: %s\n\tfrom %s\n",hostname.c_str(), host.c_str(), from.c_str());

		if (host != hostname) return;

		if (from == "client"){
			String op 		= literal_value("op", 	msg);
			String value 	= literal_value("msg", 	msg);
			parse(op, value);
		}
	
}

void webserver_reponse_setup(){

	int pos = requestReponse_initialize("/httpRequest");
	requestReponse_protocol(pos, 	requestProtocol_http);
	requestReponse_mod_set(pos, 	requestType_wParam);
	requestReponse_reponseMod(pos, 	requestReponseType_none);
	requestReponse_parseMod(pos, 	requestParseCmd_fromList);
	requestReponse_cmdList(pos, 	requestCommandsListCount, requestCommandsList);
	pos = requestReponse_initialize("/httpRequest");
	requestReponse_protocol(pos, 	requestProtocol_socket);
	requestReponse_mod_set(pos, 	requestType_wParam);
	requestReponse_reponseMod(pos, 	requestReponseType_none);
	requestReponse_parseMod(pos, 	requestParseCmd_fromList);
	requestReponse_cmdList(pos, 	requestCommandsListCount, requestCommandsList);

	// pos = requestReponse_initialize("/settingJson");
	// requestReponse_protocol(pos, 	requestProtocol_http);
	// requestReponse_mod_set(pos, 	requestType_name);
	// requestReponse_reponseMod(pos, 	requestReponseType_SPIFFwebpage);
	// requestReponse_SPIFFwebpage(pos,"/settings.json");
	// requestReponse_parseMod(pos, 	requestParseCmd_none);

	// pos = requestReponse_initialize("/effectJson");
	// requestReponse_protocol(pos, 	requestProtocol_http);
	// requestReponse_mod_set(pos, 	requestType_name);
	// requestReponse_reponseMod(pos, 	requestReponseType_SPIFFwebpage);
	// requestReponse_SPIFFwebpage(pos,"/effectSettingJson.json");
	// requestReponse_parseMod(pos, 	requestParseCmd_none);
	// 
	pos = requestReponse_initialize("/patlo");
	requestReponse_protocol(pos, 	requestProtocol_http);
	requestReponse_mod_set(pos, 	requestType_name);
	requestReponse_reponseMod(pos, 	requestReponseType_none);
	requestReponse_parseMod(pos, 	requestParseCmd_fromFunc);
	requestReponse_func(pos, 		webserver_cmd_patlo);

	pos = requestReponse_initialize("/es");
	requestReponse_protocol(pos, 	requestProtocol_udp);
	requestReponse_mod_set(pos, 	requestType_wParam);
	requestReponse_reponseMod(pos, 	requestReponseType_none);
	requestReponse_parseMod(pos, 	requestParseCmd_fromFunc);
	requestReponse_func(pos, 		parse_effect_setting);

	pos = requestReponse_initialize("/ec");
	requestReponse_protocol(pos, 	requestProtocol_udp);
	requestReponse_mod_set(pos, 	requestType_wParam);
	requestReponse_reponseMod(pos, 	requestReponseType_none);
	requestReponse_parseMod(pos, 	requestParseCmd_fromFunc);
	requestReponse_func(pos, 		parse_effect_choose);

	// pos = requestReponse_initialize("/screen_initialize");
	pos = requestReponse_initialize("/si");
	requestReponse_protocol(pos, 	requestProtocol_udp);
	requestReponse_mod_set(pos, 	requestType_wParam);
	requestReponse_reponseMod(pos, 	requestReponseType_none);
	requestReponse_parseMod(pos, 	requestParseCmd_fromFunc);
	requestReponse_func(pos, 		parse_screen_initialize);

	pos = requestReponse_initialize("/ge");
	requestReponse_protocol(pos, 	requestProtocol_udp);
	requestReponse_mod_set(pos, 	requestType_wParam);
	requestReponse_reponseMod(pos, 	requestReponseType_fromFunc);
	requestReponse_parseMod(pos, 	requestParseCmd_fromList);
	requestReponse_cmdList(pos, 	requestCommandsListCount, requestCommandsList);
	requestReponse_funcReponse(pos,	parse_general);

	pos = requestReponse_initialize("/ev");
	requestReponse_protocol(pos, 	requestProtocol_udp);
	requestReponse_mod_set(pos, 	requestType_wParam);
	requestReponse_reponseMod(pos, 	requestReponseType_fromFunc);
	requestReponse_parseMod(pos, 	requestParseCmd_fromList);
	requestReponse_cmdList(pos, 	requestEventCommandsListCount, requestEventCommandsList);
	requestReponse_funcReponse(pos,	parse_event);



}

void ALS_lampsetting::patterns_loop(){
	static mod_pattern_loop pat_check 	= programmeLoop_instance()->patternLoop_statuDefault();
	static String 			pat_name 	= "";
	static boolean 			playlist;

	programmeLoop_instance()->loop(pat_check, pat_name, playlist); 				
	if (pat_check == programmeLoop_instance()->patternLoop_statuNext()) {

		ALS_lampsettingPtr_get()->_effect_play = true;

		pat_check =  programmeLoop_instance()->patternLoop_statuDefault();
		// String out;
		// _ledsEffects->savSetting(out);

		effect_manager_instance()->changeEffectByName(pat_name);

		ALS_lampUpdateClientPtr_get()->patterns_loop("");

		#ifdef DEBUG
			fsprintf("\n[lamp_server::patterns_loop]\n%s\n", pat_name.c_str());

			#ifdef PATHEAPSTATS
				if (heap_pass>=2) patternList_instance()->heapStats_print();
			#else
				String ret;
				heapStatsPatternLoop_print(ret);

				Serial.print(ret);

				#ifdef SOCKETSERVER
					webserver * ptr = lampServer_instance()->webserver_instance();
					if (ptr->socket_isConnected())	ptr->socket_send(pat_name + "\n" + ret);
				#endif

			#endif
		#endif

		ALS_lampsettingPtr_get()->savRequiered_effect();

	}

}





lampPeripherals * lampPeripherals_ptr = nullptr;
lampPeripherals * lampPeripheralsInstance(){
	return lampPeripherals_ptr;
}
adriOled1306_driver * adriOled1306Driver_ptr;
adriOled1306_driver * myOledInstance(){
	return adriOled1306Driver_ptr;
}

void peripheralsBtn_onOff_sc(){
	fsprintf("\n[peripheralsBtn_onOff_sc]\n");
	ALS_lampsettingPtr_get()->_lamp_on = !ALS_lampsettingPtr_get()->_lamp_on;
	if (!ALS_lampsettingPtr_get()->_lamp_on){
		uint8_t pos = effect_manager_instance()->activeEffectIndex();
		effect_manager_sav(pos);	
		ALS_fnc_lampClear();
	} else {
		effectIdInstance()->loadSettingFromFile("/effectsSetting/");
	}	
	String rep = "";	
	ALS_lampUpdateClientPtr_get()->android_generalSetting(rep);
	ALS_udpServer_get()->udp_send(rep);
	ALS_udpServer_get()->udpMulti_send(rep);
	ALS_lampsettingPtr_get()->savRequiered_effect();
}

void peripheralsBtn_tw_sc(){
	fsprintf("\n[peripheralsBtn_tw_sc]\n");

	ALS_lampsettingPtr_get()->effect_color();
	ALS_lampsettingPtr_get()->effect_setvalueFromKey(ch_toString(settingEffectKey_tw), String(map(analogRead(A0), 69, 1024, 0, 255)));
	
	String rep = "";	
	ALS_lampUpdateClientPtr_get()->android_generalSetting(rep);
	ALS_udpServer_get()->udp_send(rep);
	ALS_udpServer_get()->udpMulti_send(rep);
	ALS_lampsettingPtr_get()->savRequiered_effect();

	if (lampPeripheralsInstance()->_loop_potar != 1){
		lampPeripheralsInstance()->_loop_potar = 1;
		lampPeripheralsInstance()->_timer_potar->activate();
		lampPeripheralsInstance()->oled_clear();
		lampPeripheralsInstance()->colorPrintLbl("blanc :");		
	} else {
		if (lampPeripheralsInstance()->_timer_potar->isActivate()) {
			lampPeripheralsInstance()->_loop_potar = 0;
			lampPeripheralsInstance()->_timer_potar->activate(false);
			lampPeripheralsInstance()->oled_clear();
			lampPeripheralsInstance()->wifiPrint();			
		}
	}
}
void peripheralsBtn_tw_lc_tick(){
	ALS_lampsettingPtr_get()->effect_color();
	ALS_lampsettingPtr_get()->effect_setvalueFromKey(ch_toString(settingEffectKey_tw), String(map(analogRead(A0), 69, 1024, 0, 255)));
}

void peripheralsBtn_tw_lc(){
	lampPeripheralsInstance()->_loop_potar = 0;

	if (lampPeripheralsInstance()->_timer_clear_delay == 0) {
		lampPeripheralsInstance()->_timer_clear_delay = 1000;
	}else {
		lampPeripheralsInstance()->_timer_clear_delay+=10;
		lampPeripheralsInstance()->colorPrintValue();
	}

	if (!lampPeripheralsInstance()->_timer_clear->isActivate()) {
		lampPeripheralsInstance()->_timer_clear->activate();
		lampPeripheralsInstance()->oled_clear();
		lampPeripheralsInstance()->colorPrintLbl("blanc :");		
	}	

	peripheralsBtn_tw_lc_tick();
}

void peripheralsBtn_hue_sc(){
	fsprintf("\n[peripheralsBtn_hue_sc]\n")

	ALS_lampsettingPtr_get()->effect_color();
	ALS_lampsettingPtr_get()->effect_setvalueFromKey(ch_toString(settingEffectKey_hue), String(map(analogRead(A0), 69, 1024, 0, 255)));
	
	String rep = "";	
	ALS_lampUpdateClientPtr_get()->android_generalSetting(rep);
	ALS_udpServer_get()->udp_send(rep);
	ALS_udpServer_get()->udpMulti_send(rep);
	ALS_lampsettingPtr_get()->savRequiered_effect();


	if (lampPeripheralsInstance()->_loop_potar != 2){
		lampPeripheralsInstance()->_loop_potar = 2;
		lampPeripheralsInstance()->_timer_potar->activate();
		lampPeripheralsInstance()->oled_clear();
		lampPeripheralsInstance()->colorPrintLbl("color :");
	} else {
		if (lampPeripheralsInstance()->_timer_potar->isActivate()) {
			lampPeripheralsInstance()->_loop_potar = 0;
			lampPeripheralsInstance()->_timer_potar->activate(false);
			lampPeripheralsInstance()->oled_clear();
			lampPeripheralsInstance()->wifiPrint();
		}
	}	
}

void peripheralsBtn_hue_lc_tick(){
	ALS_lampsettingPtr_get()->effect_color();
	ALS_lampsettingPtr_get()->effect_setvalueFromKey(ch_toString(settingEffectKey_hue), String(map(analogRead(A0), 69, 1024, 0, 255)));				
}	
void peripheralsBtn_hue_lc(){
	lampPeripheralsInstance()->_loop_potar = 0;

	if (lampPeripheralsInstance()->_timer_clear_delay == 0) {
		lampPeripheralsInstance()->_timer_clear_delay = 1000;
	}else {
		lampPeripheralsInstance()->_timer_clear_delay+=10;
		lampPeripheralsInstance()->colorPrintValue();			
	}

	if (!lampPeripheralsInstance()->_timer_clear->isActivate()) {
		lampPeripheralsInstance()->_timer_clear->activate();
		lampPeripheralsInstance()->oled_clear();
		lampPeripheralsInstance()->colorPrintLbl("color :");		
	}	

	peripheralsBtn_hue_lc_tick();
}	

// void lampPeripherals::tft9341_initialize
lampPeripherals::lampPeripherals(int mod) {
	lampPeripherals_ptr = this;

	_mod = mod;


	switch(mod) {
		
		case 0:
			_oled 	= 1;
			_button = 1;
			_leds  	= 1;
		break;
		case 1:
			_oled 	= 1;
			_button = 1;
			_leds  	= 0;
		break;	
		case 2:
			_oled 	= 0;
			_button = 1;
			_leds  	= 1;
		break;				
		case 3:
			_oled 	= 1;
			_button = 0;
			_leds  	= 0;
		break;
		case 4:
			_oled 	= 0;
			_button = 1;
			_leds  	= 0;
		break;
		case 5:
			_oled 	= 0;
			_button = 0;
			_leds  	= 1;
		break;	
		case 13:

			_timer_potar 	= new adri_timer(5000, 	"", false);
			_timer_clear 	= new adri_timer(0, 	"", false);
			_loop_potar 	= 0;

			_oled 	= 1;
			_button = 2;
			_leds  	= 0;
		break;						
	}
	switch(_oled){
		case 1:
				_my_screen = new adriOled1306_driver();

				adriOled1306Driver_ptr = _my_screen;

				_my_screen->_display.clearDisplay();
				_my_screen->_display.display();
				_my_screen->_display.setTextSize(1);
				_my_screen->_display.setTextColor(SSD1306_WHITE); 				
		break;
	}

}

void lampPeripherals::loop(){
	
	if (_mod < 0 ) return;

	button_lc_loop();

	if ((_timer_clear!=nullptr) && (_timer_clear->isActivate())) {
		if (_timer_clear->loop(_timer_clear_delay)) {
			_timer_clear->activate(false);
			_timer_clear_delay = 0;
			oled_clear();
			wifiPrint();
			String rep = "";	
			ALS_lampUpdateClientPtr_get()->android_generalSetting(rep);
			ALS_udpServer_get()->udp_send(rep);
			ALS_udpServer_get()->udpMulti_send(rep);
			ALS_lampsettingPtr_get()->savRequiered_effect();			
			fsprintf("\n[CLEAR]\n");
		}
	}
	if ((_timer_potar!=nullptr) && (_timer_potar->isActivate())) {
		if (_timer_potar->loop(_timer_potar_delay)) {
			_timer_potar->activate(false);
			_loop_potar = 0;
			oled_clear();
			wifiPrint();
			fsprintf("\n[LOOP CLEAR]\n");
		} else {
			if (potvale != analogRead(A0)) {
				_timer_potar_delay+=13;
				potvale = analogRead(A0);
			}
			delay(10);
		}
	}

	if (_loop_potar == 1) {
		peripheralsBtn_tw_lc_tick();
		colorPrintValue();
	}
	if (_loop_potar == 2) {
		peripheralsBtn_hue_lc_tick();
		colorPrintValue();
	}	
}
		void lampPeripherals::button_tw_create(
			int pin,
			boolean pullup, 
			int input, 
			int delay
		) {
			switch(_button){
				case 1:
					_btTw = button_create_scLc(pin, pullup, input, peripheralsBtn_tw_sc, peripheralsBtn_tw_lc, delay);
				break;
				case 2:
					_btTw = button_create_scLc(pin, pullup, input, peripheralsBtn_tw_sc, peripheralsBtn_tw_lc, delay);
				break;				
			}
		}
		
		void lampPeripherals::button_hue_create(
			int pin,
			boolean pullup, 
			int input, 
			int delay
		) {
			switch(_button){
				case 1:
					_btHue = button_create_scLc(pin, pullup, input, peripheralsBtn_hue_sc, peripheralsBtn_hue_lc, delay);
				break;
				case 2:
					_btHue = button_create_scLc(pin, pullup, input, peripheralsBtn_hue_sc, peripheralsBtn_hue_lc, delay);
				break;				
			}
		}
		void lampPeripherals::button_onOff_create(
			int pin,
			boolean pullup, 
			int input 
		) {
			switch(_button){
				case 2:
					_btOnOff = button_create_sc(pin, pullup, input, peripheralsBtn_onOff_sc);
				break;				
			}
		}
		void lampPeripherals::button_setFunc(int bt, click_func f1, click_func f2) {
			button_lc_array[bt]->_click_func 		= f1;
			button_lc_array[bt]->_longclick_func 	= f2;
		}	

void lampPeripherals::wifiPrint(){
	switch(_oled){
		case 1:
			String ssid = wifiConnect_instance()->currentSSID_get();
			_my_screen->_display.setTextSize(1);
			_my_screen->_display.fillRect(0, 30 , 128, 10, 0);
			_my_screen->_display.setCursor(5,30);
			_my_screen->_display.println(ssid);
			String ip = wifiConnect_instance()->currentIp_get();
			_my_screen->_display.fillRect(0, 40 , 128, 10, 0);
			_my_screen->_display.setCursor(5,40);
			_my_screen->_display.println(ip);
			_my_screen->_display.display();
			_my_screen->_display.setTextSize(1);
		break;
	}
}
void lampPeripherals::colorPrintLbl(String v){
		_my_screen->_display.fillRect(0, 30 , 128, 10, 0);
		_my_screen->_display.setCursor(5,30);            
		_my_screen->_display.println(v);	
		_my_screen->_display.display();
}
void lampPeripherals::colorPrintValue(){
		_my_screen->_display.fillRect(0, 40 , 128, 10, 0);
		_my_screen->_display.setCursor(5,40);             
		_my_screen->_display.println(map(analogRead(A0), 69, 1024, 0, 255));	
		_my_screen->_display.display();
}

void lampPeripherals::wificonnect(String value){
	switch(_oled){
		case 1:
			_my_screen->_display.setCursor(0,0);
			_my_screen->_display.println(value);
			_my_screen->_display.display();
		break;
	}	
}
void lampPeripherals::oled_clear(){
	switch(_oled){
		case 1:
			_my_screen->_display.clearDisplay();
		break;
	}	
}
void lampPeripherals::Tft9341_ceate(){

}