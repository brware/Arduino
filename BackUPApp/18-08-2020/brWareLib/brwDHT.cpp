
#include "brwDHT.h"





//------------------------------------------------------------------------------------------
//
//  Construtor da Classe para sensor de Temperatura DHT 
//
//-------------------------------------------------------------------------------------------

#define PINO 33




brwDHT::brwDHT (brwSpiffs* Spiffs) {

  pSpifss = Spiffs;

}


void brwDHT::initDHT (int pin, int modelo, int inter) {

  static DHT dht (pin, modelo);

  getDhtConfig ();

  printDhtCfg ();

  //setMaxT ("21.00");
  //setDhtConfig ();

  pdht = &dht;

  pdht -> begin ();
  Serial.println ("Iniciou Classe DHT");
  intervaloLeitura = inter * 1000; // intervalo entre leituras em segundos


	xTaskCreate(&brwDHT::staticDHTLoop, "DHTLoop", 4096, (void*) this, 4, NULL); 

}

//------------------------------------------------------------------------------------------
//
//  --------    Task de Leitura do DHT
//
//------------------------------------------------------------------------------------------


void brwDHT::DHTLoop () {

  
  while (true) {
    
    vTaskDelay(pdMS_TO_TICKS(intervaloLeitura)); // temp que verifica rede
    leDHT ();

    
  }
  
}

void brwDHT::staticDHTLoop (void *pvParameter) {

    brwDHT* edht = reinterpret_cast<brwDHT*>(pvParameter); //obtain the instance pointer
    edht->DHTLoop(); //dispatch to the member function, now that we have an instance pointer

}



//------------------------------------------------------------------------------------------
//
//  --------    Leitura do DHT
//
//------------------------------------------------------------------------------------------



void brwDHT::leDHT () {
	String msg;
	static uint32_t cntComunica = 0;
	static float t_old, h_old;

    
    umidade = pdht -> readHumidity();
    temperatura = pdht -> readTemperature();
    
    
   
    msg = String (temperatura,2);
    strncpy (strT, msg.c_str(), sizeof (strT));
    msg = String (umidade, 2);
    strncpy (strU, msg.c_str(), sizeof (strU));

    //Serial.println ("temperatura : " + String(temperatura,2));
    
    if ((isnan (temperatura)) || (isnan(umidade))) {
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
  
  verificaAlarme ();  

}






//------------------------------------------------------------------------------------------
//
//  --------    verifica se esta em alarme
//
//------------------------------------------------------------------------------------------

bool brwDHT::verificaAlarme () {

if (temperatura != 0.0)
   if ((temperatura < String (minT).toFloat()) || (temperatura > String (maxT).toFloat())) 
        stuAlarmeTemperatura = true;
    else   
        stuAlarmeTemperatura = false;
     
  
if (umidade != 0.00)
  if ((umidade < String (minU).toFloat()) || (umidade > String (maxU).toFloat()))
      stuAlarmeUmidade = true;
  else 
      stuAlarmeUmidade = false;  

}


void brwDHT::setMinT (char* var) {

 strncpy (minT, var, sizeof (minT));

}


void brwDHT::setMaxT (char* var) {

 strncpy (maxT, var, sizeof (maxT));

}


void brwDHT::setMinU (char* var) {

 strncpy (minU, var, sizeof (minU));

}


void brwDHT::setMaxU (char* var) {

 strncpy (maxU, var, sizeof (maxU));

}


bool brwDHT::getAlarmeUorT () {

  if ((stuAlarmeTemperatura) || (stuAlarmeUmidade)) 
    return (true);
  else
    return (false);

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
// ------    Le arquivo de cogiguracao do DHT
//
//------------------------------------------------------------------------------------------

bool brwDHT::getDhtConfig () {

  File wFile;
  char st [20];
  int i;
 
  if(SPIFFS.exists("/dht.cfg")) { 

    wFile = SPIFFS.open("/dht.cfg", "r");
    if(!wFile){
        Serial.println ("Não foi possivel abrir o arquivo de configuração");
        return (false);  
    }
    pSpifss -> leLinha (& wFile, st);
    strncpy (minT, st, sizeof (minT));
    vTaskDelay(pdMS_TO_TICKS(50)); 
    pSpifss -> leLinha (& wFile, st);
    strncpy (maxT, st, sizeof (maxT));
    vTaskDelay(pdMS_TO_TICKS(50));
    pSpifss -> leLinha (& wFile, st);
    strncpy (minU, st, sizeof (minU));
    vTaskDelay(pdMS_TO_TICKS(50));
    pSpifss -> leLinha (& wFile, st);
    strncpy (maxU, st, sizeof (maxU));
    vTaskDelay(pdMS_TO_TICKS(50));
    
    wFile.close ();
    return (true);
  } else {
      //Serial.println ("HW: leu cfg Default");
      setDefautConfig ();  
      setDhtConfig ();

      return (true);    
  }


}


//------------------------------------------------------------------------------------------
//
// ------    Grava arquivo de cogiguracao do DHT Alarme
//
//------------------------------------------------------------------------------------------

bool  brwDHT::setDhtConfig () {

    File wFile;
    char st [20];
    int i;
    
    //pSpifss -> listDir("/", 0); 

    wFile = SPIFFS.open("/dht.cfg", "w");
    if(!wFile){
        Serial.println ("Não foi possivel (abrir) salvar o arquivo de configuração");
        return (false);  
    }

    pSpifss -> escreveLinha (& wFile, minT);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pSpifss -> escreveLinha (& wFile, maxT);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pSpifss -> escreveLinha (& wFile, minU);
    vTaskDelay(pdMS_TO_TICKS(100)); 
    pSpifss -> escreveLinha (& wFile, maxU);
    vTaskDelay(pdMS_TO_TICKS(100)); 

    wFile.close ();

    return (true);

  
}

//------------------------------------------------------------------------------------------
//
// ------    Grava arquivo de cogiguracao do Harware
//
//------------------------------------------------------------------------------------------
void  brwDHT::setDefautConfig () {

  strcpy (minT, "16.00");
  strcpy (maxT, "26.00");
  strcpy (minU, "30.00");
  strcpy (maxU, "99.00");

}

//------------------------------------------------------------------------------------------
//
// ------    Imprime arquivo de cogiguracao do Harware
//
//------------------------------------------------------------------------------------------

void  brwDHT::printDhtCfg () {
  
  Serial.println (minT);
  Serial.println (maxT);
  Serial.println (minU);
  Serial.println (maxU);

  

}
