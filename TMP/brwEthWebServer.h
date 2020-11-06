// ******   REFERENCIAS

// tutorial html ->  https://www.w3schools.com/html/default.asp 
//  			 -> https://arduino-esp8266.readthedocs.io/en/latest/PROGMEM.html
//Variavei PROGMEM -> https://www.arduino.cc/reference/pt/language/variables/utilities/progmem/




//------------------------------------------------------------------------------------------
//
//  Classe de para funções e Servidor WEB
//
//-------------------------------------------------------------------------------------------

#ifndef brwEthWebServer_h
#define brwEthWebServer_h

#include <Arduino.h>
#include "brwHardware.h"
#include "brwCfgEth.h"
#include "brwMqtt.h"



class brwEthWebServer
{
    public:

        brwEthWebServer ();
		
		void wsBegin (char* App, brwHardware* hW, brwCfgEth* Eth, EthernetClient*  cli, EthernetServer* esvr);
		void wsBegin (char* App, brwHardware* hW, brwCfgEth* Eth, EthernetClient*  cli, EthernetServer* esvr , brwMqtt* MQ);

		void webServerLoop ();
		static void staticWebServerLoop  (void *pvParameter);

        


        brwHardware * phW;
        brwCfgEth* pEth; // ponteiro para classe de configuração do Eth
        brwMqtt* pMQ;
        EthernetClient*  pEcli;
        EthernetServer*  pEsvr;

    private:
    	

    	void trataHtml_eth ();
    	void trataHtml_mqtt ();

        void sendIndex ();
    	

        
        
};

#endif
