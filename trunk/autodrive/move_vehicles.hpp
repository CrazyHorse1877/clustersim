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

//#include <ulapack/matrix_types.hpp>
//#include <ulapack/scalar_fill.hpp>

#include "object_data.hpp"
#include "grid_pdf.hpp"
#include "main.hpp"
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

   void read_vehicle_position(FGFSSocket uav, int vehicle,
				       GridPDF gridobject, double timestamp);  
   
private:
 
  // RADAR MODEL - RETURN SUM OF VALUES ALONG A LINE FROM 0
  //               OF LENGTH AT ANGLE THETA
  double get_value(Object_Data &OD, Vector &position,
		   double theta, double length);
  
  // GET NEW POSITION - RETURNS THE COORDS OF NEW POSITION
  //                    GIVEN THETA AND VELOCITY
  Vector get_new_pos(Vector &pos, double theta,
		     double velocity, double timestep);
  
  //  STORE VALUES FOR THE NEXT VEHICLE POSITION
  void set_new_position(Object_Data &OD, Vector &pos, double theta,
			double &heading, double &current_val, double next_val,
			Vector &next, int &heading_set, double &look,
			double line, double &next_head, double timestep);
  
};

#endif
