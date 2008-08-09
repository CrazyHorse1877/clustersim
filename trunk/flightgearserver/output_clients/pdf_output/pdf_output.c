#include <pthread.h>
#include <stdlib.h>
#include "kml.h"
#include "position.h"
#include "color.h"
#include "tcp.h"
#include "pdf.h"
#include "output_client.h"

static const int GET_LENGTH = -1;

double pdf_value_to_altitude(unsigned char value) {
	return value;
}

Color* get_color_map() {
	Color dark_blue     = new_color(0x00, 0x00, 0x99);
	Color blue          = new_color(0x00, 0x00, 0xff);
	Color aqua          = new_color(0x00, 0xff, 0xff);
	Color yellow_green  = new_color(0xcc, 0xff, 0x00);
	Color yellow        = new_color(0xff, 0xff, 0x00);
	Color yellow_orange = new_color(0xff, 0xcc, 0x00);
	Color orange        = new_color(0xff, 0x99, 0x00);
	Color red           = new_color(0xff, 0x00, 0x00);
	Color dark_red      = new_color(0x99, 0x00, 0x00);

	Color* color_map = (Color*)malloc(sizeof(Color) * 9);

	color_map[0] = dark_blue;
	color_map[1] = blue;
	color_map[2] = aqua;
	color_map[3] = yellow_green;
	color_map[4] = yellow;
	color_map[5] = yellow_orange;
	color_map[6] = orange;
	color_map[7] = red;
	color_map[8] = dark_red;

	return color_map;
}

char* get_color_of_line_segment(double altitude){
	int max_altitude = 255;
	int min_altitude = 128;
	double normalised_altitude = (altitude - min_altitude)/(max_altitude - min_altitude);
	double num_intervals = 8;
	double altitude_interval = 1 / num_intervals;
	int prev, next;

	Color* color_map = get_color_map();
	char* color = (char*)malloc(sizeof(char) * 8);
	int red, green, blue;

	prev = (int)(normalised_altitude/altitude_interval);

	if (prev != num_intervals){
		next = prev + 1;

		red = (((color_map[next].red - color_map[prev].red)/(next - prev)) * normalised_altitude) + color_map[prev].red;
		green = (((color_map[next].green - color_map[prev].green)/(next - prev)) * normalised_altitude) + color_map[prev].green;
		blue = (((color_map[next].blue - color_map[prev].blue)/(next - prev)) * normalised_altitude) + color_map[prev].blue;
	} else {
		red = color_map[prev].red;
		green = color_map[prev].green;
		blue = color_map[prev].blue;
	}

	sprintf(color, "ff%02x%02x%02x", blue, green, red);

	return color;
}

int input_line_segment_to_kml(int sd, Position position1, Position position2) {
	int len = 0;

	char altitude_mode[] = "absolute";
	bool extrude = false;
	bool tessellate = false;

	char line_width[] = "2";
	char *color;

	if(position1.altitude > position2.altitude){
		color = get_color_of_line_segment(position1.altitude);
	}else {
		color = get_color_of_line_segment(position2.altitude);
	}

	len += kml_line_style(sd, /*style_id*/color, color, line_width);

	len += kml_open_placemark(sd);
	len += kml_styleurl(sd, /*style_id*/color);
	len += kml_open_linestring(sd);
	len += kml_point_elements(sd, altitude_mode, extrude, tessellate);
	len += kml_open_coordinates(sd);

	len += kml_input_coordinate(sd, position1);
	len += kml_input_coordinate(sd, position2);

	len += kml_close_coordinates(sd);
	len += kml_close_linestring(sd);
	len += kml_close_placemark(sd);

	return len;
}

Position row_col_position(Pdf pdf, unsigned int row, unsigned int col)
{
	double longitude = pdf.start_longitude + col * pdf.delta_longitude;
	double latitude = pdf.start_latitude + row * pdf.delta_latitude;
	double altitude = pdf_value_to_altitude(pdf.data[row * pdf.num_cols + col]);

	return new_position(latitude, longitude, altitude);
}

Position col_row_position(Pdf pdf, unsigned int col, unsigned int row)
{
	return row_col_position(pdf, row, col);
}

int input_semipdf_to_kml(int sd, Pdf pdf, unsigned int axis1_size, unsigned int axis2_size, Position(*generate_position)(Pdf, unsigned int, unsigned int))
{
	int len = 0;
	unsigned int resolution_const = 1;
	Position invalid_position = new_position(-999, -999, -999);

	for(unsigned int axis1=0; axis1 < axis1_size; axis1+=resolution_const) {
		Position last_position, prev_position = invalid_position;

		for(unsigned int axis2=0; axis2 < axis2_size; axis2+=resolution_const) {
			Position position = generate_position(pdf, axis1, axis2);

			if(axis2 == 0) {
				prev_position = position;
			} else if(last_position.altitude != position.altitude) {
				len += input_line_segment_to_kml(sd, prev_position, last_position);
				len += input_line_segment_to_kml(sd, last_position, position);
				prev_position = position;
			}

			last_position = position;
		}

		len += input_line_segment_to_kml(sd, prev_position, last_position);
	}

	return len;
}

