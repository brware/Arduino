//------------------------------------------------------------------------------------------
//
//  Classe para funções basicas de Hardware
//
//-------------------------------------------------------------------------------------------





#ifndef brwHardwareCfg_h
#define brwHardwareCfg_h




#include <Arduino.h>
#include "brwSpiffs.h"



class brwHardwareCfg 
{
    public:
      brwHardwareCfg (const char* App, const char* Versao, const char* Hard, brwSpiffs* Spiffs) ;

      void iniHardware ();
      void setUpPinos ();

      bool getStatusReles (bool atu_rele); // le Estatus dos reles
      bool setStatusReles (bool atu_rele); // Salva status dos reles

      bool getHwConfig ();
      bool setHwConfig ();
      void printHwCfg ();


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
      
      char hwApp [20];
      char infoHw [20];
      char infoVersao [10];

         
      // Para Buzzer
      int bzTempo = 0;
      int bzVezes = 0;
      bool bzAciona = false;
      //Rele
      int tempoRele [7] = { 1, 1, 1, 1, 1, 1, 1 } ;
      int statusRele [7] = { 1, 1, 1, 1, 1, 1, 1 } ;

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



 
    private:
      
      
      void setDefaut ();

      
      
 



 
      
      

		  
};

#endif
