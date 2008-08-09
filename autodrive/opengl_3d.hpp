/*
 * This program is under the GNU GPL.
 *  
 *  Some parts of this program were written by: 
 *  David Bucciarelli  (tech.hmw@plus.it) Humanware s.r.l.
 *  Mikael SkiZoWalker (Skizo@Hol.Fr)
 *
 * Otherwise coded by Phil Sammons (sammons@gmail.com)
 * University Of New South Wales (Sydney, Australia)
 */

#ifndef _OPENGL_3D_HPP_
#define _OPENGL_3D_HPP_

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ulapack/matrix_types.hpp>
#include <ulapack/scalar_fill.hpp>
//#include "grid_pdf.hpp"


#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glut.h>

#ifdef XMESA
#include "GL/xmesa.h"
static int fullscreen = 1;
#endif

#ifndef M_PI
#define M_PI 3.14159265
#endif

#define heightPDF	800
#define heightMnt	100
#define	lenghtXmnt	92
#define lenghtYmnt	92

#define stepXmnt     98
#define stepYmnt     98

#define WIDTH 640
#define HEIGHT 480

static GLint T0 = 0;
static GLint Frames = 0;

using namespace ulapack;

#define TSCALE 4
#define FOV 85

static GLfloat terrain[256 * 256];
static GLfloat pdfterrain[256 * 256];
static GLfloat terraincolor[256 * 256][3];
static GLfloat pdfterraincolor[256 * 256][3];

static int win = 0;

static int fog = 0;
static int bfcull = 1;
static int usetex = 1;
static int terrainline = 0;
static int pdfline = 0;
//static int markerline = 0;
static int showterrain = 1;
static int help = 0;
static int joyavailable = 0;
static int joyactive = 0;
static float ModZMnt;
static long GlobalMnt = 0;
static long GlobalX = 0;
static long GlobalY = 0;

static int scrwidth = WIDTH;
static int scrheight = HEIGHT;
static double boostTerrain = (heightMnt / 255.0f);
static double boostPDF = (heightPDF / 255.0f);

#define OBSSTARTX 0.0
#define OBSSTARTY -1000.0
#define OBSSTARTZ 100.0

static float obs[3] = { OBSSTARTX, OBSSTARTZ, OBSSTARTY };
static float dir[3], v1[2], v2[2];
static float v = 0.0;
static float alpha = -90.0;
static float beta = 175.0;
Vector camera_pos(5);
Vector pdfdata(256 * 256);

#include "main.hpp"
class MainClass;

MainClass bayesrun;

using namespace std;

//GETTIME - Calculate elapsed Time since start of program
static double gettime();

//CALCPOSOBS - Calculate Position of camera
static void calcposobs(void);

//RESHAPE - Create point of view
static void reshape(int width, int height);

//CLIPSTRIP - Calculate area not to show
static int clipstrip(float y, float *start, float *end);

//PRINTSTRING - Print text to the screen
static void printstring(void *font, char *string);

//PRINTHELP - Print a help meny to the screen
static void printhelp(void);

//DRAW TERRAIN - Actual creation of terrain and objects
static void drawterrain(void);

//DOJOY - Enable the use of a joystick
static void dojoy(void);

//DRAWSCENE - Actaul pushing of data to graphics handler
static void drawscene(void);

//KEY - Get keyboard input
static void key(unsigned char k, int x, int y);

//SPECIAL - Get control keys
static void special(int k, int x, int y);

//CALCCOLOR - Calulate the colours to render terrain
static void calccolor(GLfloat height, GLfloat c[3],bool type);

//LOADPIC - Load the terrain data from file and calculate 2D texture
static void loadpic(void);

//LOADPDF - Get the current pdf data
static void loadpdf(void);

//INIT - Initialise the OpenGL renderer and prepare program
static void init(void);

#endif
