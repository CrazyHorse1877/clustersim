#ifndef _FLIGHT_INFO_H_
#define _FLIGHT_INFO_H_

#include "position.h"
#include "time_stamp.h"
#include "orientation.h"

typedef struct{
	int id;
	Time_stamp time_stamp;
	Position position;
	Orientation orientation;
}Flight_info;

//void *Flight_info new_flight_info(void *threadarg);
//void *Flight_info new_self_time_stamped_flight_info(void *threadarg);
Flight_info new_flight_info(int id, Time_stamp time_stamp, Position position, Orientation orientation);
Flight_info new_self_time_stamped_flight_info(int id, Position position, Orientation orientation);
void print_flight_info(Flight_info flight_info);

#endif

