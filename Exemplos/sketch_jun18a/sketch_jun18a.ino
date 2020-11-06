
//#include <brwDHT.h> // definicoes do display estão no cpp

#include <DHT.h>


//brwDHT dht;

#define PINO 33


float temperatura;
float umidade;
char strT [20]; // temperatura em String 
char strU [20]; // Umidade em String   

DHT dht22 (PINO, DHT22);

void setup() {

  digitalWrite (15,LOW);
  
  Serial.begin(115200);
  Serial.println ("******* INICIO do SETUP **************");

  

  //dht.initDHT (3);
  dht22.begin ();

  Serial.println ("******* FIM do SETUP **************");
}




void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(pdMS_TO_TICKS(3000));
  // leDHT ();
  umidade = dht22.readHumidity();
  temperatura = dht22.readTemperature();

  Serial.print(F("Humidity: "));
  Serial.println(umidade);
  Serial.print(F("Temperatura: "));
  Serial.println(temperatura);
  float hic = dht22.computeHeatIndex(temperatura, umidade, false);
  Serial.print(F("Temperatura (Indice??): "));
  Serial.println(hic);

}



void leDHT () {
  String msg;
  static uint32_t cntComunica = 0;
  static float t_old, h_old;

    
    umidade = dht22.readHumidity();
    temperatura = dht22.readTemperature();
    
    
   
    msg = String (temperatura,2);
    strncpy (strT, msg.c_str(), sizeof (strT));
    msg = String (umidade, 2);
    strncpy (strU, msg.c_str(), sizeof (strU));

    Serial.println ("temperatura : " + String(temperatura,2));
    
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
    

}
