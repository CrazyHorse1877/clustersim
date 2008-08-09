#ifndef _POSITION_H_
#define _POSITION_H_


typedef struct{
	double latitude;
	double longitude;
	double altitude;
}Position;

Position new_position(double latitude, double longitude, double altitude);
void print_position(Position pos);

#endif

