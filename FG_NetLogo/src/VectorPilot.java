/*
 *  Written By Elias Jordan, 2009
 */


/* Controls a single instance of flight gear as it follows a flight vector.
 * This is achieved by creating a thread for each instance of this class.
 */
public class VectorPilot extends TestPilot implements Runnable
{
	// current vector (distance and heading)
	FlightVector current_vector;
	
	// Thread for the distance polling loop
	Thread thread;
	
	VectorPilot(String name, int port, FlightVector vector)
	{
		super(name, port);
		this.current_vector = vector;
	}
	
	VectorPilot(String name, int port)
	{
		this(name, port, null);
	}
	
	/* Set The current vector
	*/
	public void setVector(FlightVector vector)
	{
		this.current_vector = vector;
	}
	
	/* Clear the current vector
	*/
	public void clearVector()
	{
		this.current_vector = null;
	}
	
	/* Set the crafts heading to that of the vector
	*/
	public void plotVectorHeading()
	{
		if(current_vector != null)
		{
			setHeadingDeg(current_vector.true_heading);
		}
	}
	
	/* get the distance from the current positon to the provided position
	*/
	public double getDistanceTo(GPSPosition position)
	{
		return getCurrentPosition().getDistance(position);
	}
	
	/* Get the distance required by the vector
	*/
	public double getVectorDistance()
	{
		return current_vector.distance;
	}
	
	/* Implementation of the Runnable interface.
	 */
	public void run()
	{
		GPSPosition initial_pos = this.getCurrentPosition();
		this.plotVectorHeading();
		
		double distance_covered = this.getDistanceTo(initial_pos);
		
		// wait untill the required distance is travelled
		while(distance_covered < getVectorDistance())
		{
			distance_covered = this.getDistanceTo(initial_pos);
			
			// allow time for other threads to execute
			try
			{
				thread.sleep(10);
			}
			catch(InterruptedException e)
			{
				e.printStackTrace();
			}
		}
		return;
	}

	/*public void run()
	{
		GPSPosition initial_pos = this.getCurrentPosition();
		GPSPosition final_pos = initial_pos.getPositionWithVector(current_vector);
		//this.plotVectorHeading();
		
		setHeadingDeg(initial_pos.getHeading(final_pos));
		
		double distance_covered = this.getDistanceTo(initial_pos);
		
		// wait untill the required distance is travelled
		while(distance_covered < getVectorDistance())
		{
			GPSPosition current = getCurrentPosition();
			
			distance_covered = current.getDistance(initial_pos);
			
			setHeadingDeg(current.getHeading(final_pos));
			
			// allow time for other threads to execute
			try
			{
				thread.sleep(10);
			}
			catch(InterruptedException e)
			{
				System.err.println(e);
			}
		}
		return;
	}*/
	
	

	
	/* Starts the execution of the thread, which in turn 
	 * executes the run method.
	*/
	public void startVector()
	{
		thread = new Thread(this);
		thread.start();
	}
	
	/* Joins thread to the current thread.
	*/
	public void join()
	{
		try
		{
			thread.join();
		}
		catch(InterruptedException e)
		{
			System.err.println(e);
		}
	}
		
}