




//------------------------------------------------------------------------------------------
//
// ------------------  INICIA DHT 22
//
//------------------------------------------------------------------------------------------

void iniDhtAlrm () {

  
  dht.begin(); //setup DHT sensor
  Serial.println("DHT - Iniciado");
  
}



//------------------------------------------------------------------------------------------
//
// ------------------  TESTE SE TEM ALARME
//
//------------------------------------------------------------------------------------------

int testeAlarmeTemperatura (float temp, float umid) {

  if (temp != 0.00)
    if ((temp < vAlm.minT.toFloat()) || (temp > vAlm.maxT.toFloat())) 
        stuAlarmeTemperatura = true;
    else   
        stuAlarmeTemperatura = false;
     
  
  if (umid != 0.00)
    if ((umid < vAlm.minU.toFloat()) || (umid > vAlm.maxU.toFloat()))
        stuAlarmeUmidade = true;
    else 
        stuAlarmeUmidade = false;   

  
 }

//------------------------------------------------------------------------------------------
//
// -------------- TRATA COMANDO RECEBIDO
//
//------------------------------------------------------------------------------------------
/*
bool trataComando (String str) {
  char rec [100];
  char *mac;
  char *cmd;
  char *qtd;
  char dado [10][10];
  int i, iqtd, j;
  
  
  
  
  str.toCharArray (rec, (str.length()+1));
  
  
  mac = strtok(rec, ";");
  Serial.println (mac);
  cmd  = strtok(NULL, ";");
  Serial.println (cmd);
  qtd  = strtok(NULL, ";");
  Serial.println (qtd); 
  Serial.println ("-----");
  
  
  if (strcmp (mac,(char *)fc.MAC_str) != 0 ){
    Serial.println ("Não Foi para mim"); 
    return (false); 
  }

    iqtd = atoi(qtd); 
    for (i = 0; i < iqtd;i++){
        strcpy (dado[i],strtok(NULL, ";"));  
        Serial.println (dado[i]); 
    }
  

   if (strcmp (cmd,(char *)"SV") == 0 ){
    salvaCfgPlaiade ();
    vTaskDelay(pdMS_TO_TICKS(500));
    printCfgPlaiade ();
    vTaskDelay(pdMS_TO_TICKS(1000));
    ESP.restart (); 
    
   } 

  // atualização de temperatura
  if (strcmp (cmd,(char *)"TMP") == 0 ){
    e.minTemperatura = atoi(dado[0]);
    e.maxTemperatura = atoi(dado[1]);
    return (true);
  }

  // atualização de umidade
  if (strcmp (cmd,(char *)"UMI") == 0 ){
    e.minUmidade = atoi(dado[0]);
    e.maxUmidade = atoi(dado[1]);
    return (true);
  }

  // atualização do  IP
  if (strcmp (cmd,(char *)"IP") == 0 ){
    for (i = 0; i < 4; i++){
      iqtd = atoi (dado [i]);
      e.myIP [i] = (uint8_t) iqtd;
    }
    IpToStr (rec, e.myIP);  
    //Serial.println (rec); 
    salvaCfgPlaiade ();
    return (true);
  }

  if (strcmp (cmd,(char *)"MK") == 0 ){
    for (i = 0; i < 4; i++){
      iqtd = atoi (dado [i]);
      e.myMASK [i] = (uint8_t) iqtd;
    }
    IpToStr (rec, e.myMASK);  
    //Serial.println (rec); 
    salvaCfgPlaiade ();
    return (true);
  }
    
  if (strcmp (cmd,(char *)"GW") == 0 ){
    for (i = 0; i < 4; i++){
      iqtd = atoi (dado [i]);
      e.myGW [i] = (uint8_t) iqtd;
    }
    IpToStr (rec, e.myGW);  
    //Serial.println (rec); 
    salvaCfgPlaiade ();
    return (true);
  }

  if (strcmp (cmd,(char *)"DS") == 0 ){
    for (i = 0; i < 4; i++){
      iqtd = atoi (dado [i]);
      e.myDNS [i] = (uint8_t) iqtd;
    }
    IpToStr (rec, e.myDNS);  
    //Serial.println (rec); 
    salvaCfgPlaiade ();
    return (true);
  }

 
  
}
 */
