/*------------------------------------------------------------------------------------------
//
//  --------    FUNCOES DESTE ARQUIVO
//
//------------------------------------------------------------------------------------------

  bool existeCfg ()
  bool setaDefault ()
  bool salvaCfg ()
  bool leCfg ()
  void setVariaveis()
  void getVariaveis()
  void printCfg ()

*/

#define ARQ_PYD "/sucres.cfg"

//------------------------------------------------------------------------------------------
//
//  --------    Verififica se existe sucres.cfg, se existe le, senão usa default
//
//------------------------------------------------------------------------------------------

bool existeCfg () {

File wFile;

  if(SPIFFS.exists(ARQ_PYD)) {
    //Serial.println ("Existe Arquivo: playades-one.cfg");
    if (!leCfg ()) {
        Serial.println ("Não leu: sucres.cfg");
        efs.formatFS();
        ESP.restart ();
    }
    else{
        Serial.println ("Leu: sucres.cfg");
        return (true);   
    }
    
  } else { // não existe sucres.cfg -> garva valores default
    Serial.println ("Não existe sucres.cfg -> garva valores default");
    lerCfgBD = false;
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

  Serial.println ("Leu valores Default: sucres.cfg");

  sc.sendTopicoConfigPub ("GET");
  
  sc.setUsarIpFixo (IP_FIXO);
  sc.setIP (DEFAULT_IP);
  sc.setMascara (DEFAULT_MK);
  sc.setGateWay (DEFAULT_GW);
  sc.setDNserver (DEFAULT_DN);

  sc.setMqttID (sc.getMacAddress ());
  sc.setMqttUsar (MQTT_USAR);
  sc.setMqttHost (MQTT_HOST);
  sc.setMqttPort (MQTT_PORT);
  sc.setMqttUser (MQTT_USER);
  sc.setMqttPassWord (MQTT_PWD);

  sc.setTempoRele (1, 3);
  sc.setTempoRele (2, 3);
  sc.setTempoRele (3, 180);
  sc.setTempoRele (4, 3);
  
  
  
}


//------------------------------------------------------------------------------------------
//
//  --------    Salva configurações do mqtt em mqtt.cfg
//
//------------------------------------------------------------------------------------------

bool salvaCfg () {

  File wFile;
  bool b_aux;
     

    
    wFile = SPIFFS.open(ARQ_PYD, "w");
    if(!wFile){
        Serial.println ("Não foi possivel (abrir) salvar o arquivo de configuração");
        return (false);  
    }

    
    if (sc.getUsarIpFixo ())
      wFile.println ("true");
    else
      wFile.println ("false");
    wFile.println (sc.getIP ());
    wFile.println (sc.getMascara ());
    wFile.println (sc.getGateWay ());
    wFile.println (sc.getDNserver ());  
      


    if (sc.getMqttUsar ())
      wFile.println ("true");
    else
      wFile.println ("false");
    wFile.println (sc.getMqttHost ());
    wFile.println (sc.getMqttPort ());
    wFile.println (sc.getMqttUser ()); 
    wFile.println (sc.getMqttPassWord ());
    wFile.println (sc.getTempoRele (1));
    wFile.println (sc.getTempoRele (2));
    wFile.println (sc.getTempoRele (3));
    wFile.println (sc.getTempoRele (4));
    
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
  
  // REDE
  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setUsarIpFixo ((str == "true" ? true : false));
  
  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  if (sc.getUsarIpFixo () == true)
    sc.setIP (str);
  
  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  if (sc.getUsarIpFixo () == true)
    sc.setMascara (str);
  
  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  if (sc.getUsarIpFixo () == true)
    sc.setGateWay (str);
  
  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  if (sc.getUsarIpFixo () == true)
    sc.setDNserver (str);

  
  // MQTT
  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setMqttUsar ((str == "true" ? true : false));
  
  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setMqttHost (str);

  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setMqttPort (str.toInt());
  
  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setMqttUser (str);

  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setMqttPassWord (str);

  // Tepos dos reles
  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setTempoRele (1, str.toInt());

  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setTempoRele (2, str.toInt());

  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setTempoRele (3, str.toInt());

  str = wFile.readStringUntil('\n');
  str.remove (str.length()-1,1);
  sc.setTempoRele (4, str.toInt());
  
  wFile.close ();
  
  if (sc.getUsarIpFixo () == true)
    printCfg ();
  
  return (true);

}


//------------------------------------------------------------------------------------------
//
//  --------    Leconfigurações do mqtt em mqtt.cfg
//
//------------------------------------------------------------------------------------------


void printCfg () {
  int i;

  Serial.println ("********************************");
  Serial.println ( "    Configurações do arquivo" );
  Serial.println ("********************************");
  Serial.println ("");

  Serial.println ("MAC Address: " + sc.getMacAddress ());

  if (sc.getUsarIpFixo ())
  {
    Serial.println ("Conexão : IP FIXO");
    Serial.println ("IP : " + sc.getIP ());
    Serial.println ("Mascara : " + sc.getMascara ());
    Serial.println ("GateWay : " + sc.getGateWay ());
    Serial.println ("DNS : " + sc.getDNserver ());
    Serial.println ("Modelo Rede : " + sc.getModeloHardware ());
    Serial.println ("Status Link : " + sc.getStatusEthDescricao ());
    Serial.println ("");
  }
  else {
    Serial.println ("Conexão : DHCP");  
    Serial.println ("IP : " + sc.getIP ());
    Serial.println ("Mascara : " + sc.getMascara ());
    Serial.println ("GateWay : " + sc.getGateWay ());
    Serial.println ("DNS : " + sc.getDNserver ());
    Serial.println ("Modelo Rede : " + sc.getModeloHardware ());
    Serial.println ("Status Link : " + sc.getStatusEthDescricao ());
    Serial.println ("");

  }

  Serial.println ("IP : " + sc.getIP ());
  Serial.println ("Mascara : " + sc.getMascara ());
  Serial.println ("GateWay : " + sc.getGateWay ());
  Serial.println ("DNS : " + sc.getDNserver ());
  Serial.println ("Modelo Rede : " + sc.getModeloHardware ());
  Serial.println ("Status Link : " + sc.getStatusEthDescricao ());
  Serial.println ("");

  Serial.println ("-------------------------");
  if (sc.getMqttUsar ())
    Serial.println ("MQTT habilitado");
  else
    Serial.println ("MQTT desabilitado");  
  Serial.println ("Host : " + sc.getMqttHost ());
  Serial.println ("User : " + sc.getMqttUser ());
  Serial.println ("Porta : " + String (sc.getMqttPort ()));
  Serial.println ("Senha : " + sc.getMqttPassWord ());

  Serial.println ("");
  Serial.println ("-------------------------");
  for (i = 1; i < 5; i++) {
    Serial.println ("Tempo rele " + String(i) + " : " + sc.getTempoRele (i));
  }
  
  
  
}


//------------------------------------------------------------------------------------------
//
//  --------    Verifica se dados do banco esta igual do arquivo
//
//------------------------------------------------------------------------------------------


void verificaConfBD () { 

  if ((sc.mudouCfgEth) || (sc.mudouCfgHardware) || (sc.mudouCfgMqtt)) {
    salvaCfg (); // para testes
    leCfg ();
    boot ();

  }

}