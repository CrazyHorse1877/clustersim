////////////////////////////////////////////
//
// write_file.hpp
//
// Part of Bayesian Search and Rescue Simulator 
//
// These functions read input from the user and
// write the prior data from the stored data file.
// 
// Copyright (c) Phil Sammons
// UNIVERSITY OF NEW SOUTH WALES (Sydney, Australia)
// 
// Parameters
//-----------------------------------------
// Dependant Files : grid_pdf.hpp, grid_pdf.cpp
// 
////////////////////////////////////////////

#ifndef _WRITE_FILE_HPP_
#define _WRITE_FILE_HPP_

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

//
// CLEAR FILE FOR WRITING
//
void clear_file(char* filename)
{
   ofstream myfile (filename); // SET OUTPUT FILE
   if (myfile.is_open())
     {
	myfile << "";          // CLEAR FILE
	myfile.close();        // CLOSE FILE
     }
   else{cout << "Unable to open file\n";}
}

void write_vehicles(void)
{
   char inputt[3];
   int nvehicles, vehicle;
   char x0[10], y0[10], z0[6], th0[3];
   char vmin[6], vmax[6], tminv[6], tmaxv[6], vx[6], vy[6];
   
   clear_file("vehicle_data.dat");  //Clear File		
   
   cout << "Enter the Following pieces of information\n";
   cout << "Number of Vehicles: "; cin >> ws;
   cin.getline(inputt, 3);
   nvehicles = atol(inputt);
    
   ofstream InputData("vehicle_data.dat", ios::app);
   InputData << "Vehicles: " << nvehicles << "\n\n";
   
   for(  vehicle = 1 ; vehicle < nvehicles+1 ; vehicle++ )
     {
	cout << "Enter the start point for vehicle " << vehicle << endl;
	cout << "x0: ";cin >> ws;
	cin.getline(x0, 6);
	cout << "y0: ";cin >> ws;
	cin.getline(y0, 6);
	cout << "Height: "; cin >> ws;
	cin.getline(z0, 6);
	cout << "Heading: "; cin >> ws;
	cin.getline(th0, 6);
	cout << "Enter the velocity of the vehicle " << vehicle << endl;
	cout << "Minimum: ";cin >> ws;
	cin.getline(vmin, 6);
	cout << "Maximum: "; cin >> ws;
	cin.getline(vmax, 6);
	cout << "Enter of the steering restrictions for vehicle " << vehicle << endl;
	cout << "Steering rate at Minimun Velocity: "; cin >> ws;
	cin.getline(tminv, 6);
	cout << "Steering rate at Maximun Velocity: "; cin >> ws;
	cin.getline(tmaxv, 6);
	cout << "Variance of Sensor: "; cin >> ws;
	cin.getline(vx, 6);
	
	InputData << "Vehicle " << vehicle << "\n==========================\n\n"
	  << "Starting point:" << x0 << "," << y0 << "\n\n"
	  << "Starting Height:" << z0 << "\n"
	  << "Starting angle:"	  << th0 << "\n\n"
	  << "Minimum Velocity:" << vmin << "\n"
	  << "Maximum Velocity:" << vmax << "\n\n"
	  << "Steering at Minimum Velocity:"  << tminv << "\n"
	  << "Steering at Maximum Velocity:"  << tmaxv << "\n\n"
	  << "Variance of sensor:"  << vx << "," << vx << "\n\n";
     }
}

void write_targets(void)
{
   char inputt[3];
   int ntargets, target;
   char x0[10], y0[10], v0[6];//, z0[6], th0[3];
   //char vmin[6], vmax[6], tmin[6], tmax[6],
   char vx[6], vy[6];
   
   clear_file("target_data.dat");  //Clear File		
   
   cout << "Enter the Following pieces of information\n";
   cout << "Number of Targets: "; cin >> ws;
   cin.getline(inputt, 3);
   ntargets = atol(inputt);
   //    cin.getline(targets, 3);
   // 
   ofstream InputData("target_data.dat", ios::app);
   InputData << "Targets: " << ntargets << "\n\n";
   
   for(  target = 1 ; target < ntargets+1 ; target++ )
     {
	cout << "Enter the start point for target " << target << endl;
	cout << "Mean x: ";cin >> ws;
	cin.getline(x0, 6);
	cout << "Mean y: ";cin >> ws;
	cin.getline(y0, 6);
//	cout << "Height: "; cin >> ws;
//	cin.getline(z0, 6);
	char z0[] = "0";    
//	cout << "Heading (degrees from East): "; cin >> ws;
//	cin.getline(th0, 6);
	char th0[] = "0";
	cout << "Enter the velocity of target " << target << endl;
	cout << "start: ";cin >> ws;
	cin.getline(v0, 6);
//	cout << "Minimum: ";cin >> ws;
//	cin.getline(vmin, 6);
	char vmin[] = "0";
//	cout << "Maximum: "; cin >> ws;
//	cin.getline(vmax, 6);
	char vmax[] = "0";
//	cout << "Enter of the steering restrictions for target " << target << endl;
//	cout << "Steering rate at Minimin Velocity: "; cin >> ws;
//	cin.getline(tmin, 6);
	char tmin[] = "30";
//	cout << "Steering rate at Maximum Velocity: "; cin >> ws;
//	cin.getline(tmax, 6);
	char tmax[] = "10";
	cout << "Enter the uncertanty: "; cin >> ws;
	cin.getline(vx, 6);
	//cout << "Enter the uncertanty: "; cin >> ws;
	   
	//ofstream InputData("input.dat", ios::app);
	InputData << "target " << target << "\n==========================\n\n"
	  << "Starting point:" << x0 << "," << y0 << "\n\n"
	  << "Starting Height:" << z0 << "\n"
	  << "Starting angle:"	  << th0 << "\n\n"
	  << "Minimum Velocity:" << vmin << "\n"
	  << "Maximum Velocity:" << vmax << "\n\n"
	  << "Steering at Minimun Velocity:"  << tmin << "\n"
	  << "Steering at Maximum Velocity:"  << tmax << "\n\n"
	  << "Uncertainty:"  << vx << "," << vx << "\n\n";
     }
}


#endif
