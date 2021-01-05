#ifndef ALS_WEBSERVER_H
	#define ALS_WEBSERVER_H
	#include <adri_udp.h>
	#include <adri_tools.h>
	#include "ALS_wifi.h"

	void parse(String msg);
	void parse_multi(String msg);	

	extern adri_udp 		* ALS_udp		;
	extern adri_udpMulti 	* ALS_udpMulti 	;

	class ALS_udpServer
	{


		uint32_t _udp_port 		= 9100;
		uint32_t _udpMulti_port 	= 9400;

	public:
		// ALS_udpServer();

		ALS_udpServer(){
	
		}

		void udp_setup(uint32_t port){
			if (ALS_udp==nullptr) return;
			fsprintf("\n[udp_setup]\n");
			_udp_port = port;
			ALS_udp->set_port(port);
			ALS_udp->parse_set_func(parse);
			ALS_udp->start();
		}
		void udp_loop(){
			if (ALS_udp==nullptr) return;
			ALS_udp->loop();
		}
		void udp_send(String msg){
			if (ALS_udp==nullptr) return;
			ALS_udp->send(msg);
		}

		void udpMulti_setup(uint32_t port){
			if (ALS_udpMulti==nullptr) return;
			fsprintf("\n[udpMulti_setup]\n");
			_udpMulti_port = port;
			ALS_udpMulti->set_port(port);
			ALS_udpMulti->parse_set_func(parse_multi);
			ALS_udpMulti->start();
		}
		void udpMulti_loop(){
			if (ALS_udpMulti==nullptr) return;
			ALS_udpMulti->loop();
		}
		void udpMulti_send(String msg){
			if (ALS_udpMulti==nullptr) return;
		    String hostname;
		    ALS_wifiPtr_get()->_hostname_get(hostname);

	Serial.printf("\n[udpMulti_send]\n\tme: %s\n%s\n",
	 hostname.c_str(), msg.c_str());

			String host = literal_item("host", 	hostname);
			String from	= literal_item("from", 	"server");
			msg 		= literal_item("msg", 	msg);

			ALS_udpMulti->send(host+from+msg);

		}

		~ALS_udpServer(){};
		
	};


#endif