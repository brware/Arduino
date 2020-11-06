#include "brwHardware.h"


//------------------------------------------------------------------------------------------
//
// ------    Cosntrutor da Classe hardware
//
//------------------------------------------------------------------------------------------

brwHardware::brwHardware (const char* App, const char* Versao, const  char* Hard, brwSpiffs* Spiffs) {

  spifss = Spiffs;

  strncpy (hwApp, App, 20);
  strncpy (infoVersao, Versao, 10);
  strncpy (infoHw, Hard, 20);

  setUpPinos ();

  getStatusReles (true); 

  


}


//------------------------------------------------------------------------------------------
//
// ------    Inicia funcoes do construtor
//
//------------------------------------------------------------------------------------------

void brwHardware::iniHardware () {

  
  //actBeep (500, 2);
  
  Serial.print (hwApp);
  if (!strcmp (hwApp,"playade01"))
     Serial.println (" : Iniciando Hardware Vesão 1");
  if (!strcmp (hwApp,"sucres"))
    Serial.println (" : Iniciando Hardware Vesão 2"); 
  //else
    

    /*
    
    if (spifss -> isOpen == false)
      spifss -> beginFS();
    */ 
    getHwConfig ();
    Serial.println ("Configuração da Classa Hardware: ");
   
    
    xTaskCreate(&brwHardware::staticBeep, "Beep", 2048, (void*) this, 4, NULL);
    xTaskCreate(&brwHardware::statictkBotao, "tkBotao", 2048, (void*) this, 4, NULL);

    

}


void brwHardware::iniDisplay (brwDisplay1306 *display) { 

  dpy = display;
  dpy -> initDisplay ();
  dpy -> clear ();
  dpy -> setTextSize(2);
  dpy -> setCursor(0, 0);
  //escreveLinha ("brWare V2");
  dpy -> escreveLinha ("brWare V2");

}



//------------------------------------------------------------------------------------------
//
// ------    Le arquivo de cogiguracao do Harware
//
//------------------------------------------------------------------------------------------

bool brwHardware::getHwConfig () {

  File wFile;
  char st [20];
  int i;
 
  if(SPIFFS.exists("/hardware.cfg")) { 

    wFile = SPIFFS.open("/hardware.cfg", "r");
    if(!wFile){
        Serial.println ("Não foi possivel abrir o arquivo de configuração");
        return (false);  
    }
    for (i = 0; i < MAX_RELE; i++) { 
        spifss -> leLinha (& wFile, st);
        tempoRele [i] = atoi (st);
        vTaskDelay(pdMS_TO_TICKS(100)); 
      }
    wFile.close ();
    return (true);
  } else {
      //Serial.println ("HW: leu cfg Default");
      setDefaut ();  
      wFile = SPIFFS.open("/hardware.cfg", "w");
      if(!wFile){
          Serial.println ("Não foi possivel (abrir) salvar o arquivo de configuração");
          return (false);  
      }
      //Serial.println ("HW: vai gravar linhas");
      for (i = 0; i < MAX_RELE; i++) { 
        itoa(tempoRele [i],st,10); 
        //Serial.print ("Tempo rele(N): ");
        //Serial.print (i + 1);
        //Serial.print (" : ");
        //Serial.println(st);
        spifss -> escreveLinha (& wFile, st);
        vTaskDelay(pdMS_TO_TICKS(100)); 
      }

      wFile.close ();

      return (true);    
  }


}

//------------------------------------------------------------------------------------------
//
// ------    Grava arquivo de cogiguracao do Harware
//
//------------------------------------------------------------------------------------------

bool brwHardware::setHwConfig () {

    File wFile;
    char st [20];
    int i;
    
    spifss -> listDir("/", 0); 

    wFile = SPIFFS.open("/hardware.cfg", "w");
    if(!wFile){
        Serial.println ("Não foi possivel (abrir) salvar o arquivo de configuração");
        return (false);  
    }

    
    
    for (i = 0; i < MAX_RELE; i++) { 
      itoa(tempoRele [i],st,10); 
      spifss -> escreveLinha (& wFile, st);
      vTaskDelay(pdMS_TO_TICKS(100)); 
    }
  
    
    

    wFile.close ();

    return (true);

  
}

