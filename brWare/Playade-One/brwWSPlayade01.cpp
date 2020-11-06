

#include "brwWSPlayade01.h"


EthernetServer  serverWeb(80);


//-----------------------------------------------------------------------------------------
//
//  Construtor da Classe de Web Server para Eth
//
//-------------------------------------------------------------------------------------------

brwWSPlayade01::brwWSPlayade01 () {


}






void brwWSPlayade01::beginWebServer (brwEthCfg* EthCfg, brwDHT* bDHT) {

  EthernetServer  serverWeb(80);

  pEthCfg = EthCfg;

  pDHT = bDHT;


  serverWeb.begin ();


  xTaskCreate(brwWSPlayade01::staticWebServerLoop, "webServerLoop", 20480, (void*) this, 2, NULL);
  vTaskDelay(pdMS_TO_TICKS(300));

}






//------------------------------------------------------------------------------------------
//
//  --------    ETH WEB server Loop
//
//------------------------------------------------------------------------------------------



void brwWSPlayade01::webServerLoop () {


Serial.println("->>>>>> Task WebServerLoop");

  while (true) {
    if (pEthCfg -> getStatusEth ()) {
      trataHtml_eth ();  
      
      }
       
      vTaskDelay(pdMS_TO_TICKS(200));
    } 

  

}

void brwWSPlayade01::staticWebServerLoop  (void *pvParameter) {

    brwWSPlayade01* ws = reinterpret_cast<brwWSPlayade01*>(pvParameter); //obtain the instance pointer
    ws->webServerLoop(); //dispatch to the member function, now that we have an instance pointer

}



//------------------------------------------------------------------------------------------
//
//    Trata HTLM recebido do cliente ( vai para Task)
//
//------------------------------------------------------------------------------------------


