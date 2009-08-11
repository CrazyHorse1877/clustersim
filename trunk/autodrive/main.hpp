#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <GL/glut.h>
#include <iostream>
#include "grid_pdf.hpp"
#include "object_data.hpp"
#include "move_vehicles.hpp"
//#include "gnuplot_i.hpp"
#include "fgcontroller.hpp"
//#include "opengl_3d.hpp"

using namespace std;

 #define GRIDX 100
 #define GRIDY 100
 #define XMIN  -50
 #define YMIN  -50
 #define STEP  1

class Vehicle;

class MainClass
{
  int x, y;
public:

  Gnuplot plot;  
  Vehicle *vehicleobject;
  GridPDF *gridobject;

  void grid_init (void); 
  int run_grid (double, Vector &pdfdata, Vector &camera_pos);
  void user_input (void);

};

#endif
