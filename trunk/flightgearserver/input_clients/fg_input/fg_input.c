#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "tcp.h"
#include "flight_info.h"
#include "input_client.h"
#include "fg_input.h"
#include "convert_units.h"

int sd;

/* Returns a null terminated string */
char* receive_line(int sd) {
	int size = 1;
	int index = 0;
	char* buffer = (char *) malloc(size * sizeof(char));

	receive_TCP(sd, buffer, 1 * sizeof(char));
	while (buffer[index] != '\n') {
		index++;
		if (index >= size) {
			size *= 2;
			buffer = (char *) realloc(buffer, size * sizeof(char));
		}
		receive_TCP(sd, &buffer[index], 1 * sizeof(char));
	}
	index++;
	if (index >= size) {
		size++;
		buffer = (char *) realloc(buffer, size * sizeof(char));
	}
	buffer[index] = '\0';
	return buffer;
}

double get_fg_value(int sd, char descriptor[]) {
	int max_size = 64;
	char* string_value = (char *) malloc(max_size * sizeof(char));
	char* line;
	double double_value;
   
   
	send_string_TCP(sd, "get ");
	send_string_TCP(sd, descriptor);
	send_string_TCP(sd, "\r\n");
	discard_TCP(sd, strlen(descriptor));
	line = receive_line(sd);
			
	sscanf(line, " = '%s'", string_value);

	string_value[(strlen(string_value))- 1] = '\0';
	double_value = atof(string_value); 

	return double_value;
}

int connect_to_flightgear_property_server(char fg_host[], int fg_port) {

	int sd = open_client_to_server_socket_TCP(fg_host, fg_port, true); /* TODO handle failed open */

	return sd;
}

void close_flightgear_property_server(int sd){
	close_TCP(sd);
}

void cd_fg_directory(char directory[]){
	int prompt_size = strlen("/") + strlen(directory) + strlen("[0]> "); 

	send_string_TCP(sd, "cd /"); /*home dircetory*/
	send_string_TCP(sd, directory);
	send_string_TCP(sd, "\r\n");

	discard_TCP(sd, prompt_size);
}

Position get_position_from_fg() {
	int prompt_size = strlen("/position[0]> ");

	char longitude_descriptor[] = "longitude-deg";
	char latitude_descriptor[] = "latitude-deg";
	char altitude_descriptor[] = "altitude-ft";

	cd_fg_directory("position");

	double latitude_value = get_fg_value(sd, latitude_descriptor);
	discard_TCP(sd, prompt_size);
	
	double longitude_value = get_fg_value(sd, longitude_descriptor);
	discard_TCP(sd, prompt_size);

	double altitude_value= feet_to_metres(get_fg_value(sd, altitude_descriptor));
	discard_TCP(sd, prompt_size);

	return new_position(latitude_value, longitude_value, altitude_value);
   printf("lat: %f   lon: %f   Alt: %f \n",latitude_value, longitude_value, altitude_value);
}

Orientation get_orientation_from_fg() {
	int prompt_size = strlen("/orientation[0]> ");

	char roll_descriptor[] = "roll-deg";
	char pitch_descriptor[] = "pitch-deg";
	char yaw_descriptor[] = "heading-deg";


	cd_fg_directory("orientation");
	
	double roll_value = get_fg_value(sd, roll_descriptor);
	discard_TCP(sd, prompt_size);
	
	double pitch_value = get_fg_value(sd, pitch_descriptor);
	discard_TCP(sd, prompt_size);

	double yaw_value= get_fg_value(sd, yaw_descriptor);
	discard_TCP(sd, prompt_size);

	return new_orientation(roll_value, pitch_value, yaw_value);
}


Flight_info get_flight_info_from_fg(int id){
	Position position = get_position_from_fg();
	Orientation orientation = get_orientation_from_fg();
	
	return new_self_time_stamped_flight_info(id, position, orientation);
}


void start_up_fg_property_server(char fg_host[], int fg_port) {
	/*char host_name[] = "127.0.0.1";*/
	/*int server_port = 5501;*/
	/*char directory[] = "cd /position\r\n"; 
	int prompt_size = strlen("/position[0]> ");
	*/

	sd = connect_to_flightgear_property_server(fg_host, fg_port);
	/*
	send_string_TCP(sd, directory);
	discard_TCP(sd, prompt_size);
	*/
}

void shut_down_fg_property_server() {
	close_flightgear_property_server(sd);
}

void input_flight_info_from_fg(int id, char fg_host[], int fg_port, char server_host[], int server_port) {
	Flight_info flight_info;

	start_up_fg_property_server(fg_host, fg_port);

	while(true) {
		flight_info = get_flight_info_from_fg(id);
		//print_flight_info(flight_info);
		while(!give_flight_info_to_server(flight_info, server_host, server_port)){
			fprintf(stderr, "Give flight info to server failed. Retrying...\n");
		//usleep(100000);
		}

	}

	shut_down_fg_property_server();
}	

