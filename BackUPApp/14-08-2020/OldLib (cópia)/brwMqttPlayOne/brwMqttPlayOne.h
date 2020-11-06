//------------------------------------------------------------------------------------------
//
//  Classe pa funções basicas de MQTT
//
//-----------------------------------------------------------------------------------------

#ifndef brwMqttPlayOne_h
#define brwMqttPlayOne_h

 
#include <Arduino.h>
#include <Ethernet.h>
#include <MQTT.h> 
#include <brwMqtt.h>
#include <brwSpiffs.h>



class brwMqttPlayOne : public brwSpiffs , public brwMqtt

{
    public:
		brwMqttPlayOne ();
		int configMqtt (String IDmqtt);
		void desconectar ();
		static void mqttMessageReceived(String &topic, String &payload);
		void setMsgPub (String msg);
		void sendTopicoPub (String msg);
		void sendTopicoConnect (String msg);
		void sendTopicoConfigPub (String msg);
		
      	
      
      
  
 
    private:
		EthernetClient ethCli; // 
		MQTTClient mqttClient; //
		String msgPub = "";

		
		void MqttLoop ();
		static void staticMqttLoop  (void *pvParameter);


};

#endif
