#include <stdlib.h>
#include "fg_output.h"
#include "parse_arg.h"

int main(int argc, char* argv[]){
	char** name_map = new_standard_name_map();
	char** usage_map = new_standard_usage_map();

	name_map['f'] = "fg_host";
	name_map['g'] = "fg_port";

	usage_map['f'] = "-f ,--fg_host=<flight gear property server host name>";
	usage_map['g'] = "-g ,--fg_port=<flight gear property server port number>";

	char* server_host = get_arg(argc, argv, "server_host", name_map, usage_map).values[0];
	int server_port = atoi(get_arg(argc, argv, "server_port", name_map, usage_map).values[0]);
	char* fg_host = get_arg(argc, argv, "fg_host", name_map, usage_map).values[0];
	int fg_port = atoi(get_arg(argc, argv, "fg_port", name_map, usage_map).values[0]);
	
	//output_flight_info_to_fg("localhost", 5502, "localhost", 6969);
	output_flight_info_to_fg(fg_host, fg_port, server_host, server_port);

	return 0;
}

