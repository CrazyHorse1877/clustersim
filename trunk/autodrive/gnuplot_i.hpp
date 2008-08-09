/*
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 */


////////////////////////////////////////
//
// A C++ interface to gnuplot.
//
// This file is derrived from the Gnuplot C interface
// written by N. Devillard and  Rajarshi Guha
// 3D support implemented by Phil Sammons
// original available from http://ndevilla.free.fr/gnuplot/
//
// As in the C interface this uses pipes and so wont
// run on a system that does'nt have POSIX pipe support
//
// 07/03/03
//
/////////////////////////////////////////

#ifndef _GNUPLOT_PIPES_H_
#define _GNUPLOT_PIPES_H_

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

#define GP_MAX_TMP_FILES    128
#define GP_TMP_NAME_SIZE    512
#define GP_CMD_SIZE         1024
#define GP_TITLE_SIZE       80

using namespace std;
using namespace ulapack;


class GnuplotException : public runtime_error
{
 public:
   GnuplotException(const string &msg) : runtime_error(msg){}
};

class Gnuplot
{
 private:
   FILE            *gnucmd;
   string           pstyle;
   vector<string>   to_delete;
   int              nplots;
   bool             get_program_path(const string);
   bool             valid;
 public:
   Gnuplot();
   
   // set a style during construction
   Gnuplot(const string &);
   
   // The equivilant of gnuplot_plot_once, the three forms
   // allow you to plot up to (x,y,z) pairs or just a single
   // vector at one go
   
   Gnuplot(const string &, // title
	   const string &, // style
	   const string &, // xlabel
	   const string &, // ylabel
	   const string &, // zlabel
	   ulapack::Matrix&);
   
   //   UNCOMMENT THIS TO ENABLE A SEPERATE XYZ VECTORS TO BE PASSED
   //   Gnuplot(const string &, // title
   //	   const string &, // style
   //	   const string &, // xlabel
   //	   const string &, // ylabel
   //	   const string &, // zlabel
   //	   vector<double>, vector<double>, vector<double>);
   
   Gnuplot(const string &, // title
	   const string &, // style
	   const string &, // xlabel
	   const string &, // ylabel
	   vector<double>, vector<double>);
   
   Gnuplot(const string &, // title
	   const string &, // style
	   const string &, // xlabel
	   const string &, // ylabel
	   vector<double>);
   
   ~Gnuplot();
   
   // send a command to gnuplot
   void cmd(const char*, ...);
   
   // set line style
   void set_style(const string &);
   
   // set y and x axis labels
   void set_ylabel(const string &);
   void set_xlabel(const string &);
   void set_zlabel(const string &);
   
   // plot a single vector
   void plot_x(vector<double>, // x
	       const string & // title
	       );
   
   // plot x,y pairs
   void plot_xy(vector<double>, // x
		vector<double>, // y
                const string  & // title
                );
   
   // plot x,y,z pairs
   void plot_xyz(ulapack::Matrix// ,
		 //const int &,
		 //const int &  // vehicle point
		 );
   
   //  void plot_xyz(vector<double>, // x
   //		 vector<double>, // y
   //		 vector<double>, // z
   //		 const string  & // title
   //		 );
   
   
   // plot an equation of the form: y = ax + b
   // You supply a and b
   void plot_slope(
		   double, // a
		   double, // b 
		   const string & // title
		   );
   
   // plot an equation supplied as a string
   void plot_equation(
		      const string &, // equation 
		      const string &  // title
		      );
   
   // if multiple plots are present it will clear the plot area
   void reset_plot(void);
   
   bool is_valid(void);
   
};

#endif
