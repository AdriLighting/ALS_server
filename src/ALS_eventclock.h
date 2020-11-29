#ifndef ALS_EVENTCLOCK_H
	#define ALS_EVENTCLOCK_H 

		
			#include <Arduino.h>
			#include <ArduinoJson.h>
			#include <calendar.h>
			#include <ALS_fnc.h>
			#include "ALS_lampsetting.h"
		
		
			#define events_toSpiff_filename String("events_sav")
			#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


			// #########################  ID CURRENT TRIGGED ALARM 
			class alarmclock_current
			{
			public:
				int _event = -1;

				alarmclock_current();
				~alarmclock_current(){};
			};
			extern alarmclock_current alarmclockCurrent;

			// ######################### SAV ALARMS
			/**
			 * @brief      STRUCTUR TO SAV A LOAD VALUE FROM AN ALARM
			 */
			struct events_spiff
			{
				char * 	_id;
				char * 	_startValue;
				char * 	_endValue;
				char * 	_repeat;
				char * 	_value;
				int 	_dow;
				int  	_caPos;
			};
			
			struct events_toSpiff
			{
				char * 	_id;
		        String (* _value ) (calendar * obj);   

			};	


			void envent_setup();
			void calendarPrint_all();
			void event_loop();
			String eventsDetails_jsonFile() ;
			void eventsDetail_jsonObject(String name, JsonObject & object);
			boolean eventsDetails_jsonObject(int sSelect, JsonObject & objectId);
			int event_getPosByName(String name);
			extern int envent_setup_ready;
			extern int appi_event_select;

			void appi_event_select_set_times(String Value, int mod);
			void appi_event_select_activate();
			void appi_event_select_setNextDay();


			typedef void (*alarmClock_func)();  

			struct alarmClock_action
			{
				int pos;
				alarmClock_func start;
				alarmClock_func end;
				alarmClock_func loop;
			};			

			class alarmClock
			{
				boolean 	_isEnabled 	= false;
			public:
				calendar *	_event;
				int 		_pos 		= 1;

				alarmClock(int pos);
				~alarmClock(){};
				void jsonOject(JsonObject & object);
				void fromJson(JsonObject  object);
				void start();
				void end();
				void loop();
				
			};

			void appi_event_select_action(int value);
#endif