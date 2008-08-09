////////////////////////////////////////////
//
// vehicle.cpp
//
// Part of Bayesian Search and Rescue Simulator 
//
// This functions control the movement of the vehicle 
// 
// Copyright (c) Phil Sammons
// UNIVERSITY OF NEW SOUTH WALES (Sydney, Australia)
// 
// Parameters
//-----------------------------------------
// Dependant Files : move_vehicles.hpp
// 
////////////////////////////////////////////

#include "move_vehicles.hpp"

using namespace std;

//----------------------------------------------------------------	\
//  MOVE VEHICLES -							\
//----------------------------------------------------------------/
void Vehicle::move_vehicles(GridPDF gridobject, double timestamp){
   static double old_time = 0;
   
   //write("set /autopilot/route-manager/input @clear");
   //write("set /autopilot/route-manager/input @insert-1:%lf,%lf@600",nextwplong, nextwplat);
   
   for(int vehicle = 0 ; vehicle <  gridobject.VD[0].numObjects ; vehicle++ )
     {
	Vector pos(2);
	Vector next(2);
	double line = 25;
	double sum;
	double nextX, nextY, heading; 
	double current_val, next_val, next_head;
	double look =  gridobject.VD[vehicle].v0 + line ;
	int heading_set = 0;
	double timestep = timestamp - old_time;if(!timestep)break;
	old_time = timestamp;
	const char *p ;
	
	//read_vehicle_position(gridobject, vehicle, timestamp);
		
	double start_long = 151.17;
	double start_lat = -33.94;
	
	//read_vehicle_position(uav1, vehicle, gridobject, timestamp);
       
	pos(0) =  (gridobject.VD[vehicle].gps_long - start_long) * 100;
	pos(1) =  (gridobject.VD[vehicle].gps_lat - start_lat) * 100;

	gridobject.VD[vehicle].x0 = (gridobject.VD[vehicle].gps_long - start_long) * 100;
	gridobject.VD[vehicle].y0 = (gridobject.VD[vehicle].gps_lat - start_lat) * 100;
	
	//       pos(0) =  gridobject.VD[vehicle].x0-gridobject.GD->xmin;
	//       pos(1) =  gridobject.VD[vehicle].y0-gridobject.GD->ymin;
      	// 
	for(int l = 0 ; l < 100 ; l++ )
	  {
	     // TEST TO GET VALUE AT ANGLE 0 AND DISTANCE "LOOK".
	     current_val = get_value(*gridobject.GD, pos, 0.0, look);
	     double theta = 0;
	     for(int i = 0 ; i <= 360 ; i ++ )
	       {
		  //GET VALUES UP TO DISTANCE "LOOK"
		  next_val = get_value(*gridobject.GD, pos, theta, look);
		  
		  //IF NEW VALUE IS BIGGER, SET NEW POSITION
		  if (next_val > current_val){
		     set_new_position(gridobject.VD[vehicle], pos, theta,
				      gridobject.VD[vehicle].th0, current_val,
				      next_val, next, heading_set, look,
				      line, next_head, timestep);
		  }
		  theta += 1;
	       }
	     //IF NO HEADING HAS BEEN SET
	     //EITHER 0 IS THE BEST HEADING OR ALL VALUES ARE EQUAL
	     if(heading_set == 0)
	       {
		  //COMPAIR ANOTHER VALUE
		  next_val = get_value(*gridobject.GD, pos, 180, look);
		  
		  if(next_val == current_val)
		    //IF SAME - ALL VALUES ARE EQUAL . INCREASE SEARCH AREA
		    {
		       look++;
		    }
		  else
		    //HEADING IS OK
		    set_new_position(gridobject.VD[vehicle], pos,0 ,heading,
				     current_val, next_val, next, heading_set,
				     look, line, next_head, timestep);
	       }
	     if(look > 200)throw 21;
	     if(heading_set)break;
	  }
	
	double nextwplong = next(0);
	double nextwplat = next(1); 
	
	//MOVE VEHICLE TO NEW POINT
     }
}

//----------------------------------------------------------------	\
//  get_value - RETURNS THE SUM OF THE VALUES ALONG A LINE @ THETA	\
//----------------------------------------------------------------/
double Vehicle::get_value(Object_Data &OD,Vector &pos,
			  double theta, double step)
{
   Vector next_cords(2);
   double sum = 0;
   for(int i = 0 ; i < step ; i++)
     {
	//GET COORINATES OF NEXT POINT
	next_cords(0) = pos(0) + ((i+0.5) * cos(theta*M_PI/180));
	next_cords(1) = pos(1) + ((i+0.5) * sin(theta*M_PI/180));
	
	//GET VALUE OF POINT + // validate point
	if( (int)next_cords(0) < OD.xsize && (int)next_cords(0) >=0 &&
	    (int)next_cords(1) < OD.ysize && (int)next_cords(1) >=0)
	  sum = sum + OD.global_target((int)next_cords(0), (int)next_cords(1));
     }
   
   //RETURN SUM OF VALUES ALONG LINE
   return sum;
}

//----------------------------------------------------------------	\
//  get_new_pos - RETURNS THE COORDS OF NEW POSITION GIVEN  THETA	\
//----------------------------------------------------------------/
Vector Vehicle::get_new_pos(Vector &pos, double theta,
			    double velocity, double timestep)
{
   Vector next_pos(2);
   next_pos(0) = pos(0) + ((velocity * timestep) * cos(theta*M_PI/180) * 0.01)+151.17;
   next_pos(1) = pos(1) + ((velocity * timestep) * sin(theta*M_PI/180) * 0.01)+-33.94;
   return next_pos;
}

//----------------------------------------------------------------	\
//  SET VALUES FOR THE NEXT VEHICLE POSITION				\
//----------------------------------------------------------------/
void Vehicle::set_new_position(Object_Data &OD, Vector &pos, double theta,
			       double &heading, double &current_val,
			       double next_val, Vector &next, 
			       int &heading_set, double &look , double line,
			       double &next_head, double timestep)
{
   next_head = theta;
   // SET NEW POSITION  
   next = get_new_pos(pos, heading, OD.v0, timestep);
   // SET AS DESIRED DIRECTION
   current_val = next_val;
   //RESET LOOK
   look = OD.v0 + line;
   heading_set = 1;
}

//----------------------------------------------------------------	\
//  READ VEHICLE POSITION -					      	\
//----------------------------------------------------------------/
void Vehicle::read_vehicle_position(FGFSSocket uav, int vehicle, 
				    GridPDF gridobject, double timestamp){
   
   const char *p;
   //FGFSSocket uav1("localhost", 5400);
   uav.write("get /position/longitude-deg");
   p = uav.read();
   sscanf(p,"%lf",&gridobject.VD[vehicle].gps_long);
   uav.write("get /position/latitude-deg");
   p = uav.read();
   sscanf(p,"%lf",&gridobject.VD[vehicle].gps_lat);
   uav.write("get /position/altitude-ft");
   p = uav.read();
   sscanf(p,"%lf",&gridobject.VD[vehicle].gps_elev);
   uav.write("get /orientation/heading-deg");
   p = uav.read();
   sscanf(p,"%lf",&gridobject.VD[vehicle].th0);


}
