#include "fg_wp_output.h"
#include <stdio.h>

int main(void){
   printf("waypoint output client\n");
   send_waypoint_to_fg("localhost", 5400, "localhost", 6969, "localhost", 6900);
   
   return 0;
}

