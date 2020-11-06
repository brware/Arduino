//------------------------------------------------------------------------------------------
//
// ------------    Crias as tasks
//
//------------------------------------------------------------------------------------------

void criaTasks () {

  // Core 0

  // Leitura do Sensor DHT
  xTaskCreatePinnedToCore(leDHT, "leDHT", 8192, NULL, 4,  NULL, taskCoreZero);
  vTaskDelay(pdMS_TO_TICKS(300)); // tempo para task iniciar

   // Task Buzzer Alarme
  xTaskCreatePinnedToCore(bzAlarme, "bzAlarme", 1024, NULL, 4,  NULL, taskCoreZero);
  vTaskDelay(pdMS_TO_TICKS(300)); // tempo para task iniciar

  // Task que monitora p Botão de BOOT
  xTaskCreatePinnedToCore(bzBotao, "bzBotao", 4096, NULL, 4,  NULL, taskCoreZero);
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
//    Le temperatura e umidade
//
//------------------------------------------------------------------------------------------

void leDHT(void* parameters) {
  String msg;
  static uint32_t cntComunica = 0;
  float t_old, h_old;
    
  
  Serial.println("- Task de Leitura do DHT");
  

  while (true) { 
    
    vTaskDelay(pdMS_TO_TICKS(intervaloLeitura));
    temperatura = dht.readTemperature();
    umidade = dht.readHumidity();

    cntComunica++;
    strT = String (temperatura,2);
    strU = String (umidade, 2);
    
    if ((strT == "nan") || (strU == "nan")) {
      //Serial.println ("*** ERRO DE LEITURA DHT22 ***");
      temperatura = t_old;
      umidade = h_old;
      strT = String (temperatura,2);
      strU = String (umidade, 2);
    } else {
      msg = eth.getMacAddress () + ";" + strT + ";" +  strU; 
      //mqtt.sendTopicoPub (msg);
      testeAlarmeTemperatura (temperatura, umidade);
      //Serial.println (msg);
      t_old = temperatura;
      h_old = umidade;
    }
    
    
    
  }

}


//------------------------------------------------------------------------------------------
//
//  --------    ETH WEB server Loop
//
//------------------------------------------------------------------------------------------


void webServerLoop (void* parameter) {


Serial.println("- Task WebServerLoop");

  while (true) {
    if (eth.getStatusEth ()) {
      trataHtml ();  

      }
       
      vTaskDelay(pdMS_TO_TICKS(100));
    } 

  

}


//------------------------------------------------------------------------------------------
//
//            task para alarme 
//
//------------------------------------------------------------------------------------------

void bzAlarme (void* parameter){
   
Serial.println ("- Buzzer de alarme");  
vTaskDelay(pdMS_TO_TICKS(5000)); // tempo para task inicia
while (true)
  { 

   vTaskDelay(pdMS_TO_TICKS(100)); // tempo para task inicia  
   
    
   if ((stuAlarmeTemperatura) || (stuAlarmeUmidade)) {
      eth.actBeep (300, 2); // beep
      eth.actRele (1,600); // aciona rele de siriene
      eth.setRele (2, HIGH);
      vTaskDelay(pdMS_TO_TICKS(2000)); 
      
   } else {
      eth.setRele (1, LOW);
      eth.setRele (2, LOW);
    }
   
    
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
      
   
   while (digitalRead (BTN) == LOW) {
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
   }
   if (cnt2 == 2) {
     Serial.println (cnt2); 
     efs.formatFS(); // para testes
     efs.listDir("/", 0);
     eth.actBeep (500, 3);
     boot ();
     
   }
   if (cnt2 == 3) {
    Serial.println (cnt2); 
    //leCfg ();
    //rintCfg ();
     
   }
   if (cnt2 == 4) {
    Serial.println (cnt2);
    //salvaCfg (); // para testes
    
   }

   cnt2 = 0;
   cnt1=0;   
   
  }



}
