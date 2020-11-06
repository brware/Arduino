

#include <brwCfgEth.h>




//------------------------------------------------------------------------------------------
//
//  Construtor da Classe de configuração de rede
//
//-------------------------------------------------------------------------------------------

brwCfgEth::brwCfgEth (){

  setMacAddress ();

  
  
}


//------------------------------------------------------------------------------------------
//
//  Inicia a rede Ethernet
//  - Se NAO existe o arquivo rede.cfg inicia como DHCP
//  - Se existe inicia conforme configurado no arquivo  
//
//-------------------------------------------------------------------------------------------


void brwCfgEth::beginEth (){

    

    Serial.println ("ETH DHCP begin.....");
    iniHardware (); // inicializa Pinos e Tasks de hardware
    // Use Ethernet.init(pin) to configure the CS pin.
    Ethernet.init(5);           // GPIO5 no ESP32.
    WizReset();
    

    setUsarIpFixo (false);

    Ethernet.begin(mac_int);
   
    setIP (Ethernet.localIP().toString());
    setMascara (Ethernet.subnetMask().toString());
    setGateWay (Ethernet.gatewayIP().toString());
    setDNserver (Ethernet.dnsServerIP().toString());
    
    

    stuEth = checaConexaotEth ();
    Serial.println ("IP.........: " + Ethernet.localIP().toString());
    Serial.println ("Conexão eth: " + String(stuEth));

    xTaskCreate(&brwCfgEth::staticPiscaLed, "PiscaLed", 1024, (void*) this, 5, NULL); 
    xTaskCreate(&brwCfgEth::staticEthLoop, "EthLoop", 4096, (void*) this, 4, NULL); 

    
}



void brwCfgEth::beginEth (String IP, String MK, String GW, String DNS){

    

    Serial.println ("ETH Ip Fixo begin.....");
    iniHardware (); // inicializa Pinos e Tasks de hardware
    // Use Ethernet.init(pin) to configure the CS pin.
    Ethernet.init(5);           // GPIO5 no ESP32.
    WizReset();
    

    setUsarIpFixo (true); 
    setIP (IP);
    setMascara (MK);
    setGateWay (GW);
    setDNserver (DNS);

    Ethernet.begin(mac_int, myIP, myMASK, myGW, myDNS);

      
    
    stuEth = checaConexaotEth ();

    xTaskCreate(&brwCfgEth::staticPiscaLed, "PiscaLed", 1024, (void*) this, 5, NULL); 
    xTaskCreate(&brwCfgEth::staticEthLoop, "EthLoop", 4096, (void*) this, 4, NULL); 

    
}



//------------------------------------------------------------------------------------------
//
// ------    pega MACADDREES do ESP
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setMacAddress () {
 //uint64_t chipid; 
 char mac01[5];
 char mac02[15];   
 char mca [50]; 
 //char aux [3];
 //int  i;
 //unsigned char* pt;

  chipid = ESP. getEfuseMac (); // O ID do chip é essencialmente seu endereço MAC (comprimento: 6 bytes).
  sprintf (mac01, "%04X" , ( uint16_t ) (chipid >> 32 )); // print High 2 bytes
  sprintf (mac02,"%08X" , ( uint32_t ) chipid); // imprime Low 4bytes.
  strcpy (mca, mac01);
  strcat (mca, mac02);
 

  mac_str = String(mca);

  atoh(mac_int, (char *)mac_str.c_str()); // mac para ETH
  
  int i;
  for (i = 0 ; i < 6; i++)
    Serial.println (mac_int [i]);
  
}

//------------------------------------------------------------------------------------------
//
// -----------   Retorna o ChipID do ESP
//
//------------------------------------------------------------------------------------------

String brwCfgEth::getChipID () {

	return (i64ToString (chipid));

}




//------------------------------------------------------------------------------------------
//
// -----------   Retorna o MAC Address do ESP
//
//------------------------------------------------------------------------------------------

String brwCfgEth::getMacAddress () {

  return (mac_str);

}


