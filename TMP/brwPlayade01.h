//------------------------------------------------------------------------------------------
//
//  Classe de para App Playade01
//
//-------------------------------------------------------------------------------------------



#ifndef brwPlayade01_h
#define brwPlayade01_h

#include "brwSpiffs.h"
#include "brwDHT.h"





class brwPlayade01  : public brwDHT
 {

	public:
		brwPlayade01 ();
		void iniPlayade01 (brwSpiffs* Spiffs);

		bool getPlay01Config ();
      	bool setPlay01Config ();
      	void setPlay01Defaut ();
      	void printPlay01Cfg ();


		void setMinT (char* v);
      	void setMaxT (char* v);
      	void setMinU (char* v);
      	void setMaxU (char* v);

      	char* getMinT ();
      	char* getMaxT ();
      	char* getMinU ();
      	char* getMaxU ();

            // variaveis de rele
            void setR01pulso (bool st);
            void setR02pulso (bool st);
            void setR03pulso (bool st);
            void setR04pulso (bool st);

            bool getR01pulso ();
            bool getR02pulso ();
            bool getR03pulso ();
            bool getR04pulso ();

            void setR01tempo (int t);
            void setR02tempo (int t);
            void setR03tempo (int t);
            void setR04tempo (int t);

            int getR01tempo ();
            int getR02tempo ();
            int getR03tempo ();
            int getR04tempo ();


      	brwSpiffs* pSpiffs; // ponteiro para classe de hardware
      	

	private:

		char minT [20]; // temperatura minima
		char maxT [20]; // Temperatura Maxima
		char minU [20]; // Umidade Minima
		char maxU [20]; // Umidade Maxima

            bool R01pulso; // se o rele vai pulsar usao tempo rele, senão apenas liga e desliga
            bool R02pulso;
            bool R03pulso;
            bool R04pulso;

            int R01tempo;
            int R02tempo;
            int R03tempo;
            int R04tempo;
 

};

#endif