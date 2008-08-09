#ifndef _PARSE_ARG_H_
#define _PARSE_ARG_H_


typedef struct{
	char name[];
	char* values[];
	int num_values;
}Argument;

Argument new_argument(char name[], char* values[], int num_values) {

#endif

