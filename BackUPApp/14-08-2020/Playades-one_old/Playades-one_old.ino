



#include <brwSpiffs.h>
#include <brwCfgEth.h>
#include <brwMqttPlayOne.h>
#include <brwDisplay1306.h>
#include <DHT.h>
#include <Ethernet.h>

// PLYADES-ONE
#define DHTPIN          33
//#define DHTPIN          21
#define DHTTYPE         DHT22
// Configuração default
#define IP_FIXO false
#define DEFAULT_IP "192.168.15.15"
#define DEFAULT_MK "255.255.255.0"
#define DEFAULT_GW "192.168.15.200"
#define DEFAULT_DN "192.168.15.200"
#define MIN_T 16
#define MAX_T 26
#define MIN_U 10
#define MAX_U 100
/*
#define MQTT_USAR false
//#define MQTT_HOST "162.241.88.226"
//#define MQTT_HOST "162.241.88.0"
#define MQTT_HOST "brware.com.br"
#define MQTT_PORT 1883
#define MQTT_USER "brware" 
#define MQTT_PWD  "SQRT(pi)!=314"
*/


typedef struct {

  String minT; // temperatura minima
  String maxT; // Temperatura Maxima
  String minU; // Umidade Minima
  String maxU; //Umidade Maxima
  
} s_alm;


//
// Variaveis Globais
//
//Indicam o Core que a Task é executada
static uint8_t taskCoreZero = 0;//variables that indicate the core
static uint8_t taskCoreOne  = 1;

// Variaveis que vão ser Struct
TaskHandle_t tk_webServerLoop; //handle for  Loop Web Server

float temperatura = 0;
float umidade = 0;
String strT;
String strU;
String strIP;
uint32_t  intervaloLeitura = 5000;
bool stuAlarmeTemperatura = false;
bool stuAlarmeUmidade = false;
int timeRelay [4];




//
// Instancias das classes
//
brwCfgEth eth; // Rede Ethernet
//brwMqttPlayOne mqtt;
brwSpiffs efs; // Sistema de arquivos 
brwDisplay1306 d;
DHT dht(DHTPIN, DHTTYPE);
EthernetServer  serverWEB(80);



// variaveis Globais
s_alm vAlm;

// Variaveis de leitura do DHT22
//String Temp; // temperatura Atual
//String Umid; // Umidade Atual



void setup() {
  pinMode (15, OUTPUT); // pino do BUZZER
  digitalWrite(BUZZER, LOW); 
  
  Serial.begin(115200);
  Serial.println ("********************************");
  Serial.println ("- Inicio do Setup");
  Serial.println ("********************************");
  
  d.init (); // Inicia pisplay
  d.clear ();
  d.setCursor(0, 0); 
  d.setTextSize(2);
  d.escreveLinha ("PlayadeOne");
  d.escreveLinha (" V. 1.0.1");
  vTaskDelay(pdMS_TO_TICKS(5000));
  d.clear ();
  d.setCursor(0, 0);
  d.escreveLinha (" (TB)Forte");
  d.setTextSize(1);
 
  efs.openFS();
  //efs.formatFS(); // para testes
  //Serial.println ("***** Formatando");
  efs.listDir("/", 0);
  existeCfg (); // configuração de alarmes
  Serial.println ("********************************");
  printCfg ();
  
  

  // Ethernet
  // eth.setUsarIpFixo (true); // para testes
  if (eth.getUsarIpFixo ()) {
    Serial.println("-- >> IP FIXO");
    Serial.println("IP..... :" +  eth.getIP ());
    Serial.println("Mascara :" +  eth.getMascara ());
    Serial.println("GateWay :" +  eth.getGateWay ());
    Serial.println("DNS     :" +  eth.getDNserver ());
    d.escreveLinha ("");
    d.escreveLinha ("IPFix: " +  eth.getIP ());
      
    
    eth.beginEth (eth.getIP (), eth.getMascara (), eth.getGateWay (), eth.getDNserver ());
    //eth.beginEth (DEFAULT_IP, DEFAULT_MK, DEFAULT_GW, DEFAULT_DN);
    
  }  
  else {
        eth.beginEth (); 
        d.escreveLinha ("");
        d.escreveLinha ("DHCP: " + eth.getIP ());     
             
        
  }
  
  d.escreveLinha ("");    
  d.escreveLinha ("MAC: " + eth.getMacAddress ());
  d.escreveLinha ("");
  strIP = eth.getIP ();
  
  eth.actBeep (2000, 1);
  printCfg ();

  Serial.println("ETH - Iniciado");

  eth.actBeep (500, 2);
 
  

   
 /* 
  //mqtt.setMqttUsar (true); // para testes
  if (mqtt.getMqttUsar ()) {
    mqtt.configMqtt (eth.getMacAddress ());
    d.escreveLinha ("MQTT: ON");     
  }
  else {
    Serial.println("MQTT - Desabilitado");  
    d.escreveLinha ("MQTT: OFF");
  }

  */

 
  iniDhtAlrm (); // DHT & alarm
  
  
  criaTasks ();
  
  
  printCfg ();
  salvaCfg ();
  
  Serial.println ("********************************");
  Serial.println ("- Fim do Setup");
  Serial.println ("********************************");
  //d.escreveLinha (" *** Fim do Setup ***");
  eth.actBeep (500, 3);
  
  

  
}

void loop() {

  vTaskDelay(pdMS_TO_TICKS(intervaloLeitura));
  displayTempUmi ();
  strIP = eth.getIP ();

}


void boot () {

 // mqtt.desconectar ();
  ESP.restart ();
  
}


void displayTempUmi () {

  d.clear ();
  d.setCursor(0, 0);
  d.setTextSize(2);
  d.escreveLinha ("T: " + strT + " C");
  d.setTextSize(2);
  d.escreveLinha ("U: " + strU + " %");
  d.setTextSize(1);
  d.escreveLinha ("");
  d.escreveLinha ("    T: " + vAlm.minT + " <-> " + vAlm.maxT);
  d.escreveLinha ("    U: " + vAlm.minU + " <-> " + vAlm.maxU);
  d.display ();
  

}
