/*
Instrucços para fazer O Python

 - Ao conecatar Publica no topico "Sucres/Connect" a menssagem: "MAC;CONECTOU";
 - Se houver uma deconexão inesperada Publica no topico "Sucres/Connect" a menssagem: "MAC;DESCONEXÃO INESPERADA"; 
 - Se deconecta por solicitação Publica no topico "Sucres/Connect" a menssagem: "MAC;DESCONECTOU"

 - Se o Iniciar as configurações são Default
    - Publica no topico "Sucres/ConfigPub" a menssagem: 
        "MAC;REDE" -> pede cfg de rede
        "MAC;MQTT" -> pede cfg de MQTT
        "MAC;RELE" -> pede configurações de tempo de rele

 - A qualquer momento pode receber no topico "Sucres/ConfigPub":
    Messagem MAC;REDE;UIP;IP;MK;GW;DNS
             MAC;MQTT;UMQ;Host;Porta;User;Senha
             MAC;RELE;RLG;RLN 



*/
#include <brwMqttSucres.h>

String brwMqttSucres::pl = ""; // fila de comamndos recebidos
String brwMqttSucres::tp = ""; // fila dos topicos recebidos


//------------------------------------------------------------------------------------------
//
//  Construtor da Classe de configuração de rede
//
//-------------------------------------------------------------------------------------------

brwMqttSucres::brwMqttSucres (){

  
}




//------------------------------------------------------------------------------------------
//
// ----------------- Configura MQTT
//
//------------------------------------------------------------------------------------------

int brwMqttSucres::configMqtt (String IDmqtt) {
  

  
  //setTopicoPub ("Sucres/Pub");
  //setTopicoSub ("Sucres/Sub");
  setTopicoPub ("sucres/efetivacao");
  setTopicoSub ("sucres/solicitacao");
  setTopicoConnect ("Sucres/Connect");
  setTopicoWill ("Sucres/Connect");
  setTopicoConfigPub ("Sucres/ConfigPub");
  setTopicoConfigSub ("Sucres/ConfigSub");
 

  //Serial.print ("Construtor MQTT: ");
  // setup mqtt client
  setMqttID (IDmqtt); 
  //Serial.println (IDmqtt.c_str());

  // SET ETH do MQTT
  

  mqttClient.setClient (ethClient);

  mqttClient.setServer(mqttHost.c_str(),1883);
  mqttClient.setCallback(callback);
  //mqttClient.setServer("162.241.88.226",1883);
  

  conectar ();

  
     
  xTaskCreate(&brwMqttSucres::staticMqttLoop, "MqttLoop", 4048, (void*) this, 2, NULL);
  vTaskDelay(pdMS_TO_TICKS(300));
  xTaskCreate(&brwMqttSucres::staticTrataComandoLoop, "trataComandoLoop", 4048, (void*) this, 4, NULL);

  return (1);
 
  
  

  
}

void brwMqttSucres::conectar () {
  int i = 0;
  String  will;
  String s;

  // Menssagem Will
  will = mqttID + ";DESCONEXÃO INESPERADA";  // messagem WILL
  
     
  
  Serial.print ("- Aguardando MQTT (com id): " + mqttID);

  while (!mqttClient.connect (mqttID.c_str(), mqttUser.c_str(), mqttPassWord.c_str(), topicoWill.c_str(), 0, 0, will.c_str(), true)) {
  //while (!mqttClient.connect ("255", mqttUser.c_str(), mqttPassWord.c_str(), topicoWill.c_str(), 0, 0, will.c_str(), true)) {
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
      Serial.print("Falhou (retorno) : ");
      Serial.println(mqttClient.state());
      setMqttUsar (false);
  } else {
      Serial.println ("MQTT Conectou");
      //s = topicoSub + "/" + getMqttID ();
      //Serial.println ("*** Topico: " + s);
      mqttClient.subscribe(topicoSub.c_str());
      //mqttClient.subscribe(s.c_str());
      //s = topicoConfigSub + "/" + getMqttID ();
      mqttClient.subscribe(topicoConfigSub.c_str());
      //mqttClient.subscribe(s.c_str());
      sendTopicoConnect (mqttID + ";CONECTOU"); 
  }

}



