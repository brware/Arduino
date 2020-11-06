//------------------------------------------------------------------------------------------
//
//    Trata HTLM recebido do cliente ( vai para Task)
//
//------------------------------------------------------------------------------------------


void trataHtml () {
 String str, s_cmd;
 

    EthernetClient client = serverWEB.available();
    
    if (client) { 
      
      //esp_task_wdt_reset();  // reset do WDT
      //Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      Serial.println("- Web Server Start");
      

      while (client.connected()) {
        
        if (client.available()) {
          char c = client.read();
          str = str + c;
          // Serial.write(c);
          Serial.print(c);
          // if you've gotten to the end of the line (received a newline
          // character) and the line is blank, the http request has ended,
          // so you can send a reply
          if (c == '\n' && currentLineIsBlank) {
            //Serial.println(str);
            s_cmd = str.substring (str.indexOf ("GET") + 4, str.indexOf ("HTTP/1.1") - 1);
            //Serial.println(s_cmd);
            if (s_cmd != "/favicon.ico") {
              if (s_cmd == "/") { // pedido de dados de temperatura e umidade
                envia_jason (client);
               
              }
              if (s_cmd == "/config") { // formulario de configuração de IP
                 config_html (client);
              }
              if (s_cmd.substring (0, 4) == "/get") { // retorno do formulario
                //Serial.println (" Get do formulario ");
                trataGet (client, s_cmd);
                Serial.println (s_cmd);  
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
      Serial.println("- Web Server Stop");
      vTaskDelay(pdMS_TO_TICKS(20)); 
      
    }  

}


//------------------------------------------------------------------------------------------
//
//    Monta pagina HTML para enviar temperatura e umidade no /
//
//------------------------------------------------------------------------------------------
void envia_jason (EthernetClient client) {
/*  
  char aux [20];
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println();
  //strcpy (aux,vsc.strIP.c_str ());
  //sc.rtox0 (aux);
  //vsc.strIP = String(aux);
  client.print("{");
  client.print("\"IP");
  client.print("\": \"");
  client.print(sc.getIP ());
  client.print("\",");
  
  client.print("\"Temp");
  client.print("\": \"");
  client.print(temperatura);
  client.print("\",");
  
  client.print("\"Hmuid");
  client.print("\": \"");
  client.print(umidade);
  client.println("\"}");
*/
/*  // Para enviar em html
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
   
    client.print("Temperatura: ");
    client.println("<div> ID=TEMP");
    client.print(t);
    client.println("</div>");
    //client.print("Umidade: ");
    client.print(h);
    client.println("<br/>");
    client.println("</html>");  
 */   
}

  
//------------------------------------------------------------------------------------------
//
//    Responde ao anetrar na pagina /ola
//
//------------------------------------------------------------------------------------------
void olaResponse (EthernetClient client) {

client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response
//client.println("Refresh: 5");  // refresh the page automatically every 5 sec
client.println();
client.println("<!DOCTYPE HTML>");
client.println ("<title>Configiracao de IP</title>");
client.println ("<meta name='viewport' content='width=device-width, initial-scale=1'>");
client.println("<html>");
client.println ("</head><body>");


client.println(sc.getMacAddress ()); 
client.println("<br>");
client.println(sc.getIP ()); 

client.println("</body></html> ");

}

//------------------------------------------------------------------------------------------
//
//    Monta pagina HTML de Configuração \config
//
//------------------------------------------------------------------------------------------

 void config_html (EthernetClient client) {

    
  client.println ("<!DOCTYPE HTML><html><head>");
  client.println ("<title>Configiracao de IP</title>");
  client.println ("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println ("</head><body>");
  client.println ("<h1><center>Sistema Sucres</center></h1>");
  
  client.println ("<h2>Configuracao para IP Fixo</h2>");
    client.println ("<form action='/get'>");
    //client.println ("<form action='/post'>");
    //client.println ("Usar IP Fixo </td> <td><input type='checkbox' name='usarIpFixo' value='true'>");
    client.println ("<table>");
      client.println ("<tr><td>Usar IP Fixo </td> <td><input type='checkbox' name='usarIpFixo' ");
      if (sc.getUsarIpFixo ())
        client.println ("checked");
      client.println ("></td></tr>");
      client.println ("<tr><td>IP : </td> <td><input type='text' name='ip' value='");
      client.println (sc.getIP ());
      client.println ("' required></td></tr>");
      client.println ("<tr><td>Mascara : </td> <td><input type='text' name='mascara' value='");
      client.println (sc.getMascara ());
      client.println ("' required></td></tr>");  
      client.println ("<tr><td>GateWay : </td> <td><input type='text' name='gateway' value='");
      client.println (sc.getGateWay ());
      client.println ("' required></td></tr>");
      client.println ("<tr><td>DNS : </td> <td><input type='text' name='dns' value='");
      client.println (sc.getDNserver ());
      client.println ("' required></td></tr>");
  client.println ("</table>"); 
  
  client.println ("<br/><hr/><br/>");
  
  
    
    client.println ("<h2>Configuracao para Mosquitto (sc.</h2>");
    client.println ("<table>");
    //printCfg ();
    client.println ("<tr><td>Enviar dados via sc.</td> <td><input type='checkbox' name='usarMQTT' ");
      if (sc.getMqttUsar ()) {
        Serial.println (" ---------  MQTT Habilitado");
        client.println ("checked");
      }
    client.println ("></td></tr>");

    
  client.println ("<tr><td>Host :  </td> <td><input type='text' name='host' value='");
  client.println (sc.getMqttHost ());
  client.println ("' required><br></td></tr>");
  client.println ("<tr><td>User : </td> <td><input type='text' name='user' value='");
  client.println (sc.getMqttUser ());
  client.println ("' required><br></td></tr>");
  
    client.println ("<tr><td><input type='submit' value='Enviar'></td></tr>");
  client.println ("</table>");  
  client.println ("</form>");
  client.println ("</body></html> ");

 }




//------------------------------------------------------------------------------------------
//
//    Trata GET recebido da configuração
//
//------------------------------------------------------------------------------------------
 void trataGet (EthernetClient client, String recget) {

  String aux;
  int i01 = 0, i02 = 0;
  
  // se vai usar IP fixo ou DHCP 
  if (recget.indexOf ("usarIpFixo=") > 0) {
    aux = recget.substring (16,18);
    //Serial.print ("Flag IP...: ");
    //Serial.println (aux);
    //Serial.println (recget.indexOf ("usarIpFixo="));
    if (aux = "on")
      sc.setUsarIpFixo (true);
    else
      sc.setUsarIpFixo (false);  
  } else
      sc.setUsarIpFixo (false); 
  //
  //  Configuração de Rede
  //
  // IP
  i01 = recget.indexOf ("ip=") + 3;
  i02 = recget.indexOf ("mascara=") - 1;
  if (i01 != 0) {
    sc.setIP (recget.substring (i01,i02));
  }

  // Mascara de rede
  i01 = recget.indexOf ("mascara=") + 8;
  i02 = recget.indexOf ("gateway=") - 1;
  if (i01 != 0) {
    sc.setMascara (recget.substring (i01,i02));
  }

  // Gate Way
  i01 = recget.indexOf ("gateway=") + 8;
  i02 = recget.indexOf ("dns=") - 1;
  if (i01 != 0) {
    sc.setGateWay (recget.substring (i01,i02));
  }

  // DNS
  i01 = recget.indexOf ("dns=") + 4;
  i02 = recget.indexOf ("usarMQTT=");
  if (i02 != 0) 
    i02 = i02 -1;
  else
    i02 = recget.indexOf ("host=") - 1;  
  if (i01 != 0) {
    sc.setDNserver (recget.substring (i01,i02));
  }
  
  

  // se vai usar MQTT
  i01 = recget.indexOf ("usarMQTT=");
  if (i01 > 0) {
    i01 += 9;
    i02 = recget.indexOf ("host=") - 1;
    aux = recget.substring (i01,i02);
    if (aux = "on")
      sc.setMqttUsar (true);
    else
      sc.setMqttUsar (false);  
  } else
      sc.setMqttUsar (false); 


   // host MQTT
  i01 = recget.indexOf ("host=") + 5;
  i02 = recget.indexOf ("user=") - 1;
  sc.setMqttHost (recget.substring (i01,i02));
    
   // User MQTT
  i01 = recget.indexOf ("user=") + 5;
  i02 = recget.indexOf ("HTTP") - 2;
  //i02 = i01 + 30;
  sc.setMqttUser (recget.substring (i01,i02));


  //salvaCfg ();
  getResposnse (client);
  
  Serial.println ("--- Antes de salvar variaveis");
  printCfg ();
  

  Serial.println ("--- depois de salvar  variaveis");
  if (salvaCfg ())
    printCfg ();
  else
    Serial.println ("Erro Salvando arquivo de configuração");
      

  Serial.println ("--- depois de  Ler variaveis");
  if (leCfg ())
    printCfg ();
  else 
    Serial.println ("Erro lendo arquivo de configuração"); 
  
  Serial.println ("Aguarda 1 segundo"); 
  vTaskDelay(pdMS_TO_TICKS(1000));
  Serial.println ("Lista Arquivos"); 
  efs.listDir("/", 0);
  Serial.println ("Aguarda 1 segundo"); 
  vTaskDelay(pdMS_TO_TICKS(1000));
   Serial.println ("Reinicia....."); 
   
  boot (); 
  
 }


//------------------------------------------------------------------------------------------
//
//    Responde ao Browser ao Subimit da pagina de configuração
//
//------------------------------------------------------------------------------------------
void getResposnse (EthernetClient client) {

  char aux [20];

client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response
//client.println("Refresh: 5");  // refresh the page automatically every 5 sec
client.println();
client.println("<!DOCTYPE HTML>");
client.println ("<title>Configiracao de IP</title>");
client.println ("<meta name='viewport' content='width=device-width, initial-scale=1'>");
client.println("<html>");
client.println ("</head><body>");
client.println ("<h1><center>Sistema Playades</center></h1>");
client.println ("<h2>Configuracao Enviada para equipamento (MacAddress): " + sc.getMacAddress () + "</h2>");
client.println ("<br/><hr/><b> Configuracoes de IP </b><br/>");
client.println ("<table>");
if (sc.getUsarIpFixo ()) 
  client.println("<tr><td> Usando IP </td><td> Fixo</tr></td>");
 else   
  client.println("<tr><td> Usando IP via </td><td> DHCP</tr></td>");

client.println("<tr><td> IP : </td><td>" + sc.getIP () + "</tr></td>"); 

client.println("<tr><td><tr><td> Mascara de Rede : </td><td>" + sc.getMascara () + "</tr></td>");

client.println("<tr><td> Gate Way : </td><td>" + sc.getGateWay () + "</tr></td>"); 

client.println("<tr><td> Servidor DNS : </td><td>" + sc.getDNserver () + "</tr></td>"); 
client.println ("</table>");


client.println ("<br/><hr/><b> Configuracoes de Alarme </b><br/>");
client.println ("<table>");
if (sc.getMqttUsar ())
  client.println("<tr><td> Mosquitto (MQTT) </td><td> Habilitado</tr></td>"); 
else
  client.println("<tr><td> Mosquitto (MQTT) </td><td>Desabilitado</tr></td>"); 
client.println("<tr><td> Host Mosquitto :</td><td>" + sc.getMqttHost () + "</tr></td>");   
client.println("<tr><td> User Mosquitto :</td><td>" + sc.getMqttUser () + "</tr></td>");   

client.println("</body></html> ");

} 
