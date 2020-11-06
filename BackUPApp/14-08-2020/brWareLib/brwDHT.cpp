
#include "brwDHT.h"



//------------------------------------------------------------------------------------------
//
//  Construtor da Classe para sensor de Temperatura DHT 
//
//-------------------------------------------------------------------------------------------
brwDHT::brwDHT () {

}


void brwDHT::initDHT (int tp, int pin, int inter) {

	setPinDHT (pin);
  Serial.println ("Pino DHT .........: " + String (pin) );
  Serial.println ("Tipo DHT .........: " + String (tp) );
	static DHT dht(tp, pin);

  
	//dht.begin();
  //pdht -> begin ();
  dht.begin ();
  intervaloLeitura = inter * 1000; // intervalo entre leituras em segundos
  pdht = &dht;

	xTaskCreate(&brwDHT::staticDHTLoop, "DHTLoop", 4096, (void*) this, 4, NULL); 

}




void brwDHT::setPinDHT (int p) {

	pinoDHT = p;
}



void brwDHT::leDHT (DHT* pdht) {
	String msg;
	static uint32_t cntComunica = 0;
	static float t_old, h_old;

    Serial.println ("Vai ler temperatura");
    //temperatura = pdht -> readTemperature();
    //umidade = pdht -> readHumidity();
    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();
    
    
   
    msg = String (temperatura,2);
    strncpy (strT, msg.c_str(), sizeof (strT));
    msg = String (umidade, 2);
    strncpy (strU, msg.c_str(), sizeof (strU));
    
    if ((strT == "nan") || (strU == "nan")) {
      //Serial.println ("*** ERRO DE LEITURA DHT22 ***");
      temperatura = t_old;
      umidade = h_old;
      msg = String (temperatura,2);
      strncpy (strT, msg.c_str(), sizeof (strT));
      msg = String (umidade, 2);
      strncpy (strU, msg.c_str(), sizeof (strU));
    } else {

      t_old = temperatura;
      h_old = umidade;
    }
    

}


bool brwDHT::getAlarmeT () {

	return (stuAlarmeTemperatura);
}


bool brwDHT::getAlarmeU () {

	return (stuAlarmeUmidade);

}

float brwDHT::get_fTemperatura () {

return (temperatura);

}

float brwDHT::get_fUmidade () {

return (umidade);

}
 

char* brwDHT::get_cTemperatura () {

return (strT);

}

char* brwDHT::get_cUmidade () {

return (strU);

}        


//------------------------------------------------------------------------------------------
//
//  --------    Monitora estado da rede Ethernet
//
//------------------------------------------------------------------------------------------


void brwDHT::DHTLoop () {

  
  while (true) {
    
    vTaskDelay(pdMS_TO_TICKS(intervaloLeitura)); // temp que verifica rede
    leDHT (pdht);

    
  }
  
}

void brwDHT::staticDHTLoop (void *pvParameter) {

    brwDHT* edht = reinterpret_cast<brwDHT*>(pvParameter); //obtain the instance pointer
    edht->DHTLoop(); //dispatch to the member function, now that we have an instance pointer

}