//------------------------------------------------------------------------------------------
//
// ------    Grava arquivo de cogiguracao do Harware
//
//------------------------------------------------------------------------------------------
void brwHardware::setDefaut () {

  if (strcmp (hwApp,"playade01")) {
    strcpy (infoHw, "brWare One");
    strcpy (infoVersao, "1.0.1"); 
    tempoRele [0] = 1;
    tempoRele [1] = 1;
    tempoRele [2] = 1;
    tempoRele [3] = 1;
  } 

  if (strcmp (hwApp,"sucres")) {
    strcpy (infoHw, "brWare SUCRES");
    strcpy (infoVersao, "1.0.1"); 
    tempoRele [0] = 1;
    tempoRele [1] = 1;
    tempoRele [2] = 180;
    tempoRele [3] = 1;
    tempoRele [4] = 1;
    tempoRele [5] = 1;
    tempoRele [6] = 1;
  }


}

//------------------------------------------------------------------------------------------
//
// ------    Imprime arquivo de cogiguracao do Harware
//
//------------------------------------------------------------------------------------------

void brwHardware::printHwCfg () {
  int i, j;
  String str = "";

  dpy -> clear ();
  dpy -> setTextSize(2);
  dpy -> setCursor(0, 0);
  //dpy -> escreveLinha (" Cfg. App ");
  //dpy -> setTextSize(1);
  /*
  for (i = 0; i < MAX_RELE; i++){
    j = i + 1;
    str = "Tempo. do Rele " + String (j) + " : " + String (tempoRele [i]); 
    dpy -> escreveLinha (str.c_str());
    Serial.println (str.c_str());
  }
  */  

}


//------------------------------------------------------------------------------------------
//
// -----------   SetUp de pinos
//
//------------------------------------------------------------------------------------------

