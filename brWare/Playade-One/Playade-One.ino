
#include <brwAuxiliares.h>
#include <brwMutex.h>
#include <brwSpiffs.h>
#include <brwDisplay1306.h> // definicoes do display estão no cpp
#include <brwDHT.h>         
#include <brwHardwareCfg.h>
#include <brwHardwareApp.h>
#include <brwEthCfg.h>
#include "brwWSPlayade01.h"





brwSpiffs brwSpi; // Tambem definido em brwHardwareV2.h
brwHardwareCfg hw_cfg ("playade01", "1.0.2","1.0", &brwSpi);
brwHardwareApp hw_app (&hw_cfg);
brwEthCfg Eth;
brwDisplay1306 dpy;
brwDHT dht1 (&brwSpi);
brwWSPlayade01 wsP;


void setup() {

  

  Serial.begin(115200);
  Serial.println ("******* INICIO do SETUP **************");

 
  

  brwSpi.beginFS();
      
  hw_cfg.iniHardware ();
  hw_app.iniHardware ();

  hw_app.actBeep (1000, 1);
  
  
  
  
  //Eth.printEthCfg ();

  
  dpy.initDisplay ();
  dpy.clear ();
  dpy.setTextSize(1);
  dpy.setCursor(0, 0);
  dpy.escreveLinha ("Aguarde...");
  dpy.escreveLinha ("  ");
  dpy.escreveLinha ("Iniciando...");
  

  
  dht1.initDHT (hw_cfg.getS01 (), DHT22, 3);
  dpy.escreveLinha ("Sensor: OK");

  //xTaskCreate (bzAlarme, "bzAlarme", 2048, NULL, 4,  NULL);
  //vTaskDelay(pdMS_TO_TICKS(300)); // tempo para task iniciar
  //dpy.escreveLinha ("Alarme: OK");

  Eth.beginEth (&hw_cfg);
  dpy.escreveLinha ("Rede: OK");

  wsP.beginWebServer (&Eth, &dht1);
  dpy.escreveLinha ("WebServer: OK");

    // Task Buzzer Alarme
  


  vTaskDelay(pdMS_TO_TICKS(2000));


  displayCfg (); // mostra confirurações de rede eth

  Serial.println ("******* FIM do SETUP **************");

}

void loop() {


  displayTempUmi ();  

  if (dht1.getAlarmeUorT ()) {
       hw_app.actBeep (300, 2); // beep
       hw_app.actRele (1,600); // aciona rele de siriene
       hw_app.setRele (2, HIGH);
       //vTaskDelay(pdMS_TO_TICKS(2000)); 
        
     } else {
        hw_app.setRele (1, LOW);
        hw_app.setRele (2, LOW);
        //vTaskDelay(pdMS_TO_TICKS(2000));
      }


  vTaskDelay(pdMS_TO_TICKS(2000));
  
}

//*********************************************************************************************
//
//  Task para alarme
//
//*********************************************************************************************

void bzAlarme (void* parameter){
   
  Serial.println ("- Buzzer de alarme");  
  vTaskDelay(pdMS_TO_TICKS(5000)); // tempo para task inicia
  while (true)
    { 

     vTaskDelay(pdMS_TO_TICKS(500)); // tempo para task inicia  
     
      
     if (dht1.getAlarmeUorT ()) {
       // hw_app.actBeep (300, 2); // beep
        //hw_app.actRele (1,600); // aciona rele de siriene
        //hw_app.setRele (2, HIGH);
        vTaskDelay(pdMS_TO_TICKS(2000)); 
        
     } else {
        //hw_app.setRele (1, LOW);
        //hw_app.setRele (2, LOW);
        vTaskDelay(pdMS_TO_TICKS(2000));
      }
     
      
    }

}



//*********************************************************************************************
//
//  Mostra no Display configuração da placa
//
//*********************************************************************************************

void displayCfg () {

  String str;

  dpy.clear ();
  dpy.setTextSize(2);
  dpy.setCursor(0, 0);
  dpy.escreveLinha (hw_cfg.hwApp);
  dpy.escreveLinha ("V: " + String (hw_cfg.infoVersao));
  dpy.escreveLinha ("H: " + String (hw_cfg.infoHw));
  vTaskDelay(pdMS_TO_TICKS(2000));

  dpy.clear ();
  dpy.setTextSize(1);
  dpy.setCursor(0, 0);

  if (Eth.getUsarIpFixo ())
    str = "IP: FIXO";
  else
    str = "IP: DHCP";   
  dpy.escreveLinha (str);
  dpy.escreveLinha ("");

  str = "MAC: " + String (Eth.getMacAddress ());
  dpy.escreveLinha (str);

  str = "IP : " + String (Eth.getIP ());
  dpy.escreveLinha (str);
  dpy.escreveLinha ("");

  str = "MK : " + String (Eth.getMascara ());
  dpy.escreveLinha (str);

  str = "GW : " + String (Eth.getGateWay ());
  dpy.escreveLinha (str);

  str = "DN : " + String (Eth.getDNserver ());
  dpy.escreveLinha (str);

  vTaskDelay(pdMS_TO_TICKS(5000));

  dpy.clear ();

}

//*********************************************************************************************
//
//  Mostra no Display Temperatura e Umidade
//
//*********************************************************************************************

void displayTempUmi () {

  dpy.clear ();
  dpy.setCursor(0, 0);
  dpy.setTextSize(1);
  dpy.escreveLinha ("     T: " + String (dht1.get_cTemperatura ()) + " C");
  dpy.escreveLinha (" ");
  dpy.setTextSize(1);
  dpy.escreveLinha ("     U: " + String (dht1.get_cUmidade ()) + " %");
  dpy.setTextSize(1);
  dpy.escreveLinha ("");
  dpy.escreveLinha ("");
  dpy.escreveLinha (" T: " + String (dht1.getMinT ()) + " <-> " + String (dht1.getMaxT ()));
  dpy.escreveLinha (" U: " + String (dht1.getMinU ()) + " <-> " + String (dht1.getMaxU ()));
  dpy.display ();
  

}
