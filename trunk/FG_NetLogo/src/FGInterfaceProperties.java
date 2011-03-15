/*
 *  Written By Elias Jordan, 2009
 */

import java.util.*;
import java.io.*;

/* A properties class, utilising the singleton pattern.
 * Stores FGInterface server information as well as 
 * system properties.
 */
public class FGInterfaceProperties extends Properties
{
	// path to the properties file
	private static final String properties_file = "./extensions/fginterface/FGInterface.properties";
	
	// singleton instance
	private static FGInterfaceProperties instance = null;
	
	/* Singleton method, return an instance of this class
	 */
	public static FGInterfaceProperties getInstance()
	{
		if(instance == null)
		{
			instance = new FGInterfaceProperties();
		}
		
		return instance;
	}
	
	/* Build the singleton from the file on disk
	 */
	private FGInterfaceProperties()
	{
		try 
		{
			FileInputStream input = new FileInputStream(properties_file);
			this.load(input);
			input.close();
		}
		catch(IOException e) 
		{
			System.err.println("FGInterfaceProperties: " + e);
		}
	}
	
	/* Save this properties object to disk
	 */
	public void store()
	{
		try
		{
			FileOutputStream out = new FileOutputStream(properties_file);
			this.store(out, "NetLogo to Flight Gear Interface Properties");
			out.close();
		}
		catch(IOException e) 
		{
			System.err.println(e);
		}
	}
	
	/* returns a list of all the server info in the properties object
	 */
	public ArrayList<ServerPortTuple> getServers()
	{
		ArrayList<ServerPortTuple> server_info = new ArrayList<ServerPortTuple>();
		for(String key : this.stringPropertyNames())
		{
			if(!key.equals("scale"))
			{
				String value = getProperty(key);
				int index = value.indexOf(':');
				String s = value.substring(0, index);
				int p = Integer.valueOf(value.substring(index + 1, value.length()));
				
				server_info.add(new ServerPortTuple(s, p));
			}
		}
		return server_info;
	}
	
	/* Returns the system property scale
	 */
	public double getScale()
	{
		return Double.parseDouble(getProperty("scale"));
	}	
}


/* Simple tuple class used to store server info.
 */
class ServerPortTuple
{
	public String server;
	public int port;
	
	ServerPortTuple(String server, int port)
	{
		this.server = server;
		this.port = port;
	}
	
	public String toString()
	{
		return server + ":" + port;
	}
}
