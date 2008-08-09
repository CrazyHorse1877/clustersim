////////////////////////////////////////////
//
// process_grid.cpp
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
// ADD_EXECUTABLE(multisearch multisearch.cpp grid_pdf.cpp 
//               gnuplot_i.cc object_data.cpp move_vehicles.cpp)
// SET(SPASM_LIBS spasmfilters spasm lapack gsl)
// TARGET_LINK_LIBRARIES( multisearch ${SPASM_LIBS})
// LINK_DIRECTORIES(/usr/local/lib /usr/lib/atlas)
// 
////////////////////////////////////////////

//#include "read_file.hpp"
#include "write_file.hpp"
//#include <sys/stat.h>
//#include "grid_pdf.hpp"
//#include "object_data.hpp"
//#include "terrain.cpp"
#include "process_grid.hpp"


// USER DEFINED GRID SIZE
// This will be changed later to allow dynamic changes to grid size
//#define GRIDX 100
//#define GRIDY 100
//#define XMIN  -50
//#define YMIN  -50
//#define STEP  1

//using namespace ulapack;

Process_Grid::Process_Grid(void){}
Process_Grid::~Process_Grid(){return;}

void Process_Grid::grid_init(void)
{
  //  static bool run=0;
  //if(run==0)
  //  {
  for(int i = 0 ; i < 5 ; i ++)
    {
      gridobject._VD[i] = new Object_Data[i];
      gridobject._SD[i] = new Object_Data[i];
      gridobject._TD[i] = new Object_Data[i];
      gridobject._PD[i] = new Object_Data[i];
    }
  gridobject._GD = new Object_Data;

#if 0     
  cout << endl << "Hello, The simulator will begin soon." << endl;
  
  int input=2;
  cout << "Do you want to use old target data? "; 	
  cin >> input;	
  
  switch(input)
    {
    case 0 : write_targets(); break;   // PROMPT TO ENTER TARGET DATA
    case 1 : cout << "OK!" << endl; break;
    default: cout << "Input not understood, using old data"	<< endl;
    }
  
  input = 2;  
  cout << "Do you want to use old vehicle data? "; 
  cin >> input;
  
  switch(input)
    {
    case 0 : write_vehicles(); break;  // PROMPT TO ENTER TARGET DATA
    case 1 : cout << "OK!" << endl; break;
    default: cout << "Input not understood, using old data"	<< endl;
    }
#endif
  // read_vehicles(gridobject._VD);//get initial vehicle locations & speeds
  //read_targets(gridobject._TD);  //get initial target locations & speeds
  
  read_targets();  //get initial target locations & speeds
  read_vehicles(); //get initial vehicle locations & speeds

  // SETUP GLOBAL SPACE
  *gridobject._GD = gridobject->_TD[0];
  gridobject._GD->make_space(XMIN, YMIN, GRIDX, GRIDY, STEP);

  cout << endl << "Vehicle " << gridobject._VD[0]->numObjects << endl;
  cout <<         "Sensor  " << gridobject._SD[0]->numObjects << endl;
  cout <<         "Target  " << gridobject._TD[0]->numObjects << endl;
  cout <<         "Predict " << gridobject._PD[0]->numObjects << endl << endl;
  
  // SETUP CONVOLOUTION SETTINGS
  // XXX - function to calculate motion model
  // based on user inputs of wind and current estimates
  for(int i = 0; i < gridobject._TD[0]->numObjects ; i++)
    {
      gridobject._PD[i] = .gridobject._TD[i];
      gridobject._PD[i]->vx = 0.2;// Amount of prediction blur
      gridobject._PD[i]->vy = 0.2;
      gridobject._PD[i]->mx = 0;//-gridobject._TD[i].v0;// add wind
      gridobject._PD[i]->my = 0;
    }
  
  // SETUP TARGET SPACE
  for(int target = 0; target < gridobject._TD[0]->numObjects ; target++)
    gridobject._TD[target]->make_space(-50, -50, 100, 100, STEP);
  
  // CREATE PREDICTION SPACE
  for(int target = 0; target < gridobject._TD[0]->numObjects ; target++)
    gridobject._PD[target]->make_space(-1, -1, 3, 3, 1);	
  
  // CREATE VEHICLE SPACE
  for(int vehicle = 0; vehicle < gridobject._VD[0]->numObjects ; vehicle++)
    {
      gridobject._VD[vehicle]->make_space(XMIN, YMIN, GRIDX, GRIDY, STEP);
      gridobject._SD[vehicle] = gridobject._VD[vehicle];
    }
  
  // CREATE SENSOR SPACE
  for(int sensor = 0; sensor < gridobject._VD[0]->numObjects ; sensor++)
    gridobject._SD[sensor]->make_space(-5, -5, 11, 11, 1);

  
  // SETUP FILTER
  //gridobject.enter_data(gridobject._VD, gridobject._SD, gridobject._TD,
  //		    gridobject._PD, gridobject._GD);	
  
  // START MAIN PLOT AND PLOT GLOBAL PDF
  //------------------------------------
  gridobject.make_global();
  
  //plot.plot_xyz(gridobject._GD.pdf);//usleep(100000);
  //plot.plot_xyz(gridobject._GD.global_target);
  
  //      run = 1;
  //}
}	

