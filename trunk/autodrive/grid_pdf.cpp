////////////////////////////////////////////
//
// grid_pdf.cpp
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
// Dependant Files : grid_pdf.hpp
// 
////////////////////////////////////////////

#include "grid_pdf.hpp"
#include "gnuplot_i.hpp"

using namespace std;
//using namespace ulapack;

//class Vehicle;

GridPDF::GridPDF(void)
{

  VD = new Object_Data[5];
  SD = new Object_Data[5];
  TD = new Object_Data[5];
  PD = new Object_Data[5];
  
  GD = new Object_Data;

  //  for(int i = 0 ; i < 5 ; i ++)
  // {
  //  _VD[i] = new Object_Data[i];
  //  _SD[i] = new Object_Data[i];
  //  _TD[i] = new Object_Data[i];
  //  _PD[i] = new Object_Data[i];
  // }
  //_GD = new Object_Data;
  return;
}
GridPDF::~GridPDF(void){return;}

//----------------------------------------------------------------	\
//        MAKEGAUSS - CREATE A 2D GAUSIAN PDF GIVEN MEAN AND VARs	\
//----------------------------------------------------------------/
Matrix GridPDF::makegauss(Object_Data OD)
{
  Matrix XY_pdf(OD.xsize,OD.ysize);
  double K = 2.0*M_PI*sqrt((OD.vx)*(OD.vy));
  for(int j=0; j < OD.ysize ; j++)
    for(int i=0; i < OD.xsize ; i++)
      XY_pdf(i,j) = exp(( gsl_pow_2(OD.xvector(i)-(OD.mx)) /
     (OD.vx) + gsl_pow_2(OD.yvector(j)-(OD.my)) /(OD.vy)) / -2.0) /K;
  return XY_pdf;
}

//----------------------------------------------------------------	\
  //      PREDICT // APPLY CONVOLUTION					\
//----------------------------------------------------------------/
  void GridPDF::predict(void)
{
  for(int target = 0 ; target < TD[0].numObjects ; target++ )
    {
      if(TD[target].target_aquired == 0)
	{
	  //SAVE PRIOR
	  TD[target].prior = TD[target].pdf;
	  
	  //APPLY PREDICTION MODEL
	  for(int j = 0 ; j < TD[target].ysize ; j++)
	    for(int i = 0 ; i < TD[target].xsize ; i++)
	      times(TD[target], i, j, PD[target].pdf,
		    TD[target].prior, TD[target].pdf);
	  
	  //NORMALISE
	  normalise(TD[target].pdf);
	}
    }
}

//----------------------------------------------------------------	\
// TIMES - MULTIPLY ELEMENTS OF A & B @ B(X,Y) AND SET VALUE		\
//----------------------------------------------------------------/
void GridPDF::times(Object_Data &OD, int xi, int yi,
		    Matrix &A, Matrix &B , Matrix &predicted)
{
  int axsize = A.size1();
  int aysize = A.size2();   
  int x = xi - (int)((axsize)/2);
  int y = yi - (int)((aysize)/2);
  double sum = 0;
  
  for(int j = 0 ; j < aysize ; j++)
    {
      for(int i = 0 ; i < axsize ; i++)
	{
	  //MAKE SURE POINT IS VALID
	  if( x < OD.xsize && x >= 0 && y < OD.ysize && y >= 0)
	    if(B(x,y) > 0) sum = sum + (B(x,y) * A(i,j));//MULTIPLY
	  x++;
	}
      y++;
      x = xi - (int)((axsize)/2);
    }
  predicted(xi,yi) = sum;
}

