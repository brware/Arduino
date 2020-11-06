//------------------------------------------------------------------------------------------
//
//  Classe de para Configuração basica de Rede
//
//-------------------------------------------------------------------------------------------



#ifndef brwEthCfg_h
#define brwEthCfg_h




#include <Arduino.h>
#include <Ethernet.h>
#include "brwAuxiliares.h"
#include "brwHardwareCfg.h"


	

class brwEthCfg : public brwAuxiliares
{
    public:
		brwEthCfg ();
		void beginEth (brwHardwareCfg* brwHW); // 
		
		
		void setMacAddress ();

		void setUsarIpFixo (bool val);
		void setIP (char* ip);
		void setMascara (char* mk);
		void setGateWay (char* gw);
		void setDNserver (char* dns);
		void setModeloHardware ();
		void setStatusEthDescricao ();

		bool getUsarIpFixo ();
		char* getChipID ();
		char* getMacAddress ();
		char* getIP ();
		char* getMascara ();
		char* getGateWay ();
		char* getDNserver ();
		char* getModeloHardware ();
		char* getStatusEthDescricao ();
		bool getStatusEth ();
		bool checaConexaotEth ();

		bool getEthConfig ();
      	bool setEthConfig ();
		void printEthCfg ();


        brwHardwareCfg* pBrwHW; // ponteiro para classe de hardware
    
      

    private:



    	// Default para criação do arquivo 
		char* cIP_FIXO  = "false";
		char* cIP       = "192.168.15.17";
		char* cMASK     = "255.255.255.0";
		char* cGATEWAY  = "192.168.15.200";
		char* cDNS      = "192.168.15.200";

    	

		// Funcoes de rede

		void initEth (); // para DHCP
		void initEthFixo (); // para Ip Fixo

		void WizReset();
		String prt_hwval(uint8_t refval);
		String prt_ethval(uint8_t refval);

		// Tasks da Classe
		static void staticPiscaLed (void *pvParameter);
		void PiscaLed ();
		static void staticEthLoop  (void *pvParameter);
		void EthLoop (); // lop que checha conexão Ethernet (** tem de estar numa task)

		void webServerLoop ();
		static void staticWebServerLoop  (void *pvParameter);
        

		
      	void setEthDefaut ();
  

		uint64_t chipid; 
		uint8_t mac_int[6]; // Mac em formato "inteiro"
		char mac_str [20];     // Mac  em formato String

		bool usarIpFixo = false;

		char strIP [16];
		char strMASK [16];
		char strGW [16];
		char strDNS [16];

		uint8_t  myIP [4];
		uint8_t  myMASK [4];
		uint8_t  myGW [4];
		uint8_t  myDNS [4];

		bool stuEth = false; // status da placa de rede
		bool stuOldEth = true;// status anterior da rede
		char stuEthDescricao [50]; // descrição do Status do Harware
		char modeloHardware [50]; // modelo da placa de rede

	
};

#endif
