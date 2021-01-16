#include "ALS_eventclock.h"



// #define DEBUG

#include <adri_tools.h>


/*


id 		name 			mod 				trigger start 			action start  	action loop		action end 		trigger end

1		light on/off	dtWeeklyEndAlarm	strat at time 			light tw	 	none			light tw off	end at satrt time + duration
						dtDailyEndAlarm

1		light on	 	dtDailyAlarm		strat at time 			light tw on	 	none 			none 			end at strat time
						dtWeeklyAlarm

2		light off	 	dtDailyAlarm		strat at time 			light tw off	none 			none 			end at strat time
						dtWeeklyAlarm
	allumage ->


*/
namespace {

	const size_t serializeSize = 512 * 20;



}


alarmClock * alarmClock_array[EVENT_MAX];

void lightOnOff_start(String opt){
	// fsprintf("\n[lightOn_start]\n");	
	// lampSettingInstance()->_lamp_on = true;
	// lampSettingInstance()->effect_color();
	// effect_manager_load(effect_manager_instance()->activeEffectIndex());
	// String rep = "";	
	// lampUpdateClientInctance()->android_generalSetting(rep);
	// lampServer_instance()->udpserverr_instance()->_server->send(rep);
	// lampServer_instance()->udpmultiserver_instance()->send(rep);
	// lampSettingInstance()->savRequiered_effect();
}
void lightOnOff_end(String opt){
	// fsprintf("\n[lightOff_start]\n");	
	// lampSettingInstance()->_lamp_on = false;
	// uint8_t pos = effect_manager_instance()->activeEffectIndex();
	// effect_manager_sav(pos);	
	// leds_settingPtr_get()->lamp_clear();
	// String rep = "";	
	// lampUpdateClientInctance()->android_generalSetting(rep);
	// lampServer_instance()->udpserverr_instance()->_server->send(rep);
	// lampServer_instance()->udpmultiserver_instance()->send(rep);
	// lampSettingInstance()->savRequiered_effect();		
}
void lightOnOff_loop(String opt){}

PROGMEM alarmClock_action alarmClockAction_end[] = {   
	{1, lightOnOff_start, lightOnOff_end, lightOnOff_loop}
};


void lightOn_start(String opt){
	String rep;
	programmeLoop_instance()->patternLoop_play_set(false);
	if (!ALS_lampsettingPtr_get()->_lamp_on) ALS_lampsettingPtr_get()->_lamp_on = true;
	ALS_lampsettingPtr_get()->effect_color();
	effectIdInstance()->settingFromSerial(ch_toString(settingEffectKey_isTw), "1", true);	
	ALS_lampUpdateClientPtr_get()->android_generalSetting(rep);
	ALS_udpServer_get()->udp_send(rep);
	ALS_udpServer_get()->udpMulti_send(rep);	
}
void lightOff_start(String opt){
	String rep;
	ALS_lampsettingPtr_get()->_lamp_on = false;	
	ALS_fnc_lampClear();
	ALS_lampUpdateClientPtr_get()->android_generalSetting(rep);
	ALS_udpServer_get()->udp_send(rep);
	ALS_udpServer_get()->udpMulti_send(rep);
	ALS_lampsettingPtr_get()->currentSetting_jsonFile();
}
void effect_start(String opt){
	int 	p_pos 		= -1;
	String 	rep			= "";
	if (ALS_lampsettingPtr_get()->patternList_changeByName(opt, p_pos)){
		if (!ALS_lampsettingPtr_get()->_lamp_on) ALS_lampsettingPtr_get()->_lamp_on = true;
		programmeLoop_instance()->patternLoop_play_set(false);
		ALS_lampUpdateClientPtr_get()->android_generalSetting(rep);
		ALS_udpServer_get()->udp_send(rep);
		ALS_udpServer_get()->udpMulti_send(rep);
		ALS_lampsettingPtr_get()->currentSetting_jsonFile();
	}	
}
PROGMEM alarmClock_action alarmClockAction_start [] = {   
	{1, lightOn_start, 	NULL, NULL},
	{2, lightOff_start, NULL, NULL},
	{3, effect_start, 	NULL, NULL}

};


alarmClock::alarmClock(int pos){_event = calendar_array[pos];}

