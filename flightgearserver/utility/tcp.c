#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "bool.h"
#include "file.h"
#include "tcp.h"


void print_errno_error(FILE* fp) {
	switch(errno){
		case EBADF:            
			fprintf(fp, "S is not a valid descriptor\n");
			break;
		case EISCONN:          
			fprintf(fp, "The socket is already connected\n");
			break;
		case ETIMEDOUT:        
			fprintf(fp, "Connection establishment timed out without establishing a connection.\n");
			break;
		case EADDRINUSE:       
			fprintf(fp, "The address is already in use.\n");
			break;
		case ECONNREFUSED:     
			fprintf(fp, "The attempt to connect was forcefully rejected.\n");
			break;
		case EINPROGRESS:      
			fprintf(fp, "The socket is non-blocking and the connection cannot be completed immediately.\n");
			break;
		case EALREADY:         
			fprintf(fp, "The socket is non-blocking and a previous connection attempt has not yet been completed.\n");
			break;
		case ENOTCONN:
			fprintf(fp, "The socket is associated with a connection-oriented protocol and has not been connected.\n");
			break;
		case ENOTSOCK: 
			fprintf(fp, "The argument s does not refer to a socket\n");
			break;
		case EAGAIN: 
			fprintf(fp, "The socket is marked non-blocking, and the receive operation would block, or a receive timeout had been set, and the timeout expired before data was received.\n");
			break;
		case EINTR:            
			fprintf(fp, "The receive was interrupted by delivery of a signal before any data were available.\n");
			break;
		case EFAULT:
			fprintf(fp, "The receive buffer pointer(s) point outside the process's address space\n");
		default:
			fprintf(fp, "Unknown error\n");
	}
}

void print_tcp_error(FILE* fp, int error) {
   switch(error){
    case TCP_ERROR_UNKNOWN_HOST:
      fprintf(fp, "Unknown host.\n");
      break;
    case TCP_ERROR_FAILED_TO_OPEN_SOCKET:
      fprintf(fp, "Failed to open socket.\n");
      break;
    case TCP_ERROR_FAILED_TO_BIND:
      fprintf(fp, "Failed to bind.\n");
      break;
    case TCP_ERROR_FAILED_TO_CONNECT:
      fprintf(fp, "Failed to connect.\n");
      print_errno_error(fp);
      break;
    case TCP_ERROR_FAILED_TO_LISTEN:
      fprintf(fp, "Failed to listen.\n");
      break;
    default:
      if(error < 0) {
	 fprintf(fp, "Unknown error: %d.\n", error);
      }else{
	 fprintf(fp, "Does not appear to be error: %d\n", error);
      }
   }
}

bool bind_to_port(int sd, int port) {
   struct sockaddr_in address;
   
   /* bind any port number */
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = htonl(INADDR_ANY);
   address.sin_port = htons(port);
   
   return (bind(sd, (struct sockaddr *) &address, sizeof(address)) == 0);	
}

bool connect_to_server(int sd, struct hostent *host, int port) {
   struct sockaddr_in host_address;
   
   host_address.sin_family = host->h_addrtype;
   memcpy((char *) &host_address.sin_addr.s_addr, host->h_addr_list[0], host->h_length);
   host_address.sin_port = htons(port);
   
   while(true) {
      if(connect(sd, (struct sockaddr *) &host_address, sizeof(host_address)) < 0){
	 if(errno == EINPROGRESS){
	    //usleep(500);
	 }else{
	    return false;
	 }
      }else{
	 return true;
      }
   }
   
   return true;
}

bool listen_on_port(int sd) {
	int back_log = 512; /*defines max length of queue of pending connections*/
	return (listen(sd, back_log) == 0);
}


int accept_connection_from_client_TCP(int sd) {
	struct sockaddr_in address;
	socklen_t length = sizeof(address);

	/* returns new socket descriptor*/
	return accept(sd, (struct sockaddr *) &address, &length);
}


int create_socket(int port, bool blocking) {
	int sd = socket(AF_INET, SOCK_STREAM, 0);

	if (sd < 0) {
		return TCP_ERROR_FAILED_TO_OPEN_SOCKET;
	}

	if (! bind_to_port(sd, port)) {
		return TCP_ERROR_FAILED_TO_BIND;
	}

	if (blocking) { 
		set_blocking(sd); 
	} else { 
		set_non_blocking(sd); 
	}

	return sd;
}

int open_client_to_server_socket_TCP(char host_name[], int server_port, bool blocking) {
	struct hostent *host = gethostbyname(host_name);

	if (host == NULL) {
		return TCP_ERROR_UNKNOWN_HOST;
	}

	int sd = create_socket(0, blocking);

	if (sd < 0) {
		return sd;
	}

	set_blocking(sd);/* connect_to_server requires the socket to be blocking to work correctly*/
	if(! connect_to_server(sd, host, server_port)) {
		return TCP_ERROR_FAILED_TO_CONNECT;
	}
	if(!blocking) set_non_blocking(sd);/* blocking state needs to be restored for non-blocking sockets*/

	return sd;
}

int open_welcome_socket_TCP(int server_port, bool blocking) {
	int sd = create_socket(server_port, blocking);

	if (sd < 0) {
		return sd;
	}

	if (! listen_on_port(sd)) {
		return TCP_ERROR_FAILED_TO_LISTEN;
	}

	return sd;
}

bool send_TCP(int sd, void *data, size_t len) {
	unsigned char* buffer = data;
	while (len > 0) {
		int bytes_sent = send(sd, buffer, len, 0);
		if (bytes_sent > 0) {
			len -= bytes_sent;
			buffer += bytes_sent;
		} else {
			return false;
		}
	}
	return true;
}

int receive_TCP(int sd, void *data, size_t len) {
	unsigned char* buffer = data;
	int total_bytes_received = 0;

	while (len > 0) {
		int bytes_received = recv(sd, buffer, len, 0);
		if (bytes_received > 0) {
			len -= bytes_received;
			buffer += bytes_received;
			total_bytes_received += bytes_received;
		} else {
			if (errno == EAGAIN) { 
				return TCP_ERROR_AGAIN; 
			}
			/*print_errno_error(stderr);*/
			return total_bytes_received;
		}
	}
	return total_bytes_received;
}


bool send_string_TCP(int sd, char string[]) {
	return send_TCP(sd, (void*)string, strlen(string)); /* TODO handle failed send*/
}

bool send_string_line_TCP(int sd, char string[]){
	bool send;

	send = send_string_TCP(sd, string);
	send &= send_string_TCP(sd, "\r\n");

	return send;
}


void discard_TCP(int sd, size_t len) {
	void *data = malloc(len);
	receive_TCP(sd, data, len);
	free(data);
}

void close_TCP(int sd) {
	close(sd);
}


