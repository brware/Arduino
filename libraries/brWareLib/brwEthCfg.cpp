

#include "brwEthCfg.h"




//------------------------------------------------------------------------------------------
//
//  Construtor da Classe de configuração de rede
//
//-------------------------------------------------------------------------------------------

brwEthCfg::brwEthCfg (){


  
}

void brwEthCfg::beginEth (brwHardwareCfg * brwHW){
    
    Serial.println ("Inicilalizando Classe Eth");

    pBrwHW = brwHW;

    setMacAddress ();

     if (pBrwHW -> spifss -> isOpen == false)
      pBrwHW -> spifss -> beginFS();



   

    getEthConfig ();
    Serial.println ("--- Listando: getEthConfig ()");
    printEthCfg ();

    if (getUsarIpFixo ()) {

        initEthFixo ();

    } else {

        initEth ();
    }

    pBrwHW -> getHwConfig ();
    //Serial.println ("--- Listando: getHwConfig ()");
    //pBrwHW -> printHwCfg ();

   


    


}



//------------------------------------------------------------------------------------------
//
//  Inicia a rede Ethernet
//  - Se NAO existe o arquivo rede.cfg inicia como DHCP
//  - Se existe inicia conforme configurado no arquivo  
//
//-------------------------------------------------------------------------------------------


void brwEthCfg::initEth (){
    String str;
    char *p;
    
    

    Serial.println ("ETH DHCP begin.....");
    
   
    Ethernet.init(5);           // GPIO5 no ESP32.
    WizReset();
    

    setUsarIpFixo (false);

    Ethernet.begin(mac_int);

    Serial.println ("ETH  begin.....");
    
    

    str = Ethernet.localIP().toString();
    setIP ((char*)str.c_str());
    Serial.println ("IP.........: " + Ethernet.localIP().toString());
    str = Ethernet.subnetMask().toString();
    setMascara ((char*)str.c_str());
    Serial.println ("MASCARA....: " + String(getMascara ()));
    str = Ethernet.gatewayIP().toString();
    setGateWay ((char*)str.c_str());
    Serial.println ("GATEWAY....: " + String (getGateWay()));
    str = Ethernet.dnsServerIP().toString();
    setDNserver ((char*)str.c_str());
    Serial.println ("DNS........: " + String(getDNserver()));
    
    setEthConfig ();

    stuEth = checaConexaotEth ();
    
    Serial.println ("Conexão eth: " + String(stuEth));

 
    xTaskCreate(&brwEthCfg::staticPiscaLed, "PiscaLed", 1024, (void*) this, 5, NULL); 
    xTaskCreate(&brwEthCfg::staticEthLoop, "EthLoop", 4096, (void*) this, 4, NULL); 


    getEthConfig ();

    
}



