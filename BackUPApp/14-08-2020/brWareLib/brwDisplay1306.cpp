

#include "brwDisplay1306.h"

Adafruit_SSD1306 Cdisplay ( WIDTH, HEIGHT, &Wire, -1);


brwDisplay1306::brwDisplay1306 () {



}


bool brwDisplay1306::initDisplay () {

//waitMutex (0);
takeMutex();	
if(!Cdisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
	    Serial.println(F("SSD1306 Não Encontrado"));
      giveMutex();
	    return (false);
  	} else {
  		Cdisplay.setTextSize(1);
  		Cdisplay.setTextColor(WHITE);
  		Cdisplay.setCursor(0, 0);
  		Serial.println(F("SSD1306 Inicializado"));
  		giveMutex();
  		return (true);
  	}	

}

void brwDisplay1306::display() {
	//waitMutex (0);
	takeMutex();
	Cdisplay.display();
	giveMutex();

}

void brwDisplay1306::clear () {
	//waitMutex (0);
    takeMutex();
	Cdisplay.clearDisplay();
	Cdisplay.display ();
	giveMutex();

}



void brwDisplay1306::setTextSize(int size) {
	//waitMutex (0);
    takeMutex ();
	Cdisplay.setTextSize(size);
    giveMutex();
}

void brwDisplay1306::setTextColor(int color) {
	waitMutex (0);
    takeMutex();
	Cdisplay.setTextColor(color);
    giveMutex();
}


void brwDisplay1306::setCursor(int x, int y) {
	//waitMutex (0);
	takeMutex();
	Cdisplay.setCursor(x, y);
	giveMutex ();
}

void brwDisplay1306::print(String s) {
	//waitMutex (0);
	takeMutex();
	Cdisplay.print(s);
	giveMutex();
}


void brwDisplay1306::escreveLinha (String s) {
	//waitMutex (0);
	takeMutex();
	Cdisplay.println(s);
	Cdisplay.display ();
	giveMutex();
}

