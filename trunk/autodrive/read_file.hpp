////////////////////////////////////////////
//
// read_file.hpp
//
// Part of Bayesian Search and Rescue Simulator 
//
// These functions read the prior data from the stored data file
// 
// Copyright (c) Phil Sammons
// UNIVERSITY OF NEW SOUTH WALES (Sydney, Australia)
// 
// Parameters
//-----------------------------------------
// Dependant Files : grid_pdf.hpp, grid_pdf.cpp

// 
////////////////////////////////////////////

#ifndef _READ_FILE_HPP_
#define _READ_FILE_HPP_

#include "grid_pdf.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <list>

//Retain ANSI/ISO Compability
#ifdef WIN32
    #include <windows.h>
#endif

using namespace std;

void read_vehicles(Object_Data VD[])
{
  char inputt[3];
  int nvehicles, vehicle;
  double mx, my;
  char x0[10], y0[10], z0[6], th0[3] , v0[10];
  char vmin[6], vmax[6], tminv[6], tmaxv[6], vx[6] , vy[6], object_mass[6];
  char blankblank[200];
  
  ifstream VehicleData("vehicle_data.dat");
  VehicleData.getline(blankblank, 20, ':');
  VehicleData.getline(inputt, 30, '\n');nvehicles = atoi(inputt);
  
  //   Vehicle_Data vehicle_data[nvehicles];	
  
  for(int vehicle = 0 ; vehicle < nvehicles ; vehicle ++)
    {
      VD[vehicle].numObjects = nvehicles;
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(x0, 10, ',');
      VD[vehicle].x0 = atof(x0);
      VehicleData.getline(y0, 10, '\n');
      VD[vehicle].y0 = atof(y0);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(z0, 10, '\n');
      VD[vehicle].z0 = atof(z0);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(th0, 10, '\n');
      VD[vehicle].th0 = atof(th0);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(v0, 10, '\n');
      VD[vehicle].v0 = atof(v0);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(vmin, 10, '\n');
      VD[vehicle].vmin = atof(vmin);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(vmax, 10, '\n');
      VD[vehicle].vmax = atof(vmax);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(object_mass, 10, '\n');
      VD[vehicle].object_mass = atof(object_mass);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(tminv, 10, '\n');
      VD[vehicle].tminv = atof(tminv);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(tmaxv, 10, '\n');
      VD[vehicle].tmaxv = atof(tmaxv);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(vx, 10, ',');
      VD[vehicle].vx = atof(vx);
      VehicleData.getline(vy, 10, '\n');
      VD[vehicle].vy = atof(vy);
      VD[vehicle].mx = 0.0;
      VD[vehicle].my = 0.0;
      
      cout << "\n -=- Vehicle " << vehicle+1 << " -=-";
      cout << "\nStart Point: [" << VD[vehicle].x0 
	   << ", " << VD[vehicle].y0 << "]";
      cout << "\nStart Angle: " << VD[vehicle].th0 ;
      cout << "\nStart Velocity: " << VD[vehicle].v0 ;
      cout << "\nMinimum Velocity: " << VD[vehicle].vmin ;
      cout << "\nMaximum Velocity: " << VD[vehicle].vmax ;
      cout << "\nMass: " << VD[vehicle].object_mass ;
      cout << "\nSteering at Minimun Velocity:  " << VD[vehicle].tminv ;
      cout << "\nSteering at Maximum Velocity:  " << VD[vehicle].tmaxv << endl;
      cout << endl;
    }
}

void read_targets(Object_Data TD[])
{
  char inputt[3];
  int ntargets, target;
  double mx, my;
  char x0[10], y0[10], z0[6], th0[6], v0[6] ;
  char vmin[6], vmax[6], tminv[6], tmaxv[6], vx[6] , vy[6], object_mass[6];
  char blankblank[200];
  
  ifstream targetData("target_data.dat");
  targetData.getline(blankblank, 20, ':');
  targetData.getline(inputt, 30, '\n');ntargets = atoi(inputt);
  
  
  for(int target = 0 ; target < ntargets ; target ++)
    {
      TD[target].numObjects = ntargets;
      targetData.getline(blankblank, 200, ':');
      targetData.getline(x0, 10, ',');
      TD[target].x0 = atof(x0);
      targetData.getline(y0, 10, '\n');
      TD[target].y0 = atof(y0);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(z0, 10, '\n');
      TD[target].z0 = atof(z0);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(th0, 10, '\n');
      TD[target].th0 = atof(th0);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(v0, 10, '\n');
      TD[target].v0 = atof(v0);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(vmin, 10, '\n');
      TD[target].vmin = atof(vmin);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(vmax, 10, '\n');
      TD[target].vmax = atof(vmax);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(object_mass, 10, '\n');
      TD[target].object_mass= atof(object_mass);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(tminv, 10, '\n');
      TD[target].tminv = atof(tminv);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(tmaxv, 10, '\n');
      TD[target].tmaxv = atof(tmaxv);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(vx, 10, ',');
      TD[target].vx = atof(vx);
      targetData.getline(vy, 10, '\n');
      TD[target].vy = atof(vy);
      TD[target].mx = atof(x0);
      TD[target].my = atof(y0);
      
      
      cout << "\n -=- target " << target+1 << " -=-";
      cout << "\nStart Point: [" << TD[target].x0 
	   << ", " << TD[target].y0 << "]";
      cout << "\nStart Angle: " << TD[target].th0 ;
      cout << "\nStart Velocity: " << TD[target].v0 ;
      cout << "\nMinimum Velocity: " << TD[target].vmin ;
      cout << "\nMaximum Velocity: " << TD[target].vmax ;
      cout << "\nMass: " << TD[target].object_mass ;
      cout << "\nSteering at Minimun Velocity:  " 
	   << TD[target].tminv ;
      cout << "\nSteering at Maximum Velocity:  " 
	   << TD[target].tmaxv << endl;		
      cout << "Variance:  " << TD[target].vx << endl;		
    }
}

#endif