void brwEthCfg::initEthFixo (){
    String str;
  
    Serial.println ("ETH Ip Fixo begin.....");

    Ethernet.init(5);           // GPIO5 no ESP32.
    WizReset();
  /*  
    Serial.println ("**** ETH Ip Fixo da função *****");
        Serial.print ("IP: ");
        Serial.println (strIP);
        Serial.print ("MASCARA: ");
        Serial.println (strMASK);
        Serial.print ("GateWAY: ");
        Serial.println (strGW);
        Serial.print ("DNS: ");
        Serial.println (strDNS);
    Serial.println ("**** FIM ETH Ip Fixo da função *****");
   */ 

    //setDNserver ("8.8.8.8");
    Ethernet.begin(mac_int, myIP, myMASK, myGW, myDNS);
 /*   
    // sei -la porque tem que fazer isto!!!!!!
    Serial.println ("**** ETH Ip Fixo Printa da Gambiarra 01 *****");
    str = Ethernet.localIP().toString();
    Serial.print ("IP: ");
    Serial.println (str);
    //setIP ((char*)str.c_str());
    str = Ethernet.subnetMask().toString();
    Serial.print ("Mascara: ");
    Serial.println (str);
    //setMascara ((char*)str.c_str());
    str = Ethernet.gatewayIP().toString();
    Serial.print ("GateWay: ");
    Serial.println (str);
    //setGateWay ((char*)str.c_str());
    str = Ethernet.dnsServerIP().toString();
    Serial.print ("DNS: ");
    Serial.println (str);
    setDNserver ((char*)str.c_str());

    Serial.println ("**** FIM ... ETH Ip Fixo Print da Gambiarra *****");
*/
    //Ethernet.begin(mac_int, myIP, myMASK, myGW, myDNS);

    //setEthConfig ();

    stuEth = checaConexaotEth ();
    
    Serial.println (" -->> Cfg da Placa");
    Serial.println ("STATUS.....: " + String(stuEth));
    Serial.println ("IP.........: " + Ethernet.localIP().toString());
    Serial.println ("MASCARA....: " + Ethernet.subnetMask().toString());
    Serial.println ("GATEWAY....: " + Ethernet.gatewayIP().toString());
    Serial.println ("DNS........: " + Ethernet.dnsServerIP().toString());
    
    xTaskCreate(&brwEthCfg::staticPiscaLed, "PiscaLed", 1024, (void*) this, 5, NULL); 
    xTaskCreate(&brwEthCfg::staticEthLoop, "EthLoop", 4096, (void*) this, 4, NULL); 
     
    //Serial.println ("Cfg do Arquivo");  
    //getEthConfig ();

}



//------------------------------------------------------------------------------------------
//
// ------    pega MACADDREES do ESP
//
//------------------------------------------------------------------------------------------

void brwEthCfg::setMacAddress () {
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
  strncpy (mca, mac01,sizeof(mca));
  strcat (mca, mac02);
 

  strncpy (mac_str,mca,sizeof(mac_str));

  atoh(mac_int, (char *)mac_str); // mac para ETH
  
  
  //int i;
  //for (i = 0 ; i < 6; i++)
  //  Serial.println (mac_int [i]);
  
}

//------------------------------------------------------------------------------------------
//
// -----------   Retorna o ChipID do ESP
//
//------------------------------------------------------------------------------------------

char * brwEthCfg::getChipID () {
    char str [50];

    itoa (chipid, str,10);
	return (str);

}




//------------------------------------------------------------------------------------------
//
// -----------   Retorna o MAC Address do ESP
//
//------------------------------------------------------------------------------------------

char * brwEthCfg::getMacAddress () {

  return (mac_str);

}


//------------------------------------------------------------------------------------------
//
// -----------   Seta IP ETH
//
//------------------------------------------------------------------------------------------

void brwEthCfg::setIP (char* ip) {
    String tmp;
    char s[20];


    
    //tmp = ip; 
    //tmp.replace (" ", ".");
    //strcpy (ip, tmp.c_str());
    
    /*
    strcpy (ip, getIP());
    if (strcmp (ip,tmp.c_str()) != 0)
        mudouCfgEth = true;
    */
    

    strncpy (strIP, ip,sizeof(strIP));
    strncpy (s, ip,sizeof(s));
    
    toFormatoIP (myIP, s);

}


//------------------------------------------------------------------------------------------
//
// -----------   Seta Mascara de Rede ETH
//
//------------------------------------------------------------------------------------------

void brwEthCfg::setMascara (char* mk) {
    char s[20];
    String tmp;

    //tmp = mk;
    //tmp.replace (" ", ".");
   /* tmp = getMasacara();
    if (tmp.compareTo(mk) != 0)
        mudouCfgEth = true;*/

	strncpy (strMASK, mk,sizeof(strMASK));
    strncpy (s, mk,sizeof(s));
	toFormatoIP (myMASK, s);
    

}

//------------------------------------------------------------------------------------------
//
// -----------   Seta Gate Way ETH
//
//------------------------------------------------------------------------------------------

