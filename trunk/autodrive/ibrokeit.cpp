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

#include "opengl_3d.hpp"

//GETTIME - Calculate elapsed Time since start of program
static double gettime()
{
  return (double)(glutGet(GLUT_ELAPSED_TIME)) / 1000.;
}

//CALCPOSOBS - Calculate Position of camera
static void calcposobs(void)
{
  float alpha1, alpha2;
  static double t0 = -1.;
  double t, dt;
  if (t0 < 0.) t0 = gettime();
  t = gettime();
  dt = t - t0;
  t0 = t;
  
  dir[0] = sin(alpha * M_PI / 180.0);
  dir[2] = cos(alpha * M_PI / 180.0) * sin(beta * M_PI / 180.0);
  dir[1] = cos(beta * M_PI / 180.0);
  
  if (dir[0] < 1.0e-5 && dir[0] > -1.0e-5) dir[0] = 0;
  if (dir[1] < 1.0e-5 && dir[1] > -1.0e-5) dir[1] = 0;
  if (dir[2] < 1.0e-5 && dir[2] > -1.0e-5) dir[2] = 0;
  
  alpha1 = alpha + FOV / 2.0;
  v1[0] = sin(alpha1 * M_PI / 180.0);
  v1[1] = cos(alpha1 * M_PI / 180.0);
  
  alpha2 = alpha - FOV / 2.0;
  v2[0] = sin(alpha2 * M_PI / 180.0);
  v2[1] = cos(alpha2 * M_PI / 180.0);
  
  // ADD MOVEMANT TO CAMERA
  obs[0] += v * dir[0] * dt;
  obs[1] += v * dir[1] * dt;
  obs[2] += v * dir[2] * dt;
  
  // COMMUNICATION WITH BAYSIAN FILTER
  // SET #IF value TO "0" TO ENABLE MANUAL FLYING
  // OR IF CONNECTING TO FLIGHTGEAR
  //  obs[1] = 500;//camera_pos(2);
#if 0
  obs[0] = camera_pos(0) * (lenghtXmnt * stepXmnt)/100;
  obs[2] = camera_pos(1)  * (lenghtYmnt * stepYmnt)/100;
  alpha  = camera_pos(3) + 90;
  beta   = camera_pos(4);
  //#else 
  camera_pos(0) = obs[0];
  camera_pos(1) = obs[2];
  camera_pos(3) = alpha;
  camera_pos(4) = beta;
  camera_pos(2) = obs[1];
#endif

  if (obs[1] < 0.0)
    obs[1] = 0.0;
}

//RESHAPE - Create point of view
static void reshape(int width, int height)
{
  scrwidth = width;
  scrheight = height;
  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.0, ((GLfloat) width / (GLfloat) height),
		 lenghtXmnt * stepYmnt * 0.01,
		 lenghtXmnt * stepYmnt * 0.7);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