//----------------------------------------------------------------	\
//     UPDATE     // multiply and normalise prior with observation	\
//----------------------------------------------------------------/
void GridPDF::update(void)
{
  for(int target = 0 ; target < TD[0].numObjects ; target++ )
    {
      if(TD[target].target_aquired == 0)
	{
	  for(int vehicle = 0 ; vehicle < VD[0].numObjects ; vehicle++ )
	    {
	      // Simulated target detection range
	      double range = 10;//2 * sqrt(SD[vehicle].vy);
	      
	      // Distance to simulated target
	      double distance = sqrt(gsl_pow_2(VD[vehicle].x0-
	       TD[target].x0)+gsl_pow_2(VD[vehicle].y0-TD[target].y0));
	      //
	      // IF TARGET NOT DETECTED, CONTINUE SEARCHING
	      //
	      if(target_within_range(TD[target], target, distance, range)==0)
		{
		  //cout << "TWR" << endl; getchar();
		  if(TD[target].tracking_target == 0) // >>>>>SEARCHING<<<<<
		    {
		      //cout << "." ;
		      // APPLY COMPLEMENT OBSERVATION TO TARGETS
		      add_info(VD[vehicle].x0 - TD[target].xmin,
			       VD[vehicle].y0 - TD[target].ymin,
			       SD[vehicle].complement, TD[target].pdf);
		    }
		}
	      else // >>>>>TRACKING<<<<<
		{
		  //cout << "tracking" << endl;
		  srand ( time(NULL) );
		  int x1 = rand()%10 - rand()%10;
		  int y1 = rand()%10 - rand()%10;
		  
		  //TD[target].mx = TD[target].x0;// + x1;
		  //TD[target].my = TD[target].y0;// + y1;
		  //VD[vehicle].vy = 20;VD[vehicle].vy = 20;
		  //TD[target].pdf = makegauss(TD[target]);
		  
		  TD[target].mx = TD[target].x0 + x1;
		  TD[target].my = TD[target].y0 + y1;
		  //uncertanty in detection location
		  
		  TD[target].vx = 15*distance;
		  TD[target].vy = 15*distance;
		  SD[vehicle].pdf = makegauss(TD[target]);
		  
		  cout <<  "target distance  "  << TD[target].vx << endl;
		   
		  //Gnuplot p;
		  //p.plot_xyz(SD[vehicle].pdf);
		  //p.plot_xyz(TD[target].pdf);getchar();		
		  
		  //UPDATE TARGET PDF
		  add_info(TD[target].xsize/2, TD[target].ysize/2,
			   SD[vehicle].pdf, TD[target].pdf);
		  
		  normalise(TD[target].pdf);
		}
	    }
	}
    }
}

//--------------------------------------------------------------------	\
// ADD_INFO - MULTIPLY ELEMENTS IN MATRIX A TO B @ B(centerX,centerY)	\
//--------------------------------------------------------------------/
void GridPDF::add_info(double centerx, double centery, Matrix &A, Matrix &B)
{
  int axsize = A.size1();
  int aysize = A.size2();   
  int bxsize = B.size1();
  int bysize = B.size2();   
  int x = (int)(centerx - (axsize)/2);
  int y = (int)(centery - (aysize)/2);
  
  for(int j = 0 ; j < aysize ; j++)
    {
      for(int i = 0 ; i < axsize ; i++)
	{
	  //MAKE SURE POINT IS VALID
	  if( x < bxsize &&  x >=0 && y < bysize && y >=0)
	    if(B(x,y) > 0) B(x,y) = B(x,y) * A(i,j);//MULTIPLY
	  x++;
	}
      y++;
      x = (int)(centerx - ((axsize)/2));
    }
}

//----------------------------------------------------------------	\
// PLUS_INFO - ADD ELEMENTS IN MATRIX A TO B @ B(centerX,centerY)	\
//----------------------------------------------------------------/
void GridPDF::plus_info(double centerx, double centery, Matrix &A, Matrix &B)
{
  int axsize = A.size1();
  int aysize = A.size2();   
  int bxsize = B.size1();
  int bysize = B.size2();   
  int x = (int)(centerx - (axsize)/2);
  int y = (int)(centery - (aysize)/2);
  
  for(int j = 0 ; j < aysize ; j++)
    {
      for(int i = 0 ; i < axsize ; i++)
	{
	  //MAKE SURE POINT IS VALID
	  if( x < bxsize &&  x >=0 && y < bysize && y >=0)
	    B(x,y) = B(x,y) + A(i,j);
	  x++;
	}
      y++;
      x = (int)(centerx - ((axsize)/2));
    }
}

//----------------------------------------------------------------	\
//     COMPLEMENT - make a complement from observation likelyhood	\
//----------------------------------------------------------------/
void GridPDF::get_complement(Matrix &observation, Matrix &complement)
{
  double value = observation(0,0);
  Matrix m(observation.size1(),observation.size2());
  scalar_fill(m,1.0);
  
  // NORMALISE TO 1
  
  for(int j = 0 ; j < observation.size1() ; j++)
    for(int i = 0 ; i < observation.size2() ; i++)
      if(observation(i,j) > value) value = observation(i,j);
  for(int j = 0 ; j < observation.size1() ; j++)
    for(int i = 0 ; i < observation.size2() ; i++)
      complement(i,j) = observation(i,j) / value;
  // TURN PDF UPDSIDE DOWN
  complement = (m - complement);
}

//----------------------------------------------------------------	\
//         INTEGRATE A PDF						\
//----------------------------------------------------------------/
double GridPDF::integratePdf(Matrix &matrix)
{
  double sum = 0;
  int x = matrix.size1();
  int y = matrix.size2();
  for(int j = 0; j < y ; j ++)
    for(int i = 0; i < x ; i ++){
      if(matrix(i,j)<0);   //ONLY SUM POSITIVE VALUES
      else sum = sum + matrix(i,j);
    }
  return sum;
}


