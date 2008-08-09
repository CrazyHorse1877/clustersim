////////////////////////////////////////////
//
// space.cpp
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
// Dependant Files: space.hpp
// 
////////////////////////////////////////////

//#include "grid_pdf.hpp"
//#include "object_data.hpp"
#include "space.hpp"

using namespace std;
using namespace ulapack;

Space::Space(void){}
Space::Space(int x_min, int y_min, int x_grid, int y_grid, int nstep){}
Space::~Space(){return;}

void Space::make_space(int x_min, int y_min, int x_grid, int y_grid, int nstep)
{
   xmin = x_min;
   ymin = y_min;
   xstep = nstep;
   ystep = nstep;
 
   xsize = x_grid;
   ysize = y_grid;
   
   xmax = (xmin+xsize*xstep)-1;
   ymax = (ymin+ysize*ystep)-1;
 
   Matrix tempm(xsize, ysize);   // used for initialising and testing
   scalar_fill(tempm, 0.0);      // used for initialising and testing
   Matrix blank_matrix = tempm;
   create_axis_vector();
}

//----------------------------------------------------------------\
//  This creates two vectors that are the axis values of a matrx  \
//----------------------------------------------------------------/
void Space::create_axis_vector()
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
   xvector = xvect;
   yvector = yvect;
}
