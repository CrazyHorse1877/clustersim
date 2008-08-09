#ifndef _INPUT_CLIENT_H_
#define _INPUT_CLIENT_H_

#include "flight_info.h"
#include "pdf.h"
#include "bool.h"

bool give_flight_info_to_server(Flight_info flight_info, char server_host[], int server_port);
bool give_pdf_to_server(Pdf pdf, char server_host[], int server_port);

#endif

