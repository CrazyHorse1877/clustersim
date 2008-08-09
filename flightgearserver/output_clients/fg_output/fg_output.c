#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "tcp.h"
#include "flight_info.h"
#include "output_client.h"
#include "fg_output.h"
#include "convert_units.h"

int sd;

void set_fg_value(char descriptor[], double value) {
	int max_size = 64;
	char* value_string = (char *) malloc(max_size * sizeof(char));

	sprintf(value_string, "%lf", value);
	
	send_string_TCP(sd, "set ");
	send_string_TCP(sd, descriptor);
	send_string_TCP(sd, " ");
	send_string_TCP(sd, value_string);
	send_string_TCP(sd, "\r\n");
	free(value_string);
			
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


void start_up_fg_property_server(char fg_host[], int fg_port) {
	sd = connect_to_flightgear_property_server(fg_host, fg_port);
}

void shut_down_fg_property_server() {
	close_flightgear_property_server(sd);
}

void set_position_in_fg(Flight_info flight_info) {
	Position position = flight_info.position;
	char longitude_descriptor[] = "longitude-deg";
	char latitude_descriptor[] = "latitude-deg";
	char altitude_descriptor[] = "altitude-ft";

	cd_fg_directory("position");

	set_fg_value(latitude_descriptor, position.latitude);
	set_fg_value(longitude_descriptor, position.longitude);
	set_fg_value(altitude_descriptor, metres_to_feet(position.altitude));

}


void set_orientation_in_fg(Flight_info flight_info) {
	Orientation orientation = flight_info.orientation;
	char roll_descriptor[] = "roll-deg";
	char pitch_descriptor[] = "pitch-deg";
	char yaw_descriptor[] = "heading-deg";

	cd_fg_directory("orientation");

	set_fg_value(roll_descriptor, orientation.roll);
	set_fg_value(pitch_descriptor, orientation.pitch);
	set_fg_value(yaw_descriptor, orientation.yaw);

}


bool write_flight_info_to_fg(Flight_info flight_info, void* user_data){
	(void)user_data;
	set_position_in_fg(flight_info);
	set_orientation_in_fg(flight_info);
	return true;
}


void output_flight_info_to_fg(char fg_host[], int fg_port, char server_host[], int server_port) {
	register_for_flight_info_to_server(server_host, server_port, true);
	start_up_fg_property_server(fg_host, fg_port);
	get_multiple_flight_info_from_server(write_flight_info_to_fg, NULL);
	shut_down_fg_property_server();
}








