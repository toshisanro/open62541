/*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */

#ifndef NETWORKLAYERTCP_H_
#define NETWORKLAYERTCP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ua_server.h"
#include "ua_client.h"

/** @brief Create the TCP networklayer and listen to the specified port */
UA_ServerNetworkLayer UA_EXPORT UA_ServerNetworkLayerTCP(UA_UInt32 port);

UA_Connection UA_EXPORT
UA_ClientConnectionTCP(UA_ConnectionConfig conf, const char *endpointUrl, UA_Logger logger);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* NETWORKLAYERTCP_H_ */
