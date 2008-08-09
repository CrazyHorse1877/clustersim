////////////////////////////////////////////
//
// process_grid.hpp
//
// Bayesian Search and Rescue Simulator 
//
// This program uses grid based, recursive baysian 
// filter techniques to cooperativly find and track
// multiple targets with multiple UAV's
//
// Copyright (c) Phil Sammons
// UNIVERSITY OF NEW SOUTH WALES (Sydney, Australia)
// 
//-----------------------------------------
// To compile this program using CMake
// enter the following into the cmakeLists.txt file
//
// PROJECT (bayes)
// ADD_EXECUTABLE(multisearch multisearch.cpp grid_pdf.cpp gnuplot_i.cc object_data.cpp)
// SET(SPASM_LIBS spasmfilters spasm lapack gsl)
// TARGET_LINK_LIBRARIES( multisearch ${SPASM_LIBS})
// LINK_DIRECTORIES(/usr/local/lib /usr/lib/atlas)
// 
////////////////////////////////////////////

#ifndef _PROCESS_GRID_HPP_
#define _PROCESS_GRID_HPP_

#include <stdexcept>
#include "gnuplot_i.hpp"
//#include "read_file.hpp"
//#include "write_file.hpp"
#include <sys/stat.h>
#include "grid_pdf.hpp"
#include "object_data.hpp"
#include "move_vehicles.hpp"

// USER DEFINED GRID SIZE
// This will be changed later to allow dynamic changes to grid size
 #define GRIDX 100
 #define GRIDY 100
 #define XMIN  -50
 #define YMIN  -50
 #define STEP  1

//Retain ANSI/ISO Compability
#ifdef WIN32
#include <windows.h>
#endif

using namespace std;
//using namespace ulapack;

class Process_Grid : public GridPDF 
{

private: 
	GridPDF gridobject;
        Vehicle vehicleobject;
	Gnuplot plot;

public: 
  Process_Grid();
  Process_Grid(int time);
  ~Process_Grid();
  
  void read_vehicles(void);
  void read_targets(void);
  void grid_init(void);
  void run_grid(int);
};

#endif
