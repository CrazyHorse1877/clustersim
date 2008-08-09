#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "tcp.h"
#include "flight_info.h"
#include "input_client.h"
#include "file_input.h"
#include "convert_units.h"
#include "file.h"
//#include "orientation.h"

static const int BUFSIZE = 1024;

void input_flight_info_from_file(int id, char file_name[], char server_host[], int server_port) {
   //int year, month, day, hour, minute, second;
   //double millisecond;
   double latitude, longitude, altitude;
   double roll = 0.0 , pitch= 0.0, yaw = 0.0;
   
   char buffer[BUFSIZE*sizeof(char)];
   
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
      //while(count)
      int count = sscanf(buffer, "%lf,%lf,%lf ", &latitude, &longitude, &altitude);
      //if </coordinates>
      if (count == 3){
	 Time_stamp time_stamp = new_time_stamp(2008, 01, 01, 00, 01, 00, 00.0);
	 //Time_stamp time_stamp = new_time_stamp(year, month, day, hour, minute, second, millisecond);
	 Position position = new_position(longitude, latitude, feet_to_metres(altitude));
	 print_position(position);
	 Orientation orientation = new_orientation(roll, pitch, yaw);
	 
	 id = 1;
	 Flight_info flight_info = new_flight_info(id, time_stamp, position, orientation);
	 
	 if (!give_flight_info_to_server(flight_info, server_host, server_port)){
	    fprintf(stderr, "Give flight info to server failed\n");
	 }
	 
      }else if (count > 0){
	 fprintf(stderr, "Found a sad line in input, parsed %d elements\n", count);
      }
   }
}

