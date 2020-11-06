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


#include "brwMqtt.h"

// Variasveis globias que tem acesso em função static (callback)

static char pl [255]; // fila de comamndos recebidos
static char tp [255]; // fila dos topicos recebidos


brwMqtt::brwMqtt () {

	

}


//-----------------------------------------------------------------------------------
//
//------------------  INICIALIZA O MQTT
//
// --------->> App : Tipo de Apliacação, tls se vai usar certificado digital
//				- sucres
//				- netuno
//
//
//-----------------------------------------------------------------------------------

bool brwMqtt::beginMQTT (brwEthCfg *brwEth, brwHardwareApp* BrwHwApp, char *App) {
	int port;

	

  pEth = brwEth;
  pHwApp = BrwHwApp;

	setMqttID (pEth -> getMacAddress ());
	mqttClient.setClient (ethClient);
    if (tls)
    	port = 8883;
    else
    	port = 1883;
    setMqttHost (host);
    setMqttPort (port);

  	mqttClient.setServer(host,port);
  	mqttClient.setCallback(callback);


	if (strcmp (App,"sucres") == 0) {
		beginSucres ();
		
	}

	if (strcmp (App,"netuno") == 0) {
		beginNetUno ();
		
	}

	getMqttConfig ();

  //waitMutex (0);

 // if (conectar ()) {
  	xTaskCreate(&brwMqtt::staticMqttLoop, "MqttLoop", 4048, (void*) this, 2, NULL);
  	vTaskDelay(pdMS_TO_TICKS(300));
  	xTaskCreate(&brwMqtt::staticTrataComandoLoop, "trataComandoLoop", 4048, (void*) this, 4, NULL);	
  //}

  conectar ();
 

  return (true);

}


//----------------------------------------------------------------------------------
//
//------------------  INICIALIZA MQTT PARA SUCRES
//
//----------------------------------------------------------------------------------
bool brwMqtt::beginSucres () {

	if (pEth -> pBrwHW -> spifss -> isOpen == false)
      pEth -> pBrwHW -> spifss -> beginFS(); 

  setMqttUser ("brware");
  setMqttPassWord ("SQRT(pi)!=314");
  setTopicoPub ("sucres/efetivacao");
  setTopicoSub ("sucres/solicitacao");
  setTopicoConnect ("Sucres/Connect");
  setTopicoWill ("Sucres/Connect");
  setTopicoConfigPub ("Sucres/ConfigPub");
  setTopicoConfigSub ("Sucres/ConfigSub");

}


//----------------------------------------------------------------------------------
//
//------------------  INICIALIZA MQTT PARA SUCRES
//
//----------------------------------------------------------------------------------
bool brwMqtt::beginNetUno () {

	if (pEth -> pBrwHW -> spifss -> isOpen == false)
      pEth -> pBrwHW -> spifss -> beginFS(); 



}

//------------------------------------------------------------------------------------------
//
// ----------------- Funçao de Call Back Do MQTT
//
//------------------------------------------------------------------------------------------


void brwMqtt::callback (char* topic, byte* payload, unsigned int length) {
  int i;
  char aux [255];

  
  Serial.print("Recebeu pelo MQTT: ");
  Serial.print(topic);
  //Serial.print( " - ");
  

  for (int i = 0; i < length; i++) {
    aux[i] = (char)payload[i];
    aux[i + 1] = '\0';
  }

  
  strncpy (pl, aux, sizeof(pl));
  strncpy (tp,topic, sizeof(tp));
  
  //Serial.println ("Topico: " + tp);
  //Serial.println ("PayLoad: " + pl);


  
}

//----------------------------------------------------------------------------------
//
//------------------  CONECTA AO SERVER MQTT
//
//----------------------------------------------------------------------------------


