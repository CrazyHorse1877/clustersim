#ifndef _ORIENTATION_H_
#define _ORIENTATION_H_


typedef struct{
	double roll;
	double pitch;
	double yaw;
}Orientation;

Orientation new_orientation(double roll, double pitch, double yaw);
void print_orientation(Orientation orientation);

#endif

