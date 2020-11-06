
#include <brwMqttPlayOne.h>




//------------------------------------------------------------------------------------------
//
//  Construtor da Classe de configuração de rede
//
//-------------------------------------------------------------------------------------------

brwMqttPlayOne::brwMqttPlayOne (){

  
}





//------------------------------------------------------------------------------------------
//
// ----------------- Configura MQTT
//
//------------------------------------------------------------------------------------------

int brwMqttPlayOne::configMqtt (String IDmqtt) {
  int i = 0;
  String host, user, pwd;
  String  str, str2;
  char aux [30];
   
  
  setTopicoPub ("PlayadeOne/Pub");
  setTopicoSub ("PlayadeOne/Sub");
  setTopicoConnect ("PlayadeOne/Connect");
  setTopicoWill ("PlayadeOne/Will");
  setTopicoConfigPub ("PlayadeOne/ConfigPub");
  setTopicoConfigSub ("PlayadeOne/ConfigSub");
 

  Serial.print ("Construtor MQTT: ");
  // setup mqtt client
  setMqttID (IDmqtt); 
  Serial.println (IDmqtt.c_str());

  host = getMqttHost();
  


  mqttClient.begin(host.c_str(),ethCli);

  
  vTaskDelay(pdMS_TO_TICKS(500));
  
  // Configuração do Ultimo testamento
  str = getTopicoWill ();
  strcpy (aux,str.c_str());
  str2 = IDmqtt + ";DESCONEXÃO INESPERADA"; 
  mqttClient.setWill(aux, (char *)str2.c_str());


  
  user = getMqttUser();
  pwd = getMqttPassWord();

    
  
  Serial.print ("- Aguardando MQTT: ");
  while (!mqttClient.connect(IDmqtt.c_str(), user.c_str(), pwd.c_str(), false)) {
    vTaskDelay(pdMS_TO_TICKS(1000));
    Serial.print (".");
   i++; 
   if (i > 5) {
     break; 
   }
  }
  Serial.println ("");

  if ((!mqttClient.connected())) {
      Serial.println ("MQTT NÃO Conectou");
      setMqttUsar (false);
      return (0);
  } else {
      Serial.println ("MQTT Conectou");
      sendTopicoConnect (IDmqtt + ";CONECTOU");  
      mqttClient.onMessage(mqttMessageReceived);
      mqttClient.subscribe(getTopicoSub ());
      mqttClient.subscribe(getTopicoConfigSub ());
     
      xTaskCreate(&brwMqttPlayOne::staticMqttLoop, "MqttLoop", 4048, (void*) this, 4, NULL);

      return (1);
  }
  
  

  
}


void brwMqttPlayOne::desconectar () {

  sendTopicoConnect (getMqttID () + ";DESCONEXÃO (Reiniciou)");  
  mqttClient.disconnect();

}



//------------------------------------------------------------------------------------------
//
// ----------------- Envia menssagem para topico principal
//
//------------------------------------------------------------------------------------------

void brwMqttPlayOne::sendTopicoPub (String msg) {

     if (getMqttUsar () && mqttClient.connected()) {

       mqttClient.publish(getTopicoPub (), msg); 
       Serial.println ("Enviado para " + getTopicoPub () + " a msg: " + msg);
    } 



}


//------------------------------------------------------------------------------------------
//
// ----------------- Envia menssagem para topico de quanto conecta
//
//------------------------------------------------------------------------------------------

void brwMqttPlayOne::sendTopicoConnect (String msg) {

     if (getMqttUsar () && mqttClient.connected()) {

       mqttClient.publish(getTopicoConnect (), msg);

    }

}


//------------------------------------------------------------------------------------------
//
// ----------------- Envia menssagem para topico de configuração
//
//------------------------------------------------------------------------------------------

void brwMqttPlayOne::sendTopicoConfigPub (String msg) {

     if (getMqttUsar () && mqttClient.connected()) {

       mqttClient.publish(getTopicoConfigPub (), msg);

    }

}

void brwMqttPlayOne::setMsgPub (String msg) {
  
  msgPub = msg;

}

//------------------------------------------------------------------------------------------
//
// ----------------- Funçao de Call Back Do MQTT
//
//------------------------------------------------------------------------------------------

void brwMqttPlayOne::mqttMessageReceived(String &topic, String &payload) {
  Serial.println("Recebeu pelo MQTT: " + topic + " - " + payload);
  //trataComando (payload);
  
}