bool brwMqtt::conectar () {
  int i = 0;
  char will [255];
  char aux [50];

  // Menssagem Will
  strncpy (will,getMqttID(),sizeof(will));
  strncat (will,";DESCONEXÃO INESPERADA", sizeof(will));
 
  Serial.println (getMqttID());
  Serial.println (getMqttUser());
  Serial.println (getMqttPassWord());
  Serial.println (getTopicoWill());
  //***** testes
  /*
  strcpy (aux, pEth -> getIP ());
  while (aux [i]) {
    Serial.print (i);
    Serial.print (" - ");
    Serial.println (aux[i]);
    i++;
  }
  Serial.println ("Fim IP");

  strcpy (aux, pEth -> getMascara ());
  i = 0;
  while (aux [i]) {
    Serial.print (i);
    Serial.print (" - ");
    Serial.println (aux[i]);
    i++;
  }
  Serial.println ("Fim mascara");
  */   
  
  //Serial.print ("- Aguardando MQTT (com id): " + mqttID);

  while (!mqttClient.connect (getMqttID(), getMqttUser(), getMqttPassWord(), getTopicoWill(), 0, 0, will, true)) {
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

      mqttClient.subscribe(getTopicoSub());
    
      mqttClient.subscribe(getTopicoConfigSub());
     
      strncpy (aux,getMqttID(),sizeof(aux));
      strncat (aux,";CONECTOU",sizeof(aux));
      sendTopicoConnect (aux); 
  }

}

//----------------------------------------------------------------------------------
//
//------------------  DESCONECTA DO SERVER MQTT
//
//----------------------------------------------------------------------------------

bool brwMqtt::desconectar () {
  char str [50];

  strncpy (str,getMqttID(),sizeof(str));
  strncat (str,";DESCONECTOU",sizeof(str));
  

  sendTopicoConnect (str); 

  mqttClient.disconnect();

}


//------------------------------------------------------------------------------------------
//
// ----------------- Envia menssagem para topico de quanto conecta
//
//------------------------------------------------------------------------------------------

void brwMqtt::sendTopicoConnect (char* msg) {

     if (getMqttUsar () && mqttClient.connected()) {

       mqttClient.publish(getTopicoConnect(), msg);

    }

}


//----------------------------------------------------------------------------------
//
//------------------  Funcoes de configuração (SET)
//
//----------------------------------------------------------------------------------

void brwMqtt::setMqttID (char* ID) {

    strncpy (mqttID,ID,sizeof(mqttID)); // iD da coneneão ( MAC da placa)

}




void brwMqtt::setMqttUsar (bool usar) {


	usarMQTT = usar;
}


void brwMqtt::setMqttHost (char* host) {

	strncpy (mqttHost,host,sizeof(mqttHost));
}


void brwMqtt::setMqttPort (int port) {

	mqttPort = port;
}


void brwMqtt::setMqttUser (char* user) {

	strncpy (mqttUser,user,sizeof(mqttUser));

}


void brwMqtt::setMqttPassWord (char* pwd) {

	strncpy (mqttPassWord,pwd,sizeof(mqttPassWord));

}

void brwMqtt::setTopicoPub (char* topico) {

	strncpy (topicoPub,topico,sizeof(topicoPub));
	

}


void brwMqtt::setTopicoSub (char* topico) {

	strncpy (topicoSub,topico,sizeof(topicoSub));
	

}

void brwMqtt::setTopicoConnect (char* topico) {

	strncpy (topicoConnect,topico,sizeof(topicoConnect));
	

}


void brwMqtt::setTopicoWill (char* topico) {

	strncpy (topicoWill,topico,sizeof(topicoWill));
	


}


void brwMqtt::setTopicoConfigPub (char* topico) {

	strncpy (topicoConfigPub,topico, sizeof (topicoConfigPub));
	


}


void brwMqtt::setTopicoConfigSub (char* topico) {

	strncpy (topicoConfigSub,topico,sizeof(topicoConfigSub));
	


}

