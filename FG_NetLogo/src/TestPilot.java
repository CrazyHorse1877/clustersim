/*
 *  Written By Elias Jordan, 2009
 */

import java.util.*;
import java.net.*;

/* Uses the network connectivity provided by FGTelnet to interface 
 * with FlightGear by sending commands it recognises.
 */

public class TestPilot extends FGTelnet
{
	TestPilot(String name, int port)
	{
		super(name, port);
	}
	
	TestPilot(String name)
	{
		super(name, 5501);
	}
	
	public void appendWaypoint(GPSPosition pos)
	{
		this.set("/autopilot/route-manager/input", pos.toString());
	}
	
	public void insertWaypoint(int index, GPSPosition pos)
	{
		this.set("/autopilot/route-manager/input", "@insert" + index + ":" + pos.toString());
	}
	
	public void clearWaypoints()
	{
		this.set("/autopilot/route-manager/input", "@clear");
	}
	
	public void deleteWaypoint(int index)
	{
		this.set("/autopilot/route-manager/input", "@delete" + index);
	}
	
	
	public void setAltitudeFeet(int altitude)
	{
		this.set("/autopilot/locks/altitude", "altitude-hold");
		this.set("/autopilot/settings/target-altitude-ft", "" + altitude);
	}
	
	public void setSpeedKts(double speed)
	{
		this.set("/autopilot/locks/speed", "speed-with-throttle");
        this.set("/autopilot/settings/target-speed-kt", "" + speed);
	}
	
	public void setHeadingDeg(double heading)
	{
		this.set("/autopilot/locks/heading", "true-heading-hold");
		this.set("/autopilot/settings/true-heading-deg", "" + heading);
	}
	
	public double getLongitude()
	{
		String longitude = this.get("/position/longitude-deg");
		return Double.parseDouble(parseGet(longitude));
	}
	
	public double getLatitude()
	{
		String latitude = this.get("/position/latitude-deg");
		return Double.parseDouble(parseGet(latitude));
	}
	
	public double getAltitude()
	{
		String altitude = this.get("/position/altitude-ft");
		return Double.parseDouble(parseGet(altitude));
	}
	
	public double getTargetAltitude()
	{
		String target = this.get("/autopilot/settings/target-altitude-ft");
		target = parseGet(target);
		double d = Double.parseDouble(target);
		
		return d;
	}
	
	public GPSPosition getCurrentPosition()
	{
		return new GPSPosition(getLatitude(), getLongitude(), getAltitude());
	}
	
	public double getCurrentHeading()
	{
		String heading = this.get("/orientation/heading-deg");
		heading = parseGet(heading);
		
		return Double.parseDouble(heading);
	}
	
	private String parseGet(String s)
	{
		int first = s.indexOf('\'') + 1;
		int last = s.lastIndexOf('\'');
		
		return s.substring(first, last);
	}
	
	public void takeOff()
	{
		this.setAltitudeFeet(300);
		this.setHeadingDeg(0);
		this.setSpeedKts(45);
	}
}