//CLIPSTRIP - Calculate area not to show
static int clipstrip(float y, float *start, float *end)
{
  float x1, x2, t1, t2, tmp;
  
  if (v1[1] == 0.0)
    {
      t1 = 0.0;
      x1 = -HUGE_VAL;
    }
  else
    {
      t1 = y / v1[1];
      x1 = t1 * v1[0];
    }
  
  if (v2[1] == 0.0)
    {
      t2 = 0.0;
      x2 = HUGE_VAL;
    }
  else
    {
      t2 = y / v2[1];
      x2 = t2 * v2[0];
    }
  
  if (((x1 < -(lenghtXmnt * stepXmnt) / 2) && (t2 <= 0.0)) ||
      ((t1 <= 0.0) && (x2 > (lenghtXmnt * stepXmnt) / 2)) ||
      ((t1 < 0.0) && (t2 < 0.0)))
    return 0;
  
  if ((t1 == 0.0) && (t2 == 0.0))
    {
      if ((v1[0] < 0.0) && (v1[1] > 0.0) &&
	  (v2[0] < 0.0) && (v2[1] < 0.0)) 
	{
	  *start = -(lenghtXmnt * stepXmnt) / 2;
	  *end = stepXmnt;
	  return 1;
	}
      else
	{
	  if ((v1[0] > 0.0) && (v1[1] < 0.0) &&
	      (v2[0] > 0.0) && (v2[1] > 0.0))
	    {
	      *start = -stepXmnt;
	      *end = (lenghtXmnt * stepXmnt) / 2;
	      return 1;
	    }
	  else
	    return 0;
	}
    }
  else
    {
      if (t2 < 0.0)
	{
	  if (x1 < 0.0)
	    x2 = -(lenghtXmnt * stepXmnt) / 2;
	  else
	    x2 = (lenghtXmnt * stepXmnt) / 2;
	}
      if (t1 < 0.0)
	{
	  if (x2 < 0.0)
	    x1 = -(lenghtXmnt * stepXmnt) / 2;
	  else
	    x1 = (lenghtXmnt * stepXmnt) / 2;
	}
    }
  
  if (x1 > x2)
    {
      tmp = x1;
      x1 = x2;
      x2 = tmp;
    }
  
  x1 -= stepXmnt;
  if (x1 < -(lenghtXmnt * stepXmnt) / 2)
    x1 = -(lenghtXmnt * stepXmnt) / 2;
  
  x2 += stepXmnt;
  if (x2 > (lenghtXmnt * stepXmnt) / 2)
    x2 = (lenghtXmnt * stepXmnt) / 2;
  
  *start = ((int) (x1 / stepXmnt)) * stepXmnt;
  *end = ((int) (x2 / stepXmnt)) * stepXmnt;
  
  return 1;
}

//PRINTSTRING - Print text to the screen
static void printstring(void *font, char *string)
{
  int len, i;
  
  len = (int) strlen(string);
  for (i = 0; i < len; i++)
    glutBitmapCharacter(font, string[i]);
}

//PRINTHELP - Print a help meny to the screen
static void printhelp(void)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.0, 0.0, 0.0, 0.5);
  glRecti(40, 40, 600, 440);
  glDisable(GL_BLEND);
  
  glColor3f(1.0, 0.0, 0.0);
  glRasterPos2i(300, 420);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "Help");
  
  glRasterPos2i(60, 390);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "h - Togle Help");
  glRasterPos2i(60, 360);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "t - Togle Textures");
  glRasterPos2i(60, 330);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "g - Toggle Terrain");
  glRasterPos2i(60, 300);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "f - Togle Fog");
  glRasterPos2i(60, 270);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "p - Wire frame");
  glRasterPos2i(60, 240);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "b - Togle Back face culling");
  glRasterPos2i(60, 210);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "Arrow Keys - Rotate");
  glRasterPos2i(60, 180);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "a - Increase velocity");
  glRasterPos2i(60, 150);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "z - Decrease velocity");
  glRasterPos2i(60, 110);
  printstring(GLUT_BITMAP_TIMES_ROMAN_24, "q - Quit");

  
  //   glRasterPos2i(60, 150);
  //if (joyavailable)
  //   printstring(GLUT_BITMAP_TIMES_ROMAN_24,
  //   		  "j - Togle jostick control (Joystick control available)");
  //else
  //   printstring(GLUT_BITMAP_TIMES_ROMAN_24,
  //		  "(No Joystick control available)");
  
}

