#ifndef _KML_H_
#define _KML_H_

#include <stdio.h>
#include "bool.h"
#include "position.h"
#include "orientation.h"

int kml_header(int sd);
int kml_close(int sd);
int kml_open_document(int sd);
int kml_close_document(int sd);
int kml_name(int sd, char name[]);
int kml_line_style(int sd, char style_id[], char color[], char width[]); 
int kml_polyline_style(int sd, char style_id[], char line_color[], char poly_color[], char width[]); 
int kml_open_point(int sd);
int kml_close_point(int sd);
int kml_open_placemark(int sd);
int kml_close_placemark(int sd);
int kml_styleurl(int sd, char style_id[]);
int kml_open_linestring(int sd);
int kml_close_linestring(int sd);
int kml_altitude_mode(int sd, char altitude_mode[]);
int kml_point_elements(int sd, char altitude_mode[], bool extrude, bool tessellate);
int kml_open_coordinates(int sd);
int kml_close_coordinates(int sd);
int kml_input_coordinate(int sd, Position position);
int kml_input_location(int sd, Position position);
int kml_input_orientation(int sd, Orientation orientation);
int kml_input_scale(int sd, char x_value[], char y_value[], char z_value[]);
int kml_open_model(int sd, char model_id[]);
int kml_close_model(int sd);
int kml_open_networklink(int sd);
int kml_close_networklink(int sd);
int kml_link(int sd, char link[], char refresh_mode[], char refresh_interval[]);

#endif

