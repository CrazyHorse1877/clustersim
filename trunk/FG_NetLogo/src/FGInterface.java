
import java.util.*;
import org.nlogo.api.*;

/* A new data type to use within NetLogo, which stores all information
 * about the state of the interface.
 */

public class FGInterface //implements ExtensionObject
{
	// list of the connections to flight gear instances
	ArrayList<VectorPilot> pilots = new ArrayList<VectorPilot>();
	
	private static FGInterface instance = null;
	
	private FGInterface()
	{
		// read the property file containing flight gear server info
		FGInterfaceProperties props = FGInterfaceProperties.getInstance();
		ArrayList<ServerPortTuple> servers = props.getServers();
		
		// create connections to the servers
		for(ServerPortTuple sp : servers)
		{
			pilots.add(new VectorPilot(sp.server, sp.port));
		}
	}
	
	public static FGInterface getInstance()
	{
		if(instance == null)
		{
			instance = new FGInterface();
		}
		
		return instance;
	}
	
	// close the socket connections to flight gear
	public void close()
	{
		for(VectorPilot p : pilots)
		{
			p.close();
		}
		
		// clear the pilot references that use those sockets
		pilots = new ArrayList<VectorPilot>();
	}
	
	/*public String dump(boolean readable, boolean exporting, boolean reference)
	{
		return "<FGInterface>";
	}
	
	public String getExtensionName()
	{
		return "fginterface";
	}
	
	public String getNLTypeName()
	{
		return "";
	}
	
	public boolean recursivelyEqual(Object o)
	{
		return true;
	}*/
}