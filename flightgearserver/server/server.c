#include <string.h>
#include <stdio.h>

#include "tcp.h"
#include "flight_info.h"
#include "pdf.h"
#include "server.h"
/*
DataPacketType output_client_packet_type;
int output_client_socket = -1;
*/
typedef struct{
	int socket;
	DataPacketType packet_type;
}OutputClient;

typedef struct{
	OutputClient** output_client;
	int size;
	int capacity;
}OutputClients;


OutputClient* new_output_client(int socket, DataPacketType packet_type) {
	OutputClient* output_client = (OutputClient *)malloc(sizeof(OutputClient));
	
	output_client->socket = socket;
	output_client->packet_type = packet_type;

	return output_client;
}

OutputClients* new_vector_of_output_clients(){
	OutputClients* output_clients = (OutputClients *) malloc(sizeof(OutputClients));

	output_clients->size = 0;
	output_clients->capacity = 1;
	output_clients->output_client = (OutputClient **)malloc(output_clients->capacity * sizeof(OutputClient*));

	return output_clients;
}


OutputClients* output_clients;

void add_output_client_to_vector(OutputClient* output_client) {
	if (output_clients->size == output_clients->capacity) {
		output_clients->capacity *= 2;
		output_clients->output_client = (OutputClient**) realloc(output_clients->output_client, output_clients->capacity * sizeof(OutputClient*));
	}

	output_clients->output_client[(output_clients->size++)] = output_client;

}


void input_pdf_packet(int socket) {
	Pdf_info pdf_info;
	
	receive_TCP(socket, (void*)&pdf_info, sizeof(pdf_info));
	
	size_t data_size = pdf_info.num_cols * pdf_info.num_rows * sizeof(unsigned char);
	unsigned char* data = (unsigned char*)malloc(data_size);
	
	receive_TCP(socket, (void*)data, data_size);
	
	for(int i = 0; i < output_clients->size ; i++){
		int output_client_socket = (output_clients->output_client[i])->socket;
		DataPacketType output_client_packet_type = (output_clients->output_client[i])->packet_type;

		if (output_client_socket >= 0 && output_client_packet_type == pdf_packet_type){
			if(send_TCP(output_client_socket, (void*)&pdf_info, sizeof(pdf_info)) && send_TCP(output_client_socket, (void*)data, data_size)){
				printf("Sent pdf packet to output client: %d\n", i);
			}else{
				printf("Lost connection to output client: %d\n", i);
				(output_clients->output_client[i])->socket = -1;
			}
		}
	}
}

void input_flight_info_packet(int socket) {
	Flight_info flight_info;

	receive_TCP(socket, (void*)&flight_info, sizeof(flight_info));	

	//print_flight_info(flight_info);

	for(int i = 0; i < output_clients->size ; i++){
		int output_client_socket = (output_clients->output_client[i])->socket;
		DataPacketType output_client_packet_type = (output_clients->output_client[i])->packet_type;

		if (output_client_socket >= 0 && output_client_packet_type == flight_info_packet_type){
			if(send_TCP(output_client_socket, (void*)&flight_info, sizeof(flight_info))){
			//	printf("Sent flight info packet to output client: %d\n", i);
			}else{
			//	printf("Lost connection to output client: %d\n", i);
				(output_clients->output_client[i])->socket = -1;
			}
		}
	}
}

void parse_output_connection(int socket, DataPacketType type) {
	switch(type){
		case flight_info_packet_type:
		case pdf_packet_type:
			add_output_client_to_vector(new_output_client(socket, type));
			break;
		default:
			printf("Received unknown output packet type\n");
	}
}

void parse_input_connection(int socket, DataPacketType type) {
	switch(type){
		case flight_info_packet_type:
			//printf("Received flight info input packet\n");
			input_flight_info_packet(socket);
			break;
		case pdf_packet_type:
			//printf("Received pdf input packet\n");
			input_pdf_packet(socket);
			break;
		default:
			printf("Received unknown input packet\n");
	}
}

int server(int server_port) {
	output_clients = new_vector_of_output_clients();
	int welcome_socket = open_welcome_socket_TCP(server_port, true);

	printf("\nServer Started on port %d\n",server_port);
	printf("---------------------------\n");

	if (welcome_socket < 0) { 
		print_errno_error(stderr); 
		exit(EXIT_FAILURE);
	}

	Header_packet header_packet;
	//clear_client_info();

	while(true) {
		//printf("\nWaiting for clients to connect\n");
		int connected_socket = accept_connection_from_client_TCP(welcome_socket);

		if (connected_socket < 0) {
			print_errno_error(stderr); 
			exit(EXIT_FAILURE);
		}

		receive_TCP(connected_socket, (void*)&header_packet, sizeof(header_packet));

		if(header_packet.isInput){
			//printf("Input client connected\n");
			parse_input_connection(connected_socket, header_packet.type);
			close_TCP(connected_socket);
		}else{
			//printf("Output client connected\n");
			parse_output_connection(connected_socket, header_packet.type);
		}

	}
	close_TCP(welcome_socket);

	return EXIT_SUCCESS;
}


/*
int server(int server_port) {
	output_clients = new_vector_of_output_clients();
	int welcome_socket = open_welcome_socket_TCP(server_port, true);

	printf("\nServer Started on port %d\n",server_port);
	printf("---------------------------\n");

	if (welcome_socket < 0) { 
		print_errno_error(stderr); 
		exit(EXIT_FAILURE);
	}



	Header_packet header_packet;
	//clear_client_info();

		//printf("\nWaiting for clients to connect\n");
		int connected_socket = accept_connection_from_client_TCP(welcome_socket);

		if (connected_socket < 0) {
			print_errno_error(stderr); 
			exit(EXIT_FAILURE);
		}

		receive_TCP(connected_socket, (void*)&header_packet, sizeof(header_packet));

		if(header_packet.isInput){
			//printf("Input client connected\n");
			parse_input_connection(connected_socket, header_packet.type);
			close_TCP(connected_socket);
		}else{
			//printf("Output client connected\n");
			parse_output_connection(connected_socket, header_packet.type);
		}

	}
	close_TCP(welcome_socket);

	return EXIT_SUCCESS;
}

*/
