#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "server.h"

void send_header(int socket, bool isInput, DataPacketType type);


#endif
