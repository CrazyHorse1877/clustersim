#ifndef _PARSE_ARG_H_
#define _PARSE_ARG_H_


typedef struct{
	char* name;
	int num_values;
	char** values;
}Argument;

//Argument new_argument(char name[], char** values, int num_values);


char** new_standard_name_map();
char** new_standard_usage_map();
Argument get_arg(int argc, char* argv[], char* arg_name, char** name_map, char** usage_map);

#endif

