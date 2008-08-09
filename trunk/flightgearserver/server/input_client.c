#include <stdio.h>
#include "input_client.h"
#include "client.h"
#include "tcp.h"
#include "pdf.h"


bool give_packet_and_data_to_server(DataPacketType packet_type, void* packet, size_t packet_size, unsigned char* data, size_t data_size, char server_host[], int server_port){

	int socket = open_client_to_server_socket_TCP(server_host, server_port, true);

	if (socket < 0){
		print_tcp_error(stderr, socket);
		return false;
	}else{
		send_header(socket, true, packet_type);
		send_TCP(socket, packet, packet_size);
		send_TCP(socket, data, data_size);

		close_TCP(socket);
	}
	return true;
}

bool give_flight_info_to_server(Flight_info flight_info, char server_host[], int server_port){

	return give_packet_and_data_to_server(flight_info_packet_type, &flight_info, sizeof(flight_info), NULL, 0, server_host, server_port);
}

bool give_pdf_to_server(Pdf pdf, char server_host[], int server_port){
	Pdf_info pdf_info = get_pdf_info_from_pdf(pdf);
	unsigned char* data = pdf.data;
	size_t data_size = pdf.num_cols * pdf.num_rows * sizeof(unsigned char);

	return give_packet_and_data_to_server(pdf_packet_type, &pdf_info, sizeof(pdf_info), data, data_size, server_host, server_port);
}



