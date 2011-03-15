/*
 *  Written By Elias Jordan, 2009
 */

public class GPSPosition
{
	// earth radius
	final double R = 6378137;
	
	// latitude and longitude
	double latitude;
	double longitude;
	double altitude;

	GPSPosition(double lat, double lng, double altitude)
	{
		this.latitude = lat;
		this.longitude = lng;
		this.altitude = altitude;
	}
	
	GPSPosition(double lat, double lng)
	{
		this(lat, lng, 1000);
	}
	
	public String toString()
	{
		return this.longitude + ", " + this.latitude + "@" + this.altitude;
	}
	
	// returns the heading from this position to the provided one
	public double getHeading(GPSPosition destination)
	{
		// convert latitudes and longitudes into radians
		double long1 = this.longitude*Math.PI/180;
		double long2 = destination.longitude*Math.PI/180;
		double lat1 = this.latitude*Math.PI/180;
		double lat2 = destination.latitude*Math.PI/180;
		
		double dlong = long2 - long1;
		double x = Math.sin(dlong) * Math.cos(lat2);
		double y = Math.cos(lat1)*Math.sin(lat2) - Math.sin(lat1)*Math.cos(lat2)*Math.cos(dlong);
		double raw_angle = Math.atan2(x, y);
		
		return (raw_angle % (2*Math.PI)) * 180/Math.PI;
	}
	
	public double getDistance(GPSPosition destination)
	{
		// convert latitudes and longitudes into radians
		double long1 = this.longitude*Math.PI/180;
		double long2 = destination.longitude*Math.PI/180;
		double lat1 = this.latitude*Math.PI/180;
		double lat2 = destination.latitude*Math.PI/180;
		
		double dlong = long2 - long1;
		double dlat = lat2 - lat1;
		
		double ssq_lat = Math.pow(Math.sin(dlat/2), 2);
		double ssq_long = Math.pow(Math.sin(dlong/2), 2);
		double a = ssq_lat + Math.cos(lat1)*Math.cos(lat2)*ssq_long;
		double c = 2*Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
		
		return R*c;
	}
	
	// returns the position if vector is followed from this position
	public GPSPosition getPositionWithVector(FlightVector vector)
	{
		double d = vector.distance;
		double lat1 = this.latitude;
		double lon1 = this.longitude;
		double brng = vector.true_heading;
		
		double lat2 = Math.asin( Math.sin(lat1)*Math.cos(d/R) + 
							 Math.cos(lat1)*Math.sin(d/R)*Math.cos(brng) );
		double lon2 = lon1 + Math.atan2(Math.sin(brng)*Math.sin(d/R)*Math.cos(lat1), 
									 Math.cos(d/R)-Math.sin(lat1)*Math.sin(lat2));
		return new GPSPosition(lat2, lon2);
	}
	
	public GPSPosition adjustPosition(double x, double y)
	{
		double quick_dirty = 0.000001/0.111;
		return new GPSPosition(this.latitude + y*quick_dirty, this.longitude + x*quick_dirty, this.altitude);
	}

	/*public UTMPosition convertToUTM()
	{
		//System.out.println("calculate UTM");
		// convert the latitude and longitude into radians
		double lat_rad = this.latitude*(Math.PI/180);
		double long_rad = this.longitude*(Math.PI/180);
			
			
		// CONSTANTS
			
		// equitorial radius, meters
		final double a = 6378137;
	
		// polar radius, meters
		final double b = 6356752.3142;
	
		// scale along central meridian
		final double k0 = 0.9996;
			
			
			
		// VARIABLE CALCULATION
			
		// eccentricity values
		double e = Math.sqrt( 1-Math.pow(b,2) / Math.pow(a,2) );
		double e_dash_sq = Math.pow((e*a/b),2) / (1-Math.pow(e,2));
	
		double n = (a-b)/(a+b);
	
		// radii of curvature
		double tmp = 1-e*e*Math.pow((Math.sin(lat_rad)),2);
		double rho = a*(1-e*e)/(Math.pow(tmp,1.5));
		double nu = a/(Math.pow(tmp,0.5));
		
		//System.out.println("Rho: " + rho);
		//System.out.println("Nu: " + nu);
	
		// UTM longitude zone
		int long_zone = 31 + (int)Math.floor(longitude/6);
		
		//System.out.println("Long Zone: " + long_zone);
	
		// longitude of central meridian for this zone
		double long_cm_rad = (6*long_zone - 183)*(Math.PI/180);
		
		//System.out.println("Long Zone CM (deg): " + long_cm_rad*(180/Math.PI));
	
		double p = long_rad - long_cm_rad;
		
		//System.out.println("P: " + p);
	
		double A = a*(1 - n + 1.25*(n*n - n*n*n) + (81/64)*(n*n*n*n - n*n*n*n*n));
	
		double B = (3*a*n/2)*(1 - n - (7*n*n/8)*(1-n) + 55*n*n*n*n/64);
	
		double C = (15*a*n*n/16)*(1 - n +(3*n*n/4)*(1-n));
	
		double D = (35*a*n*n*n/48)*(1 - n + 11*n*n/16);
	
		double E = (315*a*n*n*n*n/51)*(1-n);
	
		double S =   A*lat_rad 
			           - B*Math.sin(2*lat_rad) 
				       + C*Math.sin(4*lat_rad) 
			           - D*Math.sin(6*lat_rad) 
			           + E*Math.sin(8*lat_rad);
				   
		//System.out.println("S: " + S);
			   
		double k1 = S*k0;
		double k2 = k0*nu*Math.sin(2*lat_rad)/4;
		double k3 = (k0*nu*Math.sin(lat_rad)*Math.pow(Math.cos(lat_rad),3)/24)*(  5 - Math.pow(Math.tan(lat_rad),2)
	                                                        + 9*e_dash_sq*Math.pow(Math.cos(lat_rad),2)
						                + 4*e_dash_sq*e_dash_sq*Math.pow(Math.cos(lat_rad),4));
		double k4 = k0*nu*Math.cos(lat_rad);
		double k5 = (k0*nu*(Math.pow(Math.cos(lat_rad),3))/6)*(  1 - Math.pow(Math.tan(lat_rad),2)
	                                                         + e_dash_sq*Math.pow(Math.cos(lat_rad),2));
															 
		double northing = k1 + k2*p*p + k3*p*p*p*p;
		double easting = k4*p + k5*p*p*p + 500000;
		
		if(northing < 0)
		{
			northing += 10000000;
		}

		return new UTMPosition(easting, northing, long_zone, (this.latitude > 0), altitude);		
	}*/
	
	public static void main(String[] argv)
	{
		GPSPosition p1 = new GPSPosition(100, 100);
		FlightVector v = new FlightVector(0, 30, 100000);
		
		GPSPosition p2 = p1.getPositionWithVector(v);
		
		System.out.println(p2);
				
		/*TestPilot pilot = new TestPilot();
		pilot.takeOff();
		GPSReader positions = new GPSReader("circle.txt");
		
		for(GPSPosition target_pos : positions)
		{
			System.out.println("-------------- Target: " + target_pos + " -----------------");
			
			GPSPosition current_pos = pilot.getCurrentPosition();
			while(current_pos.getDistance(target_pos) > 30)
			{
				System.out.println("Distance: " + current_pos.getDistance(target_pos));
				
				double heading = current_pos.getHeading(target_pos);
				pilot.setHeadingDeg(heading);
				
				current_pos = pilot.getCurrentPosition();
			}
		}*/
		
	}
}