//------------------------------------------------------------------------------------------
//
//------------------  Funcoes de configuração (GET
//
//------------------------------------------------------------------------------------------


char* brwMqtt::getMqttID () {

    return (mqttID); // iD da coneneão ( MAC da placa)
    
}


bool brwMqtt::getMqttUsar () {

	return (usarMQTT);

}


char* brwMqtt::getMqttHost () {

	return (mqttHost);
}


int brwMqtt::getMqttPort () {

	return (mqttPort);
}


char* brwMqtt::getMqttUser () {

	return (mqttUser);

}


char* brwMqtt::getMqttPassWord () {

	return (mqttPassWord);

}

char* brwMqtt::getTopicoPub () {

	return (topicoPub);

}


char* brwMqtt::getTopicoSub () {

	return (topicoSub);

}

char* brwMqtt::getTopicoConnect () {

	return (topicoConnect);

}


char* brwMqtt::getTopicoWill () {

	return (topicoWill);

}


char* brwMqtt::getTopicoConfigPub () {

	return (topicoConfigPub);

}


char* brwMqtt::getTopicoConfigSub () {

	return (topicoConfigSub);

}


//------------------------------------------------------------------------------------------
//
// ----------------- Set Valores default para o MQTT
//
//------------------------------------------------------------------------------------------

void brwMqtt::setMqttDefault () {

    Serial.println ("Recuperando valor default das variaveis"); 
	strncpy (mqttID, getMqttID (), sizeof (mqttID));
    usarMQTT = getMqttUsar ();
    strncpy (mqttHost, getMqttHost (), sizeof(mqttHost));
    mqttPort = getMqttPort ();
    strncpy (mqttUser, getMqttUser (), sizeof(mqttUser));
    strncpy (mqttPassWord, getMqttPassWord (), sizeof(mqttPassWord));

    strncpy (topicoPub, getTopicoPub (), sizeof(topicoPub));
    strncpy (topicoSub, getTopicoSub (), sizeof(topicoSub));
    strncpy (topicoConnect, getTopicoConnect (), sizeof(topicoConnect));
    strncpy (topicoWill, getTopicoWill (),sizeof(topicoWill));
    strncpy (topicoConfigPub, getTopicoConfigPub (), sizeof(topicoConfigPub));
    strncpy (topicoConfigSub, getTopicoConfigSub (), sizeof(topicoConfigSub)) ;

    setMqttConfig ();

}

//------------------------------------------------------------------------------------------
//
// ----------------- Task de Loop do MQTT
//
//------------------------------------------------------------------------------------------

