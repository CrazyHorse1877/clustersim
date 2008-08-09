#include <stdio.h>

#include "console_output.h"
#include "bool.h"
#include "output_client.h"
#include "flight_info.h"

bool write_flight_info_to_console(Flight_info flight_info, void* user_data){
	(void)user_data;

	print_flight_info(flight_info);

	return true;
}

void output_flight_info_to_console(char server_host[], int server_port){
	register_for_flight_info_to_server(server_host, server_port, true);
	get_multiple_flight_info_from_server(write_flight_info_to_console, NULL);
}