void brwEthCfg::setGateWay (char* gw) {
    char s[20];
    String tmp;
/*
    tmp = gw;
    tmp.replace (" ", ".");
    gw = getIP();
    if (strcmp (gw, tmp.c_str()) != 0)
        mudouCfgEth = true;
*/
	strncpy (strGW, gw,sizeof(strGW));
    strncpy (s, gw,sizeof(s));
	toFormatoIP (myGW, s);
    

}


//------------------------------------------------------------------------------------------
//
// -----------   Seta DNS Server ETH
//
//------------------------------------------------------------------------------------------

void brwEthCfg::setDNserver (char* dns) {
    char s[20];
    String tmp;
/*
    tmp = dns;
    tmp.replace (" ", ".");
    dns = getIP();
    if (strcmp(dns, tmp.c_str()) != 0)
        mudouCfgEth = true;*/
	strncpy (strDNS,dns,sizeof(strDNS));
    strncpy (s,dns,sizeof(s));
	toFormatoIP (myDNS, s);

}


//------------------------------------------------------------------------------------------
//
// -----------   Seta Modelo da placa de rede
//
//------------------------------------------------------------------------------------------

void brwEthCfg::setModeloHardware () {
    String tmp;
    
    tmp = prt_hwval(Ethernet.hardwareStatus());
    strncpy (modeloHardware, tmp.c_str(),sizeof(modeloHardware));


}



//------------------------------------------------------------------------------------------
//
// -----------   Link Status da placa de rede
//
//------------------------------------------------------------------------------------------

void brwEthCfg::setStatusEthDescricao () {
    String tmp;
    
    tmp = prt_ethval(Ethernet.linkStatus());
    strncpy (stuEthDescricao, tmp.c_str(),sizeof(stuEthDescricao));


}

//------------------------------------------------------------------------------------------
//
// -----------   Set se vai usar DHCP (true) ou IP Fixo (false)
//
//------------------------------------------------------------------------------------------

void brwEthCfg::setUsarIpFixo (bool valor) {
    bool tmp;
/*
  tmp = getUsarIpFixo ();
  if (tmp != valor)
    mudouCfgEth = true;
*/
  usarIpFixo = valor;

}


//------------------------------------------------------------------------------------------
//
// -----------   Get  se vai usar DHCP (true) ou IP Fixo (false)
//
//------------------------------------------------------------------------------------------

bool brwEthCfg::getUsarIpFixo () {

  return (usarIpFixo);

}


//------------------------------------------------------------------------------------------
//
// -----------   Get IP ETH
//
//------------------------------------------------------------------------------------------

char* brwEthCfg::getIP () {


	return (strIP);
	
}



//------------------------------------------------------------------------------------------
//
// -----------   Get Mascara de Rede ETH
//
//------------------------------------------------------------------------------------------

char* brwEthCfg::getMascara () {

	return (strMASK);
	
}




//------------------------------------------------------------------------------------------
//
// -----------   Get Gate Way ETH
//
//------------------------------------------------------------------------------------------

char* brwEthCfg::getGateWay () {

	return (strGW);

}



//------------------------------------------------------------------------------------------
//
// -----------   Get DNS Server ETH
//
//------------------------------------------------------------------------------------------

char* brwEthCfg::getDNserver () {

	return (strDNS);

}



//------------------------------------------------------------------------------------------
//
// -----------   Get Modelo de Harware detectado
//
//------------------------------------------------------------------------------------------

char* brwEthCfg::getModeloHardware () {

	return (modeloHardware);

}

//------------------------------------------------------------------------------------------
//
// -----------   Get Descrição do Status da placa de REde
//
//------------------------------------------------------------------------------------------

char* brwEthCfg::getStatusEthDescricao () {

	return (stuEthDescricao);

}