//------------------------------------------------------------------------------------------
//
// -----------   Seta IP ETH
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setIP (String ip) {
    String tmp;

    ip.replace (" ", ".");
    tmp = getIP();
    if (tmp.compareTo(ip) != 0)
        mudouCfgEth = true;

    

    strIP = ip;
    toFormatoIP (myIP, (char *)ip.c_str());


}


//------------------------------------------------------------------------------------------
//
// -----------   Seta Mascara de Rede ETH
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setMascara (String mk) {
    String tmp;

    mk.replace (" ", ".");
   /* tmp = getMasacara();
    if (tmp.compareTo(mk) != 0)
        mudouCfgEth = true;*/

	strMASK = mk;
	toFormatoIP (myMASK, (char *)mk.c_str());


}

//------------------------------------------------------------------------------------------
//
// -----------   Seta Gate Way ETH
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setGateWay (String gw) {
    String tmp;

    gw.replace (" ", ".");
    tmp = getIP();
    if (tmp.compareTo(gw) != 0)
        mudouCfgEth = true;

	strGW = gw;
	toFormatoIP (myGW, (char *)gw.c_str());


}


//------------------------------------------------------------------------------------------
//
// -----------   Seta DNS Server ETH
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setDNserver (String dns) {
    String tmp;

    dns.replace (" ", ".");
    tmp = getIP();
    if (tmp.compareTo(dns) != 0)
        mudouCfgEth = true;
	strDNS = dns;
	toFormatoIP (myDNS, (char *)dns.c_str());


}


//------------------------------------------------------------------------------------------
//
// -----------   Seta Modelo da placa de rede
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setModeloHardware () {

    
    modeloHardware = prt_hwval(Ethernet.hardwareStatus());


}



//------------------------------------------------------------------------------------------
//
// -----------   Link Status da placa de rede
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setStatusEthDescricao () {

    
    stuEthDescricao = prt_ethval(Ethernet.linkStatus());


}

//------------------------------------------------------------------------------------------
//
// -----------   Set se vai usar DHCP (true) ou IP Fixo (false)
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setUsarIpFixo (bool valor) {
    bool tmp;

  tmp = getUsarIpFixo ();
  if (tmp != valor)
    mudouCfgEth = true;

  usarIpFixo = valor;

}


//------------------------------------------------------------------------------------------
//
// -----------   Get  se vai usar DHCP (true) ou IP Fixo (false)
//
//------------------------------------------------------------------------------------------

bool brwCfgEth::getUsarIpFixo () {

  return (usarIpFixo);

}


//------------------------------------------------------------------------------------------
//
// -----------   Get IP ETH
//
//------------------------------------------------------------------------------------------

String brwCfgEth::getIP () {

    
	return (strIP);
	
}



//------------------------------------------------------------------------------------------
//
// -----------   Get Mascara de Rede ETH
//
//------------------------------------------------------------------------------------------

String brwCfgEth::getMascara () {

	return (strMASK);
	
}




//------------------------------------------------------------------------------------------
//
// -----------   Get Gate Way ETH
//
//------------------------------------------------------------------------------------------

String brwCfgEth::getGateWay () {

	return (strGW);

}



//------------------------------------------------------------------------------------------
//
// -----------   Get DNS Server ETH
//
//------------------------------------------------------------------------------------------

String brwCfgEth::getDNserver () {

	return (strDNS);

}



//------------------------------------------------------------------------------------------
//
// -----------   Get Modelo de Harware detectado
//
//------------------------------------------------------------------------------------------

String brwCfgEth::getModeloHardware () {

	return (modeloHardware);

}

//------------------------------------------------------------------------------------------
//
// -----------   Get Descrição do Status da placa de REde
//
//------------------------------------------------------------------------------------------

String brwCfgEth::getStatusEthDescricao () {

	return (stuEthDescricao);

}


//------------------------------------------------------------------------------------------
//
// -----------   Get  Status da placa de REde
//
//------------------------------------------------------------------------------------------

bool brwCfgEth::getStatusEth () {

    return (stuEth);

}



//------------------------------------------------------------------------------------------
//
//    Wiz W5500 Função de reset do modulo  
// 
//------------------------------------------------------------------------------------------

