#include "kml.h"
#include "tcp.h"
#include "string.h"

static const int buf_size = 100;

int kml_header(int sd) {
	int len = 0;
	char version_tag[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
	char kml_tag[] = "<kml xmlns=\"http://earth.google.com/kml/2.1\">\r\n";

	len += strlen(version_tag);
	len += strlen(kml_tag);

	if (sd >= 0) {
		send_string_TCP(sd, version_tag);
		send_string_TCP(sd, kml_tag);
	}

	return len;
}

int kml_close(int sd) {
	int len = 0;
	char close_tag[] = "</kml>\r\n";

	len += strlen(close_tag);

	if (sd >= 0) {
		send_string_TCP(sd, close_tag);
	}

	return len;
}

int kml_open_document(int sd) {
	int len = 0;
	char open_document_tag[] = "<Document>\r\n";

	len += strlen(open_document_tag);

	if (sd >= 0) {
		send_string_TCP(sd, open_document_tag);
	}

	return len;
}

int kml_close_document(int sd) {
	int len = 0;
	char close_document_tag[] = "</Document>\r\n";

	len += strlen(close_document_tag);

	if (sd >= 0) {
		send_string_TCP(sd, close_document_tag);
	}

	return len;
}

int kml_name(int sd, char name[]) {
	int len = 0;
	char name_open_tag[] = "<name>";
	char name_close_tag[] = "</name>\r\n";

	len += strlen(name_open_tag);
	len += strlen(name);
	len += strlen(name_close_tag);

	if (sd >= 0) {
		send_string_TCP(sd, name_open_tag);
		send_string_TCP(sd, name);
		send_string_TCP(sd, name_close_tag);
	}
	return len;
}

int kml_line_style(int sd, char style_id[], char color[], char width[]) { 
	int len = 0;
	char style_open_tag[buf_size];
	sprintf(style_open_tag, "<Style id=\"%s\">\r\n", style_id);

	char linestyle_open_tag[] = "<LineStyle>\r\n";
	char linestyle_close_tag[] = "</LineStyle>\r\n";
	char color_open_tag[] = "<color>";
	char color_close_tag[] = "</color>\r\n";
	char width_open_tag[] = "<width>";
	char width_close_tag[] = "</width>\r\n";
	char style_close_tag[] = "</Style>\r\n";

	len += strlen(style_open_tag);
	len += strlen(linestyle_open_tag);
	len += strlen(color_open_tag);
	len += strlen(color);
	len += strlen(color_close_tag);
	len += strlen(width_open_tag);
	len += strlen(width);
	len += strlen(width_close_tag);
	len += strlen(linestyle_close_tag);
	len += strlen(style_close_tag);

	if (sd >= 0) {

	send_string_TCP(sd, style_open_tag);
	send_string_TCP(sd, linestyle_open_tag);

	send_string_TCP(sd, color_open_tag);
	send_string_TCP(sd, color);
	send_string_TCP(sd, color_close_tag);

	send_string_TCP(sd, width_open_tag);
	send_string_TCP(sd, width);
	send_string_TCP(sd, width_close_tag);
	
	send_string_TCP(sd, linestyle_close_tag);
	send_string_TCP(sd, style_close_tag);
	}

	return len;
}

int kml_polyline_style(int sd, char style_id[], char line_color[], char poly_color[], char width[]) { 
	int len = 0;
	
	char style_open_tag[buf_size];
	sprintf(style_open_tag, "<Style id=\"%s\">\r\n", style_id);
	
	char linestyle_open_tag[] = "<LineStyle>\r\n";
	char linestyle_close_tag[] = "</LineStyle>\r\n";
	char polystyle_open_tag[] = "<PolyStyle>\r\n";
	char polystyle_close_tag[] = "</PolyStyle>\r\n";

	char color_open_tag[] = "<color>";
	char color_close_tag[] = "</color>\r\n";
	char width_open_tag[] = "<width>";
	char width_close_tag[] = "</width>\r\n";
	char style_close_tag[] = "</Style>\r\n";

	len += strlen(style_open_tag);
	len += strlen(linestyle_open_tag);
	len += strlen(color_open_tag);
	len += strlen(line_color);
	len += strlen(color_close_tag);
	len += strlen(width_open_tag);
	len += strlen(width);
	len += strlen(width_close_tag);
	len += strlen(linestyle_close_tag);
	
	len += strlen(polystyle_open_tag);
	len += strlen(color_open_tag);
	len += strlen(poly_color);
	len += strlen(color_close_tag);

	len += strlen(polystyle_close_tag);
	len += strlen(style_close_tag);



	if (sd >= 0) {
	send_string_TCP(sd, style_open_tag);
	send_string_TCP(sd, linestyle_open_tag);

	send_string_TCP(sd, color_open_tag);
	send_string_TCP(sd, line_color);
	send_string_TCP(sd, color_close_tag);

	send_string_TCP(sd, width_open_tag);
	send_string_TCP(sd, width);
	send_string_TCP(sd, width_close_tag);
	
	send_string_TCP(sd, linestyle_close_tag);

	send_string_TCP(sd, polystyle_open_tag);
	
	send_string_TCP(sd, color_open_tag);
	send_string_TCP(sd, poly_color);
	send_string_TCP(sd, color_close_tag);

	send_string_TCP(sd, polystyle_close_tag);

	send_string_TCP(sd, style_close_tag);
	}

	return len;
}

int kml_open_point(int sd) {
	int len = 0;
	char open_point[] = "<Point>\r\n";

	len += strlen(open_point);

	if(sd >= 0) {
	send_string_TCP(sd,open_point);
	}
	return len;
}

int kml_close_point(int sd) {
	int len = 0;
	char close_point[] = "</Point>\r\n";

	len += strlen(close_point);

	if(sd >= 0) {
	send_string_TCP(sd, close_point);
	}
	return len;
}

int kml_open_placemark(int sd) {
	int len = 0;
	char open_placemark[] = "<Placemark>\r\n";

	len += strlen(open_placemark);

	if(sd >= 0) {
	send_string_TCP(sd, open_placemark);
	}
	return len;
}

int kml_close_placemark(int sd) {
	int len = 0;
	char close_placemark[] = "</Placemark>\r\n";

	len += strlen(close_placemark);

	if(sd >= 0) {
	send_string_TCP(sd, close_placemark);
	}
	return len;
}

int kml_styleurl(int sd, char style_id[]) {
	int len = 0;
	char styleurl_open_tag[] = "<styleUrl>";
	char styleurl_close_tag[] = "</styleUrl>\r\n";	

	char styleurl[buf_size];
	sprintf(styleurl, "%s#%s%s", styleurl_open_tag, style_id, styleurl_close_tag);

	len += strlen(styleurl);

	if(sd >=0) {
	send_string_TCP(sd, styleurl);
	}
	
	return len;

}


int kml_open_linestring(int sd) {
	int len = 0;
	char linestring_open_tag[] = "<LineString>\r\n";

	len += strlen(linestring_open_tag);

	if( sd >= 0){	
	send_string_TCP(sd, linestring_open_tag);
	}
	return len;
}

int kml_close_linestring(int sd) {
	int len = 0;
	char linestring_close_tag[] = "</LineString>\r\n";

	len += strlen(linestring_close_tag);
	
	if(sd >= 0) {
	send_string_TCP(sd, linestring_close_tag);
	}
	return len;
}

int kml_altitude_mode(int sd, char altitude_mode[]){
	int len = 0;
	
	char altitudemode_open_tag[] = "<altitudeMode>";
	char altitudemode_close_tag[] = "</altitudeMode>\r\n";

	len += strlen(altitudemode_open_tag);
	len += strlen(altitude_mode);
	len += strlen(altitudemode_close_tag);

	if (sd >= 0) {
		send_string_TCP(sd, altitudemode_open_tag);
		send_string_TCP(sd, altitude_mode);
		send_string_TCP(sd, altitudemode_close_tag);
	}

	return len;
}


int kml_point_elements(int sd, char altitude_mode[], bool extrude, bool tessellate) {
	int len = 0;
	char extrude_open_tag[] = "<extrude>";
	char extrude_close_tag[] = "</extrude>\r\n";
	char tessellate_open_tag[] = "<tessellate>";
	char tessellate_close_tag[] = "</tessellate>\r\n";
	char altitudemode_open_tag[] = "<altitudeMode>";
	char altitudemode_close_tag[] = "</altitudeMode>\r\n";

	char point_elements[buf_size];

	sprintf(point_elements, "%s%d%s%s%d%s%s%s%s", extrude_open_tag, (int)extrude, extrude_close_tag, tessellate_open_tag, (int)tessellate, tessellate_close_tag, altitudemode_open_tag, altitude_mode, altitudemode_close_tag);

	len += strlen(point_elements);

	if(sd >= 0){
		send_string_TCP(sd, point_elements);
	}


	return len;
}

int kml_open_coordinates(int sd) {
	int len = 0;
	char open_coordinates[] = "<coordinates>\r\n";

	len += strlen(open_coordinates);

	if (sd >= 0){
	send_string_TCP(sd, open_coordinates);
	}
	return len;
}

int kml_close_coordinates(int sd) {
	int len = 0;
	char close_coordinates[] = "</coordinates>\r\n";
	
	len += strlen(close_coordinates);

	if(sd >= 0) {
	send_string_TCP(sd, close_coordinates);
	}
	return len;
}

int kml_input_coordinate(int sd, Position position) {
	int len = 0;
	char coordinate[buf_size];

	sprintf(coordinate, "%lf,%lf,%lf\r\n", position.longitude, position.latitude, position.altitude);

	len += strlen(coordinate);

	if(sd >= 0) {
		send_string_TCP(sd, coordinate);
	}
	return len;
}

int kml_input_location(int sd, Position position) {
	int len = 0;

	char open_location[] = "<Location>\r\n";
	char close_location[] = "</Location>\r\n";
	
	char open_longitude[] = "<longitude>";
	char close_longitude[] = "</longitude>\r\n";
	char longitude[buf_size];
	sprintf(longitude, "%s%lf%s", open_longitude, position.longitude, close_longitude);

	char open_latitude[] = "<latitude>";
	char close_latitude[] = "</latitude>\r\n";
	char latitude[buf_size];
	sprintf(latitude, "%s%lf%s", open_latitude, position.latitude, close_latitude);
	
	char open_altitude[] = "<altitude>";
	char close_altitude[] = "</altitude>\r\n";
	char altitude[buf_size];
	sprintf(altitude, "%s%lf%s", open_altitude, position.altitude, close_altitude);

	len += strlen(open_location);
	len += strlen(longitude);
	len += strlen(latitude);
	len += strlen(altitude);
	len += strlen(close_location);

	if(sd >= 0) {
		send_string_TCP(sd, open_location);
		send_string_TCP(sd, longitude);
		send_string_TCP(sd, latitude);
		send_string_TCP(sd, altitude);
		send_string_TCP(sd, close_location);
	}
	return len;
}


int kml_input_orientation(int sd, Orientation orientation) {
	int len = 0;
	
	char open_orientation[] = "<Orientation>\r\n";
	char close_orientation[] = "</Orientation>\r\n";
	
	char open_heading[] = "<heading>";
	char close_heading[] = "</heading>\r\n";
	char heading[buf_size];

	/* The - 180 is due to google earths heading being 180 degrees off despite the model front being the front of the plane may be a bug in the model or in google earth*/

	sprintf(heading, "%s%lf%s", open_heading, orientation.yaw - 180, close_heading);

	char open_tilt[] = "<tilt>";
	char close_tilt[] = "</tilt>\r\n";
	char tilt[buf_size];
	sprintf(tilt, "%s%lf%s", open_tilt, orientation.pitch, close_tilt);
	
	char open_roll[] = "<roll>";
	char close_roll[] = "</roll>\r\n";
	char roll[buf_size];
	sprintf(roll, "%s%lf%s", open_roll, orientation.roll, close_roll);

	len += strlen(open_orientation);
	len += strlen(heading);
	len += strlen(tilt);
	len += strlen(roll);
	len += strlen(close_orientation);

	if(sd >= 0) {
		send_string_TCP(sd, open_orientation);
		send_string_TCP(sd, heading);
		send_string_TCP(sd, tilt);
		send_string_TCP(sd, roll);
		send_string_TCP(sd, close_orientation);
	}
	return len;
}

int kml_input_scale(int sd, char x_value[], char y_value[], char z_value[]){
	int len = 0;

	char open_scale[] = "<Scale>\r\n";
	char close_scale[] = "</Scale>\r\n";

	char open_x[] = "<x>";
	char close_x[] ="</x>\r\n";
	char x[buf_size];
	sprintf(x, "%s%s%s", open_x, x_value, close_x);

	char open_y[] = "<y>";
	char close_y[] ="</y>\r\n";
	char y[buf_size];
	sprintf(y, "%s%s%s", open_y, y_value, close_y);

	char open_z[] = "<z>";
	char close_z[] ="</z>\r\n";
	char z[buf_size];
	sprintf(z, "%s%s%s", open_z, z_value, close_z);

	len += strlen(open_scale);
	len += strlen(x);
	len += strlen(y);
	len += strlen(z);
	len += strlen(close_scale);

	if(sd >= 0) {
		send_string_TCP(sd, open_scale);
		send_string_TCP(sd, x);
		send_string_TCP(sd, y);
		send_string_TCP(sd, z);
		send_string_TCP(sd, close_scale);
	}
	return len;
}
		

int kml_open_model(int sd, char model_id[]) {
	int len = 0;
	char open_model[buf_size];

	sprintf(open_model, "<Model id= \"%s\">\r\n", model_id);

	len += strlen(open_model);

	if (sd >= 0) {
		send_string_TCP(sd, open_model);
	}

	return len;
}


int kml_close_model(int sd) {
	int len = 0;
	char close_model[] = "</Model>\r\n";

	len += strlen(close_model);

	if (sd >= 0) {
		send_string_TCP(sd, close_model);
	}

	return len;
}



int kml_open_networklink(int sd) {
	int len = 0;
	char open_networklink_tag[] = "<NetworkLink>\r\n";

	len += strlen(open_networklink_tag);

	if(sd >= 0){
	send_string_TCP(sd, open_networklink_tag);
	}
	return len;
}

int kml_close_networklink(int sd) {
	int len = 0;
	char close_networklink_tag[] = "</NetworkLink>\r\n";

	len += strlen(close_networklink_tag);

	if(sd >= 0){
	send_string_TCP(sd, close_networklink_tag);
	}
	return len;
}

int kml_link(int sd, char link[], char refresh_mode[], char refresh_interval[]) {
	int len = 0;
	char link_open_tag[] = "<Link>\r\n";
	char link_close_tag[] = "</Link>\r\n";
	char href_open_tag[] = "<href>";
	char href_close_tag[] = "</href>\r\n";
	char refreshmode_open_tag[] = "<refreshMode>";
	char refreshmode_close_tag[] = "</refreshMode>\r\n";
	char refreshinterval_open_tag[] = "<refreshInterval>";
	char refreshinterval_close_tag[] = "</refreshInterval>\r\n";

	len += strlen(link_open_tag);
	len += strlen(href_open_tag);
	len += strlen(link);
	len += strlen(href_close_tag);
	len += strlen(refreshmode_open_tag);
	len += strlen(refresh_mode);
	len += strlen(refreshmode_close_tag);
	len += strlen(refreshinterval_open_tag);
	len += strlen(refresh_interval);
	len += strlen(refreshinterval_close_tag);
	len += strlen(link_close_tag);

	if (sd >= 0) {
	send_string_TCP(sd, link_open_tag);
	
	send_string_TCP(sd, href_open_tag);
	send_string_TCP(sd, link);
	send_string_TCP(sd, href_close_tag);

	send_string_TCP(sd, refreshmode_open_tag);
	send_string_TCP(sd, refresh_mode);
	send_string_TCP(sd, refreshmode_close_tag);
	
	send_string_TCP(sd, refreshinterval_open_tag);
	send_string_TCP(sd, refresh_interval);
	send_string_TCP(sd, refreshinterval_close_tag);
	
	send_string_TCP(sd, link_close_tag);
	}
	return len;

}

