/*********************************************************************

ROAD MAP

- Testar se configução do servidor difere da gravada
- Gravar Status dos reles
- Classe WIFI
- Classe DS3231




***********************************************************************/
#define SUCRES

#include <brw_two.h>

#include <brwSpiffs.h>
//#include <brwCfgEth.h>
#include <brwMqttSucres.h>
#include <Ethernet.h>

// Configuração default
#define IP_FIXO false
#define DEFAULT_IP "192.168.15.16"
#define DEFAULT_MK "255.255.255.0"
#define DEFAULT_GW "192.168.15.200"
#define DEFAULT_DN "8.8.8.8"

/*
#define MQTT_USAR true
#define MQTT_HOST "162.241.88.226"
//#define MQTT_HOST "brware.com.br"
#define MQTT_PORT 1883
#define MQTT_USER "brware" 
#define MQTT_PWD  "SQRT(pi)!=314"
*/

#define MQTT_USAR true
#define MQTT_HOST "192.168.15.182"
//#define MQTT_HOST "brware.com.br"
#define MQTT_PORT 1883
#define MQTT_USER "brware" 
#define MQTT_PWD  "SQRT(pi)!=314"



#define TMP_RL01 1
#define TMP_RL02 1
#define TMP_RL03 180  
#define TMP_RL04 1

//
// Variaveis Globais
//
//Indicam o Core que a Task é executada
static uint8_t taskCoreZero = 0;//variables that indicate the core
static uint8_t taskCoreOne  = 1;

bool lerCfgBD = false;

// Variaveis que vão ser Struct
TaskHandle_t tk_webServerLoop; //handle for  Loop Web Server


//
// Instancias das classes
//
//brwCfgEth eth; // Rede Ethernet
brwMqttSucres sc;
brwSpiffs efs; // Sistema de arquivos 
EthernetServer  serverWEB(80);


void setup() {
 Serial.begin(115200);
  Serial.println ("********************************");
  Serial.println ("- Inicio do Setup");
  Serial.println ("********************************");
 
  efs.openFS();
  efs.formatFS(); // para testes
  efs.listDir("/", 0);
  existeCfg (); // configuração de alarmes
  //verificaConfBD (); 
  


  

  // Inicia parametros da Ethernet
  if (sc.getUsarIpFixo ()) {
    //sc.beginEth ("192.168.15.10", "255.255.255.0", "192.168.15.200", "192.168.15.200");
    Serial.println ("---> Conexão IP FIXO");
    sc.beginEth (sc.getIP(), sc.getMascara (), sc.getGateWay (), sc.getDNserver ());
    }
  else {
        Serial.println ("---> Conexão DHCP");
        sc.beginEth ();  
        printCfg ();    
  }
  
  sc.beginEth ();  


  sc.actBeep (2000, 1);
  //printCfg ();

  Serial.println("ETH - Iniciado");

  sc.actBeep (200, 2);
 
  

   

  sc.setMqttUsar (true); // para testes
  if (sc.getMqttUsar ())
    sc.configMqtt (sc.getMacAddress ());
  else
    Serial.println("MQTT - Desabilitado");  

  sc.actBeep (100, 3); 

  criaTasks ();
  /*
  if (lerCfgBD)
    getCfgBD ();
  */

  //digitalWrite (S01, LOW);

  Serial.println ("********************************");
  Serial.println ( "- Fim do Setup" );
  Serial.println ("********************************");

/*
  int i;
  for (i = 1; i < 8; i++) {
    sc.setRele (i, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  for (i = 1; i < 8; i++) {
    sc.setRele (i, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));

  }

*/


}

void loop() {
  
 /*  vTaskDelay(pdMS_TO_TICKS(1000));
   Serial.print ("Entrada S01: ");
   Serial.println (digitalRead (S01));
*/
}

void boot () {

  sc.desconectar ();
  ESP.restart ();
  
}

void getCfgBD () {
  int i;

  //sc.sendTopicoConfigPub (sc.getMacAddress () + ";REDE");
  sc.getConfigPub ("REDE");
  i = 0;
  while (sc.enviouCfgSub) {
    vTaskDelay(pdMS_TO_TICKS(100));
    i++;
    if (i > 50)
      break;
  }

  

  //sc.sendTopicoConfigPub (sc.getMacAddress () + ";MQTT");
  sc.getConfigPub ("MQTT");
  i = 0;
  while (sc.enviouCfgSub){
    vTaskDelay(pdMS_TO_TICKS(100));
    i++;
    if (i > 50)
      break;
  }

  //sc.sendTopicoConfigPub (sc.getMacAddress () + ";RELE");
  sc.getConfigPub ("RELE");
  i = 0;
  while (sc.enviouCfgSub){
    vTaskDelay(pdMS_TO_TICKS(100));
    i++;
    if (i > 50)
      break;
  } 


  salvaCfg (); // para testes
  

  
}
