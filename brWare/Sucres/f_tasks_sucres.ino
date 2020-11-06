//------------------------------------------------------------------------------------------
//
// ------------    Crias as tasks
//
//------------------------------------------------------------------------------------------

void criaTasks () {

  // Core 0



  // Task que monitora p Botão de BOOT
  xTaskCreatePinnedToCore(bzBotao, "bzBotao", 4098, NULL, 4,  NULL, taskCoreZero);
  vTaskDelay(pdMS_TO_TICKS(300)); // tempo para task iniciar

 // 
  // Core 1
  //
   
   // Task do WebServer (SEMPRE ANTES DO ethLOOP)
  xTaskCreatePinnedToCore(webServerLoop, "webServerLoop", 20480, NULL, 2, &tk_webServerLoop, taskCoreOne);
  vTaskDelay(pdMS_TO_TICKS(500)); // tempo para task iniciar 

}





//------------------------------------------------------------------------------------------
//
//  --------    ETH WEB server Loop
//
//------------------------------------------------------------------------------------------

void webServerLoop (void* parameter) {


Serial.println("- Task WebServerLoop");

  while (true) {
    if (sc.getStatusEth ()) {
      trataHtml ();  

      }
       
      vTaskDelay(pdMS_TO_TICKS(100));
    } 

  

}




//------------------------------------------------------------------------------------------
//
// -----------------  task Botão de Configuração
//
//------------------------------------------------------------------------------------------

void bzBotao (void* parameter) {
  
int cnt1 = 0, cnt2; 
 
   
Serial.println ("- Monitora Botão de CFG");  
vTaskDelay(pdMS_TO_TICKS(10000)); // tempo para task inicia
while (true)  { 
   
   vTaskDelay(pdMS_TO_TICKS(100)); // tempo para task inicia        
   
   while (digitalRead (BTN) == HIGH) {
     vTaskDelay(pdMS_TO_TICKS(100));
     if (cnt1 == 10) {
       digitalWrite (BUZZER,HIGH);
      delay (100); 
      digitalWrite (BUZZER,LOW);
      cnt2++;
     }
        
    if (cnt1 == 20) {
      digitalWrite (BUZZER,HIGH);
      delay (100);  
      digitalWrite (BUZZER,LOW); 
      cnt2++;
     }
    if (cnt1 == 30) {
      digitalWrite (BUZZER,HIGH);
      delay (100);  
      digitalWrite (BUZZER,LOW); 
      cnt2++;
     } 
    if (cnt1 == 40) {
      digitalWrite (BUZZER,HIGH);
      delay (100);  
      digitalWrite (BUZZER,LOW); 
      cnt2++;
     }  
   cnt1++;  
   if (cnt1 > 40)
    cnt1  = 0;
   }
   // ve quantos segundos apertou
   if (cnt2 == 1) {
    Serial.println (cnt2); 
    getCfgBD ();
    verificaConfBD ();
   }
   if (cnt2 == 2) {
     Serial.println (cnt2); 
     efs.formatFS(); // para testes
     efs.listDir("/", 0);
     sc.actBeep (500, 3);
     boot ();
     
   }
   if (cnt2 == 3) {
    Serial.println (cnt2); 
    sc.sendTopicoConfigPub (sc.getMacAddress () + ";REDE");
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    salvaCfg (); // para testes
    leCfg ();


   }
   if (cnt2 == 4) {
    Serial.println (cnt2);
    sc.sendTopicoConfigPub (sc.getMacAddress () + ";MQTT");
    vTaskDelay(pdMS_TO_TICKS(5000));
    printCfg ();
    salvaCfg (); // para testes
    leCfg ();
   }

   if (cnt2 == 5) {
    Serial.println (cnt2);
    sc.sendTopicoConfigPub (sc.getMacAddress () + ";RELE");
    vTaskDelay(pdMS_TO_TICKS(5000));
    printCfg ();
    //salvaCfg (); // para testes
    salvaCfg ();
    leCfg ();
    
   }

   if (cnt2 == 6) {
    
    
   }

   cnt2 = 0;
   cnt1=0;   
   
  }



}
