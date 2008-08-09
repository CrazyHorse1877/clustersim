#ifndef _OUTPUT_CLIENT_H_
#define _OUTPUT_CLIENT_H_

#include "flight_info.h"
#include "pdf.h"
#include "bool.h"

void register_for_flight_info_to_server(char server_host[], int server_port, bool blocking);
void register_for_pdf_to_server(char server_host[], int server_port, bool blocking);
Flight_info get_flight_info_from_server();
Pdf get_pdf_from_server();
void get_multiple_flight_info_from_server(bool(*handler)(Flight_info, void*), void* user_data);
void get_multiple_pdf_from_server(bool(*handler)(Pdf, void*), void* user_data);

#endif