//---------------------------------------------------
//          ALGORITHM IMPLEMTATION CONTROL
// --------------------------------------------------	
void Process_Grid::run_grid(int time)
{
  static int count;
  //static Gnuplot plot;
  
  //MOVE VEHICLES
  //vehicleobject.move_with_lookahead();//multistep lookahead
  //vehicleobject.move_vehicles();//single step lookahead


  //UPDATE TARGETS WITH SENSOR OBSERVATIONS
  //gridobject.update();
  
  //PREDICT t+1 TARGET POSITIONS
  //gridobject.predict();
  
  //UPDATE SIMULATED TARGET LOCATIONS
  //gridobject.move_targets();
  
  //MAKE GLOBAL TARGET PDF FOR VIEWING
  //gridobject.make_global();
  
  //SCALE DATA AND WRITE TO FILE FOR VIEWING
  //gridobject.write_global();
  
  //PLOT GLOBAL TARGET PDF
  /*if(time % 5 == 0)
    {
       plot.reset_plot();
       plot.plot_xyz(gridobject._TD[0].pdf);usleep(400000);
       plot.plot_xyz(gridobject._GD.global_target);usleep(400000);
       plot.plot_xyz(gridobject._GD.pdf);usleep(400000);
    }*/
  cout << count << endl;
  count++;
}

