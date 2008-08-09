#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <pthread.h>
#include "flight_info.h"
#include "orientation.h"

Flight_info new_flight_info(int id, Time_stamp time_stamp, Position position, Orientation orientation){
	Flight_info flight_info;

	flight_info.id = id;
	flight_info.time_stamp = time_stamp;
	flight_info.position = position;
	flight_info.orientation = orientation;

	return flight_info;
}

Flight_info new_self_time_stamped_flight_info(int id, Position position, Orientation orientation){
	time_t seconds_since_epoch;
	time(&seconds_since_epoch);

	struct tm *now = localtime(&seconds_since_epoch);

	Time_stamp time_stamp = new_time_stamp(now->tm_year + 1900, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, 0);

	return new_flight_info(id, time_stamp, position, orientation);
}

void print_flight_info(Flight_info flight_info){
	printf("id = %d\n", flight_info.id);
	print_time_stamp(flight_info.time_stamp);
	print_position(flight_info.position);
	print_orientation(flight_info.orientation);
}

