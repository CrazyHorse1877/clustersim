#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "tcp.h"
#include "flight_info.h"
#include "output_client.h"
#include "fg_wp_output.h"

int sd;

void set_position_value(char position_descriptor[],double position_latitude,
			double position_longitude, double position_altitude) {
	int max_size = 64;
	char* position_string = (char *) malloc(max_size * sizeof(char));

	sprintf(position_string, "@insert0:%lf,%lf@%lf", position_latitude, position_longitude, position_altitude);
	
	send_string_TCP(sd, "set ");
	send_string_TCP(sd, position_descriptor);
	send_string_TCP(sd, " ");
	send_string_TCP(sd, position_string);
	send_string_TCP(sd, "\r\n");
	free(position_string);
}

int connect_to_flightgear_property_server(char fg_host[], int fg_port) {

	int sd = open_client_to_server_socket_TCP(fg_host, fg_port, true); /* TODO handle failed open */

	return sd;
}

void close_flightgear_property_server(int sd){
	close_TCP(sd);
}

void start_up_fg_property_server(char fg_host[], int fg_port) {
   //char host_name[] = "127.0.0.1";
   //int server_port = 6969;
   char directory[] = "cd /autopilot/route-manager\r\n"; 
   int prompt_size = strlen("/autopilot[0]/route-manager[0]> ");
   
   sd = connect_to_flightgear_property_server(fg_host, fg_port);
   send_string_TCP(sd, directory);
   printf("connected and into route-manager directory\n");
   discard_TCP(sd, prompt_size);
}

void shut_down_fg_property_server() {
	close_flightgear_property_server(sd);
}

void set_position_in_fg(Flight_info flight_info) {
   Position position = flight_info.position;
   char waypoint_descriptor[] = "input";
//   char longitude_descriptor[] = "longitude-deg";
//   char latitude_descriptor[] = "latitude-deg";
//   char altitude_descriptor[] = "altitude-ft";
   
   set_position_value(waypoint_descriptor, position.latitude,position.longitude,position.altitude);
//   set_position_value(latitude_descriptor, position.latitude);
//   set_position_value(longitude_descriptor, position.longitude);
//   set_position_value(altitude_descriptor, position.altitude);
   
}

bool write_flight_info_to_fg(Flight_info flight_info, void* user_data){
	(void)user_data;
	set_position_in_fg(flight_info);

	return true;
}


void send_waypoint_to_fg(char fg_host[], int fg_port, char server_host[], int server_port, char client_host[], int client_port) {
   printf("started. registering with server...\n %d  %i",client_host[1], client_port);
   register_for_flight_info_to_server(server_host, server_port, true);
// register_to_server(server_host, server_port, client_host, client_port, true);
   printf("connecting to property server...\n");
   start_up_fg_property_server(fg_host, fg_port);
   printf("connected to property server\n");
   get_multiple_flight_info_from_server(write_flight_info_to_fg, NULL);
   printf("waiting for key press\n");
   getchar();
   printf("shutting down property server\n");
   shut_down_fg_property_server();
//   deregister_from_server(server_host, server_port, client_host, client_port);
}
	







