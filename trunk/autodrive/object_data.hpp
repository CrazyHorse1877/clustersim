////////////////////////////////////////////
//
// object_data.hpp
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
// Dependant Libraries : ULapack, Spasm
// 
////////////////////////////////////////////

#ifndef _OBJECT_DATA_HPP_
#define _OBJECT_DATA_HPP_

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdlib.h>

//#include <ulapack/matrix_types.hpp>
//#include <ulapack/scalar_fill.hpp>

//Retain ANSI/ISO Compability
#ifdef WIN32
    #include <windows.h>
#endif

using namespace std;
//using namespace ulapack;

class Object_Data
{
 private:
   double velx;
   double vely;
   double velz;
   
 public:
   
   Object_Data();
   ~Object_Data();
   //   Object_Data(const string &, int grid, int mean, int var);
   //   Object_Data(const string &, int gridx, int gridy, int my, int vx, int vx);
          
   //
   //OBJECT DYNAMICS
   //   
   int numObjects;
   bool target_aquired;
   bool tracking_target;
   
   Vector xyz_location;
   Vector xyz_velocity;
   double object_mass;
   
   //REAL VALUES
   double x0; // location
   double y0;
   double z0;
   double th0;// heading
   double v0; // velocity
   
   double gps_lat; //requested GPS Latitude value
   double gps_long; //requested GPS Longitude value
   double gps_elev; //requested GPS Elevation value
   
   double vmin; // minimum velocity
   double vmax; // maximum velocity
   double tminv;// turning under minimum velocity
   double tmaxv;// turning under maximum velocity
   double accel;// linear acceleration and decelaration rates
   double decel;
   
   //ASSUMED VALUES
   double mx; // mean
   double my;
   double mz;
   double ath;// heading
   double vx; // variance
   double vy;
   
   //Matrix predict_model;
   Matrix prior;
   Matrix pdf;
   Matrix complement;   
   Matrix likelyhood;
   
   void XYZreal(Vector xyz);
   void Velocity(Vector x0y0z0);
   void XYZmean(double x, double y, double z);
   void Thrust(double thrust);
   void Acceleration(double thrust_x, double thrust_y, double thrust_z);

   void setup(void);
   
   //
   //OBJECT SPACE VALUES
   //
   double xmin;
   double xmax;
   double ymin;
   double ymax;
   double xstep;
   double ystep;
   int xsize;
   int ysize;
   Vector xvector;
   Vector yvector;
   Matrix global_target;
   Matrix global_observation;
   Matrix blank_matrix;
   
   bool all_targets_aquired;
   
   void make_space(int x_min, int y_min, int x_grid, int y_grid, int nstep);
   void create_axis_vector();
  
};

#endif
