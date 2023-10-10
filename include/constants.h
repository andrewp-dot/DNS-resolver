#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @brief
 * labels          63 octets or less
 * names           255 octets or less
 * TTL             positive values of a signed 32 bit number.
 * UDP messages    512 octets or less
 */
#define UDP_DATAGRAM_LIMIT 512
#define MAX_PORT_NUMBER 65535
#define DNS_PORT 53

#define IPV4_MIN_LENGTH 7
#define IPV4_MAX_LENGTH 15

#define IPV6_MAX_LENGTH 39

#endif
