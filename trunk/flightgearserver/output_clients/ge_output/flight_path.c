#include <stdio.h>
#include <stdlib.h>

#include "position.h"
#include "orientation.h"
#include "flight_info.h"
#include "flight_path.h"

struct FlightPath {
	Flight_info* path;
	int size;
	int capacity;
};


FlightPath* new_flight_path(){
	FlightPath* flight_path = (FlightPath *) malloc(sizeof(FlightPath));
	
	flight_path->size = 0;
	flight_path->capacity = 1;
	flight_path->path = (Flight_info *) malloc(flight_path->capacity * sizeof(Flight_info));

	return flight_path;
}

FlightPath* add_flight_info_to_path(FlightPath* flight_path, Flight_info flight_info) {
	if (flight_path->size == flight_path->capacity) {
		flight_path->capacity *= 2;
		flight_path->path = (Flight_info *) realloc(flight_path->path, flight_path->capacity * sizeof(Flight_info));
	}

	flight_path->path[flight_path->size++] = flight_info;

	return flight_path;
}

int get_path_size(FlightPath* flight_path) {
	return flight_path->size;
}

Position get_position_from_flight_path(FlightPath* flight_path, int position_num) {
	return flight_path->path[position_num].position;
}

Orientation get_orientation_from_flight_path(FlightPath* flight_path, int position_num) {
	return flight_path->path[position_num].orientation;
}


int get_id_from_flight_path(FlightPath* flight_path) {
	/* All id's in a flight path are the same so looking at the id of the first position in a flight path gives you the id of the whole flight path*/

	return flight_path->path[0].id;
}
