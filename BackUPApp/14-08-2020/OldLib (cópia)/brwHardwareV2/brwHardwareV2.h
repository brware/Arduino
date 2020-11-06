//------------------------------------------------------------------------------------------
//
//  Classe pa funções basicas de Hardware
//
//-------------------------------------------------------------------------------------------

#ifndef brwHardwareV2_h
#define brwHardwarev2_h
 
#include <Arduino.h>
#include "brw_two.h"



class brwHardware
{
    public:
      brwHardware ();

      void iniHardware ();
    
      void setUpPinos ();

      void actBeep (int msTempo, int xVezes);

      bool actRele (int numRL,int msTempo);
      bool setTempoRele (int num, int tempo);
      int  getTempoRele (int num);
      bool setRele (int num, int estado);
      bool inverteRele (int num);
      
      bool mudouCfgHardware = false;     
 
    private:
  		// Para Buzzer
      int bzTempo = 0;
      int bzVezes = 0;
      bool bzAciona = false;
      //Rele
      int tempoRele [7] = { 1, 1, 1, 1, 1, 1, 1 } ;
      

      void Beep ();
      static void staticBeep (void *pvParameter);

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