void brwWSPlayade01::trataHtml_eth () {
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
void brwWSPlayade01::trataGet (EthernetClient client, String recget) {

    if (pEthCfg -> StrContains(pEthCfg -> pBrwHW -> hwApp, "playade01")) {
        trataGetPlayade01 (client, recget);
    }

}



 void brwWSPlayade01::trataGetPlayade01 (EthernetClient client, String recget) {

  String aux;
  int i01 = 0, i02 = 0;
  
  // se vai usar IP fixo ou DHCP 
  if (recget.indexOf ("usarIpFixo=") > 0) {
    aux = recget.substring (16,18);
    //Serial.print ("Flag IP...: ");
    //Serial.println (aux);
    //Serial.println (recget.indexOf ("usarIpFixo="));
    if (aux = "on")
      pEthCfg -> setUsarIpFixo (true);
    else
      pEthCfg -> setUsarIpFixo (false);  
  } else
      pEthCfg -> setUsarIpFixo (false); 
  //
  //  Configuração de Rede
  //
  // IP
  i01 = recget.indexOf ("ip=") + 3;
  i02 = recget.indexOf ("mascara=") - 1;
  if (i01 != 0) {
    pEthCfg -> setIP ((char*)recget.substring (i01,i02).c_str());
  }

  // Mascara de rede
  i01 = recget.indexOf ("mascara=") + 8;
  i02 = recget.indexOf ("gateway=") - 1;
  if (i01 != 0) {
    pEthCfg -> setMascara ((char*)recget.substring (i01,i02).c_str());
  }

  // Gate Way
  i01 = recget.indexOf ("gateway=") + 8;
  i02 = recget.indexOf ("dns=") - 1;
  if (i01 != 0) {
    pEthCfg -> setGateWay ((char*)recget.substring (i01,i02).c_str());
  }

  // DNS
  i01 = recget.indexOf ("dns=") + 4;
  i02 = recget.indexOf ("t_min=") - 1;
  if (i01 != 0) {
    pEthCfg -> setDNserver ((char*)recget.substring (i01,i02).c_str());
  }
  
  //
  //  Configuração de Alarme
  //

  // Temperatura Minima
  i01 = recget.indexOf ("t_min=") + 6;
  i02 = recget.indexOf ("t_max=") - 1;
  if (i01 != 0) {
    pDHT -> setMinT ((char*)recget.substring (i01,i02).c_str());
    //strncpy (minT,recget.substring (i01,i02).c_str(), sizeof(minT));
  }

  // Temperatura Maxima
  i01 = recget.indexOf ("t_max=") + 6;
  i02 = recget.indexOf ("u_min=") - 1;
  if (i01 != 0) {
    pDHT -> setMaxT ((char*)recget.substring (i01,i02).c_str());
    //strncpy (maxT,recget.substring (i01,i02).c_str(),sizeof(maxT));
  }

  // Umidade Minima
  i01 = recget.indexOf ("u_min=") + 6;
  i02 = recget.indexOf ("u_max") - 1;
  if (i01 != 0) {
    pDHT -> setMinU ((char*)recget.substring (i01,i02).c_str());
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
    pDHT -> setMaxU ((char*)recget.substring (i01,i02).c_str());
    //strncpy (maxU,recget.substring (i01,i02).c_str(),sizeof(maxU));
  }

  //salvaCfg ();
  getResponsePlayade01 (client);

  Serial.println("Vai salvar eth.cfg");
  pEthCfg -> printEthCfg ();
  pEthCfg -> setEthConfig ();
  Serial.println("Salvou eth.cfg");

  vTaskDelay(pdMS_TO_TICKS(1000)); 
  pDHT -> setDhtConfig ();
  Serial.println("Salvou dht.cfg");
  //printEthCfg ();
  
 
  //setPlay01Config ();
  //printPlay01Cfg ();
  vTaskDelay(pdMS_TO_TICKS(1000)); 


  ESP.restart();
 
 }


//------------------------------------------------------------------------------------------
//
//    Responde ao Browser ao Subimit da pagina de configuração
//
//------------------------------------------------------------------------------------------
void brwWSPlayade01::getResponsePlayade01 (EthernetClient client) {
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
    html.concat ("<h2>Configuracao Enviada para equipamento (MacAddress): " + String(pEthCfg -> getMacAddress ()) + "</h2>");
    html.concat ("<br/><hr/><b> Configuracoes de IP </b><br/>");
    html.concat ("<table>");
    if (pEthCfg -> getUsarIpFixo ()) 
      html.concat ("<tr><td> Usando IP </td><td> Fixo</tr></td>");
     else   
      html.concat ("<tr><td> Usando IP via </td><td> DHCP</tr></td>");

    html.concat ("<tr><td> IP : </td><td>" + String(pEthCfg -> getIP ()) + "</tr></td>"); 

    html.concat ("<tr><td><tr><td> Mascara de Rede : </td><td>" + String(pEthCfg -> getMascara ()) + "</tr></td>");

    html.concat ("<tr><td> Gate Way : </td><td>" + String(pEthCfg -> getGateWay ()) + "</tr></td>"); 

    html.concat ("<tr><td> Servidor DNS : </td><td>" + String(pEthCfg -> getDNserver ()) + "</tr></td>"); 
    html.concat ("</table>");

    html.concat ("<br/><hr/><b> Configuracoes de Alarme </b><br/>");
    html.concat ("<table>");

    html.concat ("<tr><td> Temperatura Minima : </td><td>" +  String(pDHT -> getMinT()) + "</tr></td>");
    html.concat ("<tr><td> Temperatura Maxima : </td><td>" +  String(pDHT -> getMaxT()) + "</tr></td>");            
    html.concat ("<tr><td> Umidade Minima : </td><td>" +  String(pDHT -> getMinU()) + "</tr></td>");      
    html.concat ("<tr><td> Umidade Maxima : </td><td>" +  String(pDHT -> getMaxU()) + "</tr></td>");
    html.concat ("</table>");

    html.concat ("</body></html> ");
    client.println (html);

} 




void brwWSPlayade01::sendIndex (EthernetClient client) {
String html;

  //Serial.println ("sendIndex ()");
    if (pEthCfg -> StrContains(pEthCfg -> pBrwHW -> hwApp, "playade01")) {
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
        html.concat (String(pEthCfg -> pBrwHW -> hwApp)); 
        html.concat (" </p>");
        html.concat ("<p> Info: ");
        html.concat (String(pEthCfg -> pBrwHW -> infoHw));    
        html.concat (" </p>");
        html.concat ("<p> Versão: ");
        html.concat (String(pEthCfg -> pBrwHW -> infoVersao));    
        html.concat (" </p>");
        html.concat (" </body> \n");
        html.concat (" </html> \n");
       
        client.println(html);

    }
    

    

}


void brwWSPlayade01::indexPlayade01 (EthernetClient client) {

  String json;

  
 
  json = "HTTP/1.1 200 OK\n";
  json.concat ("Content-Type: application/json\n"); 
  //json.concat ("Refresh: 5\n"); 
  json.concat ("\n"); 
  json.concat ("{\"IP\": \"");
  json.concat (pEthCfg -> getIP ());
  json.concat ("\",\"Temp\": \"");
  json.concat (pDHT -> get_cTemperatura ());
  json.concat ("\",\"Humid\": \"");
  json.concat (pDHT -> get_cUmidade ());
  json.concat ("\"}\n");
  client.println(json);

}



//------------------------------------------------------------------------------------------
//
//    Responde ao entrar na pagina /ola
//
//------------------------------------------------------------------------------------------
void brwWSPlayade01::olaResponse (EthernetClient client) {
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


html.concat(pEthCfg -> getMacAddress ()); 
html.concat("<br>");
html.concat(pEthCfg -> getIP ()); 

html.concat("</body></html> ");

client.println(html);

}


//------------------------------------------------------------------------------------------
//
//    Monta pagina HTML de Configuração \config
//
//------------------------------------------------------------------------------------------

 void brwWSPlayade01::config_html (EthernetClient client){
    
    if (pEthCfg -> StrContains(pEthCfg -> pBrwHW -> hwApp, "playade01")) {
        
        configPlayade01 (client);
    }
 

 }


//------------------------------------------------------------------------------------------
//
//    Monta pagina HTML de Configuração para Playade 01 - temperatura
//
//------------------------------------------------------------------------------------------

void brwWSPlayade01::configPlayade01 (EthernetClient client) {

String html;  
char str [20];  
    
    Serial.println ("Configurações de rede - configPlayade01");
    
    pEthCfg -> printEthCfg ();

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
    if (pEthCfg -> getUsarIpFixo ())
        html.concat ("checked");
    html.concat ("></td></tr>");
    html.concat ("<tr><td>IP : </td> <td><input type='text' name='ip' value='");
    html.concat (pEthCfg -> getIP ());
    html.concat ("' required></td></tr>");
    html.concat ("<tr><td>Mascara : </td> <td><input type='text' name='mascara' value='");
    html.concat (pEthCfg -> getMascara ());
    html.concat ("' required></td></tr>");  
    html.concat ("<tr><td>GateWay : </td> <td><input type='text' name='gateway' value='");
    html.concat (pEthCfg -> getGateWay ());
    html.concat ("' required></td></tr>");
    html.concat ("<tr><td>DNS : </td> <td><input type='text' name='dns' value='");
    html.concat (pEthCfg -> getDNserver ());
    html.concat ("' required></td></tr>");
    html.concat ("</table>"); 
  
    html.concat ("<br/><hr/><br/>");

    html.concat ("<h2>Configuracao para Alarmes</h2>");
    html.concat ("<table>");
    html.concat ("<tr><td>Temperatura Minima: </td> <td><input type='text' name='t_min' value='");
    
    html.concat ((char*)pDHT -> getMinT());
    html.concat ("' required><br></td></tr>");

    html.concat ("<tr><td>Temperatura Maxima: </td> <td><input type='text'  name='t_max' value='");
    html.concat (pDHT -> getMaxT());
    html.concat ("' required><br></td></tr>");
    
    html.concat ("<tr><td>Umidade Minima: </td> <td><input type='text' name='u_min' value='");
    html.concat (pDHT -> getMinU());
    html.concat ("' required><br></td></tr>");
    
    html.concat ("<tr><td>Umidade Maxima: </td> <td><input type='text' name='u_max' value='");
    html.concat (pDHT -> getMaxU());
    html.concat ("' required><br></td></tr>");
  
    html.concat ("<tr><td><input type='submit' value='Enviar'></td></tr>");
    html.concat ("</table>");  
    html.concat ("</form>");
    html.concat("</body></html> ");

    client.println (html);

}

