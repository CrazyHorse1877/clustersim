/*
 *  Written By Elias Jordan, 2009
 */

import java.io.*;
import java.net.*;

/* The networking code used to interface with flight gear, by updating 
 * values in the property tree.
 */

public class FGTelnet
{
	// socket connected to flight gear
	Socket client_socket;
	String hostname;
	
	/* Create the socket connected to flight gear
	*/
	FGTelnet(String hostname, int port)
	{
		this.hostname = hostname;
		try
		{
			client_socket = new Socket(hostname, port);
		}
		catch(IOException e)
		{
			// create an error popup
			System.err.println(e);
			String message = "<font size=3>FGInterface was unable to connect to the server <b>" + hostname + ":" + port + " </b>" +
							 "listed in the <i>flight-gear-interface-properties</i> window.<br><br> The following error was encountered: <br>" + 
			                  "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + e.toString() + "</font>";
			ErrorPopup.show("SocketIOException", message);
			client_socket = null;
		}
	}
	
	/* Sets the specified property in the property tree to the specified value 
	*/
	public void set(String property, String value)
	{
		this.send("set " + property + " " + value);
	}
	
	/* Retrieves the value of the specified property from the property tree.
	*/
	public String get(String property)
	{
		return this.send("get " + property);
	}
	
	/* Sends data to flight gear
	*/
	private String send(String s)
	{
		String line = null;
		try
		{
			DataOutputStream out = new DataOutputStream(client_socket.getOutputStream());
			BufferedReader in_from_fg = new BufferedReader(new InputStreamReader(client_socket.getInputStream()));
			
			out.writeBytes(s + "\r\n");
			line = in_from_fg.readLine();
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
		return line;
	}
	
	/* Close the socket connection
	 */
	public void close()
	{
		try 
		{
			client_socket.close();
		}
		catch(Exception e)
		{
			System.err.println(e);
		}
	}
}
 