//DRAW TERRAIN - Actual creation of terrain and objects
static void drawterrain(void)
{
  int h, i, id, idx, idy, ox, oy;
  float j, k, start, end;
  
  ox = (int) (obs[0] / stepXmnt);
  oy = (int) (obs[2] / stepYmnt);
  //   GlobalMnt = ((ox * TSCALE) & 255) + ((oy * TSCALE) & 255) * 256;
  GlobalMnt = (ox * TSCALE) + ((oy * TSCALE) * 256);
  GlobalX = (ox * TSCALE);
  GlobalY = ((oy * TSCALE) * 256);
  glPushMatrix();
  glTranslatef((float) ox * stepXmnt, 0, (float) oy * stepYmnt);
  
  start = stepXmnt * -lenghtXmnt/2;
  end = stepXmnt * lenghtXmnt/2;
  
#if 0
  if (markerline) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  //glBegin(GL_TRIANGLE_STRIP);
  
  //glEnd();
  
#endif
  
#if 0
  if (pdfline) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
  for (h = 0, k = -(lenghtYmnt * stepYmnt) / 2; h < lenghtYmnt ; k += stepYmnt, h++)
    {
      glBegin(GL_TRIANGLE_STRIP);
      
      for (i = (int) (lenghtXmnt / 2 + start / stepXmnt), j = start; j <= end; j += stepXmnt, i++)
	{
	  id = ((i * TSCALE) + (h * TSCALE * 256) + GlobalMnt);// & 65535;
	  idx = ((i * TSCALE) + GlobalX);
	  idy = ((h * TSCALE) + GlobalY);
	  
	  if(id >= 0 && id <= 65535 && idx >= 0 && idx <= 255)// && idy >= -65535 && idy <= 65535)
	    {
	      glColor3fv(pdfterraincolor[id]);
	      glVertex3f(j, terrain[id], k);
	    }
	  
	  id = ((i * TSCALE) + (h * TSCALE * 256) + GlobalMnt + (256 * TSCALE));// & 65535;
	  idx = (i * TSCALE) + GlobalX;
	  idy = ((h+1) * TSCALE) + GlobalY;
	  
	  if(id >= 0 && id <= 65535 && idx >= 0 && idx <= 255)// && idy >= -65536 && idy <= 65535)
	    {
	      glColor3fv(pdfterraincolor[id]);
	      glVertex3f(j, terrain[id], k + stepYmnt);
	    }
	}
      
      glEnd();
    }
#endif
  
  
  if(showterrain)
    {
      if (terrainline) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      
      for (h = 0, k = -(lenghtYmnt * stepYmnt) / 2; h < lenghtYmnt; k += stepYmnt, h++) {
	
	glBegin(GL_TRIANGLE_STRIP);
	for (i = (int) (lenghtXmnt / 2 + start / stepXmnt), j = start; j <= end;
	     j += stepXmnt, i++) {
	  id = (i * TSCALE + h * 256 * TSCALE + GlobalMnt) & 65535;
	  glColor3fv(terraincolor[id]);
	  glTexCoord2f((ox + i) / 8.0, (oy + h) / 8.0);
	  glVertex3f(j, terrain[id], k);
	  
	  id = (i * TSCALE + h * 256 * TSCALE + 256 * TSCALE + GlobalMnt) & 65535;
	  glColor3fv(terraincolor[id]);
	  glTexCoord2f((ox + i) / 8.0, (oy + h + 1) / 8.0);
	  glVertex3f(j, terrain[id], k + stepYmnt);
	}
	glEnd();
      }
    }
  
  glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glDisable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor4f(0.1, 0.7, 1.0, 0.204);
  glVertex3f(-(lenghtXmnt * stepXmnt) / 2.0, heightMnt * 0.0505,
	     -(lenghtYmnt * stepYmnt) / 2.0);
  glVertex3f(-(lenghtXmnt * stepXmnt) / 2.0, heightMnt * 0.0505,
	     (lenghtYmnt * stepYmnt) / 2.0);
  glVertex3f((lenghtXmnt * stepXmnt) / 2.0, heightMnt * 0.0505,
	     (lenghtYmnt * stepYmnt) / 2.0);
  glVertex3f((lenghtXmnt * stepXmnt) / 2.0, heightMnt * 0.0505,
	     -(lenghtYmnt * stepYmnt) / 2.0);
  glEnd();
  
  glDisable(GL_BLEND);
  
  
  if (bfcull)
    glEnable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  
  glPopMatrix();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
}

