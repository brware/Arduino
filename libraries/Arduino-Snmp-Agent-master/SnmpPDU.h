/*
 * SnmpPDU.h - A very simple SNMP Agent Library for Arduino.
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

#ifndef SNMPPDU_H
#define SNMPPDU_H

#include "VarbindList.h"

/*
 *  An SNMP PDU (Protocol Data Unit) describes the type of
 *  SNMP packet in use.
 */
class SnmpPDU {
    public:
        uint32_t request_id;
        uint8_t error;
        uint8_t error_index;
        VarbindList varbind_list;

        SnmpPDU() {
            request_id = 0; error = 0; error_index = 0;
        };
};

#endif
