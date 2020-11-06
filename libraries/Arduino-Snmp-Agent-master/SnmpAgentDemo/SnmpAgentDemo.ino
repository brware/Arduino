#include "SnmpAgent.h"
#include <UIPEthernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SimpleTimer.h>

SNMPAgent SnmpAgent;
OneWire oneWire(2);
DallasTemperature Sensors(&oneWire);

static byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF};
char temperature[6];
uint32_t lastMillis = 0;

void setup() {

  // put your setup code here, to run once:
  Serial.begin(115200);
  Sensors.begin();
  Ethernet.begin(mac);
  
  Serial.print("SNMP Agent IP: "); Serial.println(Ethernet.localIP());

  // Start UDP Server at port 161
  SnmpAgent.begin();

  // Setup default SNMP Agent information
  SnmpAgent.SetCommunity(PSTR("public"));             // Password/Community (Read-Only)
  SnmpAgent.SetDescription(PSTR("Force Sensor"));     // 1.3.6.1.2.1.1.1.0
  SnmpAgent.SetContact(PSTR("vader@deathstar.com"));  // 1.3.6.1.2.1.1.4.0
  SnmpAgent.SetLocation(PSTR("Death Star"));          // 1.3.6.1.2.1.1.6.0
  SnmpAgent.SetSystemName(PSTR("arduino"));           // 1.3.6.1.2.1.1.5.0

  // Setup custom SNMP values (1.3.6.1.4.1.49701.1.X.0),
  // where X is a value between 1 and 5 (defined by MAX_SNMP_VALUES in SnmpAgent.h)
  SnmpAgent.SetValue(1, temperature); // 1.3.6.1.4.1.49701.1.1.0
  SnmpAgent.SetValue(2, &lastMillis); // 1.3.6.1.4.1.49701.1.2.0
}

void loop() {
  // put your main code here, to run repeatedly:
  SnmpAgent.listen();

  // Update the temperature variable every 10 seconds
  if (millis() - lastMillis > 10000) {
    Sensors.requestTemperatures(); // Send the command to get temperatures
    dtostrf(Sensors.getTempCByIndex(0), 1, 1, temperature);
    lastMillis = millis();
  }
}
