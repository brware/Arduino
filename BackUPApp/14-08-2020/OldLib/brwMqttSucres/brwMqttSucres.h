//------------------------------------------------------------------------------------------
//
//  Classe pa funções basicas de MQTT
//
//-----------------------------------------------------------------------------------------

#ifndef brwMqttSucres_h
#define brwMqttSucres_h

 
#include <Arduino.h>
#include <Ethernet.h>
#include <brwCfgEth.h>
//#include <MQTT.h> 
#include <PubSubClient.h>
#include <brwMqtt.h>
#include <brwSpiffs.h>



//class brwMqttSucres : public brwSpiffs , public brwMqtt, public brwCfgEth
class brwMqttSucres : public brwMqtt, public brwCfgEth

{
    public:
		brwMqttSucres ();
		int configMqtt (String IDmqtt);
		void conectar ();
		void desconectar ();
		static void callback(char* topic, byte* payload, unsigned int length);
		void setMsgPub (String msg);
		void sendTopicoPub (String msg);
		void sendTopicoConnect (String msg);
		void sendTopicoConfigPub (String msg);

		void trataComandoSub (String cmd);
		void trataComandoConfigSub (String cmd);
		void getConfigPub (String cmd);
		static String pl; // fila de comamndos recebidos
		static String tp; // fila dos topicos recebidos

		// semaforos de comunicação
		bool enviouCfgSub = false;
		

		

      	
        
      
  
 
    private:
		EthernetClient ethClient;
		PubSubClient mqttClient;
		String msgPub = "";
		
		
		void MqttLoop ();
		static void staticMqttLoop  (void *pvParameter);
		void trataComandoLoop ();
		static void staticTrataComandoLoop  (void *pvParameter);

		void trataRele03 ();
      	static void staticTrataRele03 (void *pvParameter);

		
		void verificaPortoes (long tmp, String cmd);
		bool leEntrada (int entrada);


};

#endif
