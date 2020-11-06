//------------------------------------------------------------------------------------------
//
//  Classe de para Configuração basica de Rede
//
//-------------------------------------------------------------------------------------------

#ifndef brwCfgEth_h
#define brwCfgEth_h

#define Hardware_v2
 
#include <Arduino.h>
#include <Ethernet.h>

#ifndef Hardware_v2
	#include <brwHardware.h>
#else
	#include <brwHardwareV2.h>
#endif

//#include <brwHardwareV2.h>
#include <brwSpiffs.h>
#include <brwAuxiliares.h>



class brwCfgEth : public brwHardware, public brwAuxiliares
{
    public:
		brwCfgEth ();
		void beginEth (); // para DHCP
		void beginEth (String IP, String MK, String GW, String DNS); // para Ip Fixo


		void setMacAddress ();

		void setUsarIpFixo (bool val);
		void setIP (String ip);
		void setMascara (String mk);
		void setGateWay (String gw);
		void setDNserver (String dns);
		void setModeloHardware ();
		void setStatusEthDescricao ();

		bool getUsarIpFixo ();
		String getChipID ();
		String getMacAddress ();
		String getIP ();
		String getMascara ();
		String getGateWay ();
		String getDNserver ();
		String getModeloHardware ();
		String getStatusEthDescricao ();
		bool getStatusEth ();
		bool checaConexaotEth ();

		bool mudouCfgEth = false;

		

      
      
      

    private:

		// Funcoes de rede
		void WizReset();
		String prt_hwval(uint8_t refval);
		String prt_ethval(uint8_t refval);

		// Tasks da Classe
		static void staticPiscaLed (void *pvParameter);
		void PiscaLed ();
		static void staticEthLoop  (void *pvParameter);
		void EthLoop (); // lop que checha conexão Ethernet (** tem de estar numa task)

		void trataHtml ();    

		uint64_t chipid; 
		uint8_t mac_int[6]; // Mac em formato "inteiro"
		String mac_str;     // Mac  em formato String

		bool usarIpFixo = false;
		String strIP;
		String strMASK;
		String strGW;
		String strDNS;

		uint8_t  myIP [4];
		uint8_t  myMASK [4];
		uint8_t  myGW [4];
		uint8_t  myDNS [4];

		bool stuEth = false; // status da placa de rede
		bool stuOldEth = true;// status anterior da rede
		String stuEthDescricao; // descrição do Status do Harware
		String modeloHardware; // modelo da placa de rede
  		 
      
};

#endif
