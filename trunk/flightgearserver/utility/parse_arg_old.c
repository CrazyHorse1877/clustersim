#include<parse_arg.h>

char** new_ascii_table_sized_array_of_strings(){
	int ascii_table_size = 128;

	char* array[] = (char**)malloc(sizeof(char*) * ascii_table_size);

	for(int i = 0; i < ascii_table_size ; i++){
		array[i] = NULL;
	}
	return array;
}


char** new_standard_name_map(){

	char* standard_name_map[] = new_ascii_table_sized_array_of_strings();

	standard_name_map['h'] = "help";
	standard_name_map['u'] = "usage";

	standard_name_map['s'] = "server_host";
	standard_name_map['p'] = "server_port";
	standard_name_map['c'] = "client_host";
	standard_name_map['o'] = "client_port";

	return standard_name_map;
}

char** new_standard_usage_map(){

	char* standard_usage_map[] = new_ascii_table_sized_array_of_strings();

	standard_usage_map['h'] = "-h, --help: display a help message";


	return standard_usage_map;
}




Argument new_argument(char name[], char* values[], int num_values) {
	Argument arg;

	arg.name = name;
	arg.values = values;
	arg.num_values = num_values;

	return arg;
}


/* long name:  --arg=value,value */
/* short name: -a=value          */
Argument parse_arg(char* arg, void(*error_handler)(char**, char**), char** name_map, char** usage_map) {
	
	int arg_length = strlen(arg);

	if(arg_length < 2 || arg[0] != '-'){
		error_handler(name_map, usage_map);
	}

	/*remove the first - from the argument*/
	arg++;
	arg_length--;

	char arg_name[];
	if(arg[0] != '-'){
		if ((arg_name = short_to_long_name[arg[0]]) == NULL){
			error_handler(name_map, usage_map);
		}
		arg++;
		arg_length--;
	}else{
		arg++;
		arg_length--;
		int arg_name_length = 0;
		while(arg[arg_name_length] != '=' && arg[arg_name_length] != '\0'){
			arg_name_length++;
		}

		arg_name = (char*)malloc(sizeof(char) * (arg_name_length + 1));
		strncpy(arg_name, arg, arg_name_length);

		arg += arg_name_length;
		arg_length -= arg_name_length;
	}

	if(arg_name_length == 0){
		error_handler(name_map, usage_map);
	}else{
		if(arg[0] != '='){
			error_handler(name_map, usage_map);
		}
		arg++;
		arg_length--;
		int num_values = 1;
		int i = 0;
		while(arg[i] != '\0'){
			if(arg[i] == ','){
				num_values++;
			}
			i++;
		}

		char* values[] = (char**)malloc(sizeof(char*) * num_values);
		int value_index = 0;

		for(i = 0; i < arg_length; i++){
			if(arg[i] == ','){
				if(i == 0){
					error_handler(name_map, usage_map);
				}
				values[value_index] = malloc(sizeof(char) * (i + 1));
				arg[i] = '\0';
				strncpy(values[value_index], arg, i);
				value_index++;
				arg += i + 1;
				arg_length -= i + 1;
				i = 0;
			}
		}
		if(i == 0){
			error_handler(name_map, usage_map);
		}
		values[value_index] = malloc(sizeof(char) * (i + 1));
		arg[i] = '\0';
		strncpy(values[value_index], arg, i);
		value_index++;
		error_handler(name_map, usage_map);
	}
}


int parse_port(Argument arg, void(*error_handler)(char**, char**), char** name_map, char** usage_map) {

}

int parse_host


int parse_server_port

int parse_server_host

int parse_client_port

int parse_client_host


