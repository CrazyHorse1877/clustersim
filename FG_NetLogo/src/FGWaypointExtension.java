/*
 *  Written By Elias Jordan, 2009
 */

import org.nlogo.api.*;
import java.util.*;
import java.io.*;

/* An extension to the NetLogo programming language, allowing it in conjunction with
 * the appropriate NetLogo code, to control several instances of the flight simulator
 * called flight gear.
 */

public class FGWaypointExtension extends DefaultClassManager
{
	/************************************************************
	* Implementation Of Class Manager
	*/
	public void load( org.nlogo.api.PrimitiveManager primManager )
    {
		primManager.addPrimitive( "setup", new Setup() );
		primManager.addPrimitive( "send-flight-vector", new SendFlightVector() );
		primManager.addPrimitive( "get-cluster-size", new GetClusterSize() );
		primManager.addPrimitive( "finish", new Finish() );
		primManager.addPrimitive( "update-interface-properties", new UpdateInterfaceProperties() );
		
 	}
	/*
	*************************************************************/
	
	
	/* Return the number of craft in the cluster, taking the fginetrface as an argument
	*/
	public static class GetClusterSize extends DefaultReporter
	{
		public Syntax getSyntax()
		{
			// take a wildcard as an argument and return a number
			return Syntax.reporterSyntax(Syntax.TYPE_NUMBER);
		}
		
		public Object report(Argument[] args, Context context) throws ExtensionException, LogoException
		{

			FGInterface fg = FGInterface.getInstance();
			return new Double(fg.pilots.size());
		}
	}
	
	/* Has the craft take off 
	*/
	public static class Setup extends DefaultCommand
	{
		public Syntax getSyntax()
		{
			return Syntax.commandSyntax() ;
		}
		
		public void perform(Argument[] args, Context context) throws ExtensionException
		{
			FGInterface fg = FGInterface.getInstance();
			
			for(TestPilot pilot : fg.pilots)
			{
				context.runCommand("print \"Craft taking off...\"", false);
				pilot.takeOff();
			}
		}
	}
	
	
	/* Closes the sockets connected to flight gear
	 */
	public static class Finish extends DefaultCommand
	{
		public Syntax getSyntax()
		{
			return Syntax.commandSyntax();
		}
		
		public void perform(Argument[] args, Context context) throws ExtensionException, LogoException
		{
			FGInterface fg = FGInterface.getInstance();
			fg.close();
		}
	}
	
	
	/* Updates the properties object based on the data entered in netlogo
	 */
	public static class UpdateInterfaceProperties extends DefaultCommand
	{
		public Syntax getSyntax()
		{
			return Syntax.commandSyntax(new int[] {Syntax.TYPE_WILDCARD});
		}
		
		public void perform(Argument[] args, Context context) throws ExtensionException, LogoException
		{
			// check type of the wildcard
			Object arg0 = args[0].get();
			if(!(arg0 instanceof String))
			{
				throw new org.nlogo.api.ExtensionException("UpdateInterfaceProperties: Argument Not a String ");
			}
			String server_info = (String) arg0;
			
			InputStream is = new ByteArrayInputStream(server_info.getBytes());
			
			FGInterfaceProperties props =  FGInterfaceProperties.getInstance();
			
			try
			{
				props.load(is);
				props.store();
			}
			catch(IOException e)
			{
				System.err.println(e);
			}
		}
	}
	
	
	/* Sends a heading to flight gear, then waits until the aircraft has
	   covered the required distance before returning control to net logo.
	*/
	public static class SendFlightVector extends DefaultCommand
	{
		public Syntax getSyntax()
		{
			// This command takes arguments: fginterface, cluster number, heading, distance covered in net logo
			return Syntax.commandSyntax(new int[] {Syntax.TYPE_NUMBER, Syntax.TYPE_NUMBER, Syntax.TYPE_NUMBER});
		}
		
		public void perform(Argument[] args, Context context) throws ExtensionException, LogoException
		{
			double heading, nl_distance;

			FGInterface fg = FGInterface.getInstance();
			
			// index of this craft in the pilots list
			int index = args[0].getIntValue();
			
			// the true heading to send to flight gear
			heading = args[1].getDoubleValue();
			
			// the distance traveled by the turtle in net logo
			nl_distance = args[2].getDoubleValue();
			
			FlightVector vector = new FlightVector(index, heading, nl_distance);
			if(index < fg.pilots.size() - 1)
			{
				context.runCommand("print \"setting vector for pilot " + index + "\"", false);
				fg.pilots.get(index).setVector(vector);
			}
			else
			{
				context.runCommand("print \"setting vector for pilot " + index + "\"", false);
				fg.pilots.get(index).setVector(vector);
				
				context.runCommand("print \"STARTING Threads\"", false);
				
				
				// start each pilot on their respective vectors
				for(VectorPilot v_pilot : fg.pilots)
				{
					v_pilot.startVector();
				}
				
				context.runCommand("print \"Vectors Started\"", false);
				
				// pause main thread while control threads execute
				for(VectorPilot v_pilot : fg.pilots)
				{
					v_pilot.join();
				}
				
				// reset vectors
				/*for(VectorPilot v_pilot : fg.pilots)
				{
					v_pilot.clearVector();
				}*/
			}

		}
	}
}

