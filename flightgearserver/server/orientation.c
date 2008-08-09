#include <stdio.h>
#include "orientation.h"


Orientation new_orientation(double roll, double pitch, double yaw){
	Orientation orientation;

	orientation.roll = roll;
	orientation.pitch= pitch;
	orientation.yaw = yaw;

	return orientation;
}


void print_orientation(Orientation orientation){
	printf("roll: '%lf'\n",orientation.roll);
	printf("pitch: '%lf'\n",orientation.pitch);
	printf("yaw: '%lf'\n",orientation.yaw);

}
