////////////////////////////////////////////
//
// movevehicles.hpp
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
// Dependant Libraries : ULapack
// 
////////////////////////////////////////////

#ifndef _VEHICLE_HPP_
#define _VEHICLE_HPP_

//Retain ANSI/ISO Compability
#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdlib.h>

#include <ulapack/matrix_types.hpp>
#include <ulapack/scalar_fill.hpp>

#include "object_data.hpp"
#include "grid_pdf.hpp"
#include "testing.hpp"
#include "fgcontroller.hpp"

using namespace std;

class Vehicle// : public GridPDF
{
public:

  //Vehicle(void);	
  //~Vehicle(void);  
  /*Vehicle(Object_Data VD[5], Object_Data SD[5], Object_Data TD[5],
    Object_Data PD[5], Object_Data GD);*/
  
  // MOVE VEHICLES
  void move_vehicles(GridPDF gridobject, double timestamp);

  // MOVE VEHICLES
  void move_with_lookahead(GridPDF *&gridobject, double timestamp);

   void Vehicle::read_vehicle_position(FGFSSocket uav, int vehicle,
				       GridPDF gridobject, double timestamp);  
   
private:
 
  //  TestClass gridobject;

  //Object_Data _GD;
  //Object_Data _VD[5], _SD[5], _TD[5], _PD[5];
  
  // VEHICLE MODEL - DECIDE REAL STEERING ANGLE
  double steer(Object_Data &OD, double request_theta);
  
  // RADAR MODEL - RETURN SUM OF VALUES ALONG A LINE FROM 0
  //               OF LENGTH AT ANGLE THETA
  double get_value(Object_Data &OD, Vector &position,
		   double theta, double length);
  double get_value2(Object_Data &OD,Object_Data &VD, Vector pos,double heading,
		    double theta, int lookaheads, Vector &final_pos);
  
  // GET NEW POSITION - RETURNS THE COORDS OF NEW POSITION
  //                    GIVEN THETA AND VELOCITY
  Vector get_new_pos(Vector &pos, double theta,
		     double velocity, double timestep);
  
  //  STORE VALUES FOR THE NEXT VEHICLE POSITION
  void set_new_position(Object_Data &OD, Vector &pos, double theta,
			double &heading, double &current_val, double next_val,
			Vector &next, int &heading_set, double &look,
			double line, double &next_head, double timestep);
  
  double get_max_timestep(double rad_at_max_v, double rad_at_min_v,
			 double velocity, double minimum_velocity, 
			 double maximum_velocity);

  double add_points(Vector &pos, double velocity, double timestep,
		    double max_velocity, double min_velocity,
		    double rad_at_max_v, double rad_at_min_v);

  //CALCULATE STEERING ACTION TO TAKE	
  double get_best_direction(double max_theta, int &lookaheads, Vector &pos,
			    Vector &final_pos, int vehicle, double &heading,
			    double &current_val, double distance, double line,
			    double &next_head, double &next_val, Vector &next,
			    int &heading_set, double &dtheta,
			    GridPDF *&gridobject, double timestep); 

	
};

#endif
