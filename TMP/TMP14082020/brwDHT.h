//------------------------------------------------------------------------------------------
//
//  Classe Para Sensor de Temperatura DHT 22
//-------------------------------------------------------------------------------------------

#ifndef brwDHT22_h
#define brwDHT22_h

#include <Arduino.h>
#include <DHT.h>

#define DHT22 22
#define PINO 33



class brwDHT
{
    public:

        brwDHT ();
        void initDHT (int tp, int pin, int inter);
        
        bool getAlarmeT ();
        bool getAlarmeU ();
        float get_fTemperatura ();
        float get_fUmidade ();
        char* get_cTemperatura ();
        char* get_cUmidade ();
        
        
        
        
        DHT* pdht; 
        
        


    private:
    	void setPinDHT (int p);
    	void leDHT (DHT* pdht);

        float temperatura;
        float umidade;
        char strT [20]; // temperatura em String 
        char strU [20]; // Umidade em String   

    	int pinoDHT; // pino que esta ligado o DHT
    	uint32_t  intervaloLeitura = 5000; // Intervalo entre leituras
    	bool stuAlarmeTemperatura = false;
		bool stuAlarmeUmidade = false;

		static void staticDHTLoop  (void *pvParameter);
		void DHTLoop (); // lop que checha conexão Ethernet (** tem de estar numa task)
         
		
    	
        
        
};

#endif