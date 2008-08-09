////////////////////////////////////////////
//
// grid_pdf.hpp
//
// Part of Bayesian Search and Rescue Simulator 
//
// This Class holds the functions that control the 
// grid based Bayesian filter
// 
// Copyright (c) Phil Sammons
// UNIVERSITY OF NEW SOUTH WALES (Sydney, Australia)
// 
// Parameters
//-----------------------------------------
// Dependant Libraries : ULapack, Spasm, GSL
// Dependant Files : object_data.hpp, space.hpp,
//                gnuplot_i.hpp, move_vehicles.hpp
// 
////////////////////////////////////////////

#ifndef _GRID_PDF_HPP_
#define _GRID_PDF_HPP_

//Retain ANSI/ISO Compability
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <stdlib.h>

#include <ulapack/matrix_types.hpp>
#include <ulapack/scalar_fill.hpp>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_math.h>

#include "object_data.hpp"
//#include "move_vehicles.hpp"

using namespace std;

class GridPDF//: public Object_Data
{
  
public:
    
  Object_Data *VD;
  Object_Data *SD;
  Object_Data *TD;
  Object_Data *PD;
  
  Object_Data *GD;
  
  //Vehicle vehicle;
  
  GridPDF();
  ~GridPDF();

  // PREDICT
  void predict(void);
  
  // UPDATE
  void update(void);
  
  // MOVE TARGETS
  void move_targets(void);
  
  //SET GLOBAL FOR PLOTTING
  void make_global(Vector &pdfdata);
  
  //SETUP FILTER
  void set_init_values(void);  
  //void set_init_values(Object_Data &VD, Object_Data &SD,
  //		       Object_Data &TD, Object_Data &PD,
  //		       Object_Data &GD);
  
  //check if game over
  void all_targets_found(void);
  
  //write data
  void write_global(Matrix &matrix, Vector &pdfdata);
  
private:
  
  // UPDATE VEHICLE WORLD
  void vehicle_init(void);
  
  // UPDATE TARGET WORLD
  void target_init(void);   
  
  // make grid and fill with gauss pdf
  Matrix makegauss(Object_Data OD);
  
  // TIMES - MULTIPLY ELEMENTS OF A & B @ B(X,Y) AND RETURN SUM
  void times(Object_Data &OD, int xcenter, int ycenter,
	     Matrix &A, Matrix &B, Matrix &predicted);
  
  // ADD A SMALL MATRIX TO A LARGER ONE
  void add_info( double centerx, double centery,
		 Matrix &SMALL, Matrix &LARGE);
  void plus_info(double centerx, double centery,
		 Matrix &SMALL, Matrix &LARGE);
  
  // CREATE COMPLEMENT SEARCH PDF
  void get_complement(Matrix &observation, Matrix &complement);
  
  // INTEGRATE A PDF
  double integratePdf(Matrix &matrix);
  
  // Normalise a PDF
  void normalise(Matrix &matrix);
  
  bool target_within_range(Object_Data &OD, int target,
			   double distance, double range);
  
  // Get Maximum
  double get_maximum(Matrix &matrix);
  
  // SCALE PDF TO VALUE OF MAX
  void scale_matrix(double max, Matrix &matrix);
  
  
};

#endif


