/*
 * SnmpAgent.cpp - A very simple SNMP Agent Library for Arduino.
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
 *
 * http://www.rane.com/pdf/ranenotes/SNMP_Simple_Network_Management_Protocol.pdf
 */

#include "SnmpAgent.h"
#include <UIPEthernet.h>

EthernetUDP udpSocket;
SnmpMessage snmpMessage;
SnmpPDU snmpPDU;

uint32_t sysUpTime_value;

/*
 *  Start listening on the UDP port 161 and
 *  setup system uptime functionality
 */
void SNMPAgent::begin() 
{
    udpSocket.begin(DEFAULT_SNMP_PORT);       // UDP Port 161

    sysUpTime_value = 0;

    // This code generates a TIMER0_COMPA interrupt
    // whenever the counter value passes 0xAF
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
}

/*
 *  This is the interrupt handler that will keep track 
 *  of the system uptime.
 */
SIGNAL(TIMER0_COMPA_vect)
{
    uint32_t current = millis();
    static uint32_t lastMillis = 0;
    if (current - lastMillis > 1000) {
        sysUpTime_value += 100;
        lastMillis = current;
    }
}

/*
 *  This method checks for incoming UDP packets and 
 *  tries to parse and process the SNMP request
 */
void SNMPAgent::listen() 
{
    // Start process the next available incoming packet
    uint16_t packetSize = udpSocket.parsePacket();
    bool validSnmpPacket = true;
    
    // If there is data available we fill the packetBuffer
    // with the new data just recieved
    if (packetSize > 0 && packetSize < UDP_PACKET_MAX_LENGTH) {

        memset(packetBuffer, 0, UDP_PACKET_MAX_LENGTH);

        udpSocket.read(packetBuffer, packetSize);
        udpSocket.flush();

        // Try to parse incoming UDP packet as an SNMP Message
        if (!ParseSnmpPacket(packetSize)) {
            validSnmpPacket = false;
        }

        // Compare the community string inside the SNMP
        // Message and allow or deny based on result
        if (!IsAuthenticated())
            validSnmpPacket = false;

        // Try to parse the PDU in the SNMP Message
        if (!ParsePDU())
            validSnmpPacket = false;

        // If the SNMP Message and the PDU inside of it
        // is valid we try to retrieve the data requested
        // and reply the SNMP Manager
        if (validSnmpPacket) {
            ProcessPDU();
            
            if (SendResponse()) {
                Serial.println(F("Successfully sent response."));
            }
        }

        // Finish the current UDP session and start a new one
        udpSocket.stop();
        udpSocket.begin(DEFAULT_SNMP_PORT);

    }
}

/*
 *  Well, just sets the community string :)
 */
bool SNMPAgent::SetCommunity(char *value PROGMEM)
{
    if (strlen_P(value) <= COMMUNITY_MAX_LENGTH) {
        strcpy_P(community, value);
        return true;
    }
    else
        return false;
}

/*
 *  Maps a used definable value to an SnmpValue inside
 *  the array snmpValues.
 *  The value is of type char pointer
 */
bool SNMPAgent::SetValue(uint8_t index, char *value)
{
    SnmpValue *snmpValue;
    
    if (index >= 0 && index < MAX_SNMP_VALUES) {
        snmpValue = &snmpValues[index-1];
        snmpValue->type = OctetString;
        snmpValue->value = value;
    }
}

/*
 *  Maps a used definable value to an SnmpValue inside
 *  the array snmpValues.
 *  The value is of type uint32_t pointer
 */
bool SNMPAgent::SetValue(uint8_t index, uint32_t *value)
{
    SnmpValue *snmpValue;
    
    if (index >= 0 && index < MAX_SNMP_VALUES) {
        snmpValue = &snmpValues[index-1];
        snmpValue->type = Integer;
        snmpValue->value = value;
    }
}

/*
 *  This method goes through byte by byte and tries to
 *  build an SNMP Message Object according to RFC 1157
 *  https://www.ietf.org/rfc/rfc1157.txt
 *  
 *  The SNMP Agent only support SNMP version 1.
 */
