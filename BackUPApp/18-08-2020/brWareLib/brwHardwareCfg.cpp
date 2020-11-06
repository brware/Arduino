#include "brwHardwareCfg.h"


//------------------------------------------------------------------------------------------
//
// ------    Cosntrutor da Classe hardware
//
//------------------------------------------------------------------------------------------

brwHardwareCfg::brwHardwareCfg (const char* App, const char* Versao, const  char* Hard, brwSpiffs* Spiffs) {

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

void brwHardwareCfg::iniHardware () {

  
  //actBeep (500, 2);
  
  Serial.print (hwApp);
  if (!strcmp (hwApp,"playade01"))
     Serial.println (" : Iniciando Hardware Vesão 1");
  if (!strcmp (hwApp,"sucres"))
    Serial.println (" : Iniciando Hardware Vesão 2"); 
 
  getHwConfig ();
  Serial.println ("Iniciando da Classe HardwareCfg: ");
   
    
    

}



//------------------------------------------------------------------------------------------
//
// ------    Le arquivo de cogiguracao do Harware
//
//------------------------------------------------------------------------------------------

bool brwHardwareCfg::getHwConfig () {

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

bool brwHardwareCfg::setHwConfig () {

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
void brwHardwareCfg::setDefaut () {

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

void brwHardwareCfg::printHwCfg () {
  int i, j;
  String str = "";

  //dpy -> clear ();
  //dpy -> setTextSize(2);
  //dpy -> setCursor(0, 0);
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

void brwHardwareCfg::setUpPinos () {

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
//  Le Status dos reles no arquivo reles.cfg
//    O flag atu_rele se true aciona o rele na hora 
//
//*****************************************************************************


bool brwHardwareCfg::getStatusReles (bool atu_rele) {

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


bool brwHardwareCfg::setStatusReles (bool atu_rele) {

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
// -----------------  FUNCOES para retosno de configuração das portas
//
//------------------------------------------------------------------------------------------

int brwHardwareCfg::getMAX_RELE () {

  return (MAX_RELE);
}

int brwHardwareCfg::getBT_BOOT () {

  return (BT_BOOT);
}

int brwHardwareCfg::getBUZZER () {

  return (BUZZER);
}

int brwHardwareCfg::getLED_IN () {

  return (LED_IN);

}

int brwHardwareCfg::getBTN () {

  return (BTN);

}

//Sensores
int brwHardwareCfg::getS01 () {

  return (S01);

}

int brwHardwareCfg::getS02 () {

  return (S02);

}
int brwHardwareCfg::getS03 (){

  return (S03);

}

int brwHardwareCfg::getS04 (){

  return (S04);

}

int brwHardwareCfg::getS05 (){

  return (S05);

}

int brwHardwareCfg::getS06 (){

  return (S06);

}

int brwHardwareCfg::getS07 (){

  return (S07);

}

int brwHardwareCfg::getS08 (){

  return (S08);

}

int brwHardwareCfg::getS09 (){

  return (S09);

}

int brwHardwareCfg::getRX10 (){

  return (RX10);

}
int brwHardwareCfg::getTX11 (){

  return (TX11);

}

// reles
int brwHardwareCfg::getRL01 (){

  return (RL01);

}

int brwHardwareCfg::getRL02 (){

  return (RL02);

}

int brwHardwareCfg::getRL03 (){

  return (RL03);

}

int brwHardwareCfg::getRL04 (){

  return (RL04);

}

// reles sucres
int brwHardwareCfg::getRL05 (){

  return (RL05);

}

int brwHardwareCfg::getRL06 (){

  return (RL06);

}

int brwHardwareCfg::getRL07 (){

  return (RL07);

}

// SPI
int brwHardwareCfg::getSPI_RST (){

  return (SPI_RST);

}

int brwHardwareCfg::getSPI_SCS (){

  return (SPI_SCS);

}

int brwHardwareCfg::getSPI_CLK (){

  return (SPI_CLK);

}

int brwHardwareCfg::getSPI_NIS0 (){

  return (SPI_NIS0);

}

int brwHardwareCfg::getSPI_MOSI (){

  return (SPI_MOSI);

}

// I2C
int brwHardwareCfg::getI2C_SCA (){

  return (I2C_SCA);

}

int brwHardwareCfg::getI2C_SCL (){

  return (I2C_SCL);

}
