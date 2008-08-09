#include <stdio.h>
#include <stdlib.h>
#include "fg_input.h"
#include "parse_arg.h"

int main(int argc, char* argv[]){
   char** name_map = new_standard_name_map();
   char** usage_map = new_standard_usage_map();
   
   name_map['i'] = "id";
   name_map['f'] = "fg_host";
   name_map['g'] = "fg_port";
   
   usage_map['i'] = "-i ,--id=<flight id number>";
   usage_map['f'] = "-f ,--fg_host=<flight gear property server host name>";
   usage_map['g'] = "-g ,--fg_port=<flight gear property server port number>";
   
   int id = atoi(get_arg(argc, argv, "id", name_map, usage_map).values[0]);
   char* server_host = get_arg(argc, argv, "server_host", name_map, usage_map).values[0];
   int server_port = atoi(get_arg(argc, argv, "server_port", name_map, usage_map).values[0]);
   char* fg_host = get_arg(argc, argv, "fg_host", name_map, usage_map).values[0];
   int fg_port = atoi(get_arg(argc, argv, "fg_port", name_map, usage_map).values[0]);
   
   printf("id = %d, server_host = %s, server_port = %d, fg_host = %s, fg_port = %d\n", id, server_host, server_port, fg_host, fg_port);
   
   
   //input_flight_info_from_fg(201,"localhost", 5501, "192.168.0.100", 6969);
   printf("Reading from %s on port %d\n", fg_host, fg_port);
   input_flight_info_from_fg(id, fg_host, fg_port, server_host, server_port);
   
   return 0;
}

