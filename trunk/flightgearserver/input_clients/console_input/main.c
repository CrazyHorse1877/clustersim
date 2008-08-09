#include <stdlib.h>
#include "console_input.h"
#include "parse_arg.h"


int main(int argc, char* argv[]){
	char** name_map = new_standard_name_map();
	char** usage_map = new_standard_usage_map();

	name_map['i'] = "id";
	usage_map['i'] = "-i ,--id=<flight id number>";

	int id = atoi(get_arg(argc, argv, "id", name_map, usage_map).values[0]);
	char* server_host = get_arg(argc, argv, "server_host", name_map, usage_map).values[0];
	int server_port = atoi(get_arg(argc, argv, "server_port", name_map, usage_map).values[0]);

	input_flight_info_from_console(id, server_host, server_port);

	return EXIT_SUCCESS;
}

