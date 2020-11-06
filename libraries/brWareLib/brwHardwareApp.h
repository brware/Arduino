//------------------------------------------------------------------------------------------
//
//  Classe para funções basicas de Hardware
//
//-------------------------------------------------------------------------------------------





#ifndef brwHardwareApp_h
#define brwHardwareApp_h




#include <Arduino.h>
#include "brwHardwareCfg.h"
#include "brwDisplay1306.h"



class brwHardwareApp 
{
    public:
      brwHardwareApp (brwHardwareCfg* HardwareCfg);

      void iniHardware ();
      

      void actBeep (int msTempo, int xVezes);

      bool actRele (int numRL,int msTempo);
      bool setTempoRele (int num, int tempo);
      int  getTempoRele (int num);
      bool setRele (int num, int estado);
      bool inverteRele (int num);

      
      // funções apenas para Sucres
      
      bool leEntrada (int entrada);



      brwHardwareCfg* hw_cfg;
      //brwDisplay1306 * dpy; // classe para display
      
      char hwApp [20];
      char infoHw [20];
      char infoVersao [10];

       
    private:
     
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

      
      

		  
};

#endif