//DOJOY - Enable the use of a joystick
static void dojoy(void)
{
#ifdef WIN32
  static UINT max[2] = { 0, 0 };
  static UINT min[2] = { 0xffffffff, 0xffffffff }, center[2];
  MMRESULT res;
  JOYINFO joy;
  
  res = joyGetPos(JOYSTICKID1, &joy);
  
  if (res == JOYERR_NOERROR)
    {
      joyavailable = 1;
      
      if (max[0] < joy.wXpos)
	max[0] = joy.wXpos;
      if (min[0] > joy.wXpos)
	min[0] = joy.wXpos;
      center[0] = (max[0] + min[0]) / 2;
      
      if (max[1] < joy.wYpos)
	max[1] = joy.wYpos;
      if (min[1] > joy.wYpos)
	min[1] = joy.wYpos;
      center[1] = (max[1] + min[1]) / 2;
      
      if (joyactive)
	{
	  if (fabs(center[0] - (float) joy.wXpos) > 0.1 * (max[0] - min[0]))
	    alpha +=
	      2.5 * (center[0] - (float) joy.wXpos) / (max[0] - min[0]);
	  if (fabs(center[1] - (float) joy.wYpos) > 0.1 * (max[1] - min[1]))
	    beta += 2.5 * (center[1] - (float) joy.wYpos) / (max[1] - min[1]);
	  
	  if (joy.wButtons & JOY_BUTTON1)
	    v += 0.5;
	  if (joy.wButtons & JOY_BUTTON2)
	    v -= 0.5;
	}
    }
  else
    joyavailable = 0;
#endif
}


//DRAWSCENE - Actaul pushing of data to graphics handler
static void drawscene(void)
{
  static char frbuf[80] = "";
  
  //dojoy();
  loadpdf();  //Get latest PDF data
  
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  
  if (usetex)
    glEnable(GL_TEXTURE_2D);
  else
    glDisable(GL_TEXTURE_2D);
  
  if (fog)
    glEnable(GL_FOG);
  else
    glDisable(GL_FOG);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glPushMatrix();
  
  calcposobs();
  gluLookAt(obs[0], obs[1], obs[2], obs[0] + dir[0],
	    obs[1] + dir[1], obs[2] + dir[2],
	    0.0, 1.0, 0.0);
  
  drawterrain();
  glPopMatrix();
  
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  glShadeModel(GL_FLAT);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-0.5, 639.5, -0.5, 479.5, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glColor3f(1.0, 0.0, 0.0);
  glRasterPos2i(10, 10);
  printstring(GLUT_BITMAP_HELVETICA_18, frbuf);
  glRasterPos2i(350, 470);
  printstring(GLUT_BITMAP_HELVETICA_10,
	      "Phil Sammons");
  glRasterPos2i(434, 457);
  printstring(GLUT_BITMAP_HELVETICA_10,
	      "Baysian Search and Rescue.");
  
  if (help)
    printhelp();
  
  reshape(scrwidth, scrheight);
  
  glutSwapBuffers();
  
  Frames++;
  {
    GLint t = glutGet(GLUT_ELAPSED_TIME);
    if (t - T0 >= 2000)
      {
	GLfloat seconds = (t - T0) / 1000.0;
	GLfloat fps = Frames / seconds;
	sprintf(frbuf, "Frame rate: %f", fps);
	T0 = t;
	Frames = 0;
      }
  }
}

//KEY - Get keyboard input
static void key(unsigned char k, int x, int y)
{
  switch (k) {
  case 27:
    exit(0);
    break;
  case 'a':
    v += 50.;
    break;
  case 'z':
    v -= 50.;
    break;
  case 'p':
    if (terrainline)
      {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	terrainline = 0;
      }
    else
      {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	terrainline = 1;
      }
    break;
  case 'o':
    if (pdfline)
      {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	pdfline = 0;
      }
    else
      {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	pdfline = 1;
      }
    break;
  case 'g':
    if (showterrain) showterrain = 0;
    else showterrain = 1;
    break;
  case 'j':
    joyactive = (!joyactive);
    break;
  case 'h':
    help = (!help);
    break;
  case 'f':
    fog = (!fog);
    break;
  case 't':
    usetex = (!usetex);
    break;
  case 'b':
    if (bfcull) {
      glDisable(GL_CULL_FACE);
      bfcull = 0;
    }
    else
      {
	glEnable(GL_CULL_FACE);
	bfcull = 1;
      }
    break;
#ifdef XMESA
  case ' ':
    XMesaSetFXmode(fullscreen ? XMESA_FX_FULLSCREEN : XMESA_FX_WINDOW);
    fullscreen = (!fullscreen);
    break;
#endif
  }
}