void brwHardware::setUpPinos () {

if (strcmp (hwApp,"netuno") == 0) {

    Serial.println ("Setup de Hardware versão: netuno");

    MAX_RELE = 5;

    BT_BOOT = 0;
    BUZZER  = 15;
    LED_IN  = 2;
    BTN = 14;


    //Sensores
    S01 = 33;  // Entrada e saida, pode ser analogico com WiFi
    S02 = 35;  // PULLUP DE HARDWARE
    S03 = 34;  // PULLUP DE HARDWARE

    S04 = 32;  // Entrada e saida, pode ser analogico com WiFi
    S05 = 39;  // PULLUP DE HARDWARE
    S06 = 36;  // PULLUP DE HARDWARE

    S07 = 4;   // Entrada e saida, pode ser analogico somente sem WiFi
    S08 = 16;  // Entrada e saida, pode ser analogico somente sem WiFi
    S09 = 13;  // Entrada e saida, pode ser analogico somente sem WiFi

    RX10 = 3;  // SERIAL  TTL RX
    TX11 = 1;  // serial TTL  TX

    // reles
    RL01 = 12;   // Falha a inialização se puxado patra alto
    RL02 = 27;
    RL03 = 26;
    RL04 = 25;

    // SPI
    SPI_RST = 17;
    SPI_SCS = 5;
    SPI_CLK = 18;
    SPI_NIS0 = 19;
    SPI_MOSI = 23;

    // I2C
    I2C_SCA = 21;
    I2C_SCL = 22;




    pinMode(S01, INPUT_PULLUP); // 
    pinMode(S02, INPUT_PULLUP); // 
    pinMode(S03, INPUT_PULLUP); // 
    pinMode(S05, INPUT_PULLUP); // 
    pinMode(S06, INPUT_PULLUP); // 
    pinMode(S07, INPUT_PULLUP); //
    pinMode(S08, INPUT_PULLUP); //
    pinMode(S09, INPUT_PULLUP); //

    pinMode (RL01, OUTPUT); // 
    pinMode (RL02, OUTPUT); // 
    pinMode (RL03, OUTPUT); // 
    pinMode (RL04, OUTPUT); // 

  } 


if (strcmp (hwApp,"playade01") == 0) { // Sensor de temperatuta TBForte

    Serial.println ("Setup de Hardware versão: Playade01");

    MAX_RELE = 5;

    BT_BOOT = 0;
    BUZZER  = 15;
    LED_IN  = 2;
    BTN = 14;


    //Sensores
    S01 = 33;  // Entrada e saida, pode ser analogico com WiFi
    S02 = 35;  // PULLUP DE HARDWARE
    S03 = 34;  // PULLUP DE HARDWARE

    S04 = 32;  // Entrada e saida, pode ser analogico com WiFi
    S05 = 39;  // PULLUP DE HARDWARE
    S06 = 36;  // PULLUP DE HARDWARE

    S07 = 4;   // Entrada e saida, pode ser analogico somente sem WiFi
    S08 = 16;  // Entrada e saida, pode ser analogico somente sem WiFi
    S09 = 13;  // Entrada e saida, pode ser analogico somente sem WiFi

    RX10 = 3;  // SERIAL  TTL RX
    TX11 = 1;  // serial TTL  TX

    // reles
    RL01 = 12;   // Falha a inialização se puxado patra alto
    RL02 = 27;
    RL03 = 26;
    RL04 = 25;

    // SPI
    SPI_RST = 17;
    SPI_SCS = 5;
    SPI_CLK = 18;
    SPI_NIS0 = 19;
    SPI_MOSI = 23;

    // I2C
    I2C_SCA = 21;
    I2C_SCL = 22;


    pinMode(S01, INPUT_PULLUP); // 
    pinMode(S02, INPUT_PULLUP); // 
    pinMode(S03, INPUT_PULLUP); // 
    pinMode(S05, INPUT_PULLUP); // 
    pinMode(S06, INPUT_PULLUP); // 
    pinMode(S07, INPUT_PULLUP); //
    pinMode(S08, INPUT_PULLUP); //
    pinMode(S09, INPUT_PULLUP); //

    pinMode (RL01, OUTPUT); // 
    pinMode (RL02, OUTPUT); // 
    pinMode (RL03, OUTPUT); // 
    pinMode (RL04, OUTPUT); // 

  } 

if (strcmp (hwApp,"playade02") == 0) { // Sensor de Gerador TBForte

    Serial.println ("Setup de Hardware versão: playade02");
    MAX_RELE = 5;

    BT_BOOT = 0;
    BUZZER  = 15;
    LED_IN  = 2;
    BTN = 14;


    //Sensores
    S01 = 33;  // Entrada e saida, pode ser analogico com WiFi
    S02 = 35;  // PULLUP DE HARDWARE
    S03 = 34;  // PULLUP DE HARDWARE

    S04 = 32;  // Entrada e saida, pode ser analogico com WiFi
    S05 = 39;  // PULLUP DE HARDWARE
    S06 = 36;  // PULLUP DE HARDWARE

    S07 = 4;   // Entrada e saida, pode ser analogico somente sem WiFi
    S08 = 16;  // Entrada e saida, pode ser analogico somente sem WiFi
    S09 = 13;  // Entrada e saida, pode ser analogico somente sem WiFi

    RX10 = 3;  // SERIAL  TTL RX
    TX11 = 1;  // serial TTL  TX

    // reles
    RL01 = 12;   // Falha a inialização se puxado patra alto
    RL02 = 27;
    RL03 = 26;
    RL04 = 25;

    // SPI
    SPI_RST = 17;
    SPI_SCS = 5;
    SPI_CLK = 18;
    SPI_NIS0 = 19;
    SPI_MOSI = 23;

    // I2C
    I2C_SCA = 21;
    I2C_SCL = 22;



    pinMode(S01, INPUT_PULLUP); // 
    pinMode(S02, INPUT_PULLUP); // 
    pinMode(S03, INPUT_PULLUP); // 
    pinMode(S05, INPUT_PULLUP); // 
    pinMode(S06, INPUT_PULLUP); // 
    pinMode(S07, INPUT_PULLUP); //
    pinMode(S08, INPUT_PULLUP); //
    pinMode(S09, INPUT_PULLUP); //

    pinMode (RL01, OUTPUT); // 
    pinMode (RL02, OUTPUT); // 
    pinMode (RL03, OUTPUT); // 
    pinMode (RL04, OUTPUT); // 
  } 




if (strcmp (hwApp,"sucres") == 0) {

    Serial.println ("Setup de Hardware versão: sucres");
    MAX_RELE = 7;

    BT_BOOT = 0;
    BUZZER  = 15;
    LED_IN  = 2;
    BTN = 14;


    //Sensores
    S01 = 36;  // Entrada e saida, pode ser analogico com WiFi
    S02 = 35;  // PULLUP DE HARDWARE
    S03 = 34;  // PULLUP DE HARDWARE

    S04 = 32;  // Entrada e saida, pode ser analogico com WiFi
    S05 = 39;  // PULLUP DE HARDWARE
    S06 = 36;  // PULLUP DE HARDWARE

   
    RX10 = 3;  // SERIAL  TTL RX
    TX11 = 1;  // serial TTL  TX

    // reles
    RL01 = 33;   // Falha a inialização se puxado patra alto
    RL02 = 4;
    RL03 = 16;
    RL04 = 12;
    RL05 = 27;
    RL06 = 26;
    RL07 = 25;

    // SPI
    SPI_RST = 17;
    SPI_SCS = 5;
    SPI_CLK = 18;
    SPI_NIS0 = 19;
    SPI_MOSI = 23;

    // I2C
    I2C_SCA = 21;
    I2C_SCL = 22;

    
      
      
    pinMode(S02, INPUT_PULLUP); // Status Portão 01
    pinMode(S03, INPUT_PULLUP); // Status Portão 02
    pinMode(S04, INPUT_PULLUP); // Status Portão 03
    pinMode(S05, INPUT_PULLUP); // Status Portão 04

    //pinMode(S09, INPUT_PULLUP); // Estado do Alarme
    
    
    pinMode (RL01, OUTPUT); // Alarme/Bollard
    pinMode (RL02, OUTPUT); // Desabilita
    pinMode (RL03, OUTPUT); // Gerador de Neblina
    pinMode (RL04, OUTPUT); // Portão 01 
    pinMode (RL05, OUTPUT); // Portão 02
    pinMode (RL06, OUTPUT); // Portão 03
    pinMode (RL07, OUTPUT); // Portão 04

  }   

//
// para todas as vesoes
//  
pinMode(BT_BOOT, INPUT_PULLUP); // pino do boot
pinMode (BTN, INPUT_PULLUP); // Pino de configuração
digitalWrite(BTN, LOW);
pinMode(BUZZER, OUTPUT); // pino do BUZZER
digitalWrite(BUZZER, LOW);
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

  

  if ((numRL < 0) || (numRL > MAX_RELE))
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
#if (HW_VERSAO == 2)
    case 5 :
      xTaskCreate(&brwHardware::staticRele05, "Rele05", 1024, (void*) this, 4, NULL);
      break;  

    case 6 :
      xTaskCreate(&brwHardware::staticRele06, "Rele06", 1024, (void*) this, 4, NULL);
      break;   

    case 7 :
      xTaskCreate(&brwHardware::staticRele07, "Rele07", 1024, (void*) this, 4, NULL);
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

void brwHardware::Rele01 (){
  
  digitalWrite (RL01,HIGH);
  statusRele [0] =  HIGH;
  vTaskDelay(pdMS_TO_TICKS(tempoRele[0] * 1000)); 
  digitalWrite (RL01,LOW);
  statusRele [0] =  LOW;

  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele01 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele01(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardware::Rele02 (){
  
  digitalWrite (RL02,HIGH);
  statusRele [1] =  HIGH;
  setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(tempoRele[1] * 1000)); 
  digitalWrite (RL02,LOW);
  statusRele [1] =  LOW;
  setStatusReles (false);  
  
  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele02 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele02(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardware::Rele03 (){
  
  digitalWrite (RL03,HIGH);
  statusRele [2] =  HIGH;
  setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(tempoRele[2] * 1000)); 
  digitalWrite (RL03,LOW);
  statusRele [2] =  LOW;
  setStatusReles (false);  
   
  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele03 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele03(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardware::Rele04 (){
  
  digitalWrite (RL04,HIGH);
  statusRele [3] =  HIGH;
  setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(tempoRele[3] * 1000)); 
  digitalWrite (RL04,LOW);
  statusRele [3] =  LOW;
  setStatusReles (false);  
 
  vTaskDelete(NULL); // delete task

}

void brwHardware::staticRele04 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele04(); //dispatch to the member function, now that we have an instance pointer

}


void brwHardware::Rele05 (){

#if (HW_VERSAO == 2)

  digitalWrite (RL05,HIGH);
  statusRele [4] =  HIGH;
  setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(tempoRele[4] * 1000)); 
  digitalWrite (RL05,LOW);
  statusRele [4] =  LOW;
  setStatusReles (false);  
 
  vTaskDelete(NULL); // delete task

#endif

}


void brwHardware::staticRele05 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele05(); //dispatch to the member function, now that we have an instance pointer

}

void brwHardware::Rele06 (){

#if (HW_VERSAO == 2)

  digitalWrite (RL06,HIGH);
  statusRele [5] =  HIGH;
  setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(tempoRele[5] * 1000)); 
  digitalWrite (RL06,LOW);
  statusRele [5] =  LOW;
  setStatusReles (false);  
 
  vTaskDelete(NULL); // delete task
#endif

}