bool SNMPAgent::ParseSnmpPacket(int packetSize) 
{
    uint8_t index = 0;
    uint8_t type = 0;
    uint8_t length = 0;
    uint8_t intVal = 0;

    // Check SNMP Message Type (must be of type SEQUENCE)
    type = packetBuffer[index++];
    if (type == Sequence)
        snmpMessage.type = Sequence;
    else
        return false;

    // SNMP Message Length
    length = packetBuffer[index++];
    if (length > 0 && length < UDP_PACKET_MAX_LENGTH)
        snmpMessage.length = length;
    else
        return false;

    // SNMP Version
    type  = packetBuffer[index++];       // Type   = Integer
    length = packetBuffer[index++];      // Length = 1
    
    if (type == Integer) {
        snmpMessage.version.type = Integer;
        snmpMessage.version.length = length;
        memcpy(snmpMessage.version.value, packetBuffer+index, length);
        index += length;
        
        // The only supported version of SNMP is version 1,
        // all other packets will be dropped
        if (snmpMessage.version.value[0] != 0)
            return false;
    }
    else
        return false;

    // SNMP Community
    type  = packetBuffer[index++];       // Type   = OctetString
    length = packetBuffer[index++];      // Length = Integer

    if (type == OctetString && length <= COMMUNITY_MAX_LENGTH) {
        snmpMessage.community.type = OctetString;
        snmpMessage.community.length = length;
        strncpy(snmpMessage.community.value, packetBuffer+index, length);
        index += length;
    }
    else
        return false;

    // SNMP PDU
    type = packetBuffer[index++];
    length = packetBuffer[index++];

    if (length <= ASN_VALUE_MAX_SIZE) {
        snmpMessage.pdu.type = type;
        snmpMessage.pdu.length = length;
        memcpy(snmpMessage.pdu.value, packetBuffer+index, length);
        index += length;
    }
    else
        return false;

    return true;
}

/*
 *  Check if the community string in the SNMP Message is 
 *  equal to community string set by the coder
 */
bool SNMPAgent::IsAuthenticated() 
{
    // Check that the community string isn't to long
    if (snmpMessage.community.length > COMMUNITY_MAX_LENGTH)
        return false;

    // Check if community string is Get Community String
    if (strncmp(snmpMessage.community.value, community, snmpMessage.community.length) == 0)
        if (snmpMessage.community.length == strlen(community))
            return true;

    return false;
}

/*
 *  This method goes through every byte in the PDU and
 *  tries to build a valid SNMP PDU Object.
 *  As of now it only support one (1) varbind.
 */
bool SNMPAgent::ParsePDU() 
{
    uint8_t index = 0;
    uint8_t type = 0;
    uint8_t length = 0;
    uint8_t intVal = 0;

    unsigned char *pduBuffer = snmpMessage.pdu.value;

    // SNMP PDU Request ID
    type  = pduBuffer[index++];       // Type   = Integer
    length = pduBuffer[index++];      // Length = 4 bytes

    if (type == Integer) {
        for (uint8_t i=0; i<length; i++)
            snmpPDU.request_id = (snmpPDU.request_id << 8) | pduBuffer[index+i];
        index += length;
    }
    else
        return false;
    
    // SNMP PDU Error
    type  = pduBuffer[index++];       // Type   = Integer
    length = pduBuffer[index++];      // Length = 1
    
    if (type == Integer) {
        snmpPDU.error = pduBuffer[index++];
    }
    else
        return false;
    
    // SNMP PDU Error Index
    type  = pduBuffer[index++];       // Type   = Integer
    length = pduBuffer[index++];      // Length = 1
    
    if (type == Integer) {
        snmpPDU.error_index = pduBuffer[index++];
    }
    else
        return false;

    // SNMP Varbind List
    type  = pduBuffer[index++];       // Type   = Sequence
    length = pduBuffer[index++];      // Length = 19

    uint8_t varbind_list_length = length;
    uint8_t varbinds_length = 0;

    if (type == Sequence) {
        snmpPDU.varbind_list.type = type;
        snmpPDU.varbind_list.length = length;

        // Parse every Varbind in the list
        for (uint8_t i=0; i<SNMP_VARBIND_LIST_MAX; i++) {
            type  = pduBuffer[index++];             // Type   = Sequence
            length = pduBuffer[index++];
            varbinds_length += length;  // Length = 17

            snmpPDU.varbind_list.list[i].type = type;
            snmpPDU.varbind_list.list[i].length = length;

            // Parse the Varbind OID
            type = pduBuffer[index++];        // Type   = Object Identifier
            length = pduBuffer[index++];      // Length = 13
            
            snmpPDU.varbind_list.list[i].oid.type   = type;
            snmpPDU.varbind_list.list[i].oid.length = length;
            memcpy(snmpPDU.varbind_list.list[i].oid.value, pduBuffer+index, length);

            char oid0[64];
            snmpPDU.varbind_list.list[i].GetOID(oid0);

            Serial.print(F("OID: ")); Serial.println(oid0);

            // Parse the Varbind Value (which is Null in a GetRequest)
            snmpPDU.varbind_list.list[i].value.type   = pduBuffer[index++];      // Type   = Null
            snmpPDU.varbind_list.list[i].value.length = pduBuffer[index++];      // Length = 0

            if (varbinds_length+2 == varbind_list_length)
                break;
        }
        
    }
    else
        return false;

    return true;
}

