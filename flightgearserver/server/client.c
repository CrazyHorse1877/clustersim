#include "tcp.h"
#include "client.h"
#include "server.h"

void send_header(int socket, bool isInput, DataPacketType type){ 
	Header_packet header;
	
	header.isInput = isInput;
	header.type = type;
	send_TCP(socket, (void*) &header, sizeof(header));
}

