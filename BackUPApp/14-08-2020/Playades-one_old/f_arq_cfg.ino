/*------------------------------------------------------------------------------------------
//
//  --------    FUNCOES DESTE ARQUIVO
//
//------------------------------------------------------------------------------------------

  bool existeCfg ()
  bool setaDefault ()
  bool salvaCfg ()
  bool leCfg ()
  void printCfg ()

*/

#define ARQ_PYD "/playades-one.cfg"

//------------------------------------------------------------------------------------------
//
//  --------    Verififica se existe playades-one.cfg, se existe le, senão usa default
//
//------------------------------------------------------------------------------------------

bool existeCfg () {

File wFile;

  if(SPIFFS.exists(ARQ_PYD)) {
    //Serial.println ("Existe Arquivo: playades-one.cfg");
    if (!leCfg ()) {
        Serial.println ("Não leu: playades-one.cfg");
        efs.formatFS();
        ESP.restart ();
    }
    else{
        //Serial.println ("Leu: playades-one.cfg");
        return (true);   
    }
    
  } else { // não existe alarme.cfg -> garva valores default
    setaDefault ();
    salvaCfg ();
    leCfg ();
  }

}


//------------------------------------------------------------------------------------------
//
//  --------    Seta valores Default 
//
//------------------------------------------------------------------------------------------

bool setaDefault () {

  Serial.println ("Leu valores Default: playades-one.cfg");
  
  eth.setUsarIpFixo (IP_FIXO);
  eth.setIP (DEFAULT_IP);
  eth.setMascara (DEFAULT_MK);
  eth.setGateWay (DEFAULT_GW);
  eth.setDNserver (DEFAULT_DN);
/*
  mqtt.setMqttID (eth.getMacAddress ());
  mqtt.setMqttUsar (MQTT_USAR);
  mqtt.setMqttHost (MQTT_HOST);
  mqtt.setMqttPort (MQTT_PORT);
  mqtt.setMqttUser (MQTT_USER);
  mqtt.setMqttPassWord (MQTT_PWD);
*/  
  vAlm.minT = MIN_T; // temperatura minima
  vAlm.maxT = MAX_T; // Temperatura Maxima
  vAlm.minU = MIN_U; // Umidade Minima
  vAlm.maxU = MAX_U; //Umidade Maxima
  
  
}


//------------------------------------------------------------------------------------------
//
//  --------    Salva configurações do mqtt em mqtt.cfg
//
//------------------------------------------------------------------------------------------

bool salvaCfg () {

  File wFile;
  bool b_aux;
     
    //efs.deleteFile(ARQ_PYD);
    
    wFile = SPIFFS.open(ARQ_PYD, "w");
    if(!wFile){
        Serial.println ("Não foi possivel (abrir) salvar o arquivo de configuração");
        return (false);  
    }

    Serial.println ("Vai Salvar os dados......");
    if (eth.getUsarIpFixo ())
      wFile.println ("true");
    else
      wFile.println ("false");
    wFile.println (eth.getIP ());
    wFile.println (eth.getMascara ());
    wFile.println (eth.getGateWay ());
    wFile.println (eth.getDNserver ());  
      
    wFile.println (vAlm.minT);
    wFile.println (vAlm.maxT);
    wFile.println (vAlm.minU);
    wFile.println (vAlm.maxU);
/*
    if (mqtt.getMqttUsar ())
      wFile.println ("true");
    else
      wFile.println ("false");
    wFile.println (mqtt.getMqttHost ());
    wFile.println (mqtt.getMqttUser ());  
*/
    wFile.close ();
    
    return (true);

}






//------------------------------------------------------------------------------------------
//
//  --------    Leconfigurações do mqtt em mqtt.cfg
//
//------------------------------------------------------------------------------------------


bool leCfg () {
  
  File wFile;
  String str;
  char aux [20];
  
  wFile = SPIFFS.open(ARQ_PYD, "r");
  if(!wFile){
      Serial.println ("Não foi possivel abrir o arquivo de configuração");
      return (false);  
  }
  
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  eth.setUsarIpFixo ((str == "true" ? true : false));
  
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  eth.setIP (str);
  
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);;
  eth.setMascara (str);
  
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  eth.setGateWay (str);
  
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  eth.setDNserver (str);

  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  vAlm.minT = str;
  
  str = wFile.readStringUntil('\n');
  eth.removeRfinal (str);
  vAlm.maxT = str;
  
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  vAlm.minU = str;
  
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  vAlm.maxU = str;
/*
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  mqtt.setMqttUsar ((str == "true" ? true : false));
  
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  mqtt.setMqttHost (str);
  
  str = wFile.readStringUntil('\n');
  str = eth.removeRfinal (str);
  mqtt.getMqttUser ();
*/  
  wFile.close ();


  
  return (true);

}




void printCfg () {

  Serial.println ("MAC Address: " + eth.getMacAddress ());

  if (eth.getUsarIpFixo ())
    Serial.println ("Conexão : IP FIXO");
  else
    Serial.println ("Conexão : DHCP");  
  Serial.println ("IP : " + eth.getIP ());
  Serial.println ("Mascara : " + eth.getMascara ());
  Serial.println ("GateWay : " + eth.getGateWay ());
  Serial.println ("DNS : " + eth.getDNserver ());
  Serial.println ("Modelo Rede : " + eth.getModeloHardware ());
  Serial.println ("Status Link : " + eth.getStatusEthDescricao ());
  Serial.println ("");
/*  
  if (mqtt.getMqttUsar ())
    Serial.println ("MQTT habilitado");
  else
    Serial.println ("MQTT desabilitado");  
  Serial.println ("Host : " + mqtt.getMqttHost ());
  Serial.println ("User : " + mqtt.getMqttUser ());
  Serial.println ("");*/
  Serial.println ("Min Temperatura : " + vAlm.minT);
  Serial.println ("Max Temperatura : " + vAlm.maxT);
  Serial.println ("Min Umidade : " + vAlm.minU); 
  Serial.println ("Max Umidade : " + vAlm.maxU);
  
}