void brwMqttSucres::desconectar () {
  String str;

  str = getMqttID () + ";DESCONECTOU";

  sendTopicoConnect (str.c_str()); 

  mqttClient.disconnect();

}



//------------------------------------------------------------------------------------------
//
// ----------------- Envia menssagem para topico principal
//
//------------------------------------------------------------------------------------------

void brwMqttSucres::sendTopicoPub (String msg) {

     
     if (getMqttUsar () && mqttClient.connected()) {
       mqttClient.publish(topicoPub.c_str(), msg.c_str()); 
       Serial.println ("Enviado para " + getTopicoPub () + " a msg: " + msg);
    } else
        Serial.println ("MQTT não Conectado");



}


//------------------------------------------------------------------------------------------
//
// ----------------- Envia menssagem para topico de quanto conecta
//
//------------------------------------------------------------------------------------------

void brwMqttSucres::sendTopicoConnect (String msg) {

     if (getMqttUsar () && mqttClient.connected()) {

       mqttClient.publish(topicoConnect.c_str(), msg.c_str());

    }

}


//------------------------------------------------------------------------------------------
//
// ----------------- Envia menssagem para topico de configuração
//
//------------------------------------------------------------------------------------------

void brwMqttSucres::sendTopicoConfigPub (String msg) {

     //Serial.println ("Vai enviar Enviar: " + msg); 
     setMqttUsar (true); 
     if (getMqttUsar () && mqttClient.connected()) {

       mqttClient.publish(topicoConfigPub.c_str(), msg.c_str());
       Serial.println ("Enviado: " + msg); 

    } else {
        Serial.println ("NAO Enviado: " + msg);
        Serial.println ("Usar MQTT : " + String(getMqttUsar ()));
        Serial.println ("MQTT Status: " + String (mqttClient.connected()));
        mqttClient.publish(topicoConfigPub.c_str(), msg.c_str());
        Serial.println ("Enviado: " + msg); 
    }
         

}

void brwMqttSucres::setMsgPub (String msg) {
  
  msgPub = msg;

}




//------------------------------------------------------------------------------------------
//
// ----------------- Task de Loop do MQTT
//
//------------------------------------------------------------------------------------------

void brwMqttSucres::MqttLoop () {

    while (true) {
      vTaskDelay(pdMS_TO_TICKS(10));
      /*if (msgPub != "") {
            sendTopicoPub (msgPub);
            msgPub = "";
          }*/
      mqttClient.loop();
      
      if (!mqttClient.connected ()) {
        Serial.println ("*** LOOP - Tentando Conecat no MQTT ***");
        conectar ();
        vTaskDelay(pdMS_TO_TICKS(200));
      }
      //vTaskDelay(pdMS_TO_TICKS(200));
      }
   
}


void brwMqttSucres::staticMqttLoop  (void *pvParameter) {

    brwMqttSucres* mq = reinterpret_cast<brwMqttSucres*>(pvParameter); //obtain the instance pointer
    mq->MqttLoop(); //dispatch to the member function, now that we have an instance pointer

}

//------------------------------------------------------------------------------------------
//
// ----------------- Task de Loop de tratar comandos
//
//------------------------------------------------------------------------------------------

void brwMqttSucres::trataComandoLoop () {
  int i;
    while (true) {
      vTaskDelay(pdMS_TO_TICKS(100));
      if (pl.length() > 0) {
          if (tp == getTopicoSub ()) { // recebeu  Dados
            trataComandoSub (pl);
          }
          if (tp == getTopicoConfigSub ()) { // recebeu u configuração
            trataComandoConfigSub (pl);
          }  
        pl = "";
        tp = "";   
      vTaskDelay(pdMS_TO_TICKS(200));     
      }
      
    }
   
}

void brwMqttSucres::staticTrataComandoLoop  (void *pvParameter) {

    brwMqttSucres* mq = reinterpret_cast<brwMqttSucres*>(pvParameter); //obtain the instance pointer
    mq->trataComandoLoop(); //dispatch to the member function, now that we have an instance pointer

}

//------------------------------------------------------------------------------------------
//
// ----------------- Funçao de Call Back Do MQTT
//
//------------------------------------------------------------------------------------------