//------------------------------------------------------------------------------------------
//
// -----------   Get  Status da placa de REde
//
//------------------------------------------------------------------------------------------

bool brwEthCfg::getStatusEth () {

    return (stuEth);

}



//------------------------------------------------------------------------------------------
//
//    Wiz W5500 Função de reset do modulo  
// 
//------------------------------------------------------------------------------------------

void brwEthCfg::WizReset() {

    
    digitalWrite(pBrwHW -> getSPI_RST() , HIGH);
    //vTaskDelay(pdMS_TO_TICKS(250));
    delay (250);
    digitalWrite(pBrwHW -> getSPI_RST() , LOW);
    //vTaskDelay(pdMS_TO_TICKS(50));
    delay (50);
    digitalWrite(pBrwHW -> getSPI_RST() , HIGH);
    //vTaskDelay(pdMS_TO_TICKS(350));
    delay (350);

}


//------------------------------------------------------------------------------------------
//
//   para reconhecer o modelo harware WZ 5XXX
// 
//------------------------------------------------------------------------------------------

String brwEthCfg::prt_hwval(uint8_t refval) {
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
        return ("WizNet W5200 foi detectado.");
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

String brwEthCfg::prt_ethval(uint8_t refval) {

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

 bool brwEthCfg::checaConexaotEth () {
  
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


void brwEthCfg::EthLoop () {

  
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

void brwEthCfg::staticEthLoop (void *pvParameter) {

    brwEthCfg* ethl = reinterpret_cast<brwEthCfg*>(pvParameter); //obtain the instance pointer
    ethl->EthLoop(); //dispatch to the member function, now that we have an instance pointer

}







//------------------------------------------------------------------------------------------
//
// -----   Pisca LED 
//
//------------------------------------------------------------------------------------------

void brwEthCfg::PiscaLed ()
{
   
Serial.println ("- Pisca LED");   
while (true)
  {  
    
  if (getStatusEth()) // se esta conectado na rede
   {
   digitalWrite(pBrwHW -> getLED_IN(), !digitalRead(pBrwHW -> getLED_IN()));
   vTaskDelay(pdMS_TO_TICKS(200));
   } 
  else  
   {
   digitalWrite(pBrwHW -> getLED_IN(), !digitalRead(pBrwHW -> getLED_IN()));
   vTaskDelay(pdMS_TO_TICKS(2000));
   }  
  }
 
}


void brwEthCfg::staticPiscaLed (void *pvParameter) {

    brwEthCfg* led = reinterpret_cast<brwEthCfg*>(pvParameter); //obtain the instance pointer
    led->PiscaLed(); //dispatch to the member function, now that we have an instance pointer

}


//------------------------------------------------------------------------------------------
//
// ------    Le arquivo de cogiguracao do ETH
//------------------------------------------------------------------------------------------

bool brwEthCfg::getEthConfig () {

  File wFile;
  char st [20];
  bool b;
  


  if(SPIFFS.exists("/eth.cfg")) { 

    wFile = SPIFFS.open("/eth.cfg", "r");
    if(!wFile){
        Serial.println ("Não foi possivel abrir o arquivo de configuração: eth.cfg");
        return (false);  
    }
    Serial.println ("Ler o arquivo de configuração: eth.cfg");
    pBrwHW -> spifss -> leLinha (& wFile, st);
    if (strcmp(st, "false") == 0)
      setUsarIpFixo (false);
    else
      setUsarIpFixo (true);  
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pBrwHW -> spifss -> leLinha (& wFile, st);
    setIP (st);
    //Serial.print ("Lendo IP: ");
    //Serial.println (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pBrwHW -> spifss -> leLinha (& wFile, st);
    setMascara (st);
    //Serial.print ("Lendo mascara: ");
    //Serial.println (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pBrwHW -> spifss -> leLinha (& wFile, st);
    setGateWay (st);
    //Serial.print ("lendo gateway: ");
    //Serial.println (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pBrwHW -> spifss -> leLinha (& wFile, st);
    setDNserver (st);
    //Serial.print ("Lendo DNS: ");
    //Serial.println (st);
    vTaskDelay(pdMS_TO_TICKS(100)); 


    wFile.close ();
    return (true);
  } else { // le default e cria arquivo
      
      setEthDefaut ();  
      
      Serial.println ("-- >> Gravando eth.cfg");
      wFile = SPIFFS.open("/eth.cfg", "w");
      if(!wFile){
          Serial.println ("Não foi possivel (abrir) salvar o arquivo de configuração");
          return (false);  
        }
        b = getUsarIpFixo(); 
        if (b)    
          strncpy (st,"true",sizeof(st));
        else
          strncpy (st,"false",sizeof(st));
        //Serial.print ("Gravando ip: ");
        //Serial.println (st);
        //Serial.print ("Gravando ip fixo?: ");
        //Serial.println (st);
        pBrwHW -> spifss -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100));

        strncpy (st,getIP(),sizeof(st));
        pBrwHW -> spifss -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100));
        //Serial.print ("Gravando ip: ");
        //Serial.println (st);

        strncpy (st,getMascara(),sizeof(st));
        pBrwHW -> spifss -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100));
        //Serial.print ("Gravando mascara: ");
        //Serial.println (st);

        strncpy (st,getGateWay(),sizeof(st));
        pBrwHW -> spifss -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100));
        //Serial.print ("Gravando gateway: ");
        //Serial.println (st);

        strncpy (st,getDNserver(),sizeof(st));
        pBrwHW -> spifss -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100));
        //Serial.print ("Gravando DNS: ");
        //Serial.println (st);
        

      wFile.close ();
      
      return (true);    
      
    }


}


