#ifndef MESSAGE_H
#define MESSAGE_H
#include "MessageHeader.h"

/**
 * @brief RR top level structure
 * NAME
 * TYPE
 * CLASS
 * TTL
 * RDLENGTH
 * RDATA
 */

/*
NAME            an owner name, i.e., the name of the node to which this
                resource record pertains.

TYPE            two octets containing one of the RR TYPE codes.

CLASS           two octets containing one of the RR CLASS codes.

TTL             a 32 bit signed integer that specifies the time interval
                that the resource record may be cached before the source
                of the information should again be consulted.  Zero
                values are interpreted to mean that the RR can only be
                used for the transaction in progress, and should not be
                cached.  For example, SOA records are always distributed
                with a zero TTL to prohibit caching.  Zero values can
                also be used for extremely volatile data.

RDLENGTH        an unsigned 16 bit integer that specifies the length in
                octets of the RDATA field.

RDATA           a variable length string of octets that describes the
                resource.  The format of this information varies
                according to the TYPE and CLASS of the resource record.
*/

/* Messages:
    +---------------------+
    |        Header       |
    +---------------------+
    |       Question      | the question for the name server
    +---------------------+
    |        Answer       | RRs answering the question
    +---------------------+
    |      Authority      | RRs pointing toward an authority
    +---------------------+
    |      Additional     | RRs holding additional information
    +---------------------+
*/

class Message
{

private:
    // data types in here
    MessageHeader header;

public:
    Message()
    {
    }
};

#endif
