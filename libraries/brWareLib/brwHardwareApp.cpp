#include "brwHardwareApp.h"


//------------------------------------------------------------------------------------------
//
// ------    Cosntrutor da Classe hardware
//
//------------------------------------------------------------------------------------------

brwHardwareApp::brwHardwareApp (brwHardwareCfg* HardwareCfg) {

  hw_cfg = HardwareCfg;

}


//------------------------------------------------------------------------------------------
//
// ------    Inicia funcoes do construtor
//
//------------------------------------------------------------------------------------------

void brwHardwareApp::iniHardware () {

  
    
  xTaskCreate(&brwHardwareApp::staticBeep, "Beep", 2048, (void*) this, 4, NULL);
  xTaskCreate(&brwHardwareApp::statictkBotao, "tkBotao", 2048, (void*) this, 4, NULL);

    

}

/*
void brwHardwareApp::iniDisplay (brwDisplay1306 *display) { 

  dpy = display;
  dpy -> initDisplay ();
  dpy -> clear ();
  dpy -> setTextSize(2);
  dpy -> setCursor(0, 0);
  //escreveLinha ("brWare V2");
  dpy -> escreveLinha ("brWare V2");

}
*/







//*****************************************************************************
//
//  Function to call Beep
//
//*****************************************************************************

void brwHardwareApp::actBeep (int msTempo, int xVezes) {
  
  
  //xSemaphoreTake(bzMutex,portMAX_DELAY);
    hw_cfg -> bzTempo = msTempo;
    hw_cfg -> bzVezes = xVezes;
    hw_cfg -> bzAciona = true;

    
  //xSemaphoreGive(bzMutex);
  
  
  
}


//*****************************************************************************
//
//  Beep task
//
//*****************************************************************************

void brwHardwareApp::Beep () {
    int i = 0, tempo;


  vTaskDelay(pdMS_TO_TICKS(100)); 
  while (true) {

    //esp_task_wdt_reset();   
    vTaskDelay(pdMS_TO_TICKS(10)); 
    if (hw_cfg -> bzAciona) { 
      while (i < hw_cfg -> bzVezes) {
        digitalWrite (hw_cfg -> BUZZER,HIGH);
        vTaskDelay(pdMS_TO_TICKS(hw_cfg -> bzTempo/2)); 
        digitalWrite (hw_cfg -> BUZZER,LOW);
        vTaskDelay(pdMS_TO_TICKS(hw_cfg -> bzTempo/2)); 
        i++; 
      }
      hw_cfg -> bzAciona = 0;
      i = 0;
    }
  }
  
}

void brwHardwareApp::staticBeep (void *pvParameter) {

    brwHardwareApp* bep = reinterpret_cast<brwHardwareApp*>(pvParameter); //obtain the instance pointer
    bep->Beep(); //dispatch to the member function, now that we have an instance pointer

}


//*****************************************************************************
//
//  Function to activate Reles
//
//*****************************************************************************

bool brwHardwareApp::actRele (int numRL,int msTempo) {

  

  if ((numRL < 0) || (numRL > hw_cfg -> MAX_RELE))
    return (false);
    
  
  //tempoRele [(numRL-1)] = msTempo;
  

  switch (numRL) { 
    case 1 :
      xTaskCreate(&brwHardwareApp::staticRele01, "Rele01", 1024, (void*) this, 4, NULL);
      break;

    case 2 :
      xTaskCreate(&brwHardwareApp::staticRele02, "Rele02", 1024, (void*) this, 4, NULL);
      break;

    case 3 :
      xTaskCreate(&brwHardwareApp::staticRele03, "Rele03", 1024, (void*) this, 6, NULL);
      break;

    case 4 :
      xTaskCreate(&brwHardwareApp::staticRele04, "Rele04", 1024, (void*) this, 4, NULL);
      break;  
#if (HW_VERSAO == 2)
    case 5 :
      xTaskCreate(&brwHardwareApp::staticRele05, "Rele05", 1024, (void*) this, 4, NULL);
      break;  

    case 6 :
      xTaskCreate(&brwHardwareApp::staticRele06, "Rele06", 1024, (void*) this, 4, NULL);
      break;   

    case 7 :
      xTaskCreate(&brwHardwareApp::staticRele07, "Rele07", 1024, (void*) this, 4, NULL);
      break;     
#endif      
  }  
  
   
  return (true);

}