//SPECIAL - Get control keys
static void special(int k, int x, int y)
{
  switch (k)
    {
    case GLUT_KEY_LEFT:
      alpha += 2.0;
      break;
    case GLUT_KEY_RIGHT:
      alpha -= 2.0;
      break;
    case GLUT_KEY_DOWN:
      beta -= 2.0;
      break;
    case GLUT_KEY_UP:
      beta += 2.0;
      break;
    }
}

//CALCCOLOR - Calulate the colours to render terrain
static void calccolor(GLfloat height, GLfloat c[3],bool type)
{
  float red1,red2,red3,red4,green1,green2,green3,green4,blue1,blue2,blue3,blue4;
  
  if(type)
    {
      red1 = 0.0; green1 = 0.4; blue1 = 0.0;	//mountains
      red2 = 0.0; green2 = 0.8; blue2 = 0.0;
      red3 = 0.8; green3 = 0.51; blue3 = 0.25;
      red4 = 0.0; green4 = 0.0; blue4 = 0.8;}
  else
    {
      red1= 1.0; green1= 0.8; blue1= 0.0; 		//PDF
      red2= 1.0; green2= 0.0; blue2= 0.0;
      red3 = 0.8; green3 = 0.2; blue3 = 0.5;
      red4 = 0.5; green4 = 0.4; blue4 = 0.7;} 
  
  GLfloat color[4][3] = {
    {red1, green1, blue1},
    {red2, green2, blue2},
    {red3, green3, blue3},
    {red4, green4, blue4}
  };
  
  
  GLfloat fact;
  
  height = height * (1.0 / 255.0);
 
  if(type)
    {
      if (height >= 0.9)
	{
	  c[0] = color[0][0];
	  c[1] = color[0][1];
	  c[2] = color[0][2];
	  return;
	}
      
      if ((height < 0.9) && (height >= 0.8))
	{
	  fact = (height - 0.7) * 3.0;
	  c[0] = fact * color[0][0] + (1.0 - fact) * color[1][0];
	  c[1] = fact * color[0][1] + (1.0 - fact) * color[1][1];
	  c[2] = fact * color[0][2] + (1.0 - fact) * color[1][2];
	  return;
	}
      
      if ((height < 0.8) && (height >= 0.7))
	{
	  fact = (height - 0.6) * 6.0;
	  c[0] = fact * color[1][0] + (1.0 - fact) * color[2][0];
	  c[1] = fact * color[1][1] + (1.0 - fact) * color[2][1];
	  c[2] = fact * color[1][2] + (1.0 - fact) * color[2][2];
	  return;
	}
      
      if ((height < 0.2) && (height >= 0.1))
	{
	  fact = (height - 0.5) * 6.0;
	  c[0] = fact * color[2][0] + (1.0 - fact) * color[3][0];
	  c[1] = fact * color[2][1] + (1.0 - fact) * color[3][1];
	  c[2] = fact * color[2][2] + (1.0 - fact) * color[3][2];
	  return;
	}
    }
  else
    {    
      if (height >= 0.9)
	{
	  c[0] = color[0][0];
	  c[1] = color[0][1];
	  c[2] = color[0][2];
	  return;
	}
      
      if ((height < 0.9) && (height >= 0.5))
	{
	  fact = (height - 0.4) * 5.0;
	  c[0] = fact * color[0][0] + (1.0 - fact) * color[1][0];
	  c[1] = fact * color[0][1] + (1.0 - fact) * color[1][1];
	  c[2] = fact * color[0][2] + (1.0 - fact) * color[1][2];
	  return;
	}
      
      if ((height < 0.5) && (height >= 0.4))
	{
	  fact = (height - 0.15) * 9.0;
	  c[0] = fact * color[1][0] + (1.0 - fact) * color[2][0];
	  c[1] = fact * color[1][1] + (1.0 - fact) * color[2][1];
	  c[2] = fact * color[1][2] + (1.0 - fact) * color[2][2];
	  return;
	}
      
      if ((height < 0.5) && (height >= 0.0))
	{
	  fact = (height - 0.2) * 9.0;
	  c[0] = fact * color[2][0] + (1.0 - fact) * color[3][0];
	  c[1] = fact * color[2][1] + (1.0 - fact) * color[3][1];
	  c[2] = fact * color[2][2] + (1.0 - fact) * color[3][2];
	  return;
	}
    }
  
  c[0] = color[3][0];
  c[1] = color[3][1];
  c[2] = color[3][2];
}

