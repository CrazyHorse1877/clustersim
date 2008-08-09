#include "pdf_output.h"
#include <stdlib.h>
#include "parse_arg.h"

int main(int argc, char* argv[]){
	char** name_map = new_standard_name_map();
	char** usage_map = new_standard_usage_map();

	char* server_host = get_arg(argc, argv, "server_host", name_map, usage_map).values[0];
	int server_port = atoi(get_arg(argc, argv, "server_port", name_map, usage_map).values[0]);

	//output_pdf_to_ge("localhost", 6969);
	output_pdf_to_ge(server_host, server_port);
}
