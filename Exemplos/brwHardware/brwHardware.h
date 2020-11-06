//------------------------------------------------------------------------------------------
//
//  Classe pa funções basicas de Hardware
//
//-------------------------------------------------------------------------------------------

#ifndef brwHardware_h
#define brwHardware_h
 
#include <Arduino.h>
#include "brw_one.h"


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
      int tempoRele [4] = { 1, 1, 1, 1} ;
      

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

		  
};

#endif
