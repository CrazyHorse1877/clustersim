#include "tcp.h"
#include "kml.h"
#include "bool.h"

static const int GET_LENGTH = -1;

int get_file_length() {
	int len = 0;

	len += kml_header(GET_LENGTH);

	return len;
}


int main(void){
	int web_server_welcome_socket = open_welcome_socket_TCP(2080, false);

	while(1) {
		int web_server_connected_socket = accept_connection_from_client_TCP(web_server_welcome_socket);


		if (web_server_connected_socket >= 0){
			int len = get_file_length() + 1350;
			char content_length[100];

			sprintf(content_length, "Content-Length: %d", len);

			send_string_line_TCP(web_server_connected_socket, "HTTP/1.1 200 OK");
			send_string_line_TCP(web_server_connected_socket, "Connection: close");

			send_string_line_TCP(web_server_connected_socket, "Content-Type: application/vnd.google-earth.kml+xml");
			send_string_line_TCP(web_server_connected_socket, content_length); 
			/*send_string_line_TCP(web_server_connected_socket, "Content-Length: 1437");*/
			send_string_line_TCP(web_server_connected_socket, "Server: Indy/DevSnapshot");
			send_string_line_TCP(web_server_connected_socket, "");

			kml_header(web_server_connected_socket);
			/*send_string_line_TCP(web_server_connected_socket, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
			send_string_line_TCP(web_server_connected_socket, "<kml xmlns=\"http://earth.google.com/kml/2.0\">");
			*/
			send_string_line_TCP(web_server_connected_socket, "  <Folder>");
			send_string_line_TCP(web_server_connected_socket, "  <Placemark>");
			send_string_line_TCP(web_server_connected_socket, "    <name>Aircraft</name>");
			send_string_line_TCP(web_server_connected_socket, "    <description>Marks current aircraft position (MyFsGoogleEarth)</description>");
			send_string_line_TCP(web_server_connected_socket, "    <visibility>1</visibility>");
			send_string_line_TCP(web_server_connected_socket, "    <styleUrl>root://styles#default+icon=0x307</styleUrl>");
			send_string_line_TCP(web_server_connected_socket, "    <Style>");
			send_string_line_TCP(web_server_connected_socket, "<labelColor>ff00ffff</labelColor>");
			send_string_line_TCP(web_server_connected_socket, "      <IconStyle>");
			send_string_line_TCP(web_server_connected_socket, "        <Icon>");
			send_string_line_TCP(web_server_connected_socket, "          <href>root://icons/palette-2.png</href>");
			send_string_line_TCP(web_server_connected_socket, "          <w>32</w>");
			send_string_line_TCP(web_server_connected_socket, "          <h>32</h>");
			send_string_line_TCP(web_server_connected_socket, "        </Icon>");
			send_string_line_TCP(web_server_connected_socket, "      </IconStyle>");
			send_string_line_TCP(web_server_connected_socket, "    </Style>");
			send_string_line_TCP(web_server_connected_socket, "    <Point>");
			send_string_line_TCP(web_server_connected_socket, "      <extrude>1</extrude> <altitudeMode>absolute</altitudeMode>");
			send_string_line_TCP(web_server_connected_socket, "      <coordinates>0.00000000,0.00000000,0.00000000</coordinates>");
			send_string_line_TCP(web_server_connected_socket, "    </Point>");
			send_string_line_TCP(web_server_connected_socket, "  </Placemark>");
			send_string_line_TCP(web_server_connected_socket, "  <Placemark>");
			send_string_line_TCP(web_server_connected_socket, "    <name>Aircraft (+1 min.)</name>");
			send_string_line_TCP(web_server_connected_socket, "    <description>Marks estimated aircraft position in 1 minute (MyFsGoogleEarth)</description>");
			send_string_line_TCP(web_server_connected_socket, "    <visibility>1</visibility>");
			send_string_line_TCP(web_server_connected_socket, "    <styleUrl>root://styles#default+icon=0x307</styleUrl>");
			send_string_line_TCP(web_server_connected_socket, "    <Style>");
			send_string_line_TCP(web_server_connected_socket, "<labelScale>0.75</labelScale><labelColor>ff00ffff</labelColor>");
			send_string_line_TCP(web_server_connected_socket, "      <IconStyle>");
			send_string_line_TCP(web_server_connected_socket, "        <Icon>");
			send_string_line_TCP(web_server_connected_socket, "          <href>root://icons/palette-2.png</href>");
			send_string_line_TCP(web_server_connected_socket, "          <w>32</w>");
			send_string_line_TCP(web_server_connected_socket, "          <h>32</h>");
			send_string_line_TCP(web_server_connected_socket, "        </Icon>");
			send_string_line_TCP(web_server_connected_socket, "        <scale>0.6</scale>");
			send_string_line_TCP(web_server_connected_socket, "      </IconStyle>");
			send_string_line_TCP(web_server_connected_socket, "    </Style>");
			send_string_line_TCP(web_server_connected_socket, "    <Point>");
			send_string_line_TCP(web_server_connected_socket, "      <extrude>1</extrude> <altitudeMode>absolute</altitudeMode>");
			send_string_line_TCP(web_server_connected_socket, "      <coordinates>0.00000000,0.00000000,0.00000000</coordinates>");
			send_string_line_TCP(web_server_connected_socket, "    </Point>");
			send_string_line_TCP(web_server_connected_socket, "  </Placemark>");
			send_string_line_TCP(web_server_connected_socket, "  </Folder>");
			send_string_line_TCP(web_server_connected_socket, "</kml>");

		}
	}
}
