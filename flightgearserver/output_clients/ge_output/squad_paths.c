#include <stdio.h>
#include <stdlib.h>

#include "flight_path.h"
#include "squad_paths.h"

struct SquadPath{
	FlightPath** flight_paths;
	int size;
	int capacity;
};


SquadPath* new_squad_path(){
	SquadPath* squad_path = (SquadPath *) malloc(sizeof(SquadPath));
	
	squad_path->size = 0;
	squad_path->capacity = 1;
	squad_path->flight_paths = (FlightPath **) malloc(squad_path->capacity * sizeof(FlightPath*));

	return squad_path;
}

SquadPath* add_flight_info_to_squad_path(SquadPath* squad_path, Flight_info flight_info) {
	if (squad_path->size == squad_path->capacity) {
		squad_path->capacity *= 2;
		squad_path->flight_paths = (FlightPath **) realloc(squad_path->flight_paths, squad_path->capacity * sizeof(FlightPath*));
	}

	
	for(int i = 0 ; i < squad_path->size; i++){

/* No flight path will be empty as we add flight_info so it is safe to look at the 0th element of a flight path to determine the id, it is also sufficient as all the flight_infos in a flight_path will have the same id */

		if (get_id_from_flight_path(squad_path->flight_paths[i]) == flight_info.id) {
			add_flight_info_to_path(squad_path->flight_paths[i], flight_info);
			return squad_path;
		}
	}

	FlightPath* flight_path = new_flight_path();
	add_flight_info_to_path(flight_path, flight_info);
	squad_path->flight_paths[squad_path->size++] = flight_path;

	return squad_path;
}

int get_squad_path_size(SquadPath* squad_path) {
	return squad_path->size;
}


FlightPath* get_flight_path_from_squad_path(SquadPath* squad_path, int path_num){
	       return squad_path->flight_paths[path_num];
}
