//------------------------------------------------------------------------------------------
//
//  Classe para funções basicas de Hardware
//
//-------------------------------------------------------------------------------------------





#ifndef brwHardware_h
#define brwHardware_h




#include <Arduino.h>
#include "brwSpiffs.h"
#include "brwDisplay1306.h"


class brwHardware 
{
    public:
      brwHardware (const char* App, const char* Versao, const char* Hard, brwSpiffs* Spiffs) ;

      void iniHardware ();
      void setUpPinos ();
      void iniDisplay (brwDisplay1306 *display);

      void actBeep (int msTempo, int xVezes);

      bool actRele (int numRL,int msTempo);
      bool setTempoRele (int num, int tempo);
      int  getTempoRele (int num);
      bool setRele (int num, int estado);
      bool inverteRele (int num);

      bool getHwConfig ();
      bool setHwConfig ();
      void printHwCfg ();


      // funções apenas para Sucres
      
      bool leEntrada (int entrada);



      // Variaveis de configuração
      int getMAX_RELE ();
      int getBT_BOOT ();
      int getBUZZER ();
      int getLED_IN ();
      int getBTN ();

      //Sensores
      int getS01 ();// Entrada e saida, pode ser analogico com WiFi
      int getS02 ();// PULLUP DE HARDWARE
      int getS03 ();// PULLUP DE HARDWARE

      int getS04 ();// Entrada e saida, pode ser analogico com WiFi
      int getS05 ();// PULLUP DE HARDWARE
      int getS06 ();// PULLUP DE HARDWARE

      int getS07 (); // Entrada e saida, pode ser analogico somente sem WiFi
      int getS08 ();// Entrada e saida, pode ser analogico somente sem WiFi
      int getS09 ();// Entrada e saida, pode ser analogico somente sem WiFi

      int getRX10 ();// SERIAL  TTL RX
      int getTX11 ();// serial TTL  TX

      // reles
      int getRL01 ();// Falha a inialização se puxado patra alto
      int getRL02 ();
      int getRL03 ();
      int getRL04 ();
      // reles sucres
      int getRL05 ();
      int getRL06 ();
      int getRL07 ();

      // SPI
      int getSPI_RST ();
      int getSPI_SCS ();
      int getSPI_CLK ();
      int getSPI_NIS0 ();
      int getSPI_MOSI ();

      // I2C
      int getI2C_SCA ();
      int getI2C_SCL ();

      
      brwSpiffs * spifss; // classe para arquivo
      brwDisplay1306 * dpy; // classe para display
      
      char hwApp [20];
      char infoHw [20];
      char infoVersao [10];

      bool mudouCfgHardware = false;    




 
    private:
      
      // classes 

      bool usaDpy = false; // se esta usando a classe display 

 
  	// Para Buzzer
      int bzTempo = 0;
      int bzVezes = 0;
      bool bzAciona = false;
      //Rele
      int tempoRele [7] = { 1, 1, 1, 1, 1, 1, 1 } ;
      int statusRele [7] = { 1, 1, 1, 1, 1, 1, 1 } ;
      
      
      
      
      void setDefaut ();

      
      bool getStatusReles (bool atu_rele); // le Estatus dos reles
      bool setStatusReles (bool atu_rele); // Salva status dos reles


      void Beep ();
      static void staticBeep (void *pvParameter);

      void tkBotao ();
      static void statictkBotao (void *pvParameter);

      void Rele01 ();
      static void staticRele01 (void *pvParameter);
      void Rele02 ();
      static void staticRele02 (void *pvParameter);
      void Rele03 ();
      static void staticRele03 (void *pvParameter);
      void Rele04 ();
      static void staticRele04 (void *pvParameter);
      void Rele05 ();
      static void staticRele05 (void *pvParameter);
      void Rele06 ();
      static void staticRele06 (void *pvParameter);
      void Rele07 ();
      static void staticRele07 (void *pvParameter);

      // Variaveis de configuração
      
      
      int MAX_RELE; 

      // Misselania
      int BT_BOOT;
      int BUZZER;
      int LED_IN;
      int BTN;

      //Sensores
      int S01;// Entrada e saida, pode ser analogico com WiFi
      int S02;// PULLUP DE HARDWARE
      int S03;// PULLUP DE HARDWARE

      int S04;// Entrada e saida, pode ser analogico com WiFi
      int S05;// PULLUP DE HARDWARE
      int S06;// PULLUP DE HARDWARE

      int S07; // Entrada e saida, pode ser analogico somente sem WiFi
      int S08;// Entrada e saida, pode ser analogico somente sem WiFi
      int S09;// Entrada e saida, pode ser analogico somente sem WiFi

      int RX10;// SERIAL  TTL RX
      int TX11;// serial TTL  TX

      // reles
      int RL01;// Falha a inialização se puxado patra alto
      int RL02;
      int RL03;
      int RL04;
      // reles sucres
      int RL05;
      int RL06;
      int RL07;

      // SPI
      int SPI_RST;
      int SPI_SCS;
      int SPI_CLK;
      int SPI_NIS0;
      int SPI_MOSI;

      // I2C
      int I2C_SCA;
      int I2C_SCL;
      

		  
};

#endif
