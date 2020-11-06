

#include "brwDisplay1306.h"

Adafruit_SSD1306 Cdisplay ( WIDTH, HEIGHT, &Wire, -1); 



brwDisplay1306::brwDisplay1306 () {


}


bool brwDisplay1306::init () {

	if(!Cdisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
	    Serial.println(F("SSD1306 Não Encontrado"));
	    return (false);
  	} else {
  		Cdisplay.setTextSize(1);
  		Cdisplay.setTextColor(WHITE);
  		Cdisplay.setCursor(0, 0);
  		return (true);
  	}

}

void brwDisplay1306::display() {

	Cdisplay.display();

}

void brwDisplay1306::clear () {

	Cdisplay.clearDisplay();
	Cdisplay.display ();

}



void brwDisplay1306::setTextSize(int size) {

	Cdisplay.setTextSize(size);

}

void brwDisplay1306::setTextColor(int color) {

	Cdisplay.setTextColor(color);

}


void brwDisplay1306::setCursor(int x, int y) {

	Cdisplay.setCursor(x, y);

}

void brwDisplay1306::print(String s) {

	Cdisplay.print(s);

}


void brwDisplay1306::escreveLinha (String s) {

	Cdisplay.println(s);
	Cdisplay.display ();
}


