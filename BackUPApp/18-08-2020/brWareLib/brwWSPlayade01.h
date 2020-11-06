#ifndef brwWSPlayade01_h
#define brwWSPlayade01_h

#include <Arduino.h>
#include <Ethernet.h>
#include "brwEthCfg.h"
#include "brwDHT.h"

class brwWSPlayade01
{

	public:
		brwWSPlayade01 ();
		void beginWebServer (brwEthCfg* EthCfg, brwDHT* DHT);


	private:	


        void trataHtml_eth ();
    	void trataHtml_mqtt ();

        void sendIndex (EthernetClient client);
        void indexPlayade01 (EthernetClient client);
		void olaResponse (EthernetClient client);
		void config_html (EthernetClient client);
		void configPlayade01 (EthernetClient client); // temperatura de CPD

		void trataGet (EthernetClient client, String recget);
		void trataGetPlayade01 (EthernetClient client, String recget);
		void getResponsePlayade01 (EthernetClient client);
		

		void webServerLoop ();
		static void staticWebServerLoop  (void *pvParameter);

		brwEthCfg* pEthCfg;
		brwDHT* pDHT;

}




#endif