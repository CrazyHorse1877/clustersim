////////////////////////////////////////////
//
// fgcontrol.hpp
//
// Part of Bayesian Search and Rescue Simulator 
//
// This Class holds the functions and values that
// are related to a vehicle or target object like
// orientation and status etc.
// 
// 
// Parameters
//-----------------------------------------
// Dependant Libraries : 
// 
////////////////////////////////////////////

#ifndef _FG_CONTROL_HPP_
#define _FG_CONTROL_HPP_

#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
//#include <string.h>
#include "grid_pdf.hpp"

   //const char *HOST = "localhost";
   //const unsigned PORT = 5501;
   const int BUFLEN = 256;

//Retain ANSI/ISO Compability
#ifdef WIN32
    #include <windows.h>
#endif

//using namespace std;

class FGFSSocket {
 public:
   FGFSSocket(char *name, unsigned port);
   ~FGFSSocket();
   
   
   char            *HOST;
   unsigned	PORT;
   
   int		write(const char *msg, ...);
   const char	*read(void);
   void		flush(void);
   char		setup(void);
   char		update(GridPDF);
   void		settimeout(unsigned t) { _timeout = t; }
   
   
 private:
   int		close(void);
   
   int		_sock;
   bool		_connected;
   unsigned	_timeout;
   char		_buffer[BUFLEN];
};

#endif
