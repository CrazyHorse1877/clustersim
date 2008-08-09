#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "output_client.h"
#include "client.h"
#include "server.h"
#include "tcp.h"
#include "file.h"

static int socket = -1;

bool register_to_server(char server_host[], int server_port, DataPacketType packet_type, bool blocking){
	socket = open_client_to_server_socket_TCP(server_host, server_port, blocking);

	if (socket < 0){
		print_tcp_error(stderr, socket);
		return false;
	}else{
		send_header(socket, false, packet_type);
	}
	return true;
}

void register_for_flight_info_to_server(char server_host[], int server_port, bool blocking){
	register_to_server(server_host, server_port, flight_info_packet_type, blocking);
}

void register_for_pdf_to_server(char server_host[], int server_port, bool blocking){
	register_to_server(server_host, server_port, pdf_packet_type, blocking);
}

void get_packet_from_server(void* packet, size_t packet_size){

	if (socket < 0){
		fprintf(stderr, "ERROR: Tried to get packet from server without a valid socket.");
		exit(EXIT_FAILURE);

	}
	
	int ret = receive_TCP(socket, packet, packet_size); 
	
	if (ret == TCP_ERROR_AGAIN){
		memset(packet, 0, packet_size);
	}else if(ret < (int)packet_size){
		fprintf(stderr, "ERROR: Recieve failed.");
		exit(EXIT_FAILURE);
	}

}

Flight_info get_flight_info_from_server(){
	Flight_info flight_info;
	get_packet_from_server(&flight_info, sizeof(flight_info));
	return flight_info;
}

Pdf get_pdf_from_server(){
	Pdf_info pdf_info;

	get_packet_from_server(&pdf_info, sizeof(pdf_info));
	size_t data_size = pdf_info.num_cols * pdf_info.num_rows * sizeof(unsigned char);
	unsigned char* data = (unsigned char*)malloc(data_size);

	get_packet_from_server(data, data_size);

	Pdf pdf = combine_pdf_info_with_pdf_data(pdf_info, data);
	
	return pdf;
}

void get_multiple_flight_info_from_server(bool(*handler)(Flight_info, void*), void* user_data){
	while(handler(get_flight_info_from_server(), user_data));
}

void get_multiple_pdf_from_server(bool(*handler)(Pdf, void*), void* user_data){
	while(handler(get_pdf_from_server(), user_data));
}

