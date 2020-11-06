# Arduino-Snmp-Agent

The Arduino SnmpAgent is a very small and feature slim implementation of the SNMP (Simple Network Management Protocol) suitable for the limited resources of the Arduino. It is also more Arduino user friendly than most other SNMP libraries for Arduino out there.

### Limitations

Due to its slimness it comes with these limitations:

* SnmpAgent only responds to SNMP Get Requests (no traps or walks)
* Only one OID per request is allowed
* It is compleatly read-only, so no SNMP Set Requests

## An example

This is a simple example using two values that can be retrieved via an SNMP Get Request.
There is one character array (str_value) and one 32 bit integer value (int_value)
that can be retrieved with snmpget.
For a better example, have a look at SnmpAgentDemo.ino

```c
#include "SnmpAgent.h"
#include <UIPEthernet.h>

SNMPAgent SnmpAgent;

static byte mac[] = {0xCC, 0x1E, 0xCC, 0x1E, 0x00, 0x01};

// These are the values that will be available via SNMP
char str_value[12];
uint32_t int_value = 0;

void setup() {

  // put your setup code here, to run once:
  Ethernet.begin(mac);
  
  // Start UDP Server at port 161
  SnmpAgent.begin();

  // Setup default SNMP Agent information
  SnmpAgent.SetCommunity(PSTR("public"));
  SnmpAgent.SetDescription(PSTR("Force Sensor"));
  SnmpAgent.SetContact(PSTR("vader@deathstar.com"));
  SnmpAgent.SetLocation(PSTR("Death Star"));
  SnmpAgent.SetSystemName(PSTR("arduino"));

  // Setup custom SNMP values (1.3.6.1.4.1.49701.1.X.0)
  SnmpAgent.SetValue(1, str_value);  // 1.3.6.1.4.1.49701.1.1.0
  SnmpAgent.SetValue(2, &int_value); // 1.3.6.1.4.1.49701.1.2.0

  // Fill the string with something useful
  strcpy(str_value, "Hello World");
}

void loop() {
  // put your main code here, to run repeatedly:
  SnmpAgent.listen();

  // Just set this to something useful
  int_value = millis();
}
```

The custom SNMP values that you can use in your program are set using the SnmpAgent.SetValue() method.<br>
SetValue() maps a variable to an OID.

### Get the string

To get the string value of the variable *str_value*, run the snmpget command with the OID matching the first SnmpAgent custom value (that is 1.3.6.1.4.1.49701.1.**1**.0):

```bash
computer:~ mikael$ snmpget -v1 -c public 192.168.0.123 1.3.6.1.4.1.49701.1.1.0
SNMPv2-SMI::enterprises.49701.1.1.0 = STRING: "Hello World"
```
### Get the integer

To get the integer value of the variable *int_value*, run the snmpget command with the OID matching the second SnmpAgent custom value (that is 1.3.6.1.4.1.49701.1.**2**.0):

```bash
computer:~ mikael$ snmpget -v1 -c public 192.168.0.123 1.3.6.1.4.1.49701.1.2.0
SNMPv2-SMI::enterprises.49701.1.2.0 = INTEGER: 772737625
```
