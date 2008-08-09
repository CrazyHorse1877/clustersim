#include <stdio.h>
#include <stdlib.h>
#include "file.h"
#include "flight_info.h"
#include "input_client.h"
#include "file_input.h"
#include "bool.h"
#include <unistd.h>

static const int BUFSIZE = 1024;

void process_line(int id, char buffer[], char server_host[], int server_port){
	int year, month, day, hour, minute, second;
	double millisecond, latitude, longitude, altitude, roll, pitch, yaw;
	
	int count = sscanf(buffer, "%d:%d:%d:%d:%d:%d:%lf:%lf:%lf:%lf:%lf:%lf:%lf", &year, &month, &day, &hour, &minute, &second, &millisecond, &latitude, &longitude, &altitude, &roll, &pitch, &yaw);

	if (count == 13){
		Time_stamp time_stamp = new_time_stamp(year, month, day, hour, minute, second, millisecond);
		Position position = new_position(latitude, longitude, altitude);
		Orientation orientation = new_orientation(roll, pitch, yaw);

		Flight_info flight_info = new_flight_info(id, time_stamp, position, orientation);

		while(!give_flight_info_to_server(flight_info, server_host, server_port)){
			fprintf(stderr, "Give flight info to server failed. Retrying...\n");
		}
	        sleep(1);
	        //usleep(500000);

	}else if (count > 0){
		fprintf(stderr, "Found a sad line in input, parsed %d elements\n", count);
	}
}



void input_flight_info_from_file(int id, char file_name[], char server_host[], int server_port) {
	char buffer[BUFSIZE];

	FILE *fp = open_file(file_name, "r");

	int valid_buffer_start = 0;
	int valid_buffer_end = fread(buffer, sizeof(char), BUFSIZE, fp);

	while(valid_buffer_start < valid_buffer_end){
		bool has_line = false;
		int line_size = 0;

		while(!has_line){
			if(valid_buffer_start == valid_buffer_end){
				int count = fread(buffer + line_size, sizeof(char), BUFSIZE - line_size, fp);

				if(count > 0){
					valid_buffer_start = line_size;
					valid_buffer_end = line_size + count;
				}else{
					buffer[valid_buffer_start] = '\n';
				}
			}
			if(line_size + 2 < BUFSIZE){
				buffer[line_size] = buffer[valid_buffer_start++];
				if (buffer[line_size] == '#'){
					buffer[line_size] = '\0';
				}else if (buffer[line_size] == '\n' || buffer[line_size] == '\r'){
					buffer[line_size] = '\0';
					has_line = true;
				}
			}else{
				fprintf(stderr, "error: line overflow.\n");
				exit(EXIT_FAILURE);
			}
			line_size++;
		}

		process_line(id, buffer, server_host, server_port);
	}
}




