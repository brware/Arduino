//------------------------------------------------------------------------------------------
//
//  Classe pa funções basicas de MQTT
//
//-----------------------------------------------------------------------------------------

#ifndef brwMqtt_h
#define brwMqtt_h
 
#include <Arduino.h>
#include <PubSubClient.h>
#include "brwAuxiliares.h"
#include "brwEthCfg.h"
#include "brwHardwareApp.h"
#include "brwMutex.h"



class brwMqtt : public brwAuxiliares, brwMutex
{
    public:
      brwMqtt ();

      bool beginMQTT (brwEthCfg *brwEth, brwHardwareApp* BrwHwApp, char *App);

      void setMqttID (char* ID);
      void setMqttUsar (bool usar);
      void setMqttHost (char* host);
      void setMqttPort (int port);
      void setMqttUser (char* user);
      void setMqttPassWord (char* pwd);

      void setTopicoPub (char* topico);
      void setTopicoSub (char* topico);

      void setTopicoConnect (char* topico);
      void setTopicoWill (char* topico);
      void setTopicoConfigPub (char* topico);
      void setTopicoConfigSub (char* topico);
      
      char* getMqttID ();
      bool getMqttUsar ();
      char* getMqttHost ();
      int    getMqttPort ();
      char* getMqttUser ();
      char* getMqttPassWord ();
      
      char* getTopicoPub ();
      char* getTopicoSub ();

      char* getTopicoConnect ();
      char* getTopicoWill ();
      char* getTopicoConfigPub ();
      char* getTopicoConfigSub ();
     

      static void callback(char* topic, byte* payload, unsigned int length);
      void printMqttCfg ();
      void MqttLoop ();
      static void staticMqttLoop  (void *pvParameter);
      void trataComandoLoop ();
      static void staticTrataComandoLoop  (void *pvParameter);

      bool getMqttConfig ();
      bool setMqttConfig ();
      void printMqttCfg ();



      char mqttID [20];
      bool usarMQTT;
      char mqttHost [50];
      int mqttPort;
      char mqttUser [30];
      char mqttPassWord [30];

      char topicoPub [50];
      char topicoSub [50];
      char topicoConnect [50];
      char topicoWill [50];
      char topicoConfigPub [50];
      char topicoConfigSub [50];

      bool mudouCfgMqtt = false;

      bool enviouCfgSub = false; // semaforos de comunicação

       
      
        
 
    private:
      brwEthCfg *pEth;
      brwHardwareApp* pHwApp;
      EthernetClient ethClient;
      PubSubClient mqttClient;
      //char msgPub [255];
      
      

      bool beginSucres (); // inicializa MQTT para SUCRES
      bool beginNetUno (); // inicializa MQTT para NetUno 

      bool getMqttConfig ();
      void setMqttDefault (); // coloca valore defalt nas variaveis
      
      bool setMqttConfig ();
      
      bool conectar ();
      bool desconectar ();

      void trataComandoSub (char * cmd);
      void trataComandoConfigSub (char* cmd);

      void sendTopicoPub (char* msg);
      void sendTopicoConnect (char* msg);

      void setMqttDefaut ();

      

      
      
      
		  
};

#endif
