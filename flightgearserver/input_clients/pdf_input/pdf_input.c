#include <stdlib.h>
#include "input_client.h"
#include "file.h"
#include "tcp.h"
#include "pdf.h"


Pdf load_pdf_from_file(char file_name[]) {
	int id = 500;
	unsigned int num_rows = 256;
	unsigned int num_cols = 256;
	double start_longitude = 151.22750;
	double start_latitude = -33.91810;
	double delta_longitude = 0.00001;
	double delta_latitude = -0.00001;
	
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * (num_rows * num_cols));
	FILE *fp = open_file(file_name, "rb");
	
	fread(data, sizeof(unsigned char), num_rows * num_cols, fp);
	close_file(fp);
	
	Pdf pdf = new_pdf(id, num_rows, num_cols, start_latitude, start_longitude, delta_latitude, delta_longitude, data);
	return pdf;
}


void input_pdf(char server_host[], int server_port){

	Pdf pdf = load_pdf_from_file("pdf.dat");

	give_pdf_to_server(pdf, server_host, server_port);
	print_pdf(pdf);	


}


