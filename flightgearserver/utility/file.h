#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>

FILE* open_file(char file_name[], char option[]);
void set_blocking(int fp);
void set_non_blocking(int fp);
void close_file(FILE *fp);

#endif

