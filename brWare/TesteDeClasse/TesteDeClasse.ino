


//********************************************************************
// *****  DEFINICÃO DO MODELO DE HARWARE EM: brwHardwareV2.h *********
//******************************************************************** 
//#include <Ethernet.h>

#include <brwAuxiliares.h>
#include <brwMutex.h>
#include <brwSpiffs.h>
#include <brwDisplay1306.h> // definicoes do display estão no cpp
#include <brwDHT.h> // definicoes do display estão no cpp
#include <brwHardwareCfg.h>
#include <brwHardwareApp.h>
#include <brwEthCfg.h>
#include <brwMqtt.h>
//#include <brwWSPlayade01.h>


//#include <EMailSender.h>



//#include <brwMqtt.h>

//#include <brwDisplay1306.h>




brwSpiffs brwSpi; // Tambem definido em brwHardwareV2.h
brwHardwareCfg hw_cfg ("playade01", "1.0.2","1.0", &brwSpi);
brwHardwareApp hw_app (&hw_cfg);
brwEthCfg Eth;
brwDisplay1306 dpy;
brwMqtt mqtt;
//brwDHT dht1 (&brwSpi);
//brwWSPlayade01 wsP;



//brwMqtt MQ;


//uint8_t connection_state = 0;
//uint16_t reconnect_interval = 10000;
//EMailSender emailSend("brware.esp32@gmail.com", "@Antares2#");




void setup() {
	int i;
	char st [20];
	

	Serial.begin(115200);
	Serial.println ("******* INICIO do SETUP **************");
	
	
  	//brwSpi.formatFS();
  	brwSpi.beginFS();
  	brwSpi.listDir("/", 0);
    
	hw_cfg.iniHardware ();
	brwSpi.listDir("/", 0);

	//dht1.initDHT (hw_cfg.getS01 (), DHT22, 3);
	//brwSpi.listDir("/", 0);

	

	
	//Eth.beginEth (&hw_cfg);
	
	Eth.printEthCfg ();

	//mqtt.beginMQTT (&hw_cfg, &hw_app, (char*)"brware.com.br", false,(char*) "netuno");

	//dpy.initDisplay ();



	//dpy.clear ();
	//dpy.setTextSize(2);
  	//dpy.setCursor(0, 0);
  	//dpy.escreveLinha ("brWare V1");
  	//dpy.setTextSize(1);
  	//dpy.escreveLinha ("T: " + String (dht1.get_cTemperatura ()));
  	//dpy.escreveLinha ("Classe: Display");

	//vTaskDelay(pdMS_TO_TICKS(100)); 

	
	//wsP.beginWebServer (&Eth, &dht1);

/*
	//Eth.printEthCfg ();

	EMailSender::EMailMessage message;
	

	message.subject = "Teste de Envio ESP32-Eth";
    message.message = "Ola, este é um teste de envio do ESP32<br>Se chegou aqui esta ok.<br>www.brware.com.br";

    EMailSender::Response resp = emailSend.send("vanderpr@gmail.com", message);

    Serial.println("Sending status: ");

     Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
*/

	Serial.println ("******* FIM do SETUP **************");
	
}

void loop () {

	
}
