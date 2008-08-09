////////////////////////////////////////////
//
// fgcontroller.cpp
//
// Part of Bayesian Search and Rescue Simulator 
//
// This Class holds the functions to control and 
// send commands to flightgear 
// 
// Public Domain
// 
// Parameters
// $Id: fgfsclient.cxx,v 1.4 2006/11/23 09:26:43 mfranz Exp $
// g++ -O2 -g -pedantic -Wall fgfsclient.cxx -o fgfsclient -lstdc++
// USAGE: ./fgfsclient [hostname [port]]
//-----------------------------------------
// 
////////////////////////////////////////////

#include "fgcontroller.hpp"

//using namespace std;

FGFSSocket::FGFSSocket(char *hostname, unsigned port) :
	_sock(-1),
	_connected(false),
	_timeout(1)
{
	_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock < 0)
		throw("FGFSSocket/socket");

	struct hostent *hostinfo;
	hostinfo = gethostbyname(hostname);
	if (!hostinfo) {
		close();
		throw("FGFSSocket/gethostbyname: unknown host");
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr = *(struct in_addr *)hostinfo->h_addr;

	if (connect(_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		close();
		throw("FGFSSocket/connect");
	}
	_connected = true;
	try {
		write("data");
	} catch (...) {
		close();
		throw;
	}
}


FGFSSocket::~FGFSSocket()
{
	close();
}


int FGFSSocket::close(void)
{
	if (_connected)
		write("quit");
	if (_sock < 0)
		return 0;
	int ret = ::close(_sock);
	_sock = -1;
	return ret;
}


int FGFSSocket::write(const char *msg, ...)
{
	va_list va;
	ssize_t len;
	char buf[BUFLEN];
	fd_set fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(_sock, &fd);
	tv.tv_sec = _timeout;
	tv.tv_usec = 0;
	if (!select(FD_SETSIZE, 0, &fd, 0, &tv))
		throw("FGFSSocket::write/select: timeout exceeded");

	va_start(va, msg);
	vsnprintf(buf, BUFLEN - 2, msg, va);
	va_end(va);
	std::cout << "SEND: " << buf << std::endl;
	strcat(buf, "\015\012");

	len = ::write(_sock, buf, strlen(buf));
	if (len < 0)
		throw("FGFSSocket::write");
	return len;
}


const char *FGFSSocket::read(void)
{
	char *p;
	fd_set fd;
	struct timeval tv;
	ssize_t len;

	FD_ZERO(&fd);
	FD_SET(_sock, &fd);
	tv.tv_sec = _timeout;
	tv.tv_usec = 0;
	if (!select(FD_SETSIZE, &fd, 0, 0, &tv)) {
		if (_timeout == 0)
			return 0;
		else
			throw("FGFSSocket::read/select: timeout exceeded");
	}

	len = ::read(_sock, _buffer, BUFLEN - 1);
	if (len < 0)
		throw("FGFSSocket::read/read");
	if (len == 0)
		return 0;

	for (p = &_buffer[len - 1]; p >= _buffer; p--)
		if (*p != '\015' && *p != '\012')
			break;
	*++p = '\0';
	return strlen(_buffer) ? _buffer : 0;
}


void FGFSSocket::flush(void)
{
	int i = _timeout;
	_timeout = 0;
	while (read())
		;
	_timeout = i;
}

char FGFSSocket::setup(void){
   try {
      //char *hostname = "192.168.0.50";
      //char *hostname = "localhost";
      //int port = 5400;
      
      //FGFSSocket uav(HOST, PORT);
      //FGFSSocket uav(hostname, port);
      
      flush();
//      write("set /autopilot/locks/heading true-heading-hold");
//      write("set /autopilot/locks/altitude altitude-hold");
//      write("set /autopilot/locks/speed speed-with-throttle");
      write("set /autopilot/settings/target-altitude-ft 300");
//      write("set /autopilot/settings/target-speed-kt 50");
//      write("set /autopilot/route-manager/input @clear");
      write("get /sim/aircraft");
      const char *p = read();
      if (p)
	std::cout << "RECV: " << p << std::endl;
      return EXIT_SUCCESS;
      
   } catch (const char s[]) {
      std::cerr << "Error: " << s << ": " << strerror(errno) << std::endl;
      return EXIT_FAILURE;
      
   } catch (...) {
      std::cerr << "Error: unknown exception" << std::endl;
      return EXIT_FAILURE;
   }
}

char FGFSSocket::update(GridPDF gridobject){

   try {
      write("set /autopilot/route-manager/input @clear");
      write("set /autopilot/route-manager/input @insert-1:%lf,%lf@%lf",
	    gridobject.VD[0].gps_long,gridobject.VD[0].gps_lat, gridobject.VD[0].gps_elev);
   }
   catch (const char s[]){
	
	std::cerr << "Error: " << s << ": " << strerror(errno) << std::endl;
	return EXIT_FAILURE;
   }
   catch (...){
      std::cerr << "Error: unknown exception" << std::endl;
      return EXIT_FAILURE;
   }
}

