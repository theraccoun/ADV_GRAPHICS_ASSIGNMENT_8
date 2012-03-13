/*
 *  Lighting and Textures Example
 *  'p' to switch projections
 *  's' to start/stop light
 *  'o' to switch objects
 *  'a' to toggle axes
 *  '0' snaps angles to 0,0
 *  arrows to rotate the world
 *  PgUp/PgDn zooms in/out
 *  -/+    change light elevation
 */
#include "CSCIx239.h"
int axes=1;       //  Display axes
int mode=0;       //  Shader mode
int move=1;       //  Move light
int proj=1;       //  Projection type
int obj=0;        //  Object
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=85;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=10.0 ;   //  Size of world
int zh=90;        //  Light azimuth
float Ylight=2;   //  Light elevation
int model;        //  Object model
int cubeMap;        //  top texture
int bottom;     // bottom texture
int left;         // Left texture
int right;        // Right Texture
#define MODE 5
int shader[MODE] = {0,0,0,0,0}; //  Shader programs
char* text[] = {"No Shader","Vertex Blinn","Vertex Phong","Pixel Blinn","Pixel Phong"};

const int D = 200;

/*
 *  Draw a cube
 */
static void Cube(void)
{
   glEnable(GL_TEXTURE_CUBE_MAP);
     glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMap);
   //  Front
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,+1);
   glTexCoord3f(-D,-D,+D); glVertex3f(-D,+D,+D);
   glTexCoord3f(+D,-D,+D); glVertex3f(+D,+D,+D);
   glTexCoord3f(+D,+D,+D); glVertex3f(+D,-D,+D);
   glTexCoord3f(-D,+D,+D); glVertex3f(-D,-D,+D);
   glEnd();
   //  Back
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord3f(+D,-D,-D); glVertex3f(+D,+D,-D);
   glTexCoord3f(-D,-D,-D); glVertex3f(-D,+D,-D);
   glTexCoord3f(-D,+D,-D); glVertex3f(-D,-D,-D);
   glTexCoord3f(+D,+D,-D); glVertex3f(+D,-D,-D);
   glEnd();
   //  Right
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord3f(+D,-D,+D); glVertex3f(+D,+D,+D);
   glTexCoord3f(+D,-D,-D); glVertex3f(+D,+D,-D);
   glTexCoord3f(+D,+D,-D); glVertex3f(+D,-D,-D);
   glTexCoord3f(+D,+D,+D); glVertex3f(+D,-D,+D);
   glEnd();
   //  Left
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord3f(-D,-D,-D); glVertex3f(-D,+D,-D);
   glTexCoord3f(-D,-D,+D); glVertex3f(-D,+D,+D);
   glTexCoord3f(-D,+D,+D); glVertex3f(-D,-D,+D);
   glTexCoord3f(-D,+D,-D); glVertex3f(-D,-D,-D);
   glEnd();
   //  top
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord3f(-D,+D,+D); glVertex3f(+D,+D,+D);
   glTexCoord3f(+D,+D,+D); glVertex3f(-D,+D,+D);
   glTexCoord3f(+D,+D,-D); glVertex3f(-D,+D,-D);
   glTexCoord3f(-D,+D,-D); glVertex3f(+D,+D,-D);
   glEnd();
   //  Bottom
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord3f(-D,-D,-D); glVertex3f(-D,-D,-D);
   glTexCoord3f(+D,-D,-D); glVertex3f(+D,-D,-D);
   glTexCoord3f(+D,-D,+D); glVertex3f(+D,-D,+D);
   glTexCoord3f(-D,-D,+D); glVertex3f(-D,-D,+D);
   glEnd();
      glDisable(GL_TEXTURE_CUBE_MAP);

}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{


   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   //  Enable Z-buffering in OpenGL
//   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (proj)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   // Enable/Disable features
       glPushAttrib(GL_ENABLE_BIT);
       glDisable(GL_LIGHTING);
       glDisable(GL_DEPTH_TEST);

   //
   //  Draw scene
   //
   //  Select shader (0 => no shader)
   glUseProgram(shader[mode]);
   Cube();


   glPopAttrib();

   //  Display parameters
   glWindowPos2i(5,5);
   Print("FPS=%d Dim=%.1f Projection=%s Mode=%s",
     FramesPerSecond(),dim,proj?"Perpective":"Orthogonal",text[mode]);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move) zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Toggle projection type
   else if (ch == 'p' || ch == 'P')
      proj = 1-proj;
   //  Toggle light movement
   else if (ch == 's' || ch == 'S')
      move = 1-move;
   //  Toggle objects
   else if (ch == 'o' || ch == 'O')
      obj = (obj+1)%3;
   //  Cycle modes
   else if (ch == 'm')
      mode = (mode+1)%MODE;
   else if (ch == 'M')
      mode = (mode+MODE-1)%MODE;
   //  Light elevation
   else if (ch == '+')
      Ylight += 0.1;
   else if (ch == '-')
      Ylight -= 0.1;
   //  Light position
   else if (ch == '[')
      zh--;
   else if (ch == ']')
      zh++;
   //  Reproject
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(proj?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Lighting and Texture");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   if (!GLEW_VERSION_2_0) Fatal("OpenGL 2.0 not supported\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load top
   const char* file[6] = {"right.bmp", "left.bmp", "top.bmp", "bottom.bmp", "front.bmp", "back.bmp"};
   cubeMap = LoadCubeMap(file);
//   bottom = LoadTexBMP("bottom.bmp");
//   left = LoadTexBMP("left.bmp");
//   right = LoadTexBMP("right.bmp");
   //  Load object
   model = LoadOBJ("cruiser.obj");
   //  Create Shader Programs
   shader[1] = CreateShaderProg("vertexblinn.vert","vertexblinn.frag");
   shader[2] = CreateShaderProg("vertexphong.vert","vertexphong.frag");
   shader[3] = CreateShaderProg("pixelblinn.vert" ,"pixelblinn.frag");
   shader[4] = CreateShaderProg("pixelphong.vert" ,"pixelphong.frag");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
