
#include "brwHardwareV2.h"

//------------------------------------------------------------------------------------------
//
// ------    Cosntrutor da Classe hardware
//
//------------------------------------------------------------------------------------------

brwHardware::brwHardware () {



}


//------------------------------------------------------------------------------------------
//
// ------    Inicia funcoes do construtor
//
//------------------------------------------------------------------------------------------

void brwHardware::iniHardware () {

    
    setUpPinos ();
    
    xTaskCreate(&brwHardware::staticBeep, "Beep", 2048, (void*) this, 4, NULL);

}



//------------------------------------------------------------------------------------------
//
// -----------   SetUp de pinos
//
//------------------------------------------------------------------------------------------

void brwHardware::setUpPinos () {

  Serial.println ("- Setup Pinos ESP32"); 
  pinMode(LED_IN, OUTPUT); // pino do LED do NODE MCU
  pinMode(BUZZER, OUTPUT); // pino do BUZZER
  digitalWrite(BUZZER, LOW); 
  pinMode (BTN, INPUT_PULLUP);
  pinMode (BT_BOOT,INPUT_PULLUP);
  
  /*
  pinMode(BT_BOOT, INPUT_PULLUP); // pino do boot
  pinMode(LED_IN, OUTPUT); // pino do LED do NODE MCU
  pinMode (BTN, OUTPUT); // Pino de reset por software
  digitalWrite(BTN, LOW);
  pinMode(BUZZER, OUTPUT); // pino do BUZZER
  digitalWrite(BUZZER, LOW);
  */
  
    
  pinMode(S02, INPUT_PULLUP); // Status Portão 01
  pinMode(S03, INPUT_PULLUP); // Status Portão 02
  pinMode(S04, INPUT_PULLUP); // Status Portão 03
  pinMode(S05, INPUT_PULLUP); // Status Portão 04

  pinMode(S01, INPUT_PULLUP); // Estado do Alarme
  
  
  pinMode (RL01, OUTPUT); // Alarme/Bollard
  pinMode (RL02, OUTPUT); // Desabilita
  pinMode (RL03, OUTPUT); // Gerador de Neblina
  pinMode (RL04, OUTPUT); // Portão 01
  pinMode (RL05, OUTPUT); // Portão 02
  pinMode (RL06, OUTPUT); // Portão 03
  pinMode (RL07, OUTPUT); // Portão 04

  pinMode(SPI_RST , OUTPUT); // RESET SPI

}



//*****************************************************************************
//
//  Function to call Beep
//
//*****************************************************************************

void brwHardware::actBeep (int msTempo, int xVezes) {
  
  
  //xSemaphoreTake(bzMutex,portMAX_DELAY);
    bzTempo = msTempo;
    bzVezes = xVezes;
    bzAciona = true;

    
  //xSemaphoreGive(bzMutex);
  
  
  
}


//*****************************************************************************
//
//  Beep task
//
//*****************************************************************************

void brwHardware::Beep () {
  
  int i = 0, tempo;
  Serial.println ("- Task Beep Iniciada"); 
  vTaskDelay(pdMS_TO_TICKS(100)); 
  while (true) {
    
    //esp_task_wdt_reset();   
    vTaskDelay(pdMS_TO_TICKS(10)); 
    if (bzAciona) { 
      while (i < bzVezes) {
        digitalWrite (BUZZER,HIGH);
        vTaskDelay(pdMS_TO_TICKS(bzTempo/2)); 
        digitalWrite (BUZZER,LOW);
        vTaskDelay(pdMS_TO_TICKS(bzTempo/2)); 
        i++; 
      }
      bzAciona = 0;
      i = 0;
    }
  }
  
}

void brwHardware::staticBeep (void *pvParameter) {

    brwHardware* bep = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    bep->Beep(); //dispatch to the member function, now that we have an instance pointer

}


//*****************************************************************************
//
//  Function to activate Reles
//
//*****************************************************************************

bool brwHardware::actRele (int numRL,int msTempo) {

  

  if ((numRL < 0) || (numRL > 7))
    return (false);
    
  
  //tempoRele [(numRL-1)] = msTempo;
  

  switch (numRL) { 
    case 1 :
      xTaskCreate(&brwHardware::staticRele01, "Rele01", 1024, (void*) this, 4, NULL);
      break;

    case 2 :
      xTaskCreate(&brwHardware::staticRele02, "Rele02", 1024, (void*) this, 4, NULL);
      break;

    case 3 :
      xTaskCreate(&brwHardware::staticRele03, "Rele03", 1024, (void*) this, 6, NULL);
      break;

    case 4 :
      xTaskCreate(&brwHardware::staticRele04, "Rele04", 1024, (void*) this, 4, NULL);
      break;  

    case 5 :
      xTaskCreate(&brwHardware::staticRele05, "Rele05", 1024, (void*) this, 4, NULL);
      break;  

    case 6 :
      xTaskCreate(&brwHardware::staticRele06, "Rele06", 1024, (void*) this, 4, NULL);
      break;   

    case 7 :
      xTaskCreate(&brwHardware::staticRele07, "Rele07", 1024, (void*) this, 4, NULL);
      break;     
  }  
      
  return (true);

}


