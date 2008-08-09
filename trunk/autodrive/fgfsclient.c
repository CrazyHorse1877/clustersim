/* $Id: fgfsclient.c,v 1.4 2006/11/23 09:26:43 mfranz Exp $ */
/* gcc -O2 -g -pedantic -Wall fgfsclient.c -o fgfsclient */
/* USAGE: ./fgfsclient [hostname [port]] */
/* Public Domain */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <string.h>


#define DFLTHOST	"localhost"
#define DFLTPORT	5501
#define MAXMSG		256
#define fgfsclose	close


void init_sockaddr(struct sockaddr_in *name, const char *hostname, unsigned port);
int fgfswrite(int sock, char *msg, ...);
const char *fgfsread(int sock, int wait);
void fgfsflush(int sock);



int fgfswrite(int sock, char *msg, ...)
{
	va_list va;
	ssize_t len;
	char buf[MAXMSG];

	va_start(va, msg);
	vsnprintf(buf, MAXMSG - 2, msg, va);
	va_end(va);
	printf("SEND: \t<%s>\n", buf);
	strcat(buf, "\015\012");

	len = write(sock, buf, strlen(buf));
	if (len < 0) {
		perror("fgfswrite");
		exit(EXIT_FAILURE);
	}
	return len;
}



const char *fgfsread(int sock, int timeout)
{
	static char buf[MAXMSG];
	char *p;
	fd_set ready;
	struct timeval tv;
	ssize_t len;

	FD_ZERO(&ready);
	FD_SET(sock, &ready);
	tv.tv_sec = timeout;
	tv.tv_usec = 0;
	if (!select(32, &ready, 0, 0, &tv))
		return NULL;

	len = read(sock, buf, MAXMSG - 1);
	if (len < 0) {
		perror("fgfsread");
		exit(EXIT_FAILURE);
	} 
	if (len == 0)
		return NULL;

	for (p = &buf[len - 1]; p >= buf; p--)
		if (*p != '\015' && *p != '\012')
			break;
	*++p = '\0';
	return strlen(buf) ? buf : NULL;
}



void fgfsflush(int sock)
{
	const char *p;
	while ((p = fgfsread(sock, 0)) != NULL) {
		printf("IGNORE: \t<%s>\n", p);
	}
}



int fgfsconnect(const char *hostname, const int port)
{
	struct sockaddr_in serv_addr;
	struct hostent *hostinfo;
	int sock;

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		perror("fgfsconnect/socket");
		return -1;
	}

	hostinfo = gethostbyname(hostname);
	if (hostinfo == NULL) {
		fprintf(stderr, "fgfsconnect: unknown host: \"%s\"\n", hostname);
		close(sock);
		return -2;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr = *(struct in_addr *)hostinfo->h_addr;

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("fgfsconnect/connect");
		close(sock);
		return -3;
	}
	return sock;
}



int main(int argc, char **argv)
{
	int sock;
	unsigned port;
	const char *hostname, *p;

	hostname = argc > 1 ? argv[1] : DFLTHOST;
	port = argc > 2 ? atoi(argv[2]) : DFLTPORT;

	sock = fgfsconnect(hostname, port);
	if (sock < 0)
		return EXIT_FAILURE;
   
   fgfswrite(sock, "data");
/*   fgfswrite(sock, "set /controls/engines/engine[%d]/throttle %d", 0, 1);*/
   fgfswrite(sock, "get /sim/aircraft");
//   fgfswrite(sock, "set /autopilot/locks/heading true-heading-hold");
//   fgfswrite(sock, "set /autopilot/locks/altitude agl-hold");
   //fgfswrite(sock, "set /autopilot/locks/speed speed-with-throttle");
//   fgfswrite(sock, "set /autopilot/settings/target-agl-ft 10");
   //fgfswrite(sock, "set /autopilot/settings/target-speed-kt 40");
   fgfswrite(sock, "set /autopilot/route-manager/input @clear");
   
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1815737832935,-33.93939934853642@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.181127289529,-33.93954070164675@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1805166157843,-33.93962683188665@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1803130575158,-33.9396555440331@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1796683372992,-33.93982437646518@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1792961711646,-33.93987682415013@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1787248283843,-33.94007342819902@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.178320503843,-33.94013040566961@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1778537673307,-33.94027303212538@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1773833367504,-33.94033932417877@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1770809088992,-33.94038195456645@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1766146692415,-33.9404859403414@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.176178410985,-33.94054743399715@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1758763748684,-33.94059000606067@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1752174868505,-33.94075907589551@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1748559802321,-33.94084796746537@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1744286918291,-33.94094593393199@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.174102554625,-33.94102948006911@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1738691987745,-33.94106225364135@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1734036578339,-33.94127757154424@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1730570628783,-33.94140060326531@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1723037500788,-33.94165398042779@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1717661739423,-33.94180276776666@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1712426656999,-33.94198489927354@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1709275731428,-33.94206490405502@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1706051097056,-33.94221751660641@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1701626266939,-33.94231497266167@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.169707933189,-33.94237844473104@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1692531736447,-33.94244192600117@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1687983632138,-33.94250542003172@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1682631691764,-33.94254439815386@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1679052425386,-33.94259436979833@20");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1675960544616,-33.94267324233537@20 ");
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.1672876756736,-33.94275191849264@20");
   
   fgfswrite(sock, "set /autopilot/route-manager/input @insert-1:151.22750,-33.91810@100");
   p = fgfsread(sock, 3);
	if (p != NULL)
		printf("READ: \t<%s>\n", p);
	fgfswrite(sock, "quit");
	fgfsclose(sock);
	return EXIT_SUCCESS;
}
