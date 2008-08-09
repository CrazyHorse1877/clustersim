#include "main.hpp"
#include "write_file.hpp"
#include "read_file.hpp"
#include <vector>

//#include <iostream>
using namespace std;

#define	lenghtXmnt	62
#define lenghtYmnt	62

#define stepXmnt     96
#define stepYmnt     96

vector<double> path;

void MainClass::grid_init(void)
{
  gridobject = new GridPDF;
  vehicleobject = new Vehicle;
  
#if 0     
   //  allow this to use wizard to enter data values
   user_input();
#endif

   
   read_vehicles(gridobject->VD);//get initial vehicle locations & speeds
   //read_vehicle_positions(gridobject->VD);
   read_targets(gridobject->TD);  //get initial target locations & speeds
   
   // SETUP GLOBAL SPACE
   *gridobject->GD = gridobject->TD[0];
   gridobject->GD->make_space(XMIN, YMIN, GRIDX, GRIDY, STEP);
   
   // SETUP CONVOLOUTION SETTINGS
   // XXX - function to calculate motion model
   // based on user inputs of wind and current estimates
   for(int i = 0; i < gridobject->TD[0].numObjects ; i++){
      gridobject->PD[i] = gridobject->TD[i];
      gridobject->PD[i].vx = 0.08;// Amount of prediction blur
      gridobject->PD[i].vy = 0.08;
      gridobject->PD[i].mx = 0;//+TD[i].v0;// add wind
      gridobject->PD[i].my = 0;
   }
   
   // SETUP TARGET SPACE
   for(int target = 0; target < gridobject->TD[0].numObjects ; target++)
     gridobject->TD[target].make_space(-50, -50, 100, 100, STEP);
  
   // CREATE PREDICTION SPACE
   for(int target = 0; target < gridobject->TD[0].numObjects ; target++)
    gridobject->PD[target].make_space(-1, -1, 3, 3, 1);	
   
  // CREATE VEHICLE SPACE
  for(int vehicle = 0; vehicle < gridobject->VD[0].numObjects ; vehicle++){
      gridobject->VD[vehicle].make_space(XMIN, YMIN, GRIDX, GRIDY, STEP);
      gridobject->SD[vehicle] = gridobject->VD[vehicle];
    }
  
  // CREATE SENSOR SPACE
  for(int sensor = 0; sensor < gridobject->VD[0].numObjects ; sensor++)
    gridobject->SD[sensor].make_space(-5, -5, 11, 11, 1);
  
  // SETUP FILTER
  gridobject->set_init_values();
  
  // START MAIN PLOT AND PLOT GLOBAL PDF
  //------------------------------------
 
   // char *hostname = "192.168.0.50";
   char *hostname = "localhost";
   int port = 5400;
   
   //FGFSSocket uav1(hostname, port);
   //uav1.setup();
   
   //usleep(100000);
   
   cout << "Initialised " << endl;
}

int MainClass::run_grid(double time, Vector &pdfdata, Vector &camera_pos)
{
  static int frame;
  gridobject->all_targets_found(); 

 //MOVE VEHICLES
  //MULTISTEP LOOKAHEAD
  //vehicleobject->move_with_lookahead(gridobject, time);

  //DYNAMIC SINGLE STEP LOOKAHEAD
  vehicleobject->move_vehicles(*gridobject, time);

  //UPDATE TARGETS WITH SENSOR OBSERVATIONS
  gridobject->update();

  //PREDICT t+1 TARGET POSITIONS
  gridobject->predict();

  //UPDATE SIMULATED TARGET LOCATIONS
  //gridobject->move_targets();
  
  //MAKE GLOBAL TARGET PDF FOR VIEWING
  gridobject->make_global(pdfdata);

  //SCALE DATA AND WRITE TO FILE FOR VIEWING (not necessary right now)
  //gridobject->write_global();
  
   
   //PLOT GLOBAL TARGET PDF
   if(frame % 30 == 0)
     {
	//FGFSSocket uav1("localhost", 5400);
	//uav1.update(*gridobject);

	//vehicleobject->read_vehicle_position(uav1, 0, *gridobject, time);
	 	     
	//uav1.write("set /autopilot/route-manager/input @insert-1:151.17,-33.94@%300");//, gridobject->VD[0].gps_long,gridobject->VD[0].gps_lat,100);
	//uav1.write("set /autopilot/settings/true-heading-deg %f", gridobject->VD[0].th0);
	//uav1.write("set /orientation/heading-magnetic-deg %f", gridobject->VD[0].th0);
	//uav1.write("set /orientation/roll-deg %f", gridobject->VD[0].th0);
	//uav1.write("set /orientation/pitch-deg %f", gridobject->VD[0].th0);
	//uav1.write("set /positions/longitude-deg %f", 151.gridobject->VD[0].y0);
	//uav1.write("set /positions/latitude-deg %f", gridobject->VD[0].x0);
	
	path.push_back(gridobject->VD[0].x0);
	path.push_back(gridobject->VD[0].y0);
	//plot.reset_plot();
	//plot.plot_xyz(gridobject->TD[0].pdf);usleep(600000);
	//plot.reset_plot();
	//plot.plot_xyz(gridobject->TD[1].pdf);usleep(600000);
	//plot.reset_plot();
	//plot.plot_xyz(gridobject->TD[2].pdf);usleep(600000);
	//plot.reset_plot();
	//plot.plot_xyz(gridobject->TD[3].pdf);usleep(600000);
	//plot.plot_xyz(gridobject->TD[1].pdf);usleep(400000);
	//plot.plot_xyz(gridobject->GD->global_target);usleep(400000);
	//plot.plot_xyz(gridobject->GD->pdf);usleep(400000);
	//cout << "time " << time << "  counter " << count << endl;  
     }
   frame++;//if(count==00)exit(1);
   //usleep(1000000);
   

   
   
  // COMMUNICATION WITH BAYSIAN FILTER
  // SET #IF value TO "0" TO ENABLE MANUAL FLYING
  // OR IF CONNECTING TO FLIGHTGEAR
  //getchar();   
#if 1
  camera_pos(0) = gridobject->VD[0].x0;
  camera_pos(1) = gridobject->VD[0].y0;
  camera_pos(2) = gridobject->VD[0].z0;
  camera_pos(3) = gridobject->VD[0].th0;  // alpha  from vertical
  //cout << "th0 " << camera_pos(3) << endl;  
  //camera_pos(4) = 140.0; // beta   from north
#else
  gridobject->VD[0].x0 = camera_pos(0) / (lenghtXmnt * stepXmnt)*100;
  gridobject->VD[0].y0 = camera_pos(1) / (lenghtYmnt * stepYmnt)*100;
  gridobject->VD[0].th0 = camera_pos(3) - 90;
#endif
  
   return 0;
}


void MainClass::user_input(void)
{
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
}

#if 0
int main(int ac, char **av)
{
  Gnuplot plot;
  Vector blank(256*256);
  cout << " Starting.... " << endl;
  TestClass a;
  cout << " Initialising.... " << endl;
  a.grid_init();
  cout << " Runnig grid processes .... " << endl;
  for(int i = 0 ; i < 100 ; i++)
    a.run_grid(0.0,blank);
  
  return 0;
}
#endif