void brwHardware::staticRele06 (void *pvParameter) {

    brwHardware* tk = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    tk->Rele06(); //dispatch to the member function, now that we have an instance pointer

}


void brwHardware::Rele07 (){

#if (HW_VERSAO == 2)
  
  digitalWrite (RL07,HIGH);
  statusRele [6] =  HIGH;
  setStatusReles (false);  
  vTaskDelay(pdMS_TO_TICKS(tempoRele[6] * 1000)); 
  digitalWrite (RL07,LOW);
  statusRele [6] =  LOW;
  setStatusReles (false);  
 
  vTaskDelete(NULL); // delete task

#endif

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
    if ((num < 1) || (num > MAX_RELE))
        return (false);
    tempoRele [num - 1] = tempo;
    return (true);
}

int brwHardware::getTempoRele (int num) {

    if ((num < 1) || (num > MAX_RELE))
        return (0);
    return (tempoRele [num - 1]);
}





//*****************************************************************************
//
//  Set Estado do Rela para LOW ou HIGH
//
//*****************************************************************************


bool brwHardware::setRele (int num, int estado) {

  

  if ((num < 0) || (num > MAX_RELE))
    return (false);

  switch (num) { 
    case 1 :
        digitalWrite (RL01,estado);
        statusRele [0] =  estado;
      break;

    case 2 :
      digitalWrite (RL02,estado);
      statusRele [1] =  estado;
      break;

    case 3 :
      digitalWrite (RL03,estado);
      statusRele [2] =  estado;
      break;

    case 4 :
      digitalWrite (RL04,estado);
      statusRele [3] =  estado;
      break; 
#if (HW_VERSAO == 2)
    case 5 :
      digitalWrite (RL05,estado);
      statusRele [4] =  estado;
      break;        

    case 6 :
      digitalWrite (RL06,estado);
      statusRele [5] =  estado;
      break;  

    case 7 :
      digitalWrite (RL07,estado);
      statusRele [6] =  estado;
      break;  
#endif
  }  
  
  setStatusReles (false);    
  return (true);

}