/*
 *  Check that the PDU contain information that can be retrieved
 *  and update the PDU information accordingly.
 */
bool SNMPAgent::ProcessPDU()
{
    // Check if the PDU is a GetRequest message
    if (snmpMessage.pdu.type == GetRequest) {

        char oid[VARBIND_OID_STRING_MAX]; 
        snmpPDU.varbind_list.list[0].GetOID(oid);
        uint8_t oid_value_length = 0;
        int8_t custom_oid_index = OidExists(oid);

        snmpMessage.pdu.type = GetResponse;

        // Requesting "1.3.6.1.2.1.1.1.0" / System Description
        if (strcmp_P(oid, sysDescr_oid) == 0) {
            snmpPDU.varbind_list.list[0].value.type = OctetString;
            oid_value_length = strlen_P(sysDescr_value);
            memcpy_P(snmpPDU.varbind_list.list[0].value.value, sysDescr_value, oid_value_length);
            snmpPDU.varbind_list.list[0].value.length = oid_value_length;
            
        // Requesting "1.3.6.1.2.1.1.3.0" / System Uptime
        } else if (strcmp_P(oid, sysUpTime_oid) == 0) {
            snmpPDU.varbind_list.list[0].value.type = TimeTicks;
            oid_value_length = 4;
            uint32_u temp;
            temp.uint32 = sysUpTime_value;
            snmpPDU.varbind_list.list[0].value.value[0] = temp.data[3];
            snmpPDU.varbind_list.list[0].value.value[1] = temp.data[2];
            snmpPDU.varbind_list.list[0].value.value[2] = temp.data[1];
            snmpPDU.varbind_list.list[0].value.value[3] = temp.data[0];
            snmpPDU.varbind_list.list[0].value.length = oid_value_length;
            
        // Requesting "1.3.6.1.2.1.1.4.0" / System Contact
        } else if (strcmp_P(oid, sysContact_oid) == 0) {
            snmpPDU.varbind_list.list[0].value.type = OctetString;
            oid_value_length = strlen_P(sysContact_value);
            memcpy_P(snmpPDU.varbind_list.list[0].value.value, sysContact_value, oid_value_length);
            snmpPDU.varbind_list.list[0].value.length = oid_value_length;
            
        // Requesting "1.3.6.1.2.1.1.5.0" / System Contact
        } else if (strcmp_P(oid, sysName_oid) == 0) {
            snmpPDU.varbind_list.list[0].value.type = OctetString;
            oid_value_length = strlen_P(sysName_value);
            memcpy_P(snmpPDU.varbind_list.list[0].value.value, sysName_value, oid_value_length);
            snmpPDU.varbind_list.list[0].value.length = oid_value_length;
            
        // Requesting "1.3.6.1.2.1.1.6.0" / System Contact
        } else if (strcmp_P(oid, sysLocation_oid) == 0) {
            snmpPDU.varbind_list.list[0].value.type = OctetString;
            oid_value_length = strlen_P(sysLocation_value);
            memcpy_P(snmpPDU.varbind_list.list[0].value.value, sysLocation_value, oid_value_length);
            snmpPDU.varbind_list.list[0].value.length = oid_value_length;

        // Requesting "1.3.6.1.4.1.49701.1.X.0"
        // That is, user programmable values
        } else if (custom_oid_index >= 0) {
            SnmpValue *currentValue = &snmpValues[custom_oid_index];

            if (currentValue->type == OctetString) {
                snmpPDU.varbind_list.list[0].value.type = currentValue->type;
                oid_value_length = strlen(currentValue->value);
                memcpy(snmpPDU.varbind_list.list[0].value.value, currentValue->value, oid_value_length);
                snmpPDU.varbind_list.list[0].value.length = oid_value_length;
            
            } else if (currentValue->type == Integer) {
                snmpPDU.varbind_list.list[0].value.type = currentValue->type;
                oid_value_length = 4;
                uint32_u temp;
                memcpy(&temp.uint32, currentValue->value, 4);
                snmpPDU.varbind_list.list[0].value.value[0] = temp.data[3];
                snmpPDU.varbind_list.list[0].value.value[1] = temp.data[2];
                snmpPDU.varbind_list.list[0].value.value[2] = temp.data[1];
                snmpPDU.varbind_list.list[0].value.value[3] = temp.data[0];
                snmpPDU.varbind_list.list[0].value.length = oid_value_length;
            }

        } else {
            snmpPDU.error = NoSuchName;
            snmpPDU.varbind_list.list[0].value.type = Null;
            snmpPDU.varbind_list.list[0].value.length = 0;
        }

        // Update the length inside all the ASN nodes within the SNMP Message
        snmpPDU.varbind_list.list[0].length += oid_value_length;
        snmpPDU.varbind_list.length += oid_value_length;
        snmpMessage.pdu.length += oid_value_length;
        snmpMessage.length += oid_value_length;
    }
}