//------------------------------------------------------------------------------------------
//
// ------    Grava arquivo de configuracao do Harware
//
//------------------------------------------------------------------------------------------
void brwEthCfg::setEthDefaut () {

    Serial.println ("-- >> SET Default eth.cfg");

    if (strcmp (cIP_FIXO,"false") == 0) 
        setUsarIpFixo (false);
    else
        setUsarIpFixo (true);  

    setIP (cIP);
    setMascara (cMASK);
    setGateWay (cGATEWAY);
    setDNserver (cDNS);


}

//------------------------------------------------------------------------------------------
//
// ------    Print arquivo de configuracao do Harware
//
//------------------------------------------------------------------------------------------

void brwEthCfg::printEthCfg () {

    Serial.print  ("IP Fixo.: ");
    if (getUsarIpFixo())
      Serial.println  ("TRUE - " + String (getUsarIpFixo()));
    else
      Serial.println  ("FALSE - " + String (getUsarIpFixo()));

    Serial.println  ("IP......: " + String (getIP()));

    Serial.println  ("MASCARA.: " + String (getMascara()));

    Serial.println  ("GATEWAY.: " + String (getGateWay()));

    Serial.println  ("DNS.....: " + String (getDNserver()));


}


//------------------------------------------------------------------------------------------
//
//  --------    Salva configurações de rede em rede.cfg
//
//------------------------------------------------------------------------------------------


bool brwEthCfg::setEthConfig () {

File wFile;
String str;

    
    wFile = SPIFFS.open("/eth.cfg", "w");
    if(!wFile){
        Serial.println ("Não foi possivel salvar o arquivo de configuração");
        return (false);  
    }

    str = (usarIpFixo ? "true" : "false");
    wFile.println (str);      
    //Serial.print ("*** On Save: usarIpFixo: ");
    //Serial.println (str);      
    wFile.println (strIP);
    wFile.println (strMASK);
    wFile.println (strGW);
    wFile.println (strDNS);

    wFile.close ();
    //Serial.println ("*** Escreveu arquivo /eth.cfg com sucesso");
    
    return (true);

}



        

