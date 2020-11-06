

#include "brwEthWebServer.h"

//------------------------------------------------------------------------------------------
//
//  Construtor da Classe de configuração de rede
//
//-------------------------------------------------------------------------------------------

brwEthWebServer::brwEthWebServer () {

}





void brwEthWebServer::wsBegin (char* App, brwHardware* hW, brwCfgEth* Eth, EthernetClient*  cli, EthernetServer* esvr) {

	phW = hW;
	pEth = Eth;
  pEcli = cli;
  pEsvr = esvr;

 
	xTaskCreate(&brwEthWebServer::staticWebServerLoop, "webServerLoop", 20480, (void*) this, 2, NULL);
	vTaskDelay(pdMS_TO_TICKS(300));

}


void brwEthWebServer::wsBegin (char* App, brwHardware* hW, brwCfgEth* Eth, EthernetClient*  cli, EthernetServer* esvr, brwMqtt* MQ) {

	phW = hW;
	pEth = Eth;
  pEcli = cli;
	pMQ = MQ;

  Serial.println("wsBegin (");

	xTaskCreate(&brwEthWebServer::staticWebServerLoop, "webServerLoop", 20480, (void*) this, 2, NULL);
	vTaskDelay(pdMS_TO_TICKS(1000));

}


//------------------------------------------------------------------------------------------
//
//  --------    ETH WEB server Loop
//
//------------------------------------------------------------------------------------------



void brwEthWebServer::webServerLoop () {


Serial.println("- Task WebServerLoop");

  while (true) {
    if (pEth -> getStatusEth ()) {
      trataHtml_eth ();  
      
      }
       
      vTaskDelay(pdMS_TO_TICKS(1000));
      //vTaskDelay(pdMS_TO_TICKS(100));
    } 

  

}

void brwEthWebServer::staticWebServerLoop  (void *pvParameter) {

    brwEthWebServer* ws = reinterpret_cast<brwEthWebServer*>(pvParameter); //obtain the instance pointer
    ws->webServerLoop(); //dispatch to the member function, now that we have an instance pointer

}


//------------------------------------------------------------------------------------------
//
//    Trata HTLM recebido do cliente ( vai para Task)
//
//------------------------------------------------------------------------------------------


void brwEthWebServer::trataHtml_eth () {
 String str, s_cmd;
     
    //pEcli = &(pEsvr -> available());
   
    if (pEcli) { 
      

      //esp_task_wdt_reset();  // reset do WDT
      //Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      //Serial.println("- Web Server Start");
      // ******************************************************enviaDados();
      
      while (pEcli -> connected()) {
        Serial.println ("while (pEcli -> connected())");
        
        if (pEcli -> available()) {
          Serial.println ("pEcli -> available()");
          char c = pEcli -> read();
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
                sendIndex ();
               
              }
              
            if (s_cmd == "/json") { // pedido de dados de temperatura e umidade
                sendIndex ();
               
              }  
            if (s_cmd == "/config") { // formulario de configuração de IP
                 //config_html (pEcli -> client);
              }
            if (s_cmd.substring (0, 4) == "/get") { // retorno do formulario
                //Serial.println (" Get do formulario ");
                //trataGet (pEcli -> client, s_cmd);
                //Serial.println (s_cmd);  
                s_cmd = "";
              }
            if (s_cmd.substring (0, 5) == "/ola") { // retorno do formulario
               //olaResponse (pEcli -> client); 
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


      pEcli -> stop();
      Serial.println("- Web Server Stop");
      vTaskDelay(pdMS_TO_TICKS(20)); 
    
    }  

}


void brwEthWebServer::sendIndex () {
String html;

  Serial.println ("sendIndex ()");
 
	html = "HTTP/1.1 200 OK\n";
	html.concat ("Content-Type: application/json\n"); 
	//index.concat ("Refresh: 5\n"); 
	html.concat ("\n"); 
	html.concat ("<html> \n <head> \n <title> brWare - IoT  <title> \n </head> \n");
	html.concat (",<body> \n <h1> \n brWare IoT  </h1>");
	html.concat ("<p> App : ");
	html.concat (String(phW -> hwApp));	
	html.concat (" </p>");
	html.concat ("<p> Info: ");
	html.concat (String(phW -> infoHw));	
	html.concat (" </p>");
	html.concat ("<p> Versão: ");
	html.concat (String(phW -> infoVersao));	
	html.concat (" </p>");
	html.concat (" </body> \n </html>");
  


  	pEcli -> println(html);

}