//------------------------------------------------------------------------------------------
//
// ----------------- Task de Loop do MQTT
//
//------------------------------------------------------------------------------------------

void brwMqttPlayOne::MqttLoop () {

    while (true) {
      vTaskDelay(pdMS_TO_TICKS(200));
      if (msgPub != "") {
            sendTopicoPub (msgPub);
            msgPub = "";
          }
      mqttClient.loop();
      vTaskDelay(pdMS_TO_TICKS(500));
      }
   
}


void brwMqttPlayOne::staticMqttLoop  (void *pvParameter) {

    brwMqttPlayOne* mq = reinterpret_cast<brwMqttPlayOne*>(pvParameter); //obtain the instance pointer
    mq->MqttLoop(); //dispatch to the member function, now that we have an instance pointer

}

/*

//------------------------------------------------------------------------------------------
//
//  --------    Verififica se existe mqtt.cfg, se existe le, senão usa DHCP
//
//------------------------------------------------------------------------------------------

bool brwMqttPlayOne::existeCgfMqtt () {

File wFile;

  
  
  if(SPIFFS.exists(ARQ_MQTT)) {
    //Serial.println ("Existe mqtt.cfg");
    if (!leCfgMqtt ()) {
        //Serial.println ("Não leu mqtt.cfg");
        formatFS();
        ESP.restart ();
    }
    else{
        //Serial.println ("Leu mqtt.cfg");
        return (true);   
    }
    
  } else {
    //Serial.println ("NAO Existe mqtt.cfg");
    setMqttUsar (false);
    setMqttHost ("brware.com.br");
    setMqttPort (1883);
    setMqttUser ("brware");
    setMqttPassWord ("SQTR(pi)!=314");
  
    salvaCfgMqtt ();
    
  }

}



//------------------------------------------------------------------------------------------
//
//  --------    Salva configurações do mqtt em mqtt.cfg
//
//------------------------------------------------------------------------------------------

bool brwMqttPlayOne::salvaCfgMqtt () {

	File wFile;
    bool b_aux;


    wFile = SPIFFS.open(ARQ_MQTT, "w");
    if(!wFile){
        Serial.println ("Não foi possivel salvar o arquivo de configuração");
        return (false);  
    }

    
    if (getMqttUsar ())
        wFile.println ("true");
    else
        wFile.println ("false");     
    wFile.println (getMqttHost ());
    wFile.println (getMqttPort ());
    wFile.println (getMqttUser ());
    wFile.println (getMqttPassWord ());

    wFile.println (getTopicoPub ());
    wFile.println (getTopicoSub ());
    wFile.println (getTopicoConnect ());
    wFile.println (getTopicoWill ());
    wFile.println (getTopicoConfigPub ());
    wFile.println (getTopicoConfigSub ());

    wFile.close ();
    
    return (true);

}

void brwMqttPlayOne::salvaCfg () {

  salvaCfgMqtt ();

}




//------------------------------------------------------------------------------------------
//
//  --------    Leconfigurações do mqtt em mqtt.cfg
//
//------------------------------------------------------------------------------------------


bool brwMqttPlayOne::leCfgMqtt () {

File wFile;
String str;

    wFile = SPIFFS.open(ARQ_MQTT, "r");
    if(!wFile){
        Serial.println ("Não foi possivel abrir o arquivo de configuração");
        return (false);  
    }

    str = wFile.readStringUntil('\n');
    if (str == "true")
        setMqttUsar (true);
    else
        setMqttUsar (false); 
           
    setMqttHost (wFile.readStringUntil('\n'));
    str = wFile.readStringUntil('\n');
    setMqttPort(str.toInt ());
    setMqttUser (wFile.readStringUntil('\n'));
    setMqttPassWord(wFile.readStringUntil('\n'));

    setTopicoPub (wFile.readStringUntil('\n'));
    setTopicoSub(wFile.readStringUntil('\n'));
    setTopicoConnect (wFile.readStringUntil('\n'));
    setTopicoWill(wFile.readStringUntil('\n'));
    setTopicoConfigPub (wFile.readStringUntil('\n'));
    setTopicoConfigSub(wFile.readStringUntil('\n'));

    wFile.close ();
    
    return (true);

}

void brwMqttPlayOne::leCfg () {

  leCfgMqtt();

}
*/