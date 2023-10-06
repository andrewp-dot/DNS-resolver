#include <iostream>
#include "Connection.h"

/**
 * @brief
 * The resolver starts with knowledge of at least one name server.
 *
 * In return, the resolver either receives the desired
 * information or a referral to another name server.
 *
 * AUTHORITATIVE DATA
 * Name servers manage two kinds of data.  The first kind of data held in
 * sets called zones; each zone is the complete database for a particular
 * "pruned" subtree of the domain space.  This data is called
 * authoritative.
 */

/**
 * @brief Name grammar
 * <domain> ::= <subdomain> | " "
 * <subdomain> ::= <label> | <subdomain> "." <label>
 * <label> ::= <letter> [ [ <ldh-str> ] <let-dig> ]
 * <ldh-str> ::= <let-dig-hyp> | <let-dig-hyp> <ldh-str>
 * <let-dig-hyp> ::= <let-dig> | "-"
 * <let-dig> ::= <letter> | <digit>
 * <letter> ::= any one of the 52 alphabetic characters A through Z in upper case and a through z in lower case
 * <digit> ::= any one of the ten digits 0 through 9
 */

/*
 * DNS server:
 * kazi.fit.vutbr.cz
 */