void brwCfgEth::WizReset() {

    
    digitalWrite(SPI_RST , HIGH);
    //vTaskDelay(pdMS_TO_TICKS(250));
    delay (250);
    digitalWrite(SPI_RST , LOW);
    //vTaskDelay(pdMS_TO_TICKS(50));
    delay (50);
    digitalWrite(SPI_RST , HIGH);
    //vTaskDelay(pdMS_TO_TICKS(350));
    delay (350);

}


//------------------------------------------------------------------------------------------
//
//   para reconhecer o modelo harware WZ 5XXX
// 
//------------------------------------------------------------------------------------------

String brwCfgEth::prt_hwval(uint8_t refval) {
    //Serial.print ("Modelo Hardware : ");
    //Serial.println (refval);
    switch (refval) {
    case 0:
        return ("Hardware não foi detectado.");
        //stuEth = false;
        break;
    case 1:
        return ("WizNet W5100 foi detectado.");
        //stuEth = true;
        break;
    case 2:
        return (modeloHardware = "WizNet W5200 foi detectado.");
        //stuEth = true;
        break;
    case 3:
        return ("WizNet W5500 foi detectado.");
        //stuEth = true;
        break;
    default:
        return ("ERRO desconhecido - Atualiza espnow_gw.ino para conhecidir Ethernet.h");
        //stuEth = false;    
    }
}


//------------------------------------------------------------------------------------------
//
//     para checar Link
// 
//------------------------------------------------------------------------------------------

String brwCfgEth::prt_ethval(uint8_t refval) {

    //Serial.print ("Status Link: ");
    //Serial.println (refval);
    switch (refval) {
    case 0:
        return ("Status deconhecido.");
        //stuEth = false;
        break;
    case 1:
        return ("Link esta OK.");
        //stuEth = true;
        break;
    case 2:
        return ("Link esta NOK. cheque os cabos de conexão.");
        //stuEth = false;
        break;
    default:
        return ("ERRO desconhecido - Atualiza espnow_gw.ino para conhecidir Ethernet.h");
        //stuEth = false;    
    }
}

//------------------------------------------------------------------------------------------
//
//     Faz a chexagem da conexao
// 
//------------------------------------------------------------------------------------------

 bool brwCfgEth::checaConexaotEth () {
  
  bool rdy_flag = false;
  int i;
  
  

  //setModeloHardware ();
  //setStatusEthDescricao ();

    for (i = 0; i <= 20; i++) {
        if ((Ethernet.hardwareStatus() == EthernetNoHardware)
            || (Ethernet.linkStatus() == LinkOFF)) {
            
            rdy_flag = false;
            stuEth = false;
            vTaskDelay(pdMS_TO_TICKS(80));
        } else {
            rdy_flag = true;
            stuEth = true;
            break;
        }
    }
    if (rdy_flag == false) {
        /*Serial.println
            ("\n\r\tFalha no Harware, ou no cabo... Não é possivel continuar.");
        Serial.print("Status do Harware: ");
        prt_hwval(Ethernet.hardwareStatus());
        Serial.print("   Status do cabo: ");
        prt_ethval(Ethernet.linkStatus());*/
        stuEth = false;
        return (false);
        
    } else {
        stuEth = true;
        return (true);
    }

    
 }


//------------------------------------------------------------------------------------------
//
//  --------    Monitora estado da rede Ethernet
//
//------------------------------------------------------------------------------------------


void brwCfgEth::EthLoop () {

  
  while (true) {
    
    
    stuOldEth = stuEth;
    stuEth = checaConexaotEth ();
     
   
    if (stuEth  && !stuOldEth) { // se estava desconectado e agora esta conectado
      Serial.println("- REBOOT: Task Checa Conexão ");
      Serial.println(stuEth); 
      Serial.println(stuOldEth);  
      ESP.restart ();   
    }
    
    
    vTaskDelay(pdMS_TO_TICKS(1000)); // temp que verifica rede
    
    
  }
  
}

void brwCfgEth::staticEthLoop (void *pvParameter) {

    brwCfgEth* ethl = reinterpret_cast<brwCfgEth*>(pvParameter); //obtain the instance pointer
    ethl->EthLoop(); //dispatch to the member function, now that we have an instance pointer

}




