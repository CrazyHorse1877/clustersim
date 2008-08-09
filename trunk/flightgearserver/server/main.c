#include <stdlib.h>
#include "server.h"
#include "parse_arg.h"


int main(int argc, char* argv[]){
	char** name_map = new_standard_name_map();
	char** usage_map = new_standard_usage_map();

	name_map['s'] = NULL;
	usage_map['s'] = NULL;

	int server_port = atoi(get_arg(argc, argv, "server_port", name_map, usage_map).values[0]);
	//server(6969);
	server(server_port);

	return EXIT_SUCCESS;
}