//*****************************************************************************
//
//  Rele tasks
//
//*****************************************************************************

void brwHardware::Rele01 (){
  
  digitalWrite (RL01,HIGH);
  vTaskDelay(pdMS_TO_TICKS(tempoRele[0] * 1000)); 
  digitalWrite (RL01,LOW);
  
  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele01 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele01(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardware::Rele02 (){
  
  digitalWrite (RL02,HIGH);
  vTaskDelay(pdMS_TO_TICKS(tempoRele[1] * 1000)); 
  digitalWrite (RL02,LOW);
  
  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele02 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele02(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardware::Rele03 (){
  
  digitalWrite (RL03,HIGH);
  vTaskDelay(pdMS_TO_TICKS(tempoRele[2] * 1000)); 
  digitalWrite (RL03,LOW);
   
  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele03 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele03(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardware::Rele04 (){
  
  digitalWrite (RL04,HIGH);
  vTaskDelay(pdMS_TO_TICKS(tempoRele[3] * 1000)); 
  digitalWrite (RL04,LOW);
 
  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele04 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele04(); //dispatch to the member function, now that we have an instance pointer

}


void brwHardware::Rele05 (){
  
  digitalWrite (RL05,HIGH);
  vTaskDelay(pdMS_TO_TICKS(tempoRele[4] * 1000)); 
  digitalWrite (RL05,LOW);
 
  vTaskDelete(NULL); // delete task

}


void brwHardware::staticRele05 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele05(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardware::Rele06 (){
  
  digitalWrite (RL06,HIGH);
  vTaskDelay(pdMS_TO_TICKS(tempoRele[5] * 1000)); 
  digitalWrite (RL06,LOW);
 
  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele06 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele06(); //dispatch to the member function, now that we have an instance pointer

}


void brwHardware::Rele07 (){
  
  digitalWrite (RL07,HIGH);
  vTaskDelay(pdMS_TO_TICKS(tempoRele[6] * 1000)); 
  digitalWrite (RL07,LOW);
 
  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele07 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele07(); //dispatch to the member function, now that we have an instance pointer

}

//*****************************************************************************
//
//  Set e get tempo dos reles de 1 a 8
//
//*****************************************************************************

bool brwHardware::setTempoRele(int num, int tempo) {

    if (tempo < 1)
        tempo = 1;
    if ((num < 1) || (num > 7))
        return (false);
    tempoRele [num - 1] = tempo;
    return (true);
}

int brwHardware::getTempoRele (int num) {

    if ((num < 1) || (num > 7))
        return (0);
    return (tempoRele [num - 1]);
}





//*****************************************************************************
//
//  Set Estado do Rela para LOW ou HIGH
//
//*****************************************************************************


bool brwHardware::setRele (int num, int estado) {

  

  if ((num < 0) || (num > 7))
    return (false);

  switch (num) { 
    case 1 :
        digitalWrite (RL01,estado);
      break;

    case 2 :
      digitalWrite (RL02,estado);
      break;

    case 3 :
      digitalWrite (RL03,estado);
      break;

    case 4 :
      digitalWrite (RL04,estado);
      break; 

    case 5 :
      digitalWrite (RL05,estado);
      break;        

    case 6 :
      digitalWrite (RL06,estado);
      break;  

    case 7 :
      digitalWrite (RL07,estado);
      break;  

  }  
      
  return (true);

}


//*****************************************************************************
//
//  Set Estado do Rela para LOW ou HIGH
//
//*****************************************************************************


bool brwHardware::inverteRele (int num) {

  

  if ((num < 0) || (num > 4))
    return (false);

  switch (num) { 
    case 1 :
        digitalWrite (RL01,!digitalRead (RL01));
      break;

    case 2 :
      digitalWrite (RL02,!digitalRead (RL02));
      break;

    case 3 :
      digitalWrite (RL03,!digitalRead (RL03));
      break;

    case 4 :
      digitalWrite (RL04,!digitalRead (RL04));
      break;    

    case 5 :
      digitalWrite (RL05,!digitalRead (RL05));
      break;       

    case 6 :
      digitalWrite (RL06,!digitalRead (RL06));
      break;   

    case 7 :
      digitalWrite (RL07,!digitalRead (RL07));
      break;   

  }  
      
  return (true);

}