//*****************************************************************************
//
//  Rele tasks
//
//*****************************************************************************

void brwHardwareApp::Rele01 (){
  
  digitalWrite (hw_cfg -> RL01,HIGH);
  hw_cfg -> statusRele [0] =  HIGH;
  vTaskDelay(pdMS_TO_TICKS(hw_cfg -> tempoRele[0] * 1000)); 
  digitalWrite (hw_cfg -> RL01,LOW);
  hw_cfg -> statusRele [0] =  LOW;

  vTaskDelete(NULL); // delete task

}

void brwHardwareApp::staticRele01 (void *pvParameter) {

    brwHardwareApp* tk = reinterpret_cast<brwHardwareApp*>(pvParameter); //obtain the instance pointer
    tk->Rele01(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardwareApp::Rele02 (){
  
  digitalWrite (hw_cfg -> RL02,HIGH);
  hw_cfg -> statusRele [1] =  HIGH;
  hw_cfg -> setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(hw_cfg -> tempoRele[1] * 1000)); 
  digitalWrite (hw_cfg -> RL02,LOW);
  hw_cfg -> statusRele [1] =  LOW;
  hw_cfg -> setStatusReles (false);  
  
  vTaskDelete(NULL); // delete task

}

void brwHardwareApp::staticRele02 (void *pvParameter) {

    brwHardwareApp* tk = reinterpret_cast<brwHardwareApp*>(pvParameter); //obtain the instance pointer
    tk->Rele02(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardwareApp::Rele03 (){
  
  digitalWrite (hw_cfg -> RL03,HIGH);
  hw_cfg -> statusRele [2] =  HIGH;
  hw_cfg -> setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(hw_cfg -> tempoRele[2] * 1000)); 
  digitalWrite (hw_cfg -> RL03,LOW);
  hw_cfg -> statusRele [2] =  LOW;
  hw_cfg -> setStatusReles (false);  
   
  vTaskDelete(NULL); // delete task

}

void brwHardwareApp::staticRele03 (void *pvParameter) {

    brwHardwareApp* tk = reinterpret_cast<brwHardwareApp*>(pvParameter); //obtain the instance pointer
    tk->Rele03(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardwareApp::Rele04 (){
  
  digitalWrite (hw_cfg -> RL04,HIGH);
  hw_cfg -> statusRele [3] =  HIGH;
  hw_cfg -> setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(hw_cfg -> tempoRele[3] * 1000)); 
  digitalWrite (hw_cfg -> RL04,LOW);
  hw_cfg -> statusRele [3] =  LOW;
  hw_cfg -> setStatusReles (false);  
 
  vTaskDelete(NULL); // delete task

}

void brwHardwareApp::staticRele04 (void *pvParameter) {

    brwHardwareApp* tk = reinterpret_cast<brwHardwareApp*>(pvParameter); //obtain the instance pointer
    tk->Rele04(); //dispatch to the member function, now that we have an instance pointer

}


void brwHardwareApp::Rele05 (){

#if (HW_VERSAO == 2)

  digitalWrite (hw_cfg -> RL05,HIGH);
  statusRele [4] =  HIGH;
  hw_cfg -> setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(hw_cfg -> tempoRele[4] * 1000)); 
  digitalWrite (hw_cfg -> RL05,LOW);
  hw_cfg -> statusRele [4] =  LOW;
  hw_cfg -> setStatusReles (false);  
 
  vTaskDelete(NULL); // delete task

#endif

}


void brwHardwareApp::staticRele05 (void *pvParameter) {

    brwHardwareApp* tk = reinterpret_cast<brwHardwareApp*>(pvParameter); //obtain the instance pointer
    tk->Rele05(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardwareApp::Rele06 (){



  digitalWrite (hw_cfg -> RL06,HIGH);
  hw_cfg -> statusRele [5] =  HIGH;
  hw_cfg -> setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(hw_cfg -> tempoRele[5] * 1000)); 
  digitalWrite (hw_cfg -> RL06,LOW);
  hw_cfg -> statusRele [5] =  LOW;
  hw_cfg -> setStatusReles (false);  
 
  vTaskDelete(NULL); // delete task


}

void brwHardwareApp::staticRele06 (void *pvParameter) {

    brwHardwareApp* tk = reinterpret_cast<brwHardwareApp*>(pvParameter); //obtain the instance pointer
    tk->Rele06(); //dispatch to the member function, now that we have an instance pointer

}


void brwHardwareApp::Rele07 (){


  
  digitalWrite (hw_cfg -> RL07,HIGH);
  hw_cfg -> statusRele [6] =  HIGH;
  hw_cfg -> setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(hw_cfg -> tempoRele[6] * 1000)); 
  digitalWrite (hw_cfg -> RL07,LOW);
  hw_cfg -> statusRele [6] =  LOW;
  hw_cfg -> setStatusReles (false);  
 
  vTaskDelete(NULL); // delete task



}

void brwHardwareApp::staticRele07 (void *pvParameter) {

    brwHardwareApp* tk = reinterpret_cast<brwHardwareApp*>(pvParameter); //obtain the instance pointer
    tk->Rele07(); //dispatch to the member function, now that we have an instance pointer

}

//*****************************************************************************
//
//  Set e get tempo dos reles de 1 a 8
//
//*****************************************************************************

bool brwHardwareApp::setTempoRele(int num, int tempo) {

    if (tempo < 1)
        tempo = 1;
    if ((num < 1) || (num > hw_cfg -> MAX_RELE))
        return (false);
    hw_cfg -> tempoRele [num - 1] = tempo;
    return (true);
}

int brwHardwareApp::getTempoRele (int num) {

    if ((num < 1) || (num > hw_cfg -> MAX_RELE))
        return (0);
    return (hw_cfg -> tempoRele [num - 1]);
}





//*****************************************************************************
//
//  Set Estado do Rela para LOW ou HIGH
//
//*****************************************************************************


bool brwHardwareApp::setRele (int num, int estado) {

  

  if ((num < 0) || (num > hw_cfg -> MAX_RELE))
    return (false);

  switch (num) { 
    case 1 :
        digitalWrite (hw_cfg -> RL01,estado);
        hw_cfg -> statusRele [0] =  estado;
      break;

    case 2 :
      digitalWrite (hw_cfg -> RL02,estado);
      hw_cfg -> statusRele [1] =  estado;
      break;

    case 3 :
      digitalWrite (hw_cfg -> RL03,estado);
      hw_cfg -> statusRele [2] =  estado;
      break;

    case 4 :
      digitalWrite (hw_cfg -> RL04,estado);
      hw_cfg -> statusRele [3] =  estado;
      break; 

    case 5 :
      digitalWrite (hw_cfg -> RL05,estado);
      hw_cfg -> statusRele [4] =  estado;
      break;        

    case 6 :
      digitalWrite (hw_cfg -> RL06,estado);
      hw_cfg -> statusRele [5] =  estado;
      break;  

    case 7 :
      digitalWrite (hw_cfg -> RL07,estado);
      hw_cfg -> statusRele [6] =  estado;
      break;  

  }  
  
  hw_cfg -> setStatusReles (false);    
  return (true);

}


//*****************************************************************************
//
//  Set Estado do Rela para LOW ou HIGH
//
//*****************************************************************************


bool brwHardwareApp::inverteRele (int num) {

  

  if ((num < 0) || (num > hw_cfg -> MAX_RELE))
    return (false);

  switch (num) { 
    case 1 :
        digitalWrite (hw_cfg -> RL01,!digitalRead (hw_cfg -> RL01));
        hw_cfg -> statusRele [0] =  digitalRead (hw_cfg -> RL01);
      break;

    case 2 :
      digitalWrite (hw_cfg -> RL02,!digitalRead (hw_cfg -> RL02));
      hw_cfg -> statusRele [1] =  digitalRead (hw_cfg -> RL02);
      break;

    case 3 :
      digitalWrite (hw_cfg -> RL03,!digitalRead (hw_cfg -> RL03));
      hw_cfg -> statusRele [2] =  digitalRead (hw_cfg -> RL03);
      break;

    case 4 :
      digitalWrite (hw_cfg -> RL04,!digitalRead (hw_cfg -> RL04));
      hw_cfg -> statusRele [3] =  digitalRead (hw_cfg -> RL04);
      break;    
#if (HW_VERSAO == 2)
    case 5 :
      digitalWrite (hw_cfg -> RL05,!digitalRead (hw_cfg -> RL05));
      hw_cfg -> statusRele [4] =  digitalRead (hw_cfg -> RL05);
      break;       

    case 6 :
      digitalWrite (hw_cfg -> RL06,!digitalRead (RL06));
      hw_cfg -> statusRele [5] =  digitalRead (hw_cfg -> RL06);
      break;   

    case 7 :
      digitalWrite (hw_cfg -> RL07,!digitalRead (hw_cfg -> RL07));
      hw_cfg -> statusRele [6] =  digitalRead (hw_cfg -> RL07);
      break;   
#endif
  }  
  
  hw_cfg -> setStatusReles (false);   

  

  return (true);

}




//------------------------------------------------------------------------------------------
//
// -----------------  task Botão de Configuração
//
//------------------------------------------------------------------------------------------

void brwHardwareApp::tkBotao () {
  
int cnt1 = 0, cnt2; 
 
   
//Serial.println ("- Monitora Botão de CFG");  
vTaskDelay(pdMS_TO_TICKS(10000)); // tempo para task inicia
while (true)  { 
   
   vTaskDelay(pdMS_TO_TICKS(100)); // tempo para task inicia  
      
   
   while (digitalRead (hw_cfg -> BTN) == LOW) {
   //while (digitalRead (BTN) == HIGH) {
     vTaskDelay(pdMS_TO_TICKS(100));
     if (cnt1 == 10) {
       digitalWrite (hw_cfg -> BUZZER,HIGH);
      delay (100); 
      digitalWrite (hw_cfg -> BUZZER,LOW);
      cnt2++;
     }
        
    if (cnt1 == 20) {
      digitalWrite (hw_cfg -> BUZZER,HIGH);
      delay (100);  
      digitalWrite (hw_cfg -> BUZZER,LOW); 
      cnt2++;
     }
    if (cnt1 == 30) {
      digitalWrite (hw_cfg -> BUZZER,HIGH);
      delay (100);  
      digitalWrite (hw_cfg -> BUZZER,LOW); 
      cnt2++;
     } 
    if (cnt1 == 40) {
      digitalWrite (hw_cfg -> BUZZER,HIGH);
      delay (100);  
      digitalWrite (hw_cfg -> BUZZER,LOW); 
      cnt2++;
     }  
   cnt1++;  
   if (cnt1 > 40)
    cnt1  = 0;
   }
   // ve quantos segundos apertou
   if (cnt2 == 1) {
    Serial.println (cnt2); 
    // mostra config de rede no Display
   }
   if (cnt2 == 2) {
     Serial.println (cnt2); 
     // mostra config do MQTT
     
   }
   if (cnt2 == 3) {
    Serial.println (cnt2); 
    hw_cfg -> printHwCfg (); // mostra configiração da aplicação
     
   }
   if (cnt2 == 4) {
    Serial.println (cnt2);
    //Reseta configuração
    hw_cfg -> spifss -> formatFS(); // para testes
    //efs.listDir("/", 0);
    actBeep (500, 3);
    ESP.restart ();
    //salvaCfg (); // para testes
    
   }

   cnt2 = 0;
   cnt1 = 0;   
   
  }



}


void brwHardwareApp::statictkBotao (void *pvParameter) {

    brwHardwareApp* btn = reinterpret_cast<brwHardwareApp*>(pvParameter); //obtain the instance pointer
    btn->tkBotao(); //dispatch to the member function, now that we have an instance pointer

}



//------------------------------------------------------------------------------------------
//
// -----------------  FUNCOES SOMETE PARA SUCRES DE VERIFICAR PORTOES
//
//------------------------------------------------------------------------------------------




bool brwHardwareApp::leEntrada (int entrada) {
  int vezes, cnt;

  cnt = 0; 
  for (vezes = 0; vezes < 100; vezes++) {
    if (digitalRead (entrada) == HIGH)
      cnt++;
    vTaskDelay(pdMS_TO_TICKS(10));   
  }
 
 if (cnt > 80)
  return (true);
 else
  return (false);
     
  
}


