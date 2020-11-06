#include <brwAuxiliares.h>
#include <brwMutex.h>
#include <brwSpiffs.h>
#include <brwDisplay1306.h> // definicoes do display estão no cpp       
#include <brwHardwareCfg.h>
#include <brwHardwareApp.h>
#include <brwEthCfg.h>

#include <brwSendMail.h>

brwSpiffs brwSpi; // Tambem definido em brwHardwareV2.h
brwHardwareCfg hw_cfg ("playade02", "1.0.1","1.0", &brwSpi);
brwHardwareApp hw_app (&hw_cfg);
brwEthCfg Eth;
brwDisplay1306 dpy;
brwSendMail sm;

void setup() {
  Serial.begin(115200);
  Serial.println ("******* INICIO do SETUP **************");

 
  

  brwSpi.beginFS();
      
  hw_cfg.iniHardware ();
  hw_app.iniHardware ();

  hw_app.actBeep (1000, 1);
  


  
  dpy.initDisplay ();
  dpy.clear ();
  dpy.setTextSize(1);
  dpy.setCursor(0, 0);
  dpy.escreveLinha ("Aguarde...");
  dpy.escreveLinha ("  ");
  dpy.escreveLinha ("Iniciando...");
  

  Eth.beginEth (&hw_cfg);
  dpy.escreveLinha ("Rede: OK");

  //wsP.beginWebServer (&Eth, &dht1);
  //dpy.escreveLinha ("WebServer: OK");

  // Task Buzzer Alarme
  


  vTaskDelay(pdMS_TO_TICKS(2000));


  displayCfg (); // mostra confirurações de rede eth

  Serial.println ("******* FIM do SETUP **************");

  Serial.println("Preparing to send email");
  Serial.println();
/*
  sm.setEmailSenderAccount ("brware.esp32@gmail.com");
  sm.setEmailSenderPassword ("@antares2#");
  sm.setEmailSenderServer ("smtp.gmail.com");
  sm.setEmailSenderServerPort (465);
  sm.setEmailSubject ("Teste Playade Two");
  sm.setEmailMessage ("Esta é uma mensagem de teste");

  sm.mailSend ();


  Serial.println("Fim do Email");

*/


}

void loop() {
  // put your main code here, to run repeatedly:

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
