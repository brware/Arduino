//------------------------------------------------------------------------------------------
//
//  Classe pa funções basicas de MQTT
//
//-----------------------------------------------------------------------------------------

#ifndef brwMqtt_h
#define brwMqtt_h
 
#include <Arduino.h>
#include <brwAuxiliares.h>



class brwMqtt : public brwAuxiliares
{
    public:
      brwMqtt ();

      void setMqttID (String ID);
      void setMqttUsar (bool usar);
      void setMqttHost (String host);
      void setMqttPort (int port);
      void setMqttUser (String user);
      void setMqttPassWord (String pwd);

      void setTopicoPub (String topico);
      void setTopicoSub (String topico);

      void setTopicoConnect (String topico);
      void setTopicoWill (String topico);
      void setTopicoConfigPub (String topico);
      void setTopicoConfigSub (String topico);
      
      String getMqttID ();
      bool getMqttUsar ();
      String getMqttHost ();
      int    getMqttPort ();
      String getMqttUser ();
      String getMqttPassWord ();
      
      String getTopicoPub ();
      String getTopicoSub ();

      String getTopicoConnect ();
      String getTopicoWill ();
      String getTopicoConfigPub ();
      String getTopicoConfigSub ();
     
      

      String mqttID;
      bool usarMQTT;
      String mqttHost;
      int mqttPort;
      String mqttUser;
      String mqttPassWord;

      String topicoPub  = "";
      String topicoSub  = "";
      String topicoConnect  = "";
      String topicoWill  = "";
      String topicoConfigPub  = "";
      String topicoConfigSub  = "";

      bool mudouCfgMqtt = false;
        
 
    private:


      
      
		  
};

#endif
