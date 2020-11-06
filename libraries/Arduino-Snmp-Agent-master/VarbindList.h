/*
 * VarbindList.h - A very simple SNMP Agent Library for Arduino.
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

#ifndef SNMPVARBINDLIST_H
#define SNMPVARBINDLIST_H

#define SNMP_VARBIND_LIST_MAX 1

#include "SnmpTypes.h"
#include "Varbind.h"

/*
 *  A varbind list is a list of one or more varbind
 *  objects that matches the SNMP Request or Response
 */
class VarbindList {
    public:
        uint8_t type;
        uint8_t length;
        Varbind list[SNMP_VARBIND_LIST_MAX];

        VarbindList() {
            for (uint8_t i=0; i<SNMP_VARBIND_LIST_MAX; i++)
                list[i].type = Null;  
        };
};

#endif
