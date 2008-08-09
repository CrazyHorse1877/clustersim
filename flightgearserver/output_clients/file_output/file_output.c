#include "file_output.h"
#include "bool.h"
#include "file.h"
#include "output_client.h"
#include "flight_info.h"

bool write_flight_info_to_file(Flight_info flight_info, void* user_data){
	(void)user_data;
	char file_name[100];

	sprintf(file_name, "flight_%d.dat", flight_info.id);
	
	FILE *fp = open_file(file_name, "a"); /* appends flight info to specified file*/
	
	Time_stamp time_stamp = flight_info.time_stamp;
	Position position = flight_info.position;
	Orientation orientation = flight_info.orientation;

	fprintf(fp, "%d:%d:%d:%d:%d:%d:%lf:%lf:%lf:%lf:%lf:%lf:%lf\n", time_stamp.year, time_stamp.month, time_stamp.day, time_stamp.hour, time_stamp.minute, time_stamp.second, time_stamp.millisecond, position.latitude, position.longitude, position.altitude, orientation.roll, orientation.pitch, orientation.yaw);
	fflush(fp);
	close_file(fp);

	return true; /*TODO end condition*/
}

void output_flight_info_to_file(char server_host[], int server_port){
	register_for_flight_info_to_server(server_host, server_port, true);
	get_multiple_flight_info_from_server(write_flight_info_to_file, NULL);
}

