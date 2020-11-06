//------------------------------------------------------------------------------------------
//
//  Classe Para Sensor de Temperatura DHT 22
//-------------------------------------------------------------------------------------------

#ifndef brwDHT_h
#define brwDHT_h

#include <Arduino.h>
#include "brwSpiffs.h"
#include <DHT.h>




class brwDHT
{
    public:

        brwDHT (brwSpiffs* Spiffs);
        void initDHT (int pin, int modelo, int inter);
        
        bool getAlarmeUorT (); // se tem alarme na temperatura ou umidade
        bool getAlarmeT ();
        bool getAlarmeU ();

        float get_fTemperatura ();
        float get_fUmidade ();
        char* get_cTemperatura ();
        char* get_cUmidade ();

        char* getMinT ();
        char* getMaxT ();
        char* getMinU ();
        char* getMaxU ();

        void setMinT (char* var);
        void setMaxT (char* var);
        void setMinU (char* var);
        void setMaxU (char* var);

        bool getDhtConfig ();
        bool setDhtConfig ();
        void printDhtCfg ();


    private:
    	
    	void leDHT ();
        bool verificaAlarme ();

        void setDefautConfig ();
        


        float temperatura;
        float umidade;
        char strT [10]; // temperatura em String 
        char strU [10]; // Umidade em String   

        char minT [10]; // menor temperatura permitida
        char maxT [10]; // maior temperatura permitida
        char minU [10]; // menor umidade permitida
        char maxU [10]; // maior umidade permitida

    	int pinoDHT; // pino que esta ligado o DHT
    	uint32_t  intervaloLeitura = 5000; // Intervalo entre leituras
    	bool stuAlarmeTemperatura = false;
		bool stuAlarmeUmidade = false;

		static void staticDHTLoop  (void *pvParameter);
		void DHTLoop (); // lop que checha conexão Ethernet (** tem de estar numa task)
         
		DHT* pdht;
        brwSpiffs* pSpifss; // classe para arquivo
    	
        
        
};

#endif