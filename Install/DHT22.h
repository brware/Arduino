//------------------------------------------------------------------------------------------
//
//  Classe Para Sensor de Temperatura DHT 22
//-------------------------------------------------------------------------------------------

#ifndef brwDHT22_h
#define brwDHT22_h

#include <Arduino.h>
#include <DHT.h>


class brwDHT22
{
    public:

        brwDHT22 ();

        
        

    private:
    	int pinoDHT;
    	int typeDHT;
    	//DHT dht(DHTPIN, DHTTYPE);
        
        
};

#endif