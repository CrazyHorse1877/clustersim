#include <fcntl.h>

#include "file.h"

FILE* open_file(char file_name[], char option[]) {
	FILE *fp;
	
	if ((fp = fopen(file_name, option)) == NULL){
		fprintf(stderr, "Cannot open %s\n", file_name);
	}
	return fp;
}


void set_non_blocking(int fp){
	fcntl(fp, F_SETFL, fcntl(fp, F_GETFL) | O_NONBLOCK);
}

void set_blocking(int fp){
	fcntl(fp, F_SETFL, fcntl(fp, F_GETFL) & ~O_NONBLOCK);
}

void close_file(FILE *fp) {
	fclose(fp);
}
