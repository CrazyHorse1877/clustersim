#include <stdlib.h>
#include <stdio.h>

#include "pdf.h"

Pdf new_pdf(int id, unsigned int num_rows, unsigned int num_cols, double start_latitude, double start_longitude, double delta_latitude, double delta_longitude, unsigned char* data){
	Pdf pdf;

	pdf.id = id;
	pdf.num_rows = num_rows;
	pdf.num_cols = num_cols;
	pdf.start_latitude = start_latitude;
	pdf.start_longitude = start_longitude;
	pdf.delta_latitude = delta_latitude;
	pdf.delta_longitude = delta_longitude;
	pdf.data = data;

	return pdf;
}

void delete_pdf(Pdf pdf){
	if(pdf.data != NULL)
		free(pdf.data);
}

Pdf_info get_pdf_info_from_pdf(Pdf pdf){
	Pdf_info pdf_info;
	
	pdf_info.id = pdf.id;
	pdf_info.num_rows = pdf.num_rows;
	pdf_info.num_cols = pdf.num_cols;
	pdf_info.start_latitude = pdf.start_latitude;
	pdf_info.start_longitude = pdf.start_longitude;
	pdf_info.delta_latitude = pdf.delta_latitude;
	pdf_info.delta_longitude = pdf.delta_longitude;

	return pdf_info;
}

Pdf combine_pdf_info_with_pdf_data(Pdf_info pdf_info, unsigned char* data){

	return new_pdf(pdf_info.id, pdf_info.num_rows, pdf_info.num_cols, pdf_info.start_latitude, pdf_info.start_longitude, pdf_info.delta_latitude, pdf_info.delta_longitude, data);

}

void print_pdf(Pdf pdf) {

	for (unsigned int row = 0; row < pdf.num_rows; row++) {
		for(unsigned int col = 0; col < pdf.num_cols; col++){
			unsigned char value = pdf.data[row * pdf.num_cols + col];
			printf("%x ", value);
		}
		printf("\n");
	}
}

