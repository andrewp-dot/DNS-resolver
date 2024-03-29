/**
 * @file constants.h
 * @author Adrián Ponechal (xponec01@stud.fit.vutbr.cz)
 * @login xponec01
 * @brief Header file which contains universal constants.
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <climits>
#define USAGE "./dns [-r] [-x] [-6] -s server [-p port] address\n"

#define UDP_DATAGRAM_LIMIT 512
#define MAX_PORT_NUMBER 65535
#define DNS_PORT 53

#define IPV4_MIN_LENGTH 7
#define IPV4_MAX_LENGTH 15

#define IPV6_MIN_LENGTH 3
#define IPV6_MAX_LENGTH 39
#define IPV6_PART_SIZE 4

#define CHAR_BITS 8
#define RESPONSE_POINTER_SIGN 0xc0
#define RESPONSE_POINTER_MASK 0x3fff
#define UINT8_MASK 0x00FF
#endif