//----------------------------------------------------------------	\
//      NORMALISE A PDF : Option 1					\
//----------------------------------------------------------------/
void GridPDF::normalise(Matrix &matrix)
{
  int x = matrix.size1();
  int y = matrix.size2();
  double sum = integratePdf(matrix);
  
  for(int j = 0; j < y ; j ++)
    for(int i = 0; i < x ; i ++){	  
      if((matrix(i,j))>0){matrix(i,j) = matrix(i,j)/sum;}  //NORMALISE
      else matrix(i,j) = 0;
    }
}


//----------------------------------------------------------------	\
//  UPDATE VEHICLE WORLD						\
//----------------------------------------------------------------/
void GridPDF::vehicle_init(void)
{
  // MAKE VEHICLE OBSERVATION LIKELYHOODS
  for(int i = 0; i < VD[0].numObjects ; i++)
    {
      VD[i].pdf = makegauss(VD[i]);
      SD[i].pdf = makegauss(SD[i]);
      
      //MAKE COMPLEMENT OBSERVATIONS
      get_complement(SD[i].pdf, SD[i].complement);
    }
}

//----------------------------------------------------------------	\
//   UPDATE TARGET WORLD						\
//----------------------------------------------------------------/
void GridPDF::target_init(void)
{
  //   #define RAND_MAX = 20;
  srand ( time(NULL) );
  
  //SETUP TARGETS
  for(int target = 0; target < TD[0].numObjects ; target++)
    {
      //SETUP PRIOR USING GAUSSIAN MODEL
      TD[target].pdf = makegauss(TD[target]);	
      
      //SETUP TARGET MOTION MODEL
      PD[target].pdf = makegauss(PD[target]);
      
      int x1 = rand()%10 - rand()%10;	
      int y1 = rand()%10 - rand()%10;
      TD[target].x0 = TD[target].x0 + (TD[target].vx/100)*x1;
      TD[target].y0 = TD[target].y0 + (TD[target].vy/100)*y1;
      
      //TARGET EXACT LOCATION KNOWN/UNKNOWN
      TD[target].target_aquired = 0;
      TD[target].tracking_target = 0;
    }
}

void GridPDF::make_global(Vector &pdfdata)
{
  GD->global_target = GD->blank_matrix;
  //GD->pdf = GD->blank_matrix;
  
  //SHOW PDF
  for(int target = 0; target < TD[0].numObjects ; target++)
    if(TD[target].target_aquired == 0)
      GD->global_target += TD[target].pdf;
  write_global(GD->global_target, pdfdata);
  
  //GD->pdf = GD->global_target + GD->pdf;
  
  //SHOW TARGETS
  for(int target = 0; target < TD[0].numObjects ; target++)
    if(TD[target].target_aquired == 0)
      if( (int)(int)(TD[target].x0-GD->xmin) < GD->xsize &&
	  (int)(TD[target].x0-GD->xmin) >=0 && (int)(TD[target].y0-GD->ymin)
	  < GD->ysize && (int)(TD[target].y0-GD->ymin) >=0)// validate point
	GD->pdf((int)(TD[target].x0-GD->xmin),
		(int)(TD[target].y0-GD->ymin)) = 0.004; 	
  
  //SHOW VEHICLES
  for(int vehicle = 0; vehicle < VD[0].numObjects ; vehicle++)
    {	
      if( (int)(int)(VD[vehicle].x0-GD->xmin) < GD->xsize &&
	  (int)(VD[vehicle].x0-GD->xmin) >=0 && (int)(VD[vehicle].y0-GD->ymin)
	  < GD->ysize && (int)(VD[vehicle].y0-GD->ymin) >=0)// validate point
	GD->pdf((int)(VD[vehicle].x0-GD->xmin),
		(int)(VD[vehicle].y0-GD->ymin)) = 0.003;	
    }
}

void GridPDF::set_init_values(void)
//Object_Data &VD, Object_Data &SD, Object_Data &TD, 
//Object_Data &PD, Object_Data &GD)
{
  //  for(int i=0; i < 5 ; i++)
  //  {
  //    _VD = &VD;
  //    _SD = &SD;
  //    _TD = &TD;
  //    _PD = &PD;
  //  }
  //_GD = &GD;
  
  vehicle_init();
  target_init();
}

void GridPDF::all_targets_found(void)
{
  int aquired = 0;
  for(int target = 0; target < TD[0].numObjects; target++)
    if(TD[target].target_aquired == 1) aquired +=TD[target].target_aquired;
  if(aquired ==  TD[0].numObjects) 
    {
      cout << "GAME OVER" << endl;
      sleep(3);
      exit(1);
    }
}

