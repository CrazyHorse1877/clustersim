#include <stdio.h>
#include "color.h"


Color new_color(double red, double green, double blue){
	Color color;

	color.red = red;
	color.green = green;
	color.blue = blue;

	return color;
}


