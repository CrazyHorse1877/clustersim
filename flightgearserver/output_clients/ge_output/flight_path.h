#ifndef _FLIGHT_PATH_H_
#define _FLIGHT_PATH_H_

#include "flight_info.h"
#include "position.h"
#include "orientation.h"

typedef struct FlightPath FlightPath;

FlightPath* new_flight_path();
FlightPath* add_flight_info_to_path(FlightPath* flight_path, Flight_info flight_info);
int get_path_size(FlightPath* flight_path);
Position get_position_from_flight_path(FlightPath* flight_path, int position_num);
Orientation get_orientation_from_flight_path(FlightPath* flight_path, int position_num);
int get_id_from_flight_path(FlightPath* flight_path);

#endif