bool GridPDF::target_within_range(Object_Data &OD, int target_num,
				  double distance, double range)
{
  // SIMULATE TARGET BEING CAPTURED
  if(distance < 0.2 * range || OD.x0 >= OD.xmax || 
     OD.x0 <= OD.xmin || OD.y0 >= OD.ymax || OD.y0 <= OD.ymin)
    {
      OD.target_aquired = 1;
      OD.pdf = OD.blank_matrix;
      cout << "TARGET " << target_num << " FOUND" << endl;
    }

  // CHECK IF TARGET WITHIN RANGE OF SENSOR
  if(distance < 2 * range || OD.x0 >= OD.xmax ||
     OD.x0 <= OD.xmin || OD.y0 >= OD.ymax || OD.y0 <= OD.ymin)
    {
      OD.tracking_target = 1;
      return 1;	
    }
  else
    {
      OD.tracking_target = 0;
      return 0;
    }
}


//----------------------------------------------------------------	\
//  MOVE TARGETS							\
//----------------------------------------------------------------/
void GridPDF::move_targets(void)
{
  for(int target = 0 ; target < TD[0].numObjects ; target++ )
    {
      srand ( time(NULL) );
      double x1 = ((rand()%10)/50.9) - ((rand()%10)/50.9);
      double y1 = ((rand()%10)/40.9) - ((rand()%10)/40.9);
      
      if(TD[target].target_aquired == 0)
	{
	  for(int vehicle = 0 ; vehicle < VD[0].numObjects ; vehicle++ )
	    {
	      double distance = sqrt(gsl_pow_2(VD[vehicle].x0-TD[target].x0)
			     +gsl_pow_2(VD[vehicle].y0-TD[target].y0));
	      if(distance < 7)
		{
		  //_TD[target].v0 = 0.6;
		  double x = VD[vehicle].x0 - TD[target].x0;
		  double y = VD[vehicle].y0 - TD[target].y0;
		  
		  double theta;
		  if(x==0)
		    {
		      if(y > 0) theta = 90;
		      else theta = -90;
		    }		       
		  else if(x < 0)		    
		    {
		      if(y < 0) theta = -180 + atan(y/x)*(180/M_PI);
		      else      theta =  180 + atan(y/x)*(180/M_PI); 	 
		    }
		  else          theta =        atan(y/x)*(180/M_PI);
		  
		  TD[target].x0 -= ( cos(theta*(M_PI/180)) * TD[target].v0);
		  TD[target].y0 -= ( sin(theta*(M_PI/180)) * TD[target].v0);
		  
		}
	    }
	}
      TD[target].x0 += TD[target].v0 + x1;
      TD[target].y0 += y1;
      
    }
}

//----------------------------------------------------------------	\
//    WRITE DATA							\
//----------------------------------------------------------------/
void GridPDF::write_global(Matrix &global, Vector &pdfdata)
{
  int x = global.size1();
  int y = global.size2();
  int spot = 0;
  Matrix random(256,256);

  for(int j = 0; j < 256 ; j++)
    for(int i = 0; i < 256 ; i++)
      {
	//this scales the data from 100x100 to 256x256 
	pdfdata(spot) = (global((int)(i*(x/256.0)),(int)(j*(y/256.0))));//+128);
	spot++;
      }
}  

//scale_matrix(127,global);
//}


//----------------------------------------------------------------	\
//  SEND INFOMATION TO OTHER UAVS AND GROUND STATIONS			\
//----------------------------------------------------------------/
//void GridPDF::send_data(void)
//{
  
//}
  
//----------------------------------------------------------------	\
//      FIND MAXIMUM							\
//----------------------------------------------------------------/
double GridPDF::get_maximum(Matrix &matrix)
{
  double max = 0;
  int x = matrix.size1();
  int y = matrix.size2();
  for(int j = 0; j < y ; j ++)
    for(int i = 0; i < x ; i ++)
      {
	if(matrix(i,j) > max) max = matrix(i,j);
      }
  return max;
}


//----------------------------------------------------------------	\
//     SCALE PDF TO VALUE OF MAX					\
//----------------------------------------------------------------/
void GridPDF::scale_matrix(double max, Matrix &matrix)
{
  int x = matrix.size1();
  int y = matrix.size2();
  double sample_max = get_maximum(matrix); //GET SAMPLE MAX
  double scale = 0.1;
  scale = (max/sample_max);

  FILE * pFile;
  unsigned char c;
  pFile = fopen ("pdf.dat","w");
  if (pFile!=NULL)
    {
      
      for(int j = 0; j < y ; j ++)
	for(int i = 0; i < x ; i ++)
	  {matrix(i,j) = (matrix(i,j) * scale);} //SCALE TO MAX
      
      for(int j = 0; j < 256 ; j++)
	{
	  for(int i = 0; i < 256 ; i++)
	    {
	      c = (unsigned char)(matrix((int)(i*(100.0/256)),
					 (int)(j*(100.0/256)))+128);
	      fputc (c , pFile);
	    }
	}
      fclose (pFile);
    }  
}