//void brwMqttSucres::mqttMessageReceived(String &topic, String &payload) {
void brwMqttSucres::callback (char* topic, byte* payload, unsigned int length) {
  int i;
  char aux [128];

  
  Serial.print("Recebeu pelo MQTT: ");
  Serial.print(topic);
  //Serial.print( " - ");
  

  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    aux[i] = (char)payload[i];
    aux[i + 1] = '\0';
  }
  //Serial.println();
  //Serial.println(aux);
  //Serial.println(aux);
  
  pl = String(aux);
  tp = String(topic);
  
  Serial.println ("Topico: " + tp);
  Serial.println ("PayLoad: " + pl);


  
}


//*****************************************************************************************
//
//        TRATA DADOS RECEBIDO
//
//*****************************************************************************************

void brwMqttSucres::trataComandoSub (String cmd) {
  char rec [127];
  char *mac;
  char *usr;
  char *cli;
  char *eqp;
  char *ord;
  int rle, i;
  long tmp;
  String str;
  
  
  //Serial.println (rec);
  cmd.toCharArray (rec, (cmd.length()+1));
  
  
  mac  = strtok(rec, ";"); // mac address da placa
  
  usr  = strtok(NULL, ";"); // id do usuario que executou a ção
  cli  = strtok(NULL, ";"); // cliente is 
  eqp  = strtok(NULL, ";"); // equiapamento ID
  ord  = strtok(NULL, ";"); // comamdo
  
  
  str = getMqttID ();
  //str = "27";
  
  if (strcmp (str.c_str(),mac) != 0) {
    Serial.println ("Não Foi Pra Mim!!! - MAC: " + String(mac));
    return;
  }
 
 
  rle = atoi(ord);

  switch (rle) {
    case 1 :
        tmp = getTempoRele (1) * 1000; 
        actRele (rle,tmp);
        setRele (2,HIGH);
        vTaskDelay(pdMS_TO_TICKS(tmp));
        sendTopicoPub (cmd);
        break;
    case 2 :   
        setRele (2,LOW); 
        sendTopicoPub (cmd);
        break;
    case 3 :  
        msgPub = cmd;
        xTaskCreate(&brwMqttSucres::staticTrataRele03, "trataRele03", 2048, (void*) this, 6, NULL);
        break;    
    case 4 :  
        tmp = getTempoRele (1) * 1000;
        verificaPortoes (tmp, cmd);
        return;     
  }
  
/*
  Serial.println ("Aguardando : " + String (getTempoRele (rle)) + " segundos no Rele: " + String (rle));

  vTaskDelay(pdMS_TO_TICKS(tmp));
  
  if (rle < 4)
    sendTopicoPub (cmd);
 */
    
}


void brwMqttSucres::trataRele03 () {
  long tmp;

  
  tmp = getTempoRele (3) * 1000; 
  //actRele (3, tmp);
  setRele (3,HIGH);
  
  vTaskDelay(pdMS_TO_TICKS(tmp));
  setRele (3,LOW);
  
  sendTopicoPub (msgPub);
  msgPub = "";

  vTaskDelete(NULL); // delete task

}


void brwMqttSucres::staticTrataRele03 (void *pvParameter) {

    brwMqttSucres* tk = reinterpret_cast<brwMqttSucres*>(pvParameter); //obtain the instance pointer
    tk->trataRele03(); //dispatch to the member function, now that we have an instance pointer

}


void brwMqttSucres::verificaPortoes (long tmp, String cmd) {
  


  if (leEntrada (S01) == HIGH) { // alarme desativado
    Serial.println ("Alarme Desativado");
    sendTopicoPub (cmd + "0");
    return;
  } else
      Serial.println ("Alarme Ativado"); 

  if (leEntrada (S02) == true) {  // Portão aberto  
    Serial.println ("Portão 01 (S02): Aberto"); 
    actRele (4, tmp);
    vTaskDelay(pdMS_TO_TICKS(tmp));
    sendTopicoPub (cmd + "1");
  } else 
      Serial.println ("Portão 01 (S02): Fechado"); 
    
    
  if (leEntrada (S03) == true) {  // Portão aberto  
    Serial.println ("Portão 02 (S02): Aberto"); 
    actRele (5, tmp);
    vTaskDelay(pdMS_TO_TICKS(tmp));
    sendTopicoPub (cmd + "2");
  } else
      Serial.println ("Portão 02 (S03): Fechado");

  if (leEntrada (S04) == true) {  // Portão aberto  
    Serial.println ("Portão 03 (S03): Aberto");
    actRele (6, tmp);
    vTaskDelay(pdMS_TO_TICKS(tmp));
    sendTopicoPub (cmd + "3");
  }else
    Serial.println ("Portão 03 (S04): Fechado"); 
  
  if (leEntrada (S05) == true) {  // Portão aberto  
    Serial.println ("Portão 04 (S05): Aberto");
    actRele (7, tmp);
    vTaskDelay(pdMS_TO_TICKS(tmp));
    sendTopicoPub (cmd + "4");
  } else 
      Serial.println ("Portão 04 (S05) - Fechado");
  

}