void Process_Grid::read_vehicles(void)
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
      gridobject._VD[vehicle]->numObjects = nvehicles;
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(x0, 10, ',');
      gridobject._VD[vehicle]->x0 = atof(x0);
      VehicleData.getline(y0, 10, '\n');
      gridobject._VD[vehicle]->y0 = atof(y0);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(z0, 10, '\n');
      gridobject._VD[vehicle]->z0 = atof(z0);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(th0, 10, '\n');
      gridobject._VD[vehicle]->th0 = atof(th0);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(v0, 10, '\n');
      gridobject._VD[vehicle]->v0 = atof(v0);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(vmin, 10, '\n');
      gridobject._VD[vehicle]->vmin = atof(vmin);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(vmax, 10, '\n');
      gridobject._VD[vehicle]->vmax = atof(vmax);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(object_mass, 10, '\n');
      gridobject._VD[vehicle]->object_mass = atof(object_mass);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(tminv, 10, '\n');
      gridobject._VD[vehicle]->tminv = atof(tminv);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(tmaxv, 10, '\n');
      gridobject._VD[vehicle]->tmaxv = atof(tmaxv);
      VehicleData.getline(blankblank, 200, ':');
      VehicleData.getline(vx, 10, ',');
      gridobject._VD[vehicle]->vx = atof(vx);
      VehicleData.getline(vy, 10, '\n');
      gridobject._VD[vehicle]->vy = atof(vy);
      gridobject._VD[vehicle]->mx = 0.0;
      gridobject._VD[vehicle]->my = 0.0;
      
      cout << "\n -=- Vehicle " << vehicle+1 << " -=-";
      cout << "\nStart Point: [" << gridobject._VD[vehicle]->x0 
	   << ", " << gridobject._VD[vehicle]->y0 << "]";
      cout << "\nStart Angle: " << gridobject._VD[vehicle]->th0 ;
      cout << "\nStart Velocity: " << gridobject._VD[vehicle]->v0 ;
      cout << "\nMinimum Velocity: " << gridobject._VD[vehicle]->vmin ;
      cout << "\nMaximum Velocity: " << gridobject._VD[vehicle]->vmax ;
      cout << "\nMass: " << gridobject._VD[vehicle]->object_mass ;
      cout << "\nSteering at Minimun Velocity:  " 
	   << gridobject._VD[vehicle]->tminv ;
      cout << "\nSteering at Maximum Velocity:  " 
	   << gridobject._VD[vehicle]->tmaxv << endl;
      cout << endl;
    }
}

void Process_Grid::read_targets(void)
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
  cout << "num targets" << ntargets << endl;
  
  for(int target = 0 ; target < ntargets ; target ++)
    {
      gridobject._TD[target]->numObjects = ntargets;
      targetData.getline(blankblank, 200, ':');
      targetData.getline(x0, 10, ',');
      gridobject._TD[target]->x0 = atof(x0);
      targetData.getline(y0, 10, '\n');
      gridobject._TD[target]->y0 = atof(y0);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(z0, 10, '\n');
      gridobject._TD[target]->z0 = atof(z0);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(th0, 10, '\n');
      gridobject._TD[target]->th0 = atof(th0);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(v0, 10, '\n');
      gridobject._TD[target]->v0 = atof(v0);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(vmin, 10, '\n');
      gridobject._TD[target]->vmin = atof(vmin);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(vmax, 10, '\n');
      gridobject._TD[target]->vmax = atof(vmax);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(object_mass, 10, '\n');
      gridobject._TD[target]->object_mass= atof(object_mass);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(tminv, 10, '\n');
      gridobject._TD[target]->tminv = atof(tminv);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(tmaxv, 10, '\n');
      gridobject._TD[target]->tmaxv = atof(tmaxv);
      targetData.getline(blankblank, 200, ':');
      targetData.getline(vx, 10, ',');
      gridobject._TD[target]->vx = atof(vx);
      targetData.getline(vy, 10, '\n');
      gridobject._TD[target]->vy = atof(vy);
      gridobject._TD[target]->mx = atof(x0);
      gridobject._TD[target]->my = atof(y0);
      
      cout << "\n -=- target " << target+1 << " -=-";
      cout << "\nStart Point: [" << gridobject._TD[target]->x0 
	   << ", " << gridobject._TD[target]->y0 << "]";
      cout << "\nStart Angle: " << gridobject._TD[target]->th0 ;
      cout << "\nStart Velocity: " << gridobject._TD[target]->v0 ;
      cout << "\nMinimum Velocity: " << gridobject._TD[target]->vmin ;
      cout << "\nMaximum Velocity: " << gridobject._TD[target]->vmax ;
      cout << "\nMass: " << gridobject._TD[target]->object_mass ;
      cout << "\nSteering at Minimun Velocity:  " 
	   << gridobject._TD[target]->tminv ;
      cout << "\nSteering at Maximum Velocity:  " 
	   << gridobject._TD[target]->tmaxv << endl;		
      cout << "\nVariance:  " << gridobject._TD[target]->vx << endl;
    }
}
