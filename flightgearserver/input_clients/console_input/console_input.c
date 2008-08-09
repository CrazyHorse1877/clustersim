#include <stdio.h>
#include <stdlib.h>
#include "flight_info.h"
#include "input_client.h"
#include "console_input.h"
#include "bool.h"

bool process_line(int id, char server_host[], int server_port){
	double latitude, longitude, altitude, roll, pitch, yaw;
	
	int count = scanf("%lf %lf %lf %lf %lf %lf", &latitude, &longitude, &altitude, &roll, &pitch, &yaw);

	if (count == 6){
		Position position = new_position(latitude, longitude, altitude);
		Orientation orientation = new_orientation(roll, pitch, yaw);

		Flight_info flight_info = new_self_time_stamped_flight_info(id, position, orientation);

		while(!give_flight_info_to_server(flight_info, server_host, server_port)){
			fprintf(stderr, "Give flight info to server failed. Retrying...\n");
		}
		
		return true;
	}else if (count > 0){
		fprintf(stderr, "Found a sad line in console, parsed %d elements\n", count);
		
		return true;
	}else{
		return false;
	}
}

void input_flight_info_from_console(int id, char server_host[], int server_port) {
	printf("Console Input Client\n");
	printf("--------------------\n");
	while(process_line(id, server_host, server_port));
}