bool brwMqttSucres::leEntrada (int entrada) {
  int vezes, cnt;

  cnt = 0; 
  for (vezes = 0; vezes < 100; vezes++) {
    if (digitalRead (entrada) == HIGH)
      cnt++;
    vTaskDelay(pdMS_TO_TICKS(10));   
  }
 
 if (cnt > 80)
  return (true);
 else
  return (false);
     
  
}


//----------------------------------------------------------------------------------------
//
//        TRATA CONFIGURAÇÃO RECEBIDO
//
//----------------------------------------------------------------------------------------

void brwMqttSucres::trataComandoConfigSub (String cmd) {
  char rec [128];
  char *mac;
  char *cmdm;
  char *c01;
  char *c02;
  char *c03;
  char *c04;
  char *c05;
  char *c06;
  char *c07;
  bool tmp;
  
  String str;
  // MAC;REDE;DHCP;IP;MASCARA;GATEWAY;DNS
  // MAC;MQTT;USAR;HOST;PORTA;USUARIO;SENHA
  // MAC;RELE;TEMPO RL01;TEMPO RL03
  
  //Serial.println (cmd);
  cmd.toCharArray (rec, (cmd.length()+1));
  
  
  mac  = strtok(rec, ";");
  str = getMqttID ();
  
  
  if (strcmp (str.c_str(),mac) != 0) {
    //Serial.println ("Não Foi Pra Mim!!! - MAC");
    return;
  }
  
  cmdm  = strtok(NULL, ";");
  
  
  if (strcmp("REDE", cmdm) == 0) { // configurações de rede
    c01  = strtok(NULL, ";");
    c02  = strtok(NULL, ";");
    c03  = strtok(NULL, ";");
    c04  = strtok(NULL, ";");
    c05  = strtok(NULL, ";");
    if (strcmp("1", c01) == 0)
      setUsarIpFixo (true);
    else
      setUsarIpFixo (false);
    setIP (String (c02));
    setMascara (String(c03));
    setGateWay (String (c04));
    setDNserver (String (c05));
  }

  if (strcmp("MQTT", cmdm) == 0) { // configurações de rede
    c01  = strtok(NULL, ";");
    c02  = strtok(NULL, ";");
    c03  = strtok(NULL, ";");
    c04  = strtok(NULL, ";");
    setMqttHost (String (c01));
    setMqttPort (atoi(c02));
    setMqttUser (String (c03));
    setMqttPassWord (String (c04));
    setMqttUsar (true);
    
    
    
  }

  if (strcmp("RELE", cmdm) == 0) { // configurações de rede
    c01  = strtok(NULL, ";");
    c02  = strtok(NULL, ";");
    setTempoRele (1, atoi(c01));
    setTempoRele (2, atoi(c01));
    setTempoRele (3, atoi(c02));
    setTempoRele (4, atoi(c01));
    
  }
   
  enviouCfgSub = false;
    
}



//----------------------------------------------------------------------------------------
//
//        SOLICITA CONFIGURAÇÃO AO SERVIDOR
//
//----------------------------------------------------------------------------------------


void brwMqttSucres::getConfigPub (String cmd) {

  String msg;

  if (cmd == "REDE"){
    msg = getMqttID () + ";" + cmd;

  } 

  if (cmd == "MQTT"){
    msg = getMqttID () + ";" + cmd;

  }

  if (cmd == "RELE"){
    msg = getMqttID () + ";" + cmd;

  }
  enviouCfgSub = true;
  sendTopicoConfigPub (msg);

}