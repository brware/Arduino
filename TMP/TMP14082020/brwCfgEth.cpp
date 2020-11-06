

#include "brwCfgEth.h"




//------------------------------------------------------------------------------------------
//
//  Construtor da Classe de configuração de rede
//
//-------------------------------------------------------------------------------------------

brwCfgEth::brwCfgEth (){




  
}

void brwCfgEth::beginEth (brwHardware * brwHW){
    
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

/*
void brwCfgEth::beginEth (brwHardware * brwHW, brwPlayade01 * brwPlay01){
    
    Serial.println ("Inicilalizando Classe Eth");

    pBrwHW = brwHW;
    pBrwP01 = brwPlay01;

    setMacAddress ();

     if (pBrwHW -> spifss -> isOpen == false)
      pBrwHW -> spifss -> beginFS();


    if (!strcmp (pBrwHW -> hwApp,"playade01")) {
        pBrwP01 -> iniPlayade01 (pBrwHW -> spifss); // passa ponteiro de arquivo para classe playade01
        Serial.println ("Inicilalizando Classe Playade01");
    }

   

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

    pBrwP01 -> getPlay01Config ();
    //Serial.println ("--- Listando: getPlay01Config ()");
    //printPlay01Cfg ();


    


}

*/

//------------------------------------------------------------------------------------------
//
//  Inicia a rede Ethernet
//  - Se NAO existe o arquivo rede.cfg inicia como DHCP
//  - Se existe inicia conforme configurado no arquivo  
//
//-------------------------------------------------------------------------------------------


void brwCfgEth::initEth (){
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

 
    xTaskCreate(&brwCfgEth::staticPiscaLed, "PiscaLed", 1024, (void*) this, 5, NULL); 
    xTaskCreate(&brwCfgEth::staticEthLoop, "EthLoop", 4096, (void*) this, 4, NULL); 


    getEthConfig ();

    
}



void brwCfgEth::initEthFixo (){
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
    
    xTaskCreate(&brwCfgEth::staticPiscaLed, "PiscaLed", 1024, (void*) this, 5, NULL); 
    xTaskCreate(&brwCfgEth::staticEthLoop, "EthLoop", 4096, (void*) this, 4, NULL); 
     
    //Serial.println ("Cfg do Arquivo");  
    //getEthConfig ();

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

char * brwCfgEth::getChipID () {
    char str [50];

    itoa (chipid, str,10);
	return (str);

}




//------------------------------------------------------------------------------------------
//
// -----------   Retorna o MAC Address do ESP
//
//------------------------------------------------------------------------------------------

char * brwCfgEth::getMacAddress () {

  return (mac_str);

}


//------------------------------------------------------------------------------------------
//
// -----------   Seta IP ETH
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setIP (char* ip) {
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

void brwCfgEth::setMascara (char* mk) {
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

void brwCfgEth::setGateWay (char* gw) {
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

void brwCfgEth::setDNserver (char* dns) {
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

void brwCfgEth::setModeloHardware () {
    String tmp;
    
    tmp = prt_hwval(Ethernet.hardwareStatus());
    strncpy (modeloHardware, tmp.c_str(),sizeof(modeloHardware));


}



//------------------------------------------------------------------------------------------
//
// -----------   Link Status da placa de rede
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setStatusEthDescricao () {
    String tmp;
    
    tmp = prt_ethval(Ethernet.linkStatus());
    strncpy (stuEthDescricao, tmp.c_str(),sizeof(stuEthDescricao));


}

//------------------------------------------------------------------------------------------
//
// -----------   Set se vai usar DHCP (true) ou IP Fixo (false)
//
//------------------------------------------------------------------------------------------

void brwCfgEth::setUsarIpFixo (bool valor) {
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

bool brwCfgEth::getUsarIpFixo () {

  return (usarIpFixo);

}


//------------------------------------------------------------------------------------------
//
// -----------   Get IP ETH
//
//------------------------------------------------------------------------------------------

char* brwCfgEth::getIP () {


	return (strIP);
	
}



//------------------------------------------------------------------------------------------
//
// -----------   Get Mascara de Rede ETH
//
//------------------------------------------------------------------------------------------

char* brwCfgEth::getMascara () {

	return (strMASK);
	
}




//------------------------------------------------------------------------------------------
//
// -----------   Get Gate Way ETH
//
//------------------------------------------------------------------------------------------

char* brwCfgEth::getGateWay () {

	return (strGW);

}



//------------------------------------------------------------------------------------------
//
// -----------   Get DNS Server ETH
//
//------------------------------------------------------------------------------------------

char* brwCfgEth::getDNserver () {

	return (strDNS);

}



//------------------------------------------------------------------------------------------
//
// -----------   Get Modelo de Harware detectado
//
//------------------------------------------------------------------------------------------

char* brwCfgEth::getModeloHardware () {

	return (modeloHardware);

}

//------------------------------------------------------------------------------------------
//
// -----------   Get Descrição do Status da placa de REde
//
//------------------------------------------------------------------------------------------

char* brwCfgEth::getStatusEthDescricao () {

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


void brwCfgEth::staticPiscaLed (void *pvParameter) {

    brwCfgEth* led = reinterpret_cast<brwCfgEth*>(pvParameter); //obtain the instance pointer
    led->PiscaLed(); //dispatch to the member function, now that we have an instance pointer

}


//------------------------------------------------------------------------------------------
//
// ------    Le arquivo de cogiguracao do ETH
//------------------------------------------------------------------------------------------

bool brwCfgEth::getEthConfig () {

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
void brwCfgEth::setEthDefaut () {

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

void brwCfgEth::printEthCfg () {

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


bool brwCfgEth::setEthConfig () {

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


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//
//  --------    WEB SERVER
//
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------


EthernetServer  serverWeb(80);

void brwCfgEth::wsBegin () {

    

    serverWeb.begin ();

 
    xTaskCreate(&brwCfgEth::staticWebServerLoop, "webServerLoop", 20480, (void*) this, 2, NULL);
    vTaskDelay(pdMS_TO_TICKS(300));

}

//------------------------------------------------------------------------------------------
//
//  --------    ETH WEB server Loop
//
//------------------------------------------------------------------------------------------



void brwCfgEth::webServerLoop () {


Serial.println("- Task WebServerLoop");

  while (true) {
    if (getStatusEth ()) {
      trataHtml_eth ();  
      
      }
       
      vTaskDelay(pdMS_TO_TICKS(1000));
      //vTaskDelay(pdMS_TO_TICKS(100));
    } 

  

}

void brwCfgEth::staticWebServerLoop  (void *pvParameter) {

    brwCfgEth* ws = reinterpret_cast<brwCfgEth*>(pvParameter); //obtain the instance pointer
    ws->webServerLoop(); //dispatch to the member function, now that we have an instance pointer

}


//------------------------------------------------------------------------------------------
//
//    Trata HTLM recebido do cliente ( vai para Task)
//
//------------------------------------------------------------------------------------------


void brwCfgEth::trataHtml_eth () {
 String str, s_cmd;
     
    EthernetClient client = serverWeb.available();
   
    if (client) { 
      

      //esp_task_wdt_reset();  // reset do WDT
      //Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      //Serial.println("- Web Server Start");
      // ******************************************************enviaDados();
      
      while (client.connected()) {
        //Serial.println ("while (pEcli -> connected())");
        
        if (client.available()) {
          //Serial.println ("pEcli -> available()");
          char c = client.read();
          str = str + c;
          // Serial.write(c);
          //Serial.print(c);
          // if you've gotten to the end of the line (received a newline
          // character) and the line is blank, the http request has ended,
          // so you can send a reply
          if (c == '\n' && currentLineIsBlank) {
            //Serial.println(str);
            s_cmd = str.substring (str.indexOf ("GET") + 4, str.indexOf ("HTTP/1.1") - 1);
            //Serial.println(s_cmd);
            if (s_cmd != "/favicon.ico") {
              if (s_cmd == "/") { // pedido de dados de temperatura e umidade
                sendIndex (client);
               
              }
              
            if (s_cmd == "/json") { // pedido de dados de temperatura e umidade
                sendIndex (client);
               
              }  
            if (s_cmd == "/config") { // formulario de configuração de IP
                 config_html (client);
              }
            if (s_cmd.substring (0, 4) == "/get") { // retorno do formulario
                //Serial.println (" Get do formulario ");
                trataGet (client, s_cmd);
                //Serial.println (s_cmd);  
                s_cmd = "";
              }
            if (s_cmd.substring (0, 5) == "/ola") { // retorno do formulario
                olaResponse (client); 
              } 
               
            //Serial.println("- Break");
            //vTaskDelay(pdMS_TO_TICKS(10)); 
            break;
          } else {
            break;
          }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
            
          } else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false; 
          }
          
        }
       
      }
      // give the web browser time to receive the data
      }
      // close the connection:


      client.stop();
      //Serial.println("- Web Server Stop");
      vTaskDelay(pdMS_TO_TICKS(20)); 
    
    }  

}


//------------------------------------------------------------------------------------------
//
//    Trata GET recebido da configuração
//
//------------------------------------------------------------------------------------------
void brwCfgEth::trataGet (EthernetClient client, String recget) {

    if (StrContains(pBrwHW -> hwApp, "playade01")) {
        trataGetPlayade01 (client, recget);
    }

}



 void brwCfgEth::trataGetPlayade01 (EthernetClient client, String recget) {

  String aux;
  int i01 = 0, i02 = 0;
  
  // se vai usar IP fixo ou DHCP 
  if (recget.indexOf ("usarIpFixo=") > 0) {
    aux = recget.substring (16,18);
    //Serial.print ("Flag IP...: ");
    //Serial.println (aux);
    //Serial.println (recget.indexOf ("usarIpFixo="));
    if (aux = "on")
      setUsarIpFixo (true);
    else
      setUsarIpFixo (false);  
  } else
      setUsarIpFixo (false); 
  //
  //  Configuração de Rede
  //
  // IP
  i01 = recget.indexOf ("ip=") + 3;
  i02 = recget.indexOf ("mascara=") - 1;
  if (i01 != 0) {
    setIP ((char*)recget.substring (i01,i02).c_str());
  }

  // Mascara de rede
  i01 = recget.indexOf ("mascara=") + 8;
  i02 = recget.indexOf ("gateway=") - 1;
  if (i01 != 0) {
    setMascara ((char*)recget.substring (i01,i02).c_str());
  }

  // Gate Way
  i01 = recget.indexOf ("gateway=") + 8;
  i02 = recget.indexOf ("dns=") - 1;
  if (i01 != 0) {
    setGateWay ((char*)recget.substring (i01,i02).c_str());
  }

  // DNS
  i01 = recget.indexOf ("dns=") + 4;
  i02 = recget.indexOf ("t_min=") - 1;
  if (i01 != 0) {
    setDNserver ((char*)recget.substring (i01,i02).c_str());
  }
  
  //
  //  Configuração de Alarme
  //

  // Temperatura Minima
  i01 = recget.indexOf ("t_min=") + 6;
  i02 = recget.indexOf ("t_max=") - 1;
  if (i01 != 0) {
    setMinT ((char*)recget.substring (i01,i02).c_str());
    //strncpy (minT,recget.substring (i01,i02).c_str(), sizeof(minT));
  }

  // Temperatura Maxima
  i01 = recget.indexOf ("t_max=") + 6;
  i02 = recget.indexOf ("u_min=") - 1;
  if (i01 != 0) {
    setMaxT ((char*)recget.substring (i01,i02).c_str());
    //strncpy (maxT,recget.substring (i01,i02).c_str(),sizeof(maxT));
  }

  // Umidade Minima
  i01 = recget.indexOf ("u_min=") + 6;
  i02 = recget.indexOf ("u_max") - 1;
  if (i01 != 0) {
    setMinU ((char*)recget.substring (i01,i02).c_str());
    //strncpy (minU,recget.substring (i01,i02).c_str(),sizeof(minU));
  }

  // Umidade Maxima
  i01 = recget.indexOf ("u_max=") + 6;
  i02 = recget.indexOf ("usarMQTT=");
  if (i02 > 0)
    i02 = recget.indexOf ("usarMQTT=") - 1;
  else
    i02 = recget.indexOf ("host=") - 1;  
  if (i01 != 0) {
    setMaxU ((char*)recget.substring (i01,i02).c_str());
    //strncpy (maxU,recget.substring (i01,i02).c_str(),sizeof(maxU));
  }
/*
  // se vai usar MQTT
  i01 = recget.indexOf ("usarMQTT=");
  if (i01 > 0) {
    i01 += 9;
    i02 = recget.indexOf ("host=") - 1;
    aux = recget.substring (i01,i02);
    if (aux = "on")
      mqtt.setMqttUsar (true);
    else
      mqtt.setMqttUsar (false);  
  } else
      mqtt.setMqttUsar (false); 


   // host MQTT
  i01 = recget.indexOf ("host=") + 5;
  i02 = recget.indexOf ("user=") - 1;
  mqtt.setMqttHost (recget.substring (i01,i02));
    
   // User MQTT
  i01 = recget.indexOf ("user=") + 5;
  i02 = recget.indexOf ("HTTP") - 2;
  //i02 = i01 + 30;
  mqtt.setMqttUser (recget.substring (i01,i02));

*/
  //salvaCfg ();
  getResponsePlayade01 (client);
  Serial.println("Vai salvar eth.cfg");
  printEthCfg ();
  setEthConfig ();
  Serial.println("Salvou eth.cfg");

  vTaskDelay(pdMS_TO_TICKS(1000)); 
  //getEthConfig ();
  //printEthCfg ();
  
 
  setPlay01Config ();
  printPlay01Cfg ();
  vTaskDelay(pdMS_TO_TICKS(1000)); 


  ESP.restart();
 /* 
  //Serial.println ("--- Antes de salvar variaveis");
  printCfg ();
  

  //Serial.println ("--- depois de salvar  variaveis");
  if (salvaCfg ())
    printCfg ();
  else
    Serial.println ("Erro Salvando arquivo de configuração");
      

  //Serial.println ("--- depois de  Ler variaveis");
  if (leCfg ())
    printCfg ();
  else 
    Serial.println ("Erro lendo arquivo de configuração"); 
  
  //Serial.println ("Aguarda 1 segundo"); 
  vTaskDelay(pdMS_TO_TICKS(1000));
  //Serial.println ("Lista Arquivos"); 
  efs.listDir("/", 0);
  //Serial.println ("Aguarda 1 segundo"); 
  vTaskDelay(pdMS_TO_TICKS(1000));
   //Serial.println ("Reinicia....."); 
   
  boot (); 
  */
 }


//------------------------------------------------------------------------------------------
//
//    Responde ao Browser ao Subimit da pagina de configuração
//
//------------------------------------------------------------------------------------------
void brwCfgEth::getResponsePlayade01 (EthernetClient client) {
    String html; 


    html = "HTTP/1.1 200 OK\n";
    html.concat("Content-Type: text/html\n");
    html.concat("Connection: close\n");  // the connection will be closed after completion of the response
    //client.println("Refresh: 5\n");  // refresh the page automatically every 5 sec
    html.concat("\n");
    html.concat ("<!DOCTYPE html>\n");

    html.concat (" <html> \n");
    html.concat ("<head> \n");
    html.concat ("<title>Configiracao de IP</title>");
    html.concat ("<meta name='viewport' content='width=device-width, initial-scale=1'>\n");
    html.concat ("</head> \n");
    html.concat ("<body>");
    html.concat ("<h1><center>Sistema Playades</center></h1>");
    html.concat ("<h2>Configuracao Enviada para equipamento (MacAddress): " + String(getMacAddress ()) + "</h2>");
    html.concat ("<br/><hr/><b> Configuracoes de IP </b><br/>");
    html.concat ("<table>");
    if (getUsarIpFixo ()) 
      html.concat ("<tr><td> Usando IP </td><td> Fixo</tr></td>");
     else   
      html.concat ("<tr><td> Usando IP via </td><td> DHCP</tr></td>");

    html.concat ("<tr><td> IP : </td><td>" + String(getIP ()) + "</tr></td>"); 

    html.concat ("<tr><td><tr><td> Mascara de Rede : </td><td>" + String(getMascara ()) + "</tr></td>");

    html.concat ("<tr><td> Gate Way : </td><td>" + String(getGateWay ()) + "</tr></td>"); 

    html.concat ("<tr><td> Servidor DNS : </td><td>" + String(getDNserver ()) + "</tr></td>"); 
    html.concat ("</table>");

    html.concat ("<br/><hr/><b> Configuracoes de Alarme </b><br/>");
    html.concat ("<table>");
    html.concat ("<tr><td> Temperatura Minima : </td><td>" +  String(getMinT()) + "</tr></td>");
    html.concat ("<tr><td> Temperatura Maxima : </td><td>" +  String(getMaxT()) + "</tr></td>");            
    html.concat ("<tr><td> Umidade Minima : </td><td>" +  String(getMinU()) + "</tr></td>");      
    html.concat ("<tr><td> Umidade Maxima : </td><td>" +  String(getMaxU()) + "</tr></td>");
    html.concat ("</table>");

    html.concat ("</body></html> ");
    client.println (html);

} 




void brwCfgEth::sendIndex (EthernetClient client) {
String html;

  //Serial.println ("sendIndex ()");
    if (StrContains(pBrwHW -> hwApp, "playade01")) {
        indexPlayade01 (client);
    } else {
        html = "HTTP/1.1 200 OK\n";
        html.concat("Content-Type: text/html\n");
        html.concat("Connection: close\n");  // the connection will be closed after completion of the response
        //client.println("Refresh: 5\n");  // refresh the page automatically every 5 sec
        html.concat("\n");
        html.concat ("<!DOCTYPE html>\n");
        html.concat (" <html> \n");
        //html.concat ("Refresh: 5\n"); 
        //html.concat ("\n"); 
        html.concat ("<head> \n");
        html.concat ("<meta charset=\"UTF-8\"/>\n");
        html.concat ("<title> brWare - IoT  </title> \n "); 
        html.concat ("</head>\n");

        html.concat ("<body> \n");
        html.concat ("<h1>  brWare IoT  </h1>");
        html.concat ("<p> App : ");
        html.concat (String(pBrwHW -> hwApp)); 
        html.concat (" </p>");
        html.concat ("<p> Info: ");
        html.concat (String(pBrwHW -> infoHw));    
        html.concat (" </p>");
        html.concat ("<p> Versão: ");
        html.concat (String(pBrwHW -> infoVersao));    
        html.concat (" </p>");
        html.concat (" </body> \n");
        html.concat (" </html> \n");
       
        client.println(html);

    }
    

    

}


void brwCfgEth::indexPlayade01 (EthernetClient client) {

  String json;

  
 
  json = "HTTP/1.1 200 OK\n";
  json.concat ("Content-Type: application/json\n"); 
  //json.concat ("Refresh: 5\n"); 
  json.concat ("\n"); 
  json.concat ("{\"IP\": \"");
  json.concat (getIP ());
  json.concat ("\",\"Temp\": \"");
  //json.concat (temperatura);
  json.concat ("23.5");
  json.concat ("\",\"Humid\": \"");
  //json.concat (umidade);
  json.concat ("65,3");
  json.concat ("\"}\n");
  client.println(json);

}



//------------------------------------------------------------------------------------------
//
//    Responde ao entrar na pagina /ola
//
//------------------------------------------------------------------------------------------
void brwCfgEth::olaResponse (EthernetClient client) {
String html;


html = "HTTP/1.1 200 OK\n";
html.concat("Content-Type: text/html\n");
html.concat("Connection: close\n");  // the connection will be closed after completion of the response
//client.println("Refresh: 5\n");  // refresh the page automatically every 5 sec
html.concat("\n");
html.concat("<!DOCTYPE HTML>\n");
html.concat("<html>");
html.concat ("<head> \n");
html.concat ("<title>IP do Equipamento</title>\n");
html.concat ("<meta name='viewport' content='width=device-width, initial-scale=1'>\n");
html.concat ("</head> \n");
html.concat ("<body>\n");


html.concat(getMacAddress ()); 
html.concat("<br>");
html.concat(getIP ()); 

html.concat("</body></html> ");

client.println(html);

}


//------------------------------------------------------------------------------------------
//
//    Monta pagina HTML de Configuração \config
//
//------------------------------------------------------------------------------------------

 void brwCfgEth::config_html (EthernetClient client){

    if (StrContains(pBrwHW -> hwApp, "playade01")) {
        configPlayade01 (client);
    }
/*
    if (StrContains(pBrwHW -> hwApp, "playade02")) {
        configPlayade02 (client);
    }

    if (StrContains(pBrwHW -> hwApp, "sucres")) {
        configSucres (client);    
    }
*/



  

 }


//------------------------------------------------------------------------------------------
//
//    Monta pagina HTML de Configuração para Playade 01 - temperatura
//
//------------------------------------------------------------------------------------------

void brwCfgEth::configPlayade01 (EthernetClient client) {

String html;    
    
    Serial.println ("Configurações de rede - configPlayade01");
    
    printEthCfg ();

    html =  ("<!DOCTYPE HTML><html><head>");
    html.concat ("<title>Configiracao de IP</title>");
    html.concat ("<meta name='viewport' content='width=device-width, initial-scale=1'>");
    html.concat ("</head><body>");
    html.concat ("<h1><center>Sistema Playades 01 - Temperaura e Umidade</center></h1>");

    html.concat ("<h2>Configuracao para IP Fixo</h2>");
    html.concat ("<form action='/get'>");
    //client.println ("<form action='/post'>");
    //client.println ("Usar IP Fixo </td> <td><input type='checkbox' name='usarIpFixo' value='true'>");
    html.concat ("<table>");
    html.concat ("<tr><td>Usar IP Fixo </td> <td><input type='checkbox' name='usarIpFixo' ");
    if (getUsarIpFixo ())
        html.concat ("checked");
    html.concat ("></td></tr>");
    html.concat ("<tr><td>IP : </td> <td><input type='text' name='ip' value='");
    html.concat (getIP ());
    html.concat ("' required></td></tr>");
    html.concat ("<tr><td>Mascara : </td> <td><input type='text' name='mascara' value='");
    html.concat (getMascara ());
    html.concat ("' required></td></tr>");  
    html.concat ("<tr><td>GateWay : </td> <td><input type='text' name='gateway' value='");
    html.concat (getGateWay ());
    html.concat ("' required></td></tr>");
    html.concat ("<tr><td>DNS : </td> <td><input type='text' name='dns' value='");
    html.concat (getDNserver ());
    html.concat ("' required></td></tr>");
    html.concat ("</table>"); 
  
    html.concat ("<br/><hr/><br/>");

    html.concat ("<h2>Configuracao para Alarmes</h2>");
    html.concat ("<table>");
    html.concat ("<tr><td>Temperatura Minima: </td> <td><input type='text' name='t_min' value='");
    html.concat (getMinT());
    html.concat ("' required><br></td></tr>");

    html.concat ("<tr><td>Temperatura Maxima: </td> <td><input type='text'  name='t_max' value='");
    html.concat (getMaxT());
    html.concat ("' required><br></td></tr>");
    
    html.concat ("<tr><td>Umidade Minima: </td> <td><input type='text' name='u_min' value='");
    html.concat (getMinU());
    html.concat ("' required><br></td></tr>");
    
    html.concat ("<tr><td>Umidade Maxima: </td> <td><input type='text' name='u_max' value='");
    html.concat (getMaxU());
    html.concat ("' required><br></td></tr>");
  
    html.concat ("<tr><td><input type='submit' value='Enviar'></td></tr>");
    html.concat ("</table>");  
    html.concat ("</form>");
    html.concat("</body></html> ");

    client.println (html);

}


        