int input_pdf_to_kml(int sd, Pdf pdf) {
	int len = 0;

	/* Generate the vertical lines */
	len += input_semipdf_to_kml(sd, pdf, pdf.num_cols, pdf.num_rows, col_row_position);
	/* Generate the horizontal lines */
	len += input_semipdf_to_kml(sd, pdf, pdf.num_rows, pdf.num_cols, row_col_position);

	return len;
}

int get_kml_length(Pdf pdf, char name[]){
	int len = 0;

	len += kml_header(GET_LENGTH);
	len += kml_open_document(GET_LENGTH);
	len += kml_name(GET_LENGTH, name);

	len += input_pdf_to_kml(GET_LENGTH, pdf);

	len += kml_close_document(GET_LENGTH);
	len += kml_close(GET_LENGTH);

	return len;
}

void send_pdf_kml(int sd, Pdf pdf){
	char name[100];
	sprintf(name, "pdf_%d", pdf.id);

	int length = get_kml_length(pdf, name) ;
	char content_length[100];
	sprintf(content_length, "Content-Length: %d", length);

	send_string_line_TCP(sd, "HTTP/1.1 200 OK");
	send_string_line_TCP(sd, "Connection: close");
	send_string_line_TCP(sd, "Content-Type: application/vnd.google-earth.kml+xml");
	send_string_line_TCP(sd, content_length);
	send_string_line_TCP(sd, "Server: MyServer");
	send_string_line_TCP(sd, "");

	kml_header(sd);
	kml_open_document(sd);
	kml_name(sd, name);

	input_pdf_to_kml(sd, pdf);

	kml_close_document(sd);
	kml_close(sd);
}

typedef struct{
	pthread_mutex_t lock;
	Pdf pdf;
}UserData;

bool write_pdf_to_ge(Pdf pdf, void* user_data){
	UserData* data = (UserData*)user_data;

	if((pdf.num_cols > 0) && (pdf.num_rows > 0)){
		pthread_mutex_lock(&(data->lock));
		delete_pdf(data->pdf);
		data->pdf = pdf;
		pthread_mutex_unlock(&(data->lock));
	}
	return true;
}

void* web_server_thread_entry(void* user_data) {
	UserData* data = (UserData*)user_data;

	int web_server_welcome_socket = open_welcome_socket_TCP(2080, true);

	while(true){
		int web_server_connected_socket = accept_connection_from_client_TCP(web_server_welcome_socket);
		if (web_server_connected_socket >= 0) {
			pthread_mutex_lock(&(data->lock));
			Pdf pdf = data->pdf;
			print_pdf(pdf);
			send_pdf_kml(web_server_connected_socket, pdf);
			pthread_mutex_unlock(&(data->lock));
			close_TCP(web_server_connected_socket);
		}
	}
	pthread_exit(NULL);
	return NULL;
}

void* server_thread_entry(void* user_data) {
   get_multiple_pdf_from_server(write_pdf_to_ge, user_data);
   pthread_exit(NULL);
   return NULL;
}

//void create_thread(pthread_t* thread, void*(*thread_entry)(void* user_data), void* user_data) {
void create_thread(pthread_t* thread, void*(*thread_entry)(void* user_data), void* user_data) {
   if(0){//XXX this is a hack,  undefined reference to pthread_create'
      if((pthread_create(thread, NULL, thread_entry, user_data)) != 0) {
	 fprintf(stderr, "Failed to create thread\n");
	 exit(EXIT_FAILURE);
      }
   }
}

void output_pdf_to_ge(char server_host[], int server_port){
	printf("PDF Output Client\n");
	printf("-----------------\n");

	register_for_pdf_to_server(server_host, server_port, true);

	pthread_t web_server_thread;
	pthread_t server_thread;

	UserData user_data;
	pthread_mutex_init(&(user_data.lock), NULL);
	user_data.pdf = new_pdf(0,0,0,0,0,0,0,NULL);

	create_thread(&web_server_thread, web_server_thread_entry, &user_data);
	create_thread(&server_thread, server_thread_entry, &user_data);
	server_thread_entry(&user_data);

	void* status;
	pthread_join(web_server_thread, &status);
	pthread_join(server_thread, &status);

	pthread_mutex_destroy(&user_data.lock);
}

