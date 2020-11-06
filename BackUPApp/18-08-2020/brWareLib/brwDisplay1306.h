//------------------------------------------------------------------------------------------
//
//  Classe pa funções basicas para dsplay
//
//-------------------------------------------------------------------------------------------

#ifndef brwDisplay1306_h
#define brwDisplay1306_h
 
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define WIDTH   128 // OLED display width, in pixels
#define HEIGHT  64 // OLED display height, in pixels


class brwDisplay1306 
{
    public:
      brwDisplay1306 ();
      bool initDisplay ();
      void display();
      void clear ();
      void setTextSize(int size);
      void setTextColor(int color);
      void setCursor(int x, int y);
      void print(String s);
      void escreveLinha  (String s);
      

 
    private:
      
      


		  
};

#endif
