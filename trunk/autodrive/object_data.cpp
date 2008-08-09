////////////////////////////////////////////
//
// object_data.cpp
//
// Part of Bayesian Search and Rescue Simulator 
//
// This Class holds the functions and values that
// are related to a vehicle or target object like
// orientation and status etc. 
// 
// Copyright (c) Phil Sammons
// UNIVERSITY OF NEW SOUTH WALES (Sydney, Australia)
// 
// Parameters
//-----------------------------------------
// Dependant Files : object_data.hpp
// 
////////////////////////////////////////////

#include "object_data.hpp"

using namespace std;
using namespace ulapack;

Object_Data::Object_Data (void){}
Object_Data::~Object_Data(){return;}

void Object_Data::setup(void)
{
   
   numObjects = 0;
   target_aquired = false;
   tracking_target = false;
   all_targets_aquired = false;
   xyz_location;
   xyz_velocity;
   
   x0 = y0 = z0 = th0 = v0 = 0;
   vmin = vmax = tminv = tmaxv = accel = decel = 0;
   mx = my = mz = ath = vx = vy = 0;
   object_mass = 0;
   
   xmin = xmax = ymin = ymax = xstep = ystep = xsize = ysize = 0;
   Matrix tempm(1,1);tempm(0,0) = 0;
   Vector tempv(1);tempv(0) = 0;
   xvector = yvector = tempv;
}

// BREAK UP POSITION VECTOR
void Object_Data::XYZreal(Vector xyz)
{
   double x = xyz(0);
   double y = xyz(1);
   double z = xyz(2);
};

// BREAK UP VELOCITY VECTOR
void Object_Data::Velocity(Vector x0y0z0)
{
   double velx = x0y0z0(0);
   double vely = x0y0z0(1);
   double velz = x0y0z0(2);
};

//NOT USED YET
void Object_Data::XYZmean(double x, double y, double z)
{
   double xm = x;
   double ym = y;
   double zm = z;
};

// BREAK UP THRUST VECTOR
void Object_Data::Thrust(double thrust)
{
   double thrust_x = thrust * velx;
   double thrust_y = thrust * vely;
   double thrust_z = thrust * velz;
};

// CALCULATE ACCELERATION
void Object_Data::Acceleration(double thrust_x, double thrust_y, double thrust_z)
{
   double accx = object_mass * thrust_x;
   double accy = object_mass * thrust_y;
   double accz = object_mass * thrust_z;
};

//SETUP SPACE ATTRIBUTES
void Object_Data::make_space(int x_min, int y_min, int x_grid, int y_grid, int nstep)
{
   xmin = x_min;
   ymin = y_min;
   xstep = nstep;
   ystep = nstep;
   
   this->xsize = x_grid;
   this->ysize = y_grid;
   
   this->xmax = (xmin+xsize*xstep)-1;
   this->ymax = (ymin+ysize*ystep)-1;
   
   Matrix tempm(xsize, ysize);   // used for initialising and testing
   scalar_fill(tempm, 0.0);      // used for initialising and testing
   this-> blank_matrix = tempm;
   prior = pdf = complement = likelyhood = tempm;
   global_target = global_observation = blank_matrix = tempm;   
   create_axis_vector();
}

//----------------------------------------------------------------\
//  This creates two vectors that are the axis values of a matrix \
//----------------------------------------------------------------/
void Object_Data::create_axis_vector()
{
   Vector xvect(xsize);
   Vector yvect(ysize);
   double xval = xmin;
   double yval = ymin;
   for(int i = 0 ; i < (xsize) ; i++ )
     {
	xvect(i) = xval;
	xval = xval + xstep;
     }
   for(int i = 0 ; i < (ysize) ; i++ )
     {
	yvect(i) = yval;
	yval = yval + ystep;
     }
   this->xvector = xvect;
   this->yvector = yvect;
}