/*
 *  A simple method that checks if the current OID is
 *  within the range 1..MAX_SNMP_VALUES (which is defined in SnmpAgent.h)
 *  If it exists, the returned value is the index of the
 *  SnmpValue object inside the array snmpValues.
 */
int8_t SNMPAgent::OidExists(char *oid) 
{
    const char base_oid[] PROGMEM = "1.3.6.1.4.1.49701.1.";
    const uint8_t oid_length = strlen(oid);
    char full_oid[32];
    char index[8];
    for (uint8_t i=1; i<=MAX_SNMP_VALUES; i++) {
        strcpy(full_oid, base_oid);
        itoa(i, index, 10);
        strcat(full_oid, index);
        strcat(full_oid, ".0");
        if (strncmp(full_oid, oid, oid_length) == 0)
            return i-1;
    }
    return -1;
}

/*
 *  Build a new packet encoded with BER (Basic Encoding Rules)
 *  and send it inside a UDP packet to the SNMP Manager.
 */
bool SNMPAgent::SendResponse()
{
    // Clear the packet buffer. It will be filled
    // with the new data
    uint8_t index = 0;
    memset(packetBuffer, 0, UDP_PACKET_MAX_LENGTH);

    // SNMP Message Type and Length
    packetBuffer[index++] = snmpMessage.type;
    packetBuffer[index++] = snmpMessage.length;

    // SNMP Version
    packetBuffer[index++] = snmpMessage.version.type;
    packetBuffer[index++] = snmpMessage.version.length;
    packetBuffer[index++] = snmpMessage.version.value[0];

    // SNMP Community String
    packetBuffer[index++] = snmpMessage.community.type;
    packetBuffer[index++] = snmpMessage.community.length;
    for (uint8_t i=0; i<snmpMessage.community.length; i++) {
        packetBuffer[index++] = snmpMessage.community.value[i];
    }

    // SNMP PDU Type and Length
    packetBuffer[index++] = snmpMessage.pdu.type;
    packetBuffer[index++] = snmpMessage.pdu.length;

    // SNMP PDU Request ID
    packetBuffer[index++] = Integer;    // Type   = Integer
    packetBuffer[index++] = 4;          // Length = 4
    for (uint8_t i=0; i<4; i++) {
        packetBuffer[index+i] = snmpPDU.request_id >> (24-8*i); // Value  = 32 bit Integer
    }
    
    index += 4;

    // SNMP PDU Error
    packetBuffer[index++] = Integer;       // Type   = Integer
    packetBuffer[index++] = 1;             // Length = 1
    packetBuffer[index++] = snmpPDU.error; // Value  = 8 bit integer
    
    // SNMP PDU Error Index
    packetBuffer[index++] = Integer;             // Type   = Integer
    packetBuffer[index++] = 1;                   // Length = 1
    packetBuffer[index++] = snmpPDU.error_index; // Value  = 8 bit integer

    // SNMP Varbind List
    packetBuffer[index++] = snmpPDU.varbind_list.type;
    packetBuffer[index++] = snmpPDU.varbind_list.length;

    // SNMP Varbind Item
    packetBuffer[index++] = snmpPDU.varbind_list.list[0].type;
    packetBuffer[index++] = snmpPDU.varbind_list.list[0].length;

    // SNMP Varbind Item OID
    packetBuffer[index++] = snmpPDU.varbind_list.list[0].oid.type;
    packetBuffer[index++] = snmpPDU.varbind_list.list[0].oid.length;
    for (uint8_t i=0; i<snmpPDU.varbind_list.list[0].oid.length; i++) {
        packetBuffer[index] = snmpPDU.varbind_list.list[0].oid.value[i];
        index++;
    }

    // SNMP Varbind Item Value
    packetBuffer[index++] = snmpPDU.varbind_list.list[0].value.type;
    packetBuffer[index++] = snmpPDU.varbind_list.list[0].value.length;
    for (uint8_t i=0; i<snmpPDU.varbind_list.list[0].value.length; i++) {
        packetBuffer[index] = snmpPDU.varbind_list.list[0].value.value[i];
        index++;
    }

    // Create a new UDP packet
    while (!udpSocket.beginPacket(udpSocket.remoteIP(), udpSocket.remotePort()));

    // Write the bytes that will be sent into the packet buffer
    udpSocket.write(packetBuffer, index);

    // Finally, send the packet to the recipient
    udpSocket.endPacket();

    return true;
}

