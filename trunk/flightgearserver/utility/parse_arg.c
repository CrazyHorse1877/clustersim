#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse_arg.h"

char** new_ascii_table_sized_array_of_strings(){
	int ascii_table_size = 128;

	char** array= (char**)malloc(sizeof(char*) * ascii_table_size);

	for(int i = 0; i < ascii_table_size ; i++){
		array[i] = NULL;
	}
	return array;
}

char** new_standard_name_map(){
	char** standard_name_map = new_ascii_table_sized_array_of_strings();

	standard_name_map['h'] = "help";
	standard_name_map['u'] = "usage";

	standard_name_map['s'] = "server_host";
	standard_name_map['p'] = "server_port";

	return standard_name_map;
}

char** new_standard_usage_map(){
	char** standard_usage_map = new_ascii_table_sized_array_of_strings();

	standard_usage_map['h'] = "-h, --help: display a help message";
	standard_usage_map['s'] = "-s, --server_host=<host_name>";
	standard_usage_map['p'] = "-p, --server_port=<port_number>";


	return standard_usage_map;
}

void print_usage_msg(char** usage_map){
	printf("Required Arguments\n");
	printf("------------------------------------------------------------------------\n");
	for(int i = 0 ; i < 128 ; i++){
		if (usage_map[i] != NULL && i != 'h'){
			printf("%s\n", usage_map[i]);
		}
	}
	printf("------------------------------------------------------------------------\n");
	printf("%s\n", usage_map['h']);
	exit(EXIT_FAILURE);
}

Argument new_argument(char* name, char** values, int num_values) {
	Argument arg;

	arg.name = name;
	arg.values = values;
	arg.num_values = num_values;

	return arg;
}

int get_position_of_equal_sign(char* arg, int arg_length){
	int equal_sign_position = -1;

	for(int i=0; i < arg_length; i++){
		if(arg[i] == '='){
			if(equal_sign_position < 0){
				equal_sign_position = i;
			}else{
				return -1;
			}
		}
	}
	return equal_sign_position;
}

char* parse_name(char* arg_head, int arg_head_length, char** name_map){

	if(arg_head_length > 1 && arg_head[0] == '-'){
		arg_head++;

		if(arg_head[0] == '-'){
			arg_head++;
			return arg_head;
		}else if(arg_head_length == 2){
			return name_map[(int)arg_head[0]]; /*equals NULL if short name not in name map*/
		}
	}
	return NULL;
}

int get_num_values(char* arg_tail, int arg_tail_length){
	int num_values = 1;

	for(int i=0; i < arg_tail_length; i++){
		if(arg_tail[i] == ','){
			num_values++;
		}
	}
	return num_values;
}

char** parse_values(char* arg_tail, int arg_tail_length, int num_values){
	char** values = (char**)malloc(sizeof(char*) * num_values);

	int value_index = 0;
	int j;

	for(j=0; j <= arg_tail_length; j++){
		if(arg_tail[j] == ',' || arg_tail[j] == '\0'){
			values[value_index] = malloc(sizeof(char) * (j + 1));
			strncpy(values[value_index], arg_tail, j);
			
			values[value_index][j] = '\0';	
			value_index++;

			arg_tail += j + 1;
			arg_tail_length -= j + 1;

			j = -1;
		}
	}

	return values;
}


Argument parse_arg(char* arg, char** name_map, char** usage_map) {
	int arg_length = strlen(arg);
	int position_of_equal_sign = get_position_of_equal_sign(arg, arg_length);

	if (position_of_equal_sign < 0){
		print_usage_msg(usage_map);
	}

	int arg_head_length = position_of_equal_sign;
	char* arg_head = (char*)malloc(sizeof(char) * (arg_head_length + 1));
	strncpy(arg_head, arg, arg_head_length);
	arg_head[arg_head_length] = '\0';

	arg += arg_head_length + 1;

	int arg_tail_length = arg_length - (arg_head_length + 1 );
	char* arg_tail = (char*)malloc(sizeof(char) * (arg_tail_length + 1));
	strncpy(arg_tail, arg, arg_tail_length);
	arg_tail[arg_tail_length] = '\0';

	char* arg_name = parse_name(arg_head, arg_head_length, name_map);

	if (arg_name == NULL){
		print_usage_msg(usage_map);
	}

	int num_values = get_num_values(arg_tail,arg_tail_length);
	char** arg_values = parse_values(arg_tail, arg_tail_length, num_values);

	return new_argument(arg_name, arg_values, num_values);
}

Argument get_arg(int argc, char* argv[], char* arg_name, char** name_map, char** usage_map){
	Argument arg;

	for(int i = 1 ; i < argc; i ++){
		arg = parse_arg(argv[i], name_map, usage_map);
		if(strcmp(arg.name, arg_name)== 0){
			return arg;
		}
	}
	printf("Error: Not all required arguments supplied\n");
	print_usage_msg(usage_map);
	return new_argument(NULL, NULL, 0);
}

/*
int main(int argc, char* argv[]){
	printf("Server port = %d\n", atoi(get_arg(argc, argv, "server_port", new_standard_name_map(), new_standard_usage_map()).values[0]));
	return 0;
}
*/
