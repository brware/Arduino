/*
 * AsnObject.h - A very simple SNMP Agent Library for Arduino.
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

#ifndef ASNOBJECT_H
#define ASNOBJECT_H

#define ASN_VALUE_MAX_SIZE    32

#include "SnmpTypes.h"

/*
 *  An ASN.1 Object which is used to describe for example
 *  SNMP Version, SNMP Community, Request ID, Error,
 *  Error Index, Object Identifier and Value
 */
class AsnObject {
    public:
        uint8_t type;
        uint8_t length;
        byte value[ASN_VALUE_MAX_SIZE];

        AsnObject() { type = Null; length = 0; memset(value, 0, ASN_VALUE_MAX_SIZE); };
};

#endif
