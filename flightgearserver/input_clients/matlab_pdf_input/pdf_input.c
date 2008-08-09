#include <stdlib.h>
#include "input_client.h"
#include "tcp.h"
#include "pdf.h"


void input_pdf(char server_host[], int server_port){
   int id = 500;
   unsigned int num_rows = 256;
   unsigned int num_cols = 256;
   double start_longitude = 151.22750;
   double start_latitude = -33.91810;
   //double altitude_data = 0.00001;
   double delta_longitude = 0.00001;
   double delta_latitude = -0.00001;
   
   unsigned char* altitude_data = (unsigned char*)malloc(sizeof(unsigned char) * (num_rows * num_cols));
   //data = ;
   
   Pdf pdf = new_pdf(id, num_rows, num_cols, start_latitude, start_longitude, delta_latitude, delta_longitude, altitude_data);
   
   give_pdf_to_server(pdf, server_host, server_port);

}


