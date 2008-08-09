#ifndef _TCP_H_
#define _TCP_H_

#include <stdio.h>
#include <stdlib.h>
#include "bool.h"

enum {
	TCP_ERROR_UNKNOWN_HOST = -1,
	TCP_ERROR_FAILED_TO_OPEN_SOCKET	= -2,
	TCP_ERROR_FAILED_TO_BIND = -3,
	TCP_ERROR_FAILED_TO_CONNECT = -4,
	TCP_ERROR_FAILED_TO_LISTEN = -5,
	TCP_ERROR_AGAIN = -6
};

int open_client_to_server_socket_TCP(char host_name[], int server_port, bool blocking);
int open_welcome_socket_TCP(int server_port, bool blocking);
int accept_connection_from_client_TCP(int sd);
bool send_TCP(int sd, void *data, size_t len);
int receive_TCP(int sd, void *data, size_t len);
bool send_string_TCP(int sd, char string[]);
bool send_string_line_TCP(int sd, char string[]);
void discard_TCP(int sd, size_t len);
void close_TCP(int sd);
void print_tcp_error(FILE* fp, int error);
void print_errno_error(FILE* fp);

#endif