void alarmClock::start(){
	if (_pos <= 0 ) return;

	alarmClock_action * list;

	if ( (_event->period == dtDailyAlarm) || (_event->period == dtWeeklyAlarm) ) {
		list = alarmClockAction_start;
	}
	if ( (_event->period == dtDailyEndAlarm) || (_event->period == dtWeeklyEndAlarm) ) {
		list = alarmClockAction_end;
	}	

	alarmClock_func func = list[_pos-1].start;

	if (func!=NULL)(*func)(_option_1);
}
void alarmClock::end(){
	if (_pos <= 0 ) return;

	alarmClock_action * list;

	if ( (_event->period == dtDailyAlarm) || (_event->period == dtWeeklyAlarm) ) {
		list = alarmClockAction_start;
	}
	if ( (_event->period == dtDailyEndAlarm) || (_event->period == dtWeeklyEndAlarm) ) {
		list = alarmClockAction_end;
	}	
	
	alarmClock_func func = list[_pos-1].end;

	if (func!=NULL)(*func)(_option_1);
}

alarmclock_current alarmclockCurrent;
alarmclock_current::alarmclock_current(){

}

boolean envent_setup_ready 	= false;
int appi_event_select 	= 0;

calendar * event_espReset;


PROGMEM events_spiff events_spiff_list [] = {   
// 								lape 		repeat 	value  		dow		ca		
	{"Dimanche", 		"-1", 	"3600",		"180", 	"28800", 	1, 		8},
	{"Lundi", 			"-1", 	"3600",		"180", 	"28800", 	2, 		8},
	{"Mardi", 			"-1", 	"3600",		"180", 	"28800", 	3, 		8},
	{"Mercredi", 		"-1", 	"3600",		"180", 	"28800", 	4, 		8},
	{"Jeudi", 			"-1", 	"3600",		"180", 	"28800", 	5, 		8},
	{"Vendredi", 		"-1", 	"3600",		"180", 	"28800", 	6, 		8},
	{"Samedi", 			"-1", 	"3600",		"180", 	"28800", 	7, 		8},


	{"Journalier_4", 	"-1", 	"3600",		"180", 	"28800", 	0, 		9},
	{"Journalier_5", 	"-1", 	"2200",		"180", 	"28800", 	0, 		9},

	{"Journalier_6", 	"-1", 	"-1", 		"180", 	"28800", 	0, 		2},
	{"Journalier_7", 	"-1", 	"-1", 		"180", 	"28800", 	0, 		2},
	{"Journalier_8", 	"-1", 	"-1", 		"180", 	"28800", 	0, 		2},
	{"Journalier_9", 	"-1", 	"-1", 		"180", 	"28800", 	0, 		2},


	// 
};
int events_spiff_list_count = ARRAY_SIZE(events_spiff_list);


String   events_toSpiff_lapse(calendar * obj){
	if (obj->period == dtDailyAlarm) 	return String("-1");
	else 								return String(obj->o_endValue);	
}
String   events_toSpiff_repeat(calendar * obj){
	if (obj->period == dtDailyAlarm) 	return String("-1");
	else 								return String(obj->repeat);
}
String   events_toSpiff_value(calendar * obj){
	return String(obj->startTime);
}
String   events_toSpiff_enabled(calendar * obj){
	return String(obj->isEnabled);

}
String   events_toSpiff_period(calendar * obj){
	return calendar_period_to_string(obj->period);
}

PROGMEM events_toSpiff events_toSpiff_list [] = { 
	{"lapse" , 	events_toSpiff_lapse},
	{"repeat" , events_toSpiff_repeat},
	{"value" , 	events_toSpiff_value},
	{"activ" , 	events_toSpiff_enabled},
	{"period" ,	events_toSpiff_period},
	// {"buzzer" , events_toSpiff_buzzer},
};
int events_toSpiff_list_count = ARRAY_SIZE(events_toSpiff_list);


void event_trigger(int pos){
	if(pos < 0) return;

	alarmclockCurrent._event = pos;

	Serial.printf("\n[myFunc_1]\n");
	String period = calendar_period_to_string(calendar_array[pos]->period);
	fsprintf("\tperiod: %S\n", period.c_str());
	alarmClock_array[pos]->start();
    // calendarPrint_0(calendar_array[pos]->pos, pos);
    // alarmClock_array[pos]->start();

}
void event_endTrigger(int pos){
	
	if(pos < 0) return;
	alarmclockCurrent._event = pos;

	Serial.printf("\n[myFunc_2]\n");
	String period = calendar_period_to_string(calendar_array[pos]->period);
	fsprintf("\tperiod: %S\n", period.c_str());	
	alarmClock_array[pos]->end();
    // calendarPrint_0(calendar_array[pos]->pos, pos);
    // alarmClock_array[pos]->start();
}

