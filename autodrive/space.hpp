////////////////////////////////////////////
//
// space.hpp
//
// Part of Bayesian Search and Rescue Simulator 
//
// This Class holds all grid space functions and variables 
// 
// Copyright (c) Phil Sammons
// UNIVERSITY OF NEW SOUTH WALES (Sydney, Australia)
// 
// Parameters
//-----------------------------------------
// Dependant Libraries : ULapack
// 
////////////////////////////////////////////

#ifndef _SPACE_H_
#define _SPACE_H_

#include <stdarg.h>
#include <unistd.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <stdexcept>
#include <ulapack/matrix_types.hpp>
#include <ulapack/matrix_ops.hpp>

//#include "grid_pdf.hpp"
#include "object_data.hpp"

//Retain ANSI/ISO Compability
#ifdef WIN32
    #include <windows.h>
#endif

using namespace std;
using namespace ulapack;

class Space
{
 public:
   
   Space();
   Space(int x_min, int y_min, int x_grid, int y_grid, int nstep);
      
   ~Space();
   
   void make_space(int x_min, int y_min, int x_grid, int y_grid, int nstep);
   void create_axis_vector();
   
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
   //Object_Data prediction;
};

#endif