void brwMqtt::MqttLoop () {

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


void brwMqtt::staticMqttLoop  (void *pvParameter) {

    brwMqtt* mq = reinterpret_cast<brwMqtt*>(pvParameter); //obtain the instance pointer
    mq->MqttLoop(); //dispatch to the member function, now that we have an instance pointer

}

//------------------------------------------------------------------------------------------
//
// ----------------- Task de Loop de tratar comandos
//
//------------------------------------------------------------------------------------------

void brwMqtt::trataComandoLoop () {
  //int i;
    while (true) {
      vTaskDelay(pdMS_TO_TICKS(100));
      if (strlen(pl) > 0) {
          if (strncpy (tp,getTopicoSub (),sizeof(tp))) { // recebeu  Dados
            trataComandoSub (pl);
          }
          if (strncpy(tp,getTopicoConfigSub (),sizeof(tp))) { // recebeu u configuração
            trataComandoConfigSub (pl);
          }  
        pl [0] = NULL;
        tp [0] = NULL;   
      vTaskDelay(pdMS_TO_TICKS(200));     
      }
      
    }
   
}

void brwMqtt::staticTrataComandoLoop  (void *pvParameter) {

    brwMqtt* mq = reinterpret_cast<brwMqtt*>(pvParameter); //obtain the instance pointer
    mq->trataComandoLoop(); //dispatch to the member function, now that we have an instance pointer

}


//*****************************************************************************************
//
//        TRATA DADOS RECEBIDO
//
//*****************************************************************************************

void brwMqtt::trataComandoSub (char* cmd) {
  char *rec ;
  char *mac;
  char *usr;
  char *cli;
  char *eqp;
  char *ord;
  int rle, i;
  long tmp;
  char str [30];
  
  
  //Serial.println (rec);
  rec = cmd;
  
  
  mac  = strtok(rec, ";"); // mac address da placa
  
  usr  = strtok(NULL, ";"); // id do usuario que executou a ção
  cli  = strtok(NULL, ";"); // cliente is 
  eqp  = strtok(NULL, ";"); // equiapamento ID
  ord  = strtok(NULL, ";"); // comamdo
  
  
  strncpy (str,getMqttID (), sizeof (str));
  
  
  if (strcmp (str,mac) != 0) {
    Serial.println ("Não Foi Pra Mim!!! - MAC: " + String(mac));
    return;
  }
 
 
  rle = atoi(ord);

  switch (rle) {
    case 1 :
        tmp = pHwApp -> getTempoRele (1) * 1000; 
        pHwApp -> actRele (rle,tmp);
        pHwApp -> setRele (2,HIGH);
        vTaskDelay(pdMS_TO_TICKS(tmp));
        sendTopicoPub (cmd);
        break;
    case 2 :   
        pHwApp -> setRele (2,LOW); 
        sendTopicoPub (cmd);
        break;
    case 3 :  
        //msgPub = cmd;
        pHwApp -> getTempoRele (rle) * 1000; 
        pHwApp -> actRele (rle,tmp);
        break;    
    case 4 : 
        tmp = pHwApp -> getTempoRele (rle) * 1000;

        if (pHwApp -> leEntrada (pEth -> pBrwHW -> getS01())) {
        	Serial.println ("Alarme Desativado");
        	strncat (cmd,"0",sizeof (cmd));
    		sendTopicoPub (cmd);
    		return;
      	} else
    	  	Serial.println ("Alarme Ativado");

    	if (pHwApp -> leEntrada (pEth -> pBrwHW -> getS02())) {  // Portão aberto  
		    Serial.println ("Portão 01 (S02): Aberto"); 
		    pHwApp -> actRele (4, tmp);
		    vTaskDelay(pdMS_TO_TICKS(tmp));
		    strncat (cmd,"1",sizeof (cmd));
		    sendTopicoPub (cmd);
		  } else 
		      Serial.println ("Portão 01 (S02): Fechado");

		if (pHwApp -> leEntrada (pEth -> pBrwHW -> getS03())) {  // Portão aberto  
		    Serial.println ("Portão 02 (S03): Aberto"); 
		    pHwApp -> actRele (5, tmp);
		    vTaskDelay(pdMS_TO_TICKS(tmp));
		    strncat (cmd,"2",sizeof (cmd));
		    sendTopicoPub (cmd);
		  } else 
		      Serial.println ("Portão 02 (S03): Fechado");

		if (pHwApp -> leEntrada (pEth -> pBrwHW -> getS04())) {  // Portão aberto  
		    Serial.println ("Portão 03 (S04): Aberto"); 
		    pHwApp -> actRele (6, tmp);
		    vTaskDelay(pdMS_TO_TICKS(tmp));
		    strncat (cmd,"2",sizeof (cmd));
		    sendTopicoPub (cmd);
		  } else 
		      Serial.println ("Portão 04 (S05): Fechado"); 

		if (pHwApp -> leEntrada (pEth -> pBrwHW -> getS05())) {  // Portão aberto  
		    Serial.println ("Portão 04 (S05): Aberto"); 
		    pHwApp -> actRele (7, tmp);
		    vTaskDelay(pdMS_TO_TICKS(tmp));
		    strncat (cmd,"2",sizeof (cmd));
		    sendTopicoPub (cmd);
		  } else 
		      Serial.println ("Portão 04 (S05): Fechado");                      
        
        return;     
  }
  

    
}

//----------------------------------------------------------------------------------------
//
//        TRATA CONFIGURAÇÃO RECEBIDO
//
//----------------------------------------------------------------------------------------

void brwMqtt::trataComandoConfigSub (char* cmd) {
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
  
  char str [30];
  // MAC;REDE;DHCP;IP;MASCARA;GATEWAY;DNS
  // MAC;MQTT;USAR;HOST;PORTA;USUARIO;SENHA
  // MAC;RELE;TEMPO RL01;TEMPO RL03
  
  //Serial.println (cmd);
  strncpy (rec,cmd,sizeof (rec));
  
  
  mac  = strtok(rec, ";");
  strncpy (str,getMqttID (), sizeof(str));
  
  
  if (strcmp (str,mac) != 0) {
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
      pEth -> setUsarIpFixo (true);
    else
      pEth -> setUsarIpFixo (false);
    pEth -> setIP (c02);
    pEth -> setMascara (c03);
    pEth -> setGateWay (c04);
    pEth -> setDNserver (c05);
  }

  if (strcmp("MQTT", cmdm) == 0) { // configurações de rede
    c01  = strtok(NULL, ";");
    c02  = strtok(NULL, ";");
    c03  = strtok(NULL, ";");
    c04  = strtok(NULL, ";");
    setMqttHost (c01);
    setMqttPort (atoi(c02));
    setMqttUser (c03);
    setMqttPassWord (c04);
    setMqttUsar (true);
    
    
    
  }

  if (strcmp("RELE", cmdm) == 0) { // configurações de rede
    c01  = strtok(NULL, ";");
    c02  = strtok(NULL, ";");
    pHwApp -> setTempoRele (1, atoi(c01));
    pHwApp -> setTempoRele (2, atoi(c01));
    pHwApp -> setTempoRele (3, atoi(c02));
    pHwApp -> setTempoRele (4, atoi(c01));
    
  }
   
  enviouCfgSub = false;
    
}




//------------------------------------------------------------------------------------------
//
// ----------------- Envia menssagem para topico principal
//
//------------------------------------------------------------------------------------------

void brwMqtt::sendTopicoPub (char* msg) {

     
     if (getMqttUsar () && mqttClient.connected()) {
       mqttClient.publish(getTopicoPub (), msg); 
       Serial.print ("Enviado para ");
       Serial.print (getTopicoPub ());
       Serial.print (" a msg: ");
       Serial.println (msg);
    } else
        Serial.println ("MQTT não Conectado (funcao: sendTopicoPub)");



}


//------------------------------------------------------------------------------------------
//
// ------    Le arquivo de cogiguracao do MQTT, se não existe cria com valores Default
//
//------------------------------------------------------------------------------------------

bool brwMqtt::getMqttConfig () {

  File wFile;
  char st [20];
  bool b;
  


  if(SPIFFS.exists("/mqtt.cfg")) { 

    wFile = SPIFFS.open("/mqtt.cfg", "r");
    if(!wFile){
        Serial.println ("Não foi possivel abrir o arquivo de configuração: mqtt.cfg");
        
        return (false);  
    }
    Serial.println ("Ler o arquivo de configuração: mqtt.cfg");
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    if (atoi (st) == 1)
      setMqttUsar (true);
    else
      setMqttUsar (false);  
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setMqttHost (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setMqttPort (atoi(st));
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setMqttUser (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setMqttPassWord (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 

    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setTopicoPub (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setTopicoSub (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setTopicoConnect (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setTopicoWill (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setTopicoConfigPub (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pEth -> pBrwHW -> spifss -> leLinha (& wFile, st);
    setTopicoConfigSub (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 


    wFile.close ();
    
    return (true);
  } else { // le default e cria arquivo
  	  Serial.println ("Criando arquivo default: mqtt.cfg");	
      setMqttDefault ();  
      
      return (true);    
      
    }


}




//------------------------------------------------------------------------------------------
//
// ------    Print arquivo de configuracao do Harware
//
//------------------------------------------------------------------------------------------

void brwMqtt::printMqttCfg () {

    Serial.print  ("Usar o MQTT: ");
    if (getMqttUsar ())
      Serial.println  ("TRUE - " + String (getMqttUsar ()));
    else
      Serial.println  ("FALSE - " + String (getMqttUsar ()));

  	Serial.println  ("ID.......: " + String (getMqttID ()));
    Serial.println  ("HOST.....: " + String (getMqttHost ()));

    Serial.println  ("PORTA....: " + String (getMqttPort ()));

    Serial.println  ("USUARIO..: " + String (getMqttUser ()));

    Serial.println  ("PASSWD...: " + String (getMqttPassWord ()));

    Serial.println  ("TOP. PUB : " + String (getTopicoPub ()));
    Serial.println  ("TOP. SUB : " + String (getTopicoSub ()));
    Serial.println  ("TOP. CON : " + String (getTopicoConnect ()));
    Serial.println  ("TOP. WIL : " + String (getTopicoWill ()));
    Serial.println  ("CFG. PUB : " + String (getTopicoConfigPub ()));
    Serial.println  ("CFG. SUB : " + String (getTopicoConfigSub ()));


}


//------------------------------------------------------------------------------------------
//
//  --------    Salva configurações de rede em rede.cfg
//
//------------------------------------------------------------------------------------------


bool brwMqtt::setMqttConfig () {

File wFile;
char st [50];
int p;
bool u;

    
    wFile = SPIFFS.open("/mqtt.cfg", "w");
    if(!wFile){
        Serial.println ("Não foi possivel salvar o arquivo de configuração");
        return (false);  
    }
    Serial.println ("Criando arquivo de configuração (SetMqttConfig)");
    u = getMqttUsar ();

	if (u)    
	  strncpy (st,"true", sizeof(st));
	else
	  strncpy (st,"false", sizeof(st));

	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));
	
	strncpy (st,getMqttHost (),sizeof(st));
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));
	
	p = getMqttPort ();
	itoa (p,st,10);
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));

	strncpy (st,getMqttUser (),sizeof(st));
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));

	strncpy (st,getMqttPassWord (),sizeof(st));
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));

	strncpy (st,getTopicoPub (),sizeof(st));
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));

	strncpy (st,getTopicoSub (),sizeof(st));
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));

	strncpy (st,getTopicoConnect (),sizeof(st));
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));

	strncpy (st,getTopicoWill (),sizeof(st));
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));

	strncpy (st,getTopicoConfigPub (),sizeof(st));
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));

	strncpy (st,getTopicoConfigSub (),sizeof(st));
	pEth -> pBrwHW -> spifss -> escreveLinha (& wFile, st);
	vTaskDelay(pdMS_TO_TICKS(100));

	wFile.close ();

    return (true);    

}

//------------------------------------------------------------------------------------------
//
// ------    Grava arquivo de cogiguracao do MQTT
//
//------------------------------------------------------------------------------------------

void sbrwMqtt::etMqttDefaut () {

  if (strcmp (hwApp,"netuno")) {
    strcpy (infoHw, "brWare One");
    strcpy (infoVersao, "1.0.1"); 
    strcpy (mqttID,pEth -> getMacAddress ()); 
    usarMQTT = true;
    strcpy (mqttHost,"brware.com.br");
    mqttPort = 1883;
    strcpy (mqttUser,"brware");
    strcpy (mqttPassWord,"SQRT(pi)!=314");

    
    
    


    char topicoPub [50];
    char topicoSub [50];
    char topicoConnect [50];
    char topicoWill [50];
    char topicoConfigPub [50];
    char topicoConfigSub [50];
  }


}