//*****************************************************************************
//
//  Set Estado do Rela para LOW ou HIGH
//
//*****************************************************************************


bool brwHardware::inverteRele (int num) {

  

  if ((num < 0) || (num > MAX_RELE))
    return (false);

  switch (num) { 
    case 1 :
        digitalWrite (RL01,!digitalRead (RL01));
        statusRele [0] =  digitalRead (RL01);
      break;

    case 2 :
      digitalWrite (RL02,!digitalRead (RL02));
      statusRele [1] =  digitalRead (RL02);
      break;

    case 3 :
      digitalWrite (RL03,!digitalRead (RL03));
      statusRele [2] =  digitalRead (RL03);
      break;

    case 4 :
      digitalWrite (RL04,!digitalRead (RL04));
      statusRele [3] =  digitalRead (RL04);
      break;    
#if (HW_VERSAO == 2)
    case 5 :
      digitalWrite (RL05,!digitalRead (RL05));
      statusRele [4] =  digitalRead (RL05);
      break;       

    case 6 :
      digitalWrite (RL06,!digitalRead (RL06));
      statusRele [5] =  digitalRead (RL06);
      break;   

    case 7 :
      digitalWrite (RL07,!digitalRead (RL07));
      statusRele [6] =  digitalRead (RL07);
      break;   
#endif
  }  
  
  setStatusReles (false);   

  

  return (true);

}


//*****************************************************************************
//
//  Le Status dos reles no arquivo reles.cfg
//    O flag atu_rele se true aciona o rele na hora 
//
//*****************************************************************************


bool brwHardware::getStatusReles (bool atu_rele) {

  File wFile;
  char st[5];
  int i;


  if(!SPIFFS.exists("/reles.cfg")) {
     wFile = SPIFFS.open("/reles.cfg", "w");
     //Serial.println ("Criando Arquivo status dos reles");  
     // --------------------------------------------------
     // Não achou arquivo e grava um valor atual dos reles
     //---------------------------------------------------
     for (i = 0; i < MAX_RELE; i++) { 
      switch (i) {
         case 0 : statusRele [i] = digitalRead (RL01);
             break;  
         case 1 : statusRele [i] = digitalRead (RL02);
             break;    
         case 2 : statusRele [i] = digitalRead (RL03);
             break;    
         case 3 : statusRele [i] = digitalRead (RL04);
             break; 
    #if (HW_VERSAO == 2)            
         case 4 : statusRele [i] = digitalRead (RL05);
             break;    
         case 5 : statusRele [i] = digitalRead (RL06);
             break;    
         case 6 : statusRele [i] = digitalRead (RL07);
             break;  
    #endif         
        }  
      //wFile.println(String(statusRele [i]));
      itoa(statusRele [i],st,10);  
      spifss -> escreveLinha (& wFile, st);
      //Serial.print ("Status rele (N)");
      //Serial.print (i + 1);
      //Serial.print (" : ");
      //Serial.println(st);
      } 
    wFile.close ();  
  } else { 

          wFile = SPIFFS.open("/reles.cfg", "r");
          if(!wFile){
              Serial.println ("Não foi possivel abrir o arquivo de satus dos reles");
              return (false);  
          } 
          for (i = 0; i < MAX_RELE; i++) { 
              //str = wFile.readStringUntil('\n');
              spifss -> leLinha (& wFile, st);
              statusRele [i] = atoi (st);
             
              //Serial.print ("Status rele (r) ");
              //Serial.print (i + 1);
              //Serial.print (" : ");
              //Serial.println(statusRele [i]);
            }


          wFile.close ();
          
        }

  if (atu_rele) { 
      digitalWrite (RL01,statusRele [0]);
      digitalWrite (RL02,statusRele [1]);
      digitalWrite (RL03,statusRele [2]);
      digitalWrite (RL04,statusRele [3]);
  #if (HW_VERSAO == 2)    
      digitalWrite (RL05,statusRele [4]);
      digitalWrite (RL06,statusRele [5]);
      digitalWrite (RL07,statusRele [6]);
  #endif    
  }       

  return (true);

}


//*****************************************************************************
//
//  Grava Status dos reles da variavel de status no arquivo reles.cfg
//    O flag atu_rele se true aciona o rele na hora 
//
//*****************************************************************************


bool brwHardware::setStatusReles (bool atu_rele) {

  File wFile;
  char st [5];
  int i, c;


    wFile = SPIFFS.open("/reles.cfg", "w");
    if(!wFile){
        Serial.println ("Não foi possivel (abrir) salvar o arquivo de status dos reles");
        return (false);  
    }


    //Serial.println ("Gravando arquivo de status dos reles");

    for (i = 0; i < MAX_RELE; i++) { 
      itoa(statusRele [i],st,10); 
      spifss -> escreveLinha (& wFile, st);
      vTaskDelay(pdMS_TO_TICKS(50)); 
    }
  
    wFile.close ();

    if (atu_rele) {
      
      digitalWrite (RL01,statusRele [0]);
      digitalWrite (RL02,statusRele [1]);
      digitalWrite (RL03,statusRele [2]);
      digitalWrite (RL04,statusRele [3]);
  #if (HW_VERSAO == 2)    
      digitalWrite (RL05,statusRele [4]);
      digitalWrite (RL06,statusRele [5]);
      digitalWrite (RL07,statusRele [6]);
  #endif    
    }




    return (true);


}


//------------------------------------------------------------------------------------------
//
// -----------------  task Botão de Configuração
//
//------------------------------------------------------------------------------------------

void brwHardware::tkBotao () {
  
int cnt1 = 0, cnt2; 
 
   
//Serial.println ("- Monitora Botão de CFG");  
vTaskDelay(pdMS_TO_TICKS(10000)); // tempo para task inicia
while (true)  { 
   
   vTaskDelay(pdMS_TO_TICKS(100)); // tempo para task inicia  
      
   
   while (digitalRead (BTN) == LOW) {
   //while (digitalRead (BTN) == HIGH) {
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
    // mostra config de rede no Display
   }
   if (cnt2 == 2) {
     Serial.println (cnt2); 
     // mostra config do MQTT
     
   }
   if (cnt2 == 3) {
    Serial.println (cnt2); 
    printHwCfg (); // mostra configiração da aplicação
     
   }
   if (cnt2 == 4) {
    Serial.println (cnt2);
    //Reseta configuração
    spifss -> formatFS(); // para testes
    //efs.listDir("/", 0);
    actBeep (500, 3);
    ESP.restart ();
    //salvaCfg (); // para testes
    
   }

   cnt2 = 0;
   cnt1 = 0;   
   
  }



}


void brwHardware::statictkBotao (void *pvParameter) {

    brwHardware* btn = reinterpret_cast<brwHardware*>(pvParameter); //obtain the instance pointer
    btn->tkBotao(); //dispatch to the member function, now that we have an instance pointer

}



//------------------------------------------------------------------------------------------
//
// -----------------  FUNCOES SOMETE PARA SUCRES DE VERIFICAR PORTOES
//
//------------------------------------------------------------------------------------------




bool brwHardware::leEntrada (int entrada) {
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


//------------------------------------------------------------------------------------------
//
// -----------------  FUNCOES para retosno de configuração das portas
//
//------------------------------------------------------------------------------------------

int brwHardware::getMAX_RELE () {

  return (MAX_RELE);
}

int brwHardware::getBT_BOOT () {

  return (BT_BOOT);
}

int brwHardware::getBUZZER () {

  return (BUZZER);
}

int brwHardware::getLED_IN () {

  return (LED_IN);

}

int brwHardware::getBTN () {

  return (BTN);

}

//Sensores
int brwHardware::getS01 () {

  return (S01);

}

int brwHardware::getS02 () {

  return (S02);

}
int brwHardware::getS03 (){

  return (S03);

}

int brwHardware::getS04 (){

  return (S04);

}

int brwHardware::getS05 (){

  return (S05);

}

int brwHardware::getS06 (){

  return (S06);

}

int brwHardware::getS07 (){

  return (S07);

}

int brwHardware::getS08 (){

  return (S08);

}

int brwHardware::getS09 (){

  return (S09);

}

int brwHardware::getRX10 (){

  return (RX10);

}
int brwHardware::getTX11 (){

  return (TX11);

}

// reles
int brwHardware::getRL01 (){

  return (RL01);

}

int brwHardware::getRL02 (){

  return (RL02);

}

int brwHardware::getRL03 (){

  return (RL03);

}

int brwHardware::getRL04 (){

  return (RL04);

}

// reles sucres
int brwHardware::getRL05 (){

  return (RL05);

}

int brwHardware::getRL06 (){

  return (RL06);

}

int brwHardware::getRL07 (){

  return (RL07);

}

// SPI
int brwHardware::getSPI_RST (){

  return (SPI_RST);

}

int brwHardware::getSPI_SCS (){

  return (SPI_SCS);

}

int brwHardware::getSPI_CLK (){

  return (SPI_CLK);

}

int brwHardware::getSPI_NIS0 (){

  return (SPI_NIS0);

}

int brwHardware::getSPI_MOSI (){

  return (SPI_MOSI);

}

// I2C
int brwHardware::getI2C_SCA (){

  return (I2C_SCA);

}

int brwHardware::getI2C_SCL (){

  return (I2C_SCL);

}