//------------------------------------------------------------------------------------------
//
// -----   Pisca LED 
//
//------------------------------------------------------------------------------------------

void brwCfgEth::PiscaLed ()
{
   
Serial.println ("- Pisca LED");   
while (true)
  {  
    
  if (getStatusEth()) // se esta conectado na rede
   {
   digitalWrite(LED_IN, !digitalRead(LED_IN));
   vTaskDelay(pdMS_TO_TICKS(200));
   } 
  else  
   {
   digitalWrite(LED_IN, !digitalRead(LED_IN));
   vTaskDelay(pdMS_TO_TICKS(2000));
   }  
  }
 
}


void brwCfgEth::staticPiscaLed (void *pvParameter) {

    brwCfgEth* led = reinterpret_cast<brwCfgEth*>(pvParameter); //obtain the instance pointer
    led->PiscaLed(); //dispatch to the member function, now that we have an instance pointer

}



/*


//------------------------------------------------------------------------------------------
//
//  --------    Verififica se existe rede.cfg, se existe le, senão usa DHCP
//
//------------------------------------------------------------------------------------------

bool brwCfgEth::existeCgfRede () {

File wFile;
  //String str;
  //int i;
  
  openFS();
  if(SPIFFS.exists(ARQ_REDE)) {
    closeFS();
    //Serial.println ("Existe rede.cfg");
    if (!leCfgRede ()) {
        Serial.println ("Não leu rede.cfg");
        openFS();
        formatFS();
        closeFS();
        ESP.restart ();
    }
    else{
        Serial.println ("Leu rede.cfg");
        return (true);   
    }
    
  } else {
    closeFS();
    Serial.println ("Não existe rede.cfg");
    setUsarIpFixo (false);
    setIP (Ethernet.localIP().toString());
    setMascara (Ethernet.subnetMask().toString());
    setGateWay (Ethernet.gatewayIP().toString());
    setDNserver (Ethernet.dnsServerIP().toString());
    salvaCfgRede ();
    
  }

}


//------------------------------------------------------------------------------------------
//
//  --------    Salva configurações de rede em rede.cfg
//
//------------------------------------------------------------------------------------------


bool brwCfgEth::salvaCfgRede () {

File wFile;
String str;

    openFS ();
    wFile = SPIFFS.open(ARQ_REDE, "w");
    if(!wFile){
        Serial.println ("Não foi possivel salvar o arquivo de configuração");
        closeFS ();
        return (false);  
    }

    str = (usarIpFixo ? "true" : "false");
    wFile.println (str);      
    Serial.print ("*** On Save: usarIpFixo: ");
    Serial.println (str);      
    wFile.println (strIP);
    wFile.println (strMASK);
    wFile.println (strGW);
    wFile.println (strDNS);

    wFile.close ();
    Serial.println ("*** Escreveu arquivo REDE com sucesso");
    closeFS ();
    return (true);

}

bool brwCfgEth::salvaCfg () {

    salvaCfgRede ();

}



//------------------------------------------------------------------------------------------
//
//  --------    Leconfigurações de rede em rede.cfg
//
//------------------------------------------------------------------------------------------


bool brwCfgEth::leCfgRede () {

File wFile;
String str;
bool del_var;
    openFS ();
    wFile = SPIFFS.open(ARQ_REDE, "r");
    if(!wFile){
        Serial.println ("Não foi possivel abrir o arquivo de configuração");
        closeFS ();
        return (false);  
    }

    str = wFile.readStringUntil('\n');

    if (str == "true") {
        Serial.println ("----- On Load: usarIpFixo: " + str );
        setUsarIpFixo (true);
        del_var = getUsarIpFixo ();
        Serial.println ("----- On Load: usarIpFixo: SETADO :" + String (del_var));
         
    }
    else
        setUsarIpFixo (false);   
         
    setIP (wFile.readStringUntil('\n'));
    setMascara (wFile.readStringUntil('\n'));
    setGateWay (wFile.readStringUntil('\n'));
    setDNserver(wFile.readStringUntil('\n'));

    wFile.close ();
    openFS ();
    return (true);

}

bool brwCfgEth::leCfg () {

  leCfgRede ();

}
*/
