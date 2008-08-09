#ifndef _SERVER_H_
#define _SERVER_H_

#include "bool.h"

typedef enum{
	flight_info_packet_type,
	pdf_packet_type
}DataPacketType;

typedef struct{
	bool isInput;
	DataPacketType type;
}Header_packet;

/*
typedef struct{
	char client_host[MAX_HOST_LENGTH];
	int client_port;
}Deregister_packet;
*/

int server(int server_port);

#endif