String eventsDetails_jsonFile();
void events_jsonFile();
boolean events_fromJson(){

	timeDayOfWeek_t thisDay = dowInvalid;

	time_t repeat, lapse,  t4;

	boolean 	loadEvent = false;
	JsonObject 	objectId;   
	String 		value;
	String 		dispDame;
	String 		name;
	int 		caPos;
	int 		search_day;
	boolean 	enabled;

	DynamicJsonDocument buff(4500);
	DeserializationError err;

    File file = LittleFS.open("/json/events.json", "r");
    if (file) {
    	// fsprintf("\n[events_fromJson]\n");
    	err = deserializeJson(buff, file);
		if (err) {
			Serial.print(F("[events_fromJson] FLASHFS Error parsing settings json file: "));
			Serial.println(err.c_str());
		}  else {
			loadEvent = true;
		}  	
    	file.close();
    }	


	for (int i = 0; i < events_spiff_list_count; ++i) {

		caPos		= events_spiff_list[i]._caPos;
		name		= ch_toString(events_spiff_list[i]._id);	

		objectId 	= buff["eventslist"][String(i)];

		#ifdef DEBUG
			fsprintf("\n%d\n",i);
			dispDame = "name";
			value = objectId[F("name")].as<String>();
			fsprintf(" \t%s : %s\n", dispDame.c_str(), value.c_str());
		#endif

		if (loadEvent) {
			// if (objectId["name"].as<String>() == name) {
				value 	= objectId[F("lapse")].as<String>();
				lapse	= value.toInt();
				value 	= objectId[F("repeat")].as<String>();
				repeat	= value.toInt();
				value 	= objectId[F("value")].as<String>();
				t4		= value.toInt();
				value 	= objectId[F("activ")].as<String>();
				enabled	= stringToBool(value.toInt());
				#ifdef DEBUG
					fsprintf("\t lapse: %d\n", lapse);
					fsprintf("\t repeat: %d\n", repeat);
					fsprintf("\t t4: %d\n", t4);
					fsprintf("\t enabled: %d\n", enabled);
				#endif
			// } 
		} 
		if (!loadEvent ) {
			lapse 			= ch_toString(events_spiff_list[i]._endValue).toInt();
			repeat 			= ch_toString(events_spiff_list[i]._repeat).toInt();
			t4 				= ch_toString(events_spiff_list[i]._value).toInt();			
			enabled 		= false;	

		}

		if (events_spiff_list[i]._dow > 0) {
			search_day = calendar_date_get_dow("fr", ch_toString(events_spiff_list[i]._id));
			if (search_day >= 0) {
				thisDay = calendar_dates[search_day].dow;		
				calendar_add[caPos].func(calendar_add[caPos].id, events_spiff_list[i]._id, -1, lapse, repeat, t4, event_trigger, thisDay, event_endTrigger);
			}				
		} else {
				thisDay = dowInvalid;
				calendar_add[caPos].func(calendar_add[caPos].id, events_spiff_list[i]._id, -1, lapse, repeat, t4, event_trigger, thisDay, event_endTrigger);
		}

		calendar_array[i]->isEnabled = enabled;

		alarmClock_array[i] = new alarmClock(i);
		if (loadEvent) alarmClock_array[i]->fromJson(objectId);
		// #ifdef DEBUG
		// 	fsprintf("\n%d\n",i);
		// 	dispDame = "name";
		// 	value = objectId[F("name")].as<String>();
		// 	fsprintf("  %-10s : %s\n", dispDame.c_str(), value.c_str());
		// #endif
		// for (int j = 0; j < events_toSpiff_list_count; ++j) {
		// 	value = objectId[events_toSpiff_list[j]._id].as<String>();
		// 	#ifdef DEBUG
		// 		fsprintf("  %-10s : %s\n", events_toSpiff_list[j]._id, value.c_str());
		// 	#endif
		// }
	}	

	return true;
}
void envent_setup() {
	events_fromJson();
	events_jsonFile();
}

void alarmClock::jsonOject(JsonObject & object){
	object[F("action_id")] 	= _pos;
	object[F("option_1")] 	= _option_1;
}
void alarmClock::fromJson(JsonObject object){
	String 	value 		= object[F("action_id")].as<String>();
			_pos		= value.toInt();
			value 		= object[F("option_1")].as<String>();
			_option_1	= value;			
}

