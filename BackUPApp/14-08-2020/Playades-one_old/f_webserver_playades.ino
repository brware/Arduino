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
      //Serial.println("- Web Server Start");
      // ******************************************************enviaDados();

      while (client.connected()) {
        
        if (client.available()) {
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
                envia_jason (client);
               
              }
            if (s_cmd == "/json") { // pedido de dados de temperatura e umidade
                envia_jason (client);
               
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
//    Monta pagina HTML para enviar temperatura e umidade no /
//
//------------------------------------------------------------------------------------------
void envia_jason (EthernetClient client) {
  
  char aux [20];
  String json;

  
 
  json = "HTTP/1.1 200 OK\n";
  json.concat ("Content-Type: application/json\n"); 
  //json.concat ("Refresh: 5\n"); 
  json.concat ("\n"); 
  json.concat ("{\"IP\": \"");
  json.concat (strIP);
  json.concat ("\",\"Temp\": \"");
  json.concat (temperatura);
  json.concat ("\",\"Humid\": \"");
  json.concat (umidade);
  json.concat ("\"}\n");
  client.println(json);
   
  
/*
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.print("{");
  client.print("\"IP");
  client.print("\": \"");
  client.print(eth.getIP ());
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


client.println(eth.getMacAddress ()); 
client.println("<br>");
client.println(eth.getIP ()); 

client.println("</body></html> ");

}

//------------------------------------------------------------------------------------------
//
//    Monta pagina HTML de Configuração \config
//
//------------------------------------------------------------------------------------------

 void config_html (EthernetClient client){

    
  client.println ("<!DOCTYPE HTML><html><head>");
  client.println ("<title>Configiracao de IP</title>");
  client.println ("<meta name='viewport' content='width=device-width, initial-scale=1'>");
  client.println ("</head><body>");
  client.println ("<h1><center>Sistema Playades</center></h1>");
  
  client.println ("<h2>Configuracao para IP Fixo</h2>");
    client.println ("<form action='/get'>");
    //client.println ("<form action='/post'>");
    //client.println ("Usar IP Fixo </td> <td><input type='checkbox' name='usarIpFixo' value='true'>");
    client.println ("<table>");
      client.println ("<tr><td>Usar IP Fixo </td> <td><input type='checkbox' name='usarIpFixo' ");
      if (eth.getUsarIpFixo ())
        client.println ("checked");
      client.println ("></td></tr>");
      client.println ("<tr><td>IP : </td> <td><input type='text' name='ip' value='");
      client.println (eth.getIP ());
      client.println ("' required></td></tr>");
      client.println ("<tr><td>Mascara : </td> <td><input type='text' name='mascara' value='");
      client.println (eth.getMascara ());
      client.println ("' required></td></tr>");  
      client.println ("<tr><td>GateWay : </td> <td><input type='text' name='gateway' value='");
      client.println (eth.getGateWay ());
      client.println ("' required></td></tr>");
      client.println ("<tr><td>DNS : </td> <td><input type='text' name='dns' value='");
      client.println (eth.getDNserver ());
      client.println ("' required></td></tr>");
  client.println ("</table>"); 
  
  client.println ("<br/><hr/><br/>");
  
  client.println ("<h2>Configuracao para Alarmes</h2>");
  client.println ("<table>");
    client.println ("<tr><td>Temperatura Minima: </td> <td><input type='text' name='t_min' value='");
    client.println (vAlm.minT);
    client.println ("' required><br></td></tr>");
    
    client.println ("<tr><td>Temperatura Maxima: </td> <td><input type='text'  name='t_max' value='");
    client.println (vAlm.maxT);
    client.println ("' required><br></td></tr>");
    
    client.println ("<tr><td>Umidade Minima: </td> <td><input type='text' name='u_min' value='");
    client.println (vAlm.minU);
    client.println ("' required><br></td></tr>");
    
    client.println ("<tr><td>Umidade Maxima: </td> <td><input type='text' name='u_max' value='");
    client.println (vAlm.maxU);
    client.println ("' required><br></td></tr>");
  /*
    client.println ("</table>");
    
    client.println ("<br/><hr/><br/>");
    
    client.println ("<h2>Configuracao para Mosquitto (MQTT)</h2>");
    client.println ("<table>");
     //printCfg ();
    client.println ("<tr><td>Enviar dados via MQTT </td> <td><input type='checkbox' name='usarMQTT' ");
      if (mqtt.getMqttUsar ()) {
        Serial.println (" ---------  MQTT Habilitado");
        client.println ("checked");
      }
    client.println ("></td></tr>");

   
  client.println ("<tr><td>Host :  </td> <td><input type='text' name='host' value='");
  client.println (mqtt.getMqttHost ());
  client.println ("' required><br></td></tr>");
  client.println ("<tr><td>User : </td> <td><input type='text' name='user' value='");
  client.println (mqtt.getMqttUser ());
  client.println ("' required><br></td></tr>");
*/     
   // client.println ("<tr><td>Senha :</td> <td><input type='password' name='senha' value='");
  //  client.println (fc.mqttPassWord);
  //  client.println ("' required><br></td></tr>");
   
    //client.println ("<tr><td> <br> </td></tr>");
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
      eth.setUsarIpFixo (true);
    else
      eth.setUsarIpFixo (false);  
  } else
      eth.setUsarIpFixo (false); 
  //
  //  Configuração de Rede
  //
  // IP
  i01 = recget.indexOf ("ip=") + 3;
  i02 = recget.indexOf ("mascara=") - 1;
  if (i01 != 0) {
    eth.setIP (recget.substring (i01,i02));
  }

  // Mascara de rede
  i01 = recget.indexOf ("mascara=") + 8;
  i02 = recget.indexOf ("gateway=") - 1;
  if (i01 != 0) {
    eth.setMascara (recget.substring (i01,i02));
  }

  // Gate Way
  i01 = recget.indexOf ("gateway=") + 8;
  i02 = recget.indexOf ("dns=") - 1;
  if (i01 != 0) {
    eth.setGateWay (recget.substring (i01,i02));
  }

  // DNS
  i01 = recget.indexOf ("dns=") + 4;
  i02 = recget.indexOf ("t_min=") - 1;
  if (i01 != 0) {
    eth.setDNserver (recget.substring (i01,i02));
  }
  
  //
  //  Configuração de Alarme
  //

  // Temperatura Minima
  i01 = recget.indexOf ("t_min=") + 6;
  i02 = recget.indexOf ("t_max=") - 1;
  if (i01 != 0) {
    vAlm.minT = recget.substring (i01,i02);
  }

  // Temperatura Maxima
  i01 = recget.indexOf ("t_max=") + 6;
  i02 = recget.indexOf ("u_min=") - 1;
  if (i01 != 0) {
    vAlm.maxT = recget.substring (i01,i02);
  }

  // Umidade Minima
  i01 = recget.indexOf ("u_min=") + 6;
  i02 = recget.indexOf ("u_max") - 1;
  if (i01 != 0) {
    vAlm.minU = recget.substring (i01,i02);
  }

  // Umidade Maxima
  i01 = recget.indexOf ("u_max=") + 6;
  i02 = recget.indexOf ("usarMQTT=");
  if (i02 > 0)
    i02 = recget.indexOf ("usarMQTT=") - 1;
  else
    i02 = recget.indexOf ("host=") - 1;  
  if (i01 != 0) {
    vAlm.maxU = recget.substring (i01,i02);
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
  getResposnse (client);
  
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
client.println ("<h2>Configuracao Enviada para equipamento (MacAddress): " + eth.getMacAddress () + "</h2>");
client.println ("<br/><hr/><b> Configuracoes de IP </b><br/>");
client.println ("<table>");
if (eth.getUsarIpFixo ()) 
  client.println("<tr><td> Usando IP </td><td> Fixo</tr></td>");
 else   
  client.println("<tr><td> Usando IP via </td><td> DHCP</tr></td>");

client.println("<tr><td> IP : </td><td>" + eth.getIP () + "</tr></td>"); 

client.println("<tr><td><tr><td> Mascara de Rede : </td><td>" + eth.getMascara () + "</tr></td>");

client.println("<tr><td> Gate Way : </td><td>" + eth.getGateWay () + "</tr></td>"); 

client.println("<tr><td> Servidor DNS : </td><td>" + eth.getDNserver () + "</tr></td>"); 
client.println ("</table>");

client.println ("<br/><hr/><b> Configuracoes de Alarme </b><br/>");
client.println ("<table>");
client.println("<tr><td> Temperatura Minima : </td><td>" +  vAlm.minT + "</tr></td>");
client.println("<tr><td> Temperatura Maxima : </td><td>" +  vAlm.maxT + "</tr></td>");            
client.println("<tr><td> Umidade Minima : </td><td>" +  vAlm.minU + "</tr></td>");      
client.println("<tr><td> Umidade Maxima : </td><td>" +  vAlm.maxU + "</tr></td>");
client.println ("</table>");

/*
client.println ("<br/><hr/><b> Configuracoes de Alarme </b><br/>");
client.println ("<table>");
if (mqtt.getMqttUsar ())
  client.println("<tr><td> Mosquitto (MQTT) </td><td> Habilitado</tr></td>"); 
else
  client.println("<tr><td> Mosquitto (MQTT) </td><td>Desabilitado</tr></td>"); 
client.println("<tr><td> Host Mosquitto :</td><td>" + mqtt.getMqttHost () + "</tr></td>");   
client.println("<tr><td> User Mosquitto :</td><td>" + mqtt.getMqttUser () + "</tr></td>");   
*/
client.println("</body></html> ");

} 