//LOADPIC - Load the terrain data from file and calculate 2D texture
static void loadpic(void)
{
  GLubyte bufferter[256 * 256], terrainpic[256 * 256];
  FILE *FilePic;
  int i, tmp;
  GLenum gluerr;
  
  if ((FilePic = fopen("terrain.dat", "r")) == NULL) {
    fprintf(stderr, "Error loading terrain.dat\n");
    exit(-1);
  }
  fread(bufferter, 256 * 256, 1, FilePic);
  fclose(FilePic);
  
  for (i = 0; i < (256 * 256); i++) {
    terrain[i] = (bufferter[i] * boostTerrain)-20;
    calccolor((GLfloat) bufferter[i], terraincolor[i],1);
    tmp = (((int) bufferter[i]) + 96);
    terrainpic[i] = (tmp > 255) ? 255 : tmp;
  }
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  if ((gluerr = gluBuild2DMipmaps(GL_TEXTURE_2D, 1, 256, 256,
				  GL_LUMINANCE,GL_UNSIGNED_BYTE,
				  (GLvoid *) (&terrainpic[0]))))
    {
      fprintf(stderr, "GLULib%s\n", (char *) gluErrorString(gluerr));
      exit(-1);
    }
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);
}

//GET_SCALE_FACTOR - set falues to lie within 0 - 256
int get_scale_factor(Vector &dataset)
{
  float max = 0.0;
  int num = dataset.size();
  for(int i = 0; i < num ; i ++)
    if(dataset(i) > max) max = dataset(i);
  
  return (int)(255 / max) ;
}

//LOADPDF - Get the current pdf data
static void loadpdf(void)
{
  GLubyte bufferter[256 * 256];
  int i, tmp;
  GLenum gluerr;

  Vector pdfdata(256 * 256);

  test.run_grid(gettime(),pdfdata, camera_pos);  
  double scale_factor = get_scale_factor(camera_pos) * boostPDF;

  for (i = 0; i < (256 * 256); i++)
   {
     bufferter[i] = (GLubyte)pdfdata(i);
     pdfterrain[i] = ((bufferter[i] * scale_factor));
     calccolor((GLfloat) bufferter[i], pdfterraincolor[i],0);
     //tmp = (((int) bufferter[i]) + 46);
   }
}

//INIT - Initialise the OpenGL renderer and prepare program
static void init(void)
{
  float fogcolor[4] = { 0.6, 0.7, 0.7, 1.0 };
  
  glClearColor(fogcolor[0], fogcolor[1], fogcolor[2], fogcolor[3]);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  glDisable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_EXP2);
  glFogfv(GL_FOG_COLOR, fogcolor);
  glFogf(GL_FOG_DENSITY, 0.0007);
#ifdef FX
  glHint(GL_FOG_HINT, GL_NICEST);
#endif
  
  reshape(scrwidth, scrheight);
}

// -------------------------------
//       MAIN PROGRAM
// ------------------------------
//int run_GL_main(int ac, char **av)
int main(int ac, char **av)
{
  Vector blank(256*256);
  
  test.grid_init();  //initialise bayesian algorithm
  test.run_grid(0.0,blank, camera_pos);

  glutInitWindowPosition(640, 0);  //Set window position on screen
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInit(&ac, av);
  //glutFullScreen();
  
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  
  if (!(win = glutCreateWindow("Terrain")))
    {
      fprintf(stderr, "Error, couldn't open window\n");
      return -1;
    }
  
  ModZMnt = 0.0f;
  loadpic();
  loadpdf();
  
  init();
  
#ifndef FX
  glDisable(GL_TEXTURE_2D);
  usetex = 0;
#endif
  
  //MAIN PROGRAM LOOP
  //----------------------
  glutReshapeFunc(reshape);
  glutDisplayFunc(drawscene);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutIdleFunc(drawscene);
  //----------------------

  glutMainLoop();
  
  return 0;
}