void event_update_lbl(calendar * cEvent, String *s_period, String *startTime, String *s_triggerNext, String *s_repeatValue, String *s_endValue);
void events_jsonObject(JsonObject & object) {
	JsonObject 	objectId ;
	String 		value;	

	for (int i = 0; i < events_spiff_list_count; ++i) {

		objectId = object.createNestedObject(String(i));

		objectId[F("name")] = calendar_array[i]->name;
		// if (calendar_array[i]->period == dtDailyAlarm) 	objectId[F("lapse")] = String("-1");
		// else 											objectId[F("lapse")] = calendar_array[i]->o_endValue;
		// objectId[F("repeat")] 	=  	calendar_array[i]->repeat;
		// objectId[F("value")] 	=  	calendar_array[i]->startTime;
		// objectId[F("activ")] 	= 	String(calendar_array[i]->isEnabled);
		for (int j = 0; j < events_toSpiff_list_count; ++j) {
			String value = events_toSpiff_list[j]._value(calendar_array[i]);
			objectId[ch_toString(events_toSpiff_list[j]._id)] = value;
		}

		alarmClock_array[i]->jsonOject(objectId);
	}	


}
void events_jsonFile(String & result) {
	DynamicJsonDocument json(serializeSize);
	JsonObject 	root 	= json.to<JsonObject>();
	JsonObject 	object 	= root.createNestedObject("eventslist");
	events_jsonObject(object);
	serializeJson(json, result);
}
void events_jsonFile() {
    File file = LittleFS.open("/json/events.json", "w");
    if (file) {
		DynamicJsonDocument json(serializeSize);
		JsonObject 	root 	= json.to<JsonObject>();
		root["selected"] 	= ch_toString(events_spiff_list[appi_event_select]._id);
		JsonObject 	object 	= root.createNestedObject("eventslist");
		events_jsonObject(object);
		serializeJson(json, file);
		file.close();
    }		

}



void event_update_lbl(calendar * cEvent, String *s_period, String *startTime, String *s_triggerNext, String *s_repeatValue, String *s_endValue){
	time_t trigger;
	time_t triggerNext;
	time_t value;
	time_t endValue;
	dt_alarmPeriod period;

	String s_trigger 		= "";
	String s_startValue 	= "déactiver";;

		value 	= cEvent->value;

		if (cEvent->startValue == -1) {
			trigger 		= cEvent->nextTrigger - now();
		}
		else {
			trigger 		= cEvent->nextTrigger - now();
		}

		*startTime = timer_toString(cEvent->startTime);

		s_trigger 			= timer_toString(trigger);
		triggerNext 		= now()+trigger;
		*s_triggerNext 		= timer_toString(triggerNext);
		*s_period			= calendar_period_to_string(cEvent->period);
		endValue 			= cEvent->endValue;
		period 				= cEvent->period;
		if (period == dtWeeklyRepeatAlarm) 	{
			*s_repeatValue 	= timer_toString(cEvent->repeat);
			*s_endValue 	= timer_toString(endValue);
		}
		if (period == dtWeeklyEndAlarm) 	{
			*s_endValue 	= timer_toString(endValue);
		}		
		if (period == dtDailyRepeatAlarm) 	{
			*s_repeatValue 	= timer_toString(cEvent->repeat);
			*s_endValue 	= timer_toString(endValue);
		}
		if (period == dtDailyEndAlarm) 	{
			*s_endValue 	= timer_toString(endValue);
		}			
		// if (period == dtTimer) 	{
		// 	*s_repeatValue 	= timer_toString(cEvent->value);
		// 	if (endValue != -1) *s_endValue = timer_toString(endValue);
		// }

}
int event_getPosByName(String name){
	int ret = -1;
	appi_event_select = -1;
	for( int i = 0; i < calendar_number_of_event; i++) { 
		if (name == calendar_array[i]->name) {
			ret = i;
			break;
		}
	}
	appi_event_select = ret;
	return ret;	
}
boolean eventsDetails_jsonObject(int sSelect, JsonObject & objectId) {

	if (sSelect < 0 ) return false;

	String s_period;
	String startTime 		= "";
	String s_triggerNext 	= "";
	String s_repeatValue 	= "déactiver";	
	String s_endValue 		= s_repeatValue;

	event_update_lbl(calendar_array[sSelect], &s_period, &startTime, &s_triggerNext, &s_repeatValue, &s_endValue);

	objectId[F("name")] 		= calendar_array[sSelect]->name;
	objectId[F("period")] 		= s_period;
	objectId[F("start")] 		= startTime;
	objectId[F("next")] 		= s_triggerNext;
	objectId[F("repeat")] 		= s_repeatValue;
	objectId[F("end")] 			= s_endValue;
	objectId[F("duration")] 	= timer_toString(calendar_array[sSelect]->o_endValue);
	objectId[F("activate")] 	= String(calendar_array[sSelect]->isEnabled);
	objectId[F("action")] 		= String(alarmClock_array[sSelect]->_pos);
	objectId[F("option_1")] 	= alarmClock_array[sSelect]->_option_1;

	return true;	
}
String eventsDetails_jsonFile() {
    File file = LittleFS.open("/json/eventsDetails.json", "w");
    if (file) {
		DynamicJsonDocument json(serializeSize);
		JsonObject 	root 		= json.to<JsonObject>();
		JsonObject 	object 		= root.createNestedObject("eventsDetailsList");
		JsonObject 	objectId;
		for( int i = 0; i < calendar_number_of_event; i++) { 
			objectId = object.createNestedObject(String(i));
			eventsDetails_jsonObject(i, objectId);
		}
		serializeJson(json, file);
		file.close();
    }		
}
void eventsDetail_jsonObject(String name, JsonObject & object) {

	int pos = event_getPosByName(name);

	if (pos < 0) return;

	JsonObject objectId = object.createNestedObject(String(pos));
	eventsDetails_jsonObject(pos, objectId);
}






