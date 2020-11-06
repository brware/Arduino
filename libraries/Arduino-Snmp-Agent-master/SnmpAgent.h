/*
 * SnmpAgent.h - A very simple SNMP Agent Library for Arduino.
 * Copyright (C) 2017 Mikael Schultz <mikael@dofiloop.com>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */
 
#ifndef SNMPAGENT_H
#define SNMPAGENT_H

#define COMMUNITY_MAX_LENGTH    8
#define UDP_PACKET_MAX_LENGTH   64

#define DEFAULT_SNMP_PORT     161
#define DEFAULT_SNMP_VERSION  0 // SnmpAgent only support SNMP version 1

#define MAX_SNMP_VALUES 5

#include "Arduino.h"
#include "Udp.h"
#include "SnmpMessage.h"
#include "SnmpPDU.h"
#include "SnmpTypes.h"

// DofiLoop Private MIB = 1.3.6.1.4.1.49701

const char sysDescr_oid[] PROGMEM       = "1.3.6.1.2.1.1.1.0";    // read-only OctetString
const char sysUpTime_oid[] PROGMEM      = "1.3.6.1.2.1.1.3.0";    // read-only OctetString
const char sysContact_oid[] PROGMEM     = "1.3.6.1.2.1.1.4.0";    // read-only OctetString
const char sysName_oid[] PROGMEM        = "1.3.6.1.2.1.1.5.0";    // read-only OctetString
const char sysLocation_oid[] PROGMEM    = "1.3.6.1.2.1.1.6.0";    // read-only OctetString

const char usrValue_oid[] PROGMEM       = "1.3.6.1.4.1.49701.1.1.0";

typedef struct SnmpValue {
    uint8_t type = 0;
    void *value;
};

class SNMPAgent {
    public:
        void begin();
        void listen();

        bool SetCommunity(char *value PROGMEM);

        void SetContact(char *value PROGMEM) { sysContact_value = value; };
        void SetDescription(char *value PROGMEM) { sysDescr_value = value; };
        void SetLocation(char *value PROGMEM) { sysLocation_value = value; };
        void SetSystemName(char *value PROGMEM) { sysName_value = value; };
        bool SetValue(uint8_t index, char *value);
        bool SetValue(uint8_t index, uint32_t *value);

    private:
        char      community[COMMUNITY_MAX_LENGTH];
        uint16_t  port;

        unsigned char packetBuffer[UDP_PACKET_MAX_LENGTH];

        // SNMP System Values
        char      *sysDescr_value PROGMEM;
        char      *sysContact_value PROGMEM;
        char      *sysName_value PROGMEM;
        char      *sysLocation_value PROGMEM;
        SnmpValue snmpValues[MAX_SNMP_VALUES];

        bool      ParseSnmpPacket(int packetSize);
        bool      IsAuthenticated();
        bool      ParsePDU();
        bool      ProcessPDU();
        bool      SendResponse();
        int8_t    OidExists(char *oid);
};

#endif
