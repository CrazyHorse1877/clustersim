#include <stdio.h>
#include "position.h"


Position new_position(double latitude, double longitude, double altitude){
	Position pos;

	pos.latitude = latitude;
	pos.longitude= longitude;
	pos.altitude = altitude;

	return pos;
}


void print_position(Position pos){
	printf("latitude: '%f'\n",pos.latitude);
	printf("longitude: '%f'\n",pos.longitude);
	printf("altitude: '%f'\n",pos.altitude);

}
