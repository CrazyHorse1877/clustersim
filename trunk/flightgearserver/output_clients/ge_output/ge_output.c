#include <pthread.h>
#include "kml.h"
#include "tcp.h"
#include "flight_path.h"
#include "squad_paths.h"
#include "output_client.h"
#include "bool.h"

static const int GET_LENGTH = -1;

int input_flight_path(int sd, FlightPath* flight_path){
	int len = 0;
	int size = get_path_size(flight_path);

	for (int i = 0; i < size ; i ++) {
		len += kml_input_coordinate(sd, get_position_from_flight_path(flight_path, i));
	}

	return len;
}		
		
int input_point(int sd, FlightPath* flight_path){
	int len = 0;
	int size = get_path_size(flight_path);

	len +=	kml_open_placemark(sd);
	len +=	kml_open_point(sd);
	len +=	kml_point_elements(sd, "absolute", true, true);
	len +=	kml_open_coordinates(sd);
	len +=	kml_input_coordinate(sd, get_position_from_flight_path(flight_path, (size - 1)));
	len +=	kml_close_coordinates(sd);
	len +=	kml_close_point(sd);
	len +=	kml_close_placemark(sd);

	return len;
}

int input_model(int sd, FlightPath* flight_path) {
	int len = 0;
	int size = get_path_size(flight_path);

	len +=	kml_open_placemark(sd);
	len +=  kml_open_model(sd, "UAV");
	len +=  kml_altitude_mode(sd, "absolute");
	len +=  kml_input_location(sd, get_position_from_flight_path(flight_path, (size - 1)));
	len +=  kml_input_orientation(sd, get_orientation_from_flight_path(flight_path, (size - 1)));
	len +=  kml_input_scale(sd, "1", "1", "1");
	len +=  kml_link(sd, "C:/cygwin/home/Marie Kazacos/thesis/program/output_clients/ge_output/uav1_front.dae", "onChange", "4");
	len +=  kml_close_model(sd);
	len +=	kml_close_placemark(sd);

	return len;
}

int input_squad_paths(int sd, SquadPath* squad_path){
	int len = 0;
	int size = get_squad_path_size(squad_path);
	FlightPath* flight_path;
	int id;
	char style_id[100];
	char name[100];

	for (int i = 0; i < size ; i++) {
		flight_path = get_flight_path_from_squad_path(squad_path, i);
		id = get_id_from_flight_path(flight_path);
		sprintf(style_id, "style_%d", id);
		sprintf(name, "flight_%d", id);

		len += input_model(sd, flight_path);
		len += kml_polyline_style(sd, style_id, "7f00ffff", "7f00ff00", "4");
		len += kml_open_placemark(sd);
		len += kml_name(sd, name);
		len += kml_styleurl(sd, style_id);
		len += kml_open_linestring(sd);
		len += kml_point_elements(sd, "absolute", true, true);

		len += kml_open_coordinates(sd);
		len += input_flight_path(sd, flight_path);
		len += kml_close_coordinates(sd);

		len += kml_close_linestring(sd);
		len += kml_close_placemark(sd);
	}

	return len;
}

int get_kml_length(SquadPath* squad_path){
	int len = 0;

	len +=	kml_header(GET_LENGTH);
	len +=	kml_open_document(GET_LENGTH);
	
	len += input_squad_paths(GET_LENGTH, squad_path);	
	
	len +=	kml_close_document(GET_LENGTH);
	len +=	kml_close(GET_LENGTH);

	return len;
}

void send_kml(int sd, SquadPath* squad_path){
	int length = get_kml_length(squad_path);
	
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

	input_squad_paths(sd, squad_path);
	
	kml_close_document(sd);
	kml_close(sd);
}

typedef struct{
	pthread_mutex_t lock;
	SquadPath* squad_path;
}UserData;

bool write_flight_info_to_ge(Flight_info flight_info, void* user_data) {
	UserData* data = (UserData*)user_data;
	SquadPath* squad_path = data->squad_path;
	
	if (flight_info.time_stamp.year > 0){
		print_flight_info(flight_info);
		pthread_mutex_lock(&(data->lock));
		add_flight_info_to_squad_path(squad_path, flight_info);
		pthread_mutex_unlock(&(data->lock));
	}

	return true;
}

void* web_server_thread_entry(void* user_data) {
	UserData* data = (UserData*)user_data;
	SquadPath* squad_path = data->squad_path;

	int web_server_welcome_socket = open_welcome_socket_TCP(2081, true);

	while(true){
		int web_server_connected_socket = accept_connection_from_client_TCP(web_server_welcome_socket);
		if (web_server_connected_socket >= 0) {
			pthread_mutex_lock(&(data->lock));
			send_kml(web_server_connected_socket, squad_path);
			pthread_mutex_unlock(&(data->lock));
			close_TCP(web_server_connected_socket);
		}
	}
	pthread_exit(NULL);
	return NULL;
}

void* server_thread_entry(void* user_data) {
	get_multiple_flight_info_from_server(write_flight_info_to_ge, user_data);
	pthread_exit(NULL);
	return NULL;
}

void create_thread(pthread_t* thread, void*(*thread_entry)(void* user_data), void* user_data) {
	if(pthread_create(thread, NULL, thread_entry, user_data) != 0) {
		fprintf(stderr, "Failed to create thread\n");
		exit(EXIT_FAILURE);
	}
}

void output_flight_info_to_ge(char server_host[], int server_port) {
	printf("Google Earth Output Client\n");
	printf("--------------------------\n");

	register_for_flight_info_to_server(server_host, server_port, true);

	pthread_t web_server_thread;
	pthread_t server_thread;

	UserData user_data;
	pthread_mutex_init(&(user_data.lock), NULL);
	user_data.squad_path = new_squad_path();

	create_thread(&web_server_thread, web_server_thread_entry, &user_data);
	create_thread(&server_thread, server_thread_entry, &user_data);

	void* status;
	pthread_join(web_server_thread, &status);
	pthread_join(server_thread, &status);

	pthread_mutex_destroy(&user_data.lock);
}

