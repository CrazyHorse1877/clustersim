#ifndef _SQUAD_PATHS_H_
#define _SQUAD_PATHS_H_

#include "flight_info.h"
#include "flight_path.h"


typedef struct SquadPath SquadPath;

SquadPath* new_squad_path();
SquadPath* add_flight_info_to_squad_path(SquadPath* squad_path, Flight_info flight_info);
int get_squad_path_size(SquadPath* squad_path);
FlightPath* get_flight_path_from_squad_path(SquadPath* squad_path, int path_num);

#endif