void appi_event_select_set_times(String Value, int mod) {
	if (appi_event_select < 0 ) return;

	int pos = appi_event_select;

    Serial.printf("\n[event_select_set_times] name: %s mod: %d\n", calendar_array[pos]->name.c_str(), mod);

	int 	*split_time = splitTime(Value, '.');
	int 	hr 			= split_time[0];
	int 	min 		= split_time[1];
	int 	sec 		= split_time[2];
	fsprintf("h: %d, m: %d s: %d", hr, min, sec);

	time_t 	t 			= AlarmHMS(hr,min,sec);

	time_t endValue;
	time_t repeat;
	time_t startTime;

	if (mod == 0){

		endValue 	= calendar_array[pos]->o_endValue;
		repeat 		= calendar_array[pos]->repeat;

		calendar_edit(calendar_array[pos], t, repeat, endValue, calendar_array[pos]->isEnabled);
		// screen_alarmClock_edit(false);
	}
	if (mod == 1){

		repeat 		= calendar_array[pos]->repeat;
		startTime 	= calendar_array[pos]->startTime;

		if (t>startTime) t = t - startTime;

		calendar_edit(calendar_array[pos], startTime, repeat, t, calendar_array[pos]->isEnabled);
		// screen_alarmClock_edit(false);
	}	
	if (mod == 2) {
		endValue 	= calendar_array[pos]->o_endValue;
		startTime 	= calendar_array[pos]->startTime;

		calendar_edit(calendar_array[pos], startTime, t, endValue, calendar_array[pos]->isEnabled);
		// screen_alarmClock_edit(false);
	}

	// events_jsonFile();
	// events_sav_toSpiff();	
}

void appi_event_select_activate(){

	if (appi_event_select < 0 ) return;

	calendar_array[appi_event_select]->isEnabled = !calendar_array[appi_event_select]->isEnabled;

	// events_jsonFile();


}

void appi_event_select_setNextDay(){ 
	if (appi_event_select < 0 ) return;
	if(appi_event_select >= 0) {
		calendar_array[appi_event_select]->setNext();
		// if (screen_mod_curr != SCM_MAIN) screen_main();
	}
}


void appi_event_select_action(String value){ 
	alarmClock_array[appi_event_select]->_pos = value.toInt();
}
void appi_event_select_actionEffect(String value){ 
	alarmClock_array[appi_event_select]->_option_1 = value;
}




void event_loop(){
	calendar_update_all() ;
}


void appi_events_print(String & ret){



	for( int i = 0; i < calendar_number_of_event; i++) { 

		if (calendar_array[i] == NULL) continue;
		if (!calendar_array[i]->isEnabled) continue;
		// if (calendar_array[i]->period == dtWeeklyRepeatAlarm) {
		//     char* cDay = dayStr(weekday());
		//     String search_day = calendar_date_get_dowStr("en", String(cDay)); 
		//     if (search_day != "") {
		//     	if (calendar_array[i]->name != search_day) continue;
		//     }			
		// }
		calendarPrint_f(calendar_array[i]->pos, i, ret);


	}
}



