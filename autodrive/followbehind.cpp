// g++ -O2 -g -pedantic -Wall fgfsclient.cxx -o fgfsclient -lstdc++
// USAGE: ./fgfsclient [hostname [port]]

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
#include <string.h>
#include <math.h>

const char *HOST = "localhost";
const unsigned PORT = 5501;
const int BUFLEN = 256;


typedef struct
{
   
      float lat,
      lng,
      hdn;
}
 GPSPosition;

class Vehicle {
 public:
   Vehicle(double, double, float);
   ~Vehicle();
   
   //IP and Port
   const char *hostname;
   unsigned port;
   
   //Position and Velocity
   double longitude; // X axis
   double latitude;  // Y axis
   float  heading;
   float  velocity;
   float  distance2target;
   float  armlength;
   
 private:
   int		_sock;
   bool		_connected;
   unsigned	_timeout;
   char		_buffer[BUFLEN];
};

Vehicle::Vehicle(double latitude, double longitude, float heading)
{
// anythign that needs to be calculated goes here i.e. momentum
}

class FGFSSocket {
 public:
   FGFSSocket(const char *name, unsigned port);
   ~FGFSSocket();
   
   int		write(const char *msg, ...);
   const char	*read(void);
   inline void	flush(void);
   void		settimeout(unsigned t) { _timeout = t; };

   void         SetVelocity(float velocity);
   float        ComputeDistance(Vehicle, Vehicle);
   
 private:
   int		close(void);
   
   int		_sock;
   bool		_connected;
   unsigned	_timeout;
   char		_buffer[BUFLEN];
};


FGFSSocket::FGFSSocket(const char *hostname = HOST, unsigned port = PORT) :
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


inline void FGFSSocket::flush(void)
{
   int i = _timeout;
   _timeout = 0;
   while (read())
     ;
   _timeout = i;
}

void FGFSSocket::SetVelocity(float velocity)
{
   write("set /controls/engines/engine[%d]/throttle %lg", 0, velocity);    
   write("set /controls/engines/engine[%d]/throttle %lg", 1, velocity);    
}

// Returns distance between two lat,long pairs in meters
// return: how far you have to go
// heading is ignored
float ComputeDistance(Vehicle *current, Vehicle *desired){
   float dlon, dlat, a, c;
   dlon = desired->longitude - current->longitude;
   dlat = desired->latitude - current->latitude;
   a = pow(sin(dlat/2),2) + cos(current->latitude) * cos(desired->latitude) * pow(sin(dlon/2),2);
   c = 2 * atan2(sqrt(a), sqrt(1-a));
   return 6378140 * c;  // 6378140 is the radius of the Earth in meters
}

// Returns radians east of north that you should head if you are
// at current and wish to go to desired.
// current: your current lat, long
// desired: the lat, long you wish to go to
// return: radians east of north (-PI to +PI I think)
double ComputeHeading(Vehicle *current, Vehicle *desired){
   double midterm, dummy;
   midterm = atan2
     (
      sin(desired->longitude-current->longitude)*cos(desired->latitude),
      cos(current->latitude)*sin(desired->latitude)
      -sin(current->latitude)*cos(desired->latitude)*cos(desired->longitude-current->longitude)
      );
   return modf(midterm, &dummy);
}

int main(const int argc, const char *argv[])
  try {
     const char *hostname = argc > 1 ? argv[1] : "cmr1";
     int port = argc > 2 ? atoi(argv[2]) : 5501;
     
     //FGFSSocket f(hostname, port);
     
     FGFSSocket cmr1("cmr1", 5501);
     FGFSSocket cmr2("cmr2", 5501);
     //FGFSSocket cmr3("cmr3", 5501);
     //FGFSSocket cmr4("cmr4", 5501);
     //FGFSSocket cmr5("cmr5", 5501);
     //FGFSSocket cmr6("cmr6", 5501);
     //FGFSSocket cmr7("cmr7", 5501);
     //FGFSSocket cmr8("cmr8", 5501);
     
     cmr1.flush();
     cmr2.flush();
     //cmr3.flush();
     //cmr4.flush();
     //cmr5.flush();
     //cmr6.flush();
     //cmr7.flush();
     //cmr8.flush();
     
     //cmr1.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.1);
     //cmr1.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.1);
     //cmr2.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.1);
     //cmr2.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.1);
     //cmr3.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.1);
     //cmr3.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.1);
     //cmr4.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.1);
     //cmr4.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.1);
     //cmr5.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.1);
     //cmr5.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.1);
     //cmr6.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.1);
     //cmr6.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.1);
     //cmr7.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.1);
     //cmr7.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.1);
     //cmr8.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.1);
     //cmr8.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.1);
     
     while(1) {
	
	/*
	 //set waypont of leading aircraft
	 cmr1.write("get /orientation/heading-deg");
	 const char *cmr1->heading = cmr1.read();
	  
	 cmr1.write("get /position/heading-deg");
	 const char *cmr1->heading = cmr1.read();
	 */	
	
	//get distance to aircraft in front
	
	//cmr1.GetAttributes();
	cmr1.write("get /position/longitude-deg");
	vehicle1.longitude = cmr1.read();    // Get X
	cmr1.write("get /position/latitude-deg");
	vehicle1.latitude = cmr1.read();     // Get Y
	cmr1.write("get /position/sea-level-radius-ft");
	vehicle1.searadius = cmr1.read();    // Get earth radius
	cmr1.write("get /position/altitude-ft");
	vehicle1.altitude = cmr1.read();     // Get Altitude
	cmr1.write("get /position/altitude-agl-ft");
	vehicle1.altitide-agl = cmr1.read();     // Get height above ground in feet
	cmr1.write("get /position/ground-elev-m");
	vehicle1.ground-elev = cmr1.read();     // Get height above ground in meters
	
	
	
	cmr2.write("get /position/longitude-deg");
	const char *myposx = cmr2.read();
	cmr2.write("get /position/latitude-deg");
	const char *myposy = cmr2.read();
	
	double dist = sqrt(pow((uposx-myposx)*100000,2) + pow((uposx-myposx)*100000,2));
	std::cout << "DIST: " << dist << std::endl;	
	   // if distance < tailing distance increase velocity
	   if(dist < 20){
	     // cmr2.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.2);
	     // cmr2.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.2);
	   }
	   else{
	      // else slow velocity
	   //   cmr2.write("set /controls/engines/engine[%d]/throttle %lg", 0, 0.05);
	    //  cmr2.write("set /controls/engines/engine[%d]/throttle %lg", 1, 0.05);
	   }
	}
	
	
     return EXIT_SUCCESS;
     
  } catch (const char s[]) {
     std::cerr << "Error: " << s << ": " << strerror(errno) << std::endl;
     return EXIT_FAILURE;
     
  } catch (...) {
     std::cerr << "Error: unknown exception" << std::endl;
     return EXIT_FAILURE;
  }


// vim:cindent
