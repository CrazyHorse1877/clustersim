/*
 *  Written By Elias Jordan, 2009
 */

/* Simple tuple class used to store a flight vector
 */
public class FlightVector
{
	private static int instance_num = 0;
	private static double scale;
	
	
	int who_number;
	double true_heading;
	double distance;
		
	FlightVector(int who_number, double true_heading, double distance)
	{
		if(instance_num == 0)
		{
			FGInterfaceProperties props = FGInterfaceProperties.getInstance();
			scale = props.getScale();
			instance_num++;
		}
		
		this.who_number = who_number;
		this.true_heading = true_heading;
		this.distance = distance*this.scale;
	}
}