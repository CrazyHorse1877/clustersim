#ifndef _PDF_H_
#define _PDF_H_


typedef struct{
	int id;
	unsigned int num_rows;
	unsigned int num_cols;
	double start_latitude;
	double start_longitude;
	double delta_latitude;
	double delta_longitude;
}Pdf_info;


typedef struct{
	int id;
	unsigned int num_rows;
	unsigned int num_cols;
	double start_latitude;
	double start_longitude;
	double delta_latitude;
	double delta_longitude;
	unsigned char* data;
}Pdf;


Pdf new_pdf(int id, unsigned int num_rows, unsigned int num_cols, double start_latitude, double start_longitude, double delta_latitude, double delta_longitude, unsigned char* data);
void delete_pdf(Pdf pdf);
Pdf_info get_pdf_info_from_pdf(Pdf pdf);
Pdf combine_pdf_info_with_pdf_data(Pdf_info pdf_info, unsigned char* data);
void print_pdf(Pdf pdf);

#endif

