
#include <brwCfgEth.h>
#include <brwSpiffs.h>

brwCfgEth e;
brwSpiffs efs; // Sistema de arquivos 

void setup() {
  // put your setup code here, to run once:
String mac;


Serial.begin(115200); 
e.iniHardware ();

efs.openFS();
efs.formatFS(); // para testes
Serial.println ("*****************************************");
Serial.println ("");
Serial.println ("MAC ADDRESS: " + e.getMacAddress());
Serial.println ("");
Serial.println ("*****************************************");



}

void loop() {
  // put your main code here, to run repeatedly:

}
