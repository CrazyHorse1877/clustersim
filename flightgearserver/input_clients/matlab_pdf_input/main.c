#include <stdlib.h>
#include "pdf_input.h"
#include "parse_arg.h"


int main(int argc, char* argv[]){
	char** name_map = new_standard_name_map();
	char** usage_map = new_standard_usage_map();

	char* server_host = get_arg(argc, argv, "server_host", name_map, usage_map).values[0];
	int server_port = atoi(get_arg(argc, argv, "server_port", name_map, usage_map).values[0]);

	//input_pdf("localhost", 6969);
	input_pdf(server_host, server_port);
}
