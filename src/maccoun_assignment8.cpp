#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_TITLE "GLSL SkyBox"
#define SHADER_PATH "./shaders/"
#define SKYBOX_VS SHADER_PATH"SkyBox.vsh"
#define SKYBOX_FS SHADER_PATH"SkyBox.fsh"
#define REFLECTION_VS SHADER_PATH"reflection.vert"
#define REFLECTION_FS SHADER_PATH"reflection.frag"


using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::string;

void initShaders();
void initGL();
void initTextures();

// Attributes
float th = 0.0;
float ph = 0.0;
float dim = 80.0;
int model = 0;
int isZoom = 0;
int isReflect = 1;

int curCubePath = 0;

GLuint cubemap;

const string TEXTURE_PATH = "./textures/cubemaps/";
string imageNames[6] = {"positive_x.png", "negative_x.png", "positive_y.png",
                        "negative_y.png", "positive_z.png", "negative_z.png"};

string cubemapPaths[6] = {TEXTURE_PATH +"up_in_sky/positive_x.png", TEXTURE_PATH +"up_in_sky/negative_x.png", TEXTURE_PATH +"up_in_sky/positive_y.png",
                          TEXTURE_PATH + "up_in_sky/negative_y.png", TEXTURE_PATH + "up_in_sky/positive_z.png", TEXTURE_PATH + "up_in_sky/negative_z.png"};

GLuint skyBoxProg, reflectionProg;

/*
 *  Draw a cube
 */
static void Cube(int D, int isReflect)
{


    //  Front
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,+1);
    glTexCoord3f(-D,-D,+D); glVertex3f(-D,-D,+D);
    glTexCoord3f(+D,-D,+D); glVertex3f(+D,-D,+D);
    glTexCoord3f(+D,+D,+D); glVertex3f(+D,+D,+D);
    glTexCoord3f(-D,+D,+D); glVertex3f(-D,+D,+D);
    glEnd();
    //  Back
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord3f(+D,-D,-D); glVertex3f(+D,-D,-D);
    glTexCoord3f(-D,-D,-D); glVertex3f(-D,-D,-D);
    glTexCoord3f(-D,+D,-D); glVertex3f(-D,+D,-D);
    glTexCoord3f(+D,+D,-D); glVertex3f(+D,+D,-D);
    glEnd();
    //  Right
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord3f(+D,-D,+D); glVertex3f(+D,-D,+D);
    glTexCoord3f(+D,-D,-D); glVertex3f(+D,-D,-D);
    glTexCoord3f(+D,+D,-D); glVertex3f(+D,+D,-D);
    glTexCoord3f(+D,+D,+D); glVertex3f(+D,+D,+D);
    glEnd();
    //  Left
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord3f(-D,-D,-D); glVertex3f(-D,-D,-D);
    glTexCoord3f(-D,-D,+D); glVertex3f(-D,-D,+D);
    glTexCoord3f(-D,+D,+D); glVertex3f(-D,+D,+D);
    glTexCoord3f(-D,+D,-D); glVertex3f(-D,+D,-D);
    glEnd();
    //  top
    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord3f(-D,+D,+D); glVertex3f(-D,+D,-D);
    glTexCoord3f(+D,+D,+D); glVertex3f(+D,+D,-D);
    glTexCoord3f(+D,+D,-D); glVertex3f(+D,+D,+D);
    glTexCoord3f(-D,+D,-D); glVertex3f(-D,+D,+D);
    glEnd();
    //  Bottom
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord3f(-D,-D,-D); glVertex3f(-D,-D,-D);
    glTexCoord3f(+D,-D,-D); glVertex3f(+D,-D,-D);
    glTexCoord3f(+D,-D,+D); glVertex3f(+D,-D,+D);
    glTexCoord3f(-D,-D,+D); glVertex3f(-D,-D,+D);
    glEnd();

}

void reshapeFunc(int w, int h)
{
    if (h==0)
        h=1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0f,(GLfloat)w/(GLfloat)h,0.1f,900.0f);
    glMatrixMode(GL_MODELVIEW);
}

void displayFunc()
{
    static float rot = 0.0f;
    rot += 0.0005f;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    double Ex = -2*dim*sin(th)*cos(ph);
    double Ey = +2*dim        *sin(ph);
    double Ez = +2*dim*cos(th)*cos(ph);
    gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,cos(ph),0);

    //    gluLookAt(-sinf(rot)*10, cosf(rot)*30, 20+sinf(rot)*20, 0, 0, 0, 0, 1, 0);


    glUniform1ui(glGetUniformLocation(skyBoxProg, "cubeMap"), cubemap);
    glUniform1ui(glGetUniformLocation(reflectionProg, "cubeMap"), cubemap);
    glPushMatrix();
    glUseProgram(skyBoxProg);
    glutSolidCube(500);
    glUseProgram(NULL);
    glPopMatrix();
    glUseProgram(reflectionProg);
    glPushMatrix();

    if(model == 0){
        Cube(30, 1);
    }
    else if(model == 1){
        glutSolidSphere(30, 32, 32);
    }
    else{
        Cube(30, 1);
    }

    glPopMatrix();
    glUseProgram(NULL);
    glutSwapBuffers();
    glutPostRedisplay();
}

char* loadFileToString(const char *path)
{
    char* data;
    int len;
    ifstream myfile(path);
    if (myfile.is_open())
    {
        myfile.seekg(0, ios::end);
        len = myfile.tellg();
        myfile.seekg(0, ios::beg);
        data = new char[len];
        while ( myfile.good() )
        {
            myfile.read(data, len);
        }
        data[len] = 0;
        cout << "File '" << path << "' has been read" << endl;
        myfile.close();
    }
    else cout << "Unable to open file";

    return data;
}

void printLog(GLuint obj)
{
    int infologLength = 0;
    int maxLength;

    if(glIsShader(obj))
        glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
    else
        glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);

    char infoLog[maxLength];

    if (glIsShader(obj))
        glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
    else
        glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);

    if (infologLength > 0)
        cout << infoLog << endl;
}

void initGL()
{
    glClearColor(0, 0, 0, 0);
    glClearDepth(1);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    initShaders();
    initTextures();
}

void initShaders()
{
    GLuint skyBoxFS, skyBoxVS, reflectionFS, reflectionVS;
    const GLchar* skyBoxFSSource = loadFileToString(SKYBOX_FS);
    const GLchar* skyBoxVSSource = loadFileToString(SKYBOX_VS);
    const GLchar* reflectionFSSource = loadFileToString(REFLECTION_FS);
    const GLchar* reflectionVSSource = loadFileToString(REFLECTION_VS);

    glewInit();

    skyBoxFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(skyBoxFS, 1, &skyBoxFSSource, NULL);
    glCompileShader(skyBoxFS);
    printLog(skyBoxFS);

    skyBoxVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(skyBoxVS, 1, &skyBoxVSSource, NULL);
    glCompileShader(skyBoxVS);
    printLog(skyBoxVS);

    skyBoxProg = glCreateProgram();
    glAttachShader(skyBoxProg, skyBoxVS);
    glAttachShader(skyBoxProg, skyBoxFS);
    glLinkProgram(skyBoxProg);
    printLog(skyBoxProg);

    reflectionFS = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(reflectionFS, 1, &reflectionFSSource, NULL);
    glCompileShader(reflectionFS);
    printLog(reflectionFS);

    reflectionVS = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(reflectionVS, 1, &reflectionVSSource, NULL);
    glCompileShader(reflectionVS);
    printLog(reflectionVS);

    reflectionProg = glCreateProgram();
    glAttachShader(reflectionProg, reflectionVS);
    glAttachShader(reflectionProg, reflectionFS);
    glLinkProgram(reflectionProg);
    printLog(reflectionProg);
}

void setCubeMapPaths(string cmapName){

    for(int i = 0; i < 6; ++i){
        string imagePath = cmapName + "/" + imageNames[i];
        cubemapPaths[i] = TEXTURE_PATH +  imagePath;
    }
}

void initTextures()
{
    ILuint ilTex;
    ilInit();
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (int i = 0; i < 6; i++)
    {
        ilGenImages(1, &ilTex);
        ilBindImage(ilTex);
        ilLoadImage(cubemapPaths[i].c_str());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
                     ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
    }
}

/*
 *  GLUT calls this rotexMaputine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 0.1;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 0.1;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
        ph += 0.1;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
        ph -= 0.1;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.3;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
        dim -= 0.3;

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
    else if (ch == 'c'){
        curCubePath = (curCubePath + 1)%2;

        if(curCubePath == 0){
            setCubeMapPaths("up_in_sky");
            initTextures();
        }
        else{
            setCubeMapPaths("bright_day");
            initTextures();
        }

    }
    else if (ch == 'o' || ch == 'O')
        model = (model+1)%3;
    else if (ch == 'z'){
        isZoom = (isZoom + 1)%2;

        if(isZoom == 0){
            dim = 42;
            cout << dim;
        }
        else{
            dim = 100;
            cout << dim;
        }
    }
    else if(ch == 'r'){
        isReflect = (isReflect + 1)%2;
    }

    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();


    //    //  Reset view angle
    //    else if (ch == '0')
    //        th = ph = 0;
    //    //  Toggle axes
    //    else if (ch == 'a' || ch == 'A')
    //        axes = 1-axes;
    //    //  Toggle projection type
    //    else if (ch == 'p' || ch == 'P')
    //        proj = 1-proj;
    //    //  Toggle light movement
    //    else if (ch == 's' || ch == 'S')
    //        move = 1-move;
    //    //  Toggle objects
    //    else if (ch == 'o' || ch == 'O')
    //        obj = (obj+1)%3;
    //    //  Cycle modes
    //    else if (ch == 'm')
    //        mode = (mode+1)%MODE;
    //    else if (ch == 'M')
    //        mode = (mode+MODE-1)%MODE;
    //    //  Light elevation
    //    else if (ch == '+')
    //        Ylight += 0.1;
    //    else if (ch == '-')
    //        Ylight -= 0.1;
    //    //  Light position
    //    else if (ch == '[')
    //        zh--;
    //    else if (ch == ']')
    //        zh++;
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    glutInitWindowPosition(0, 0);

    glutCreateWindow(DEFAULT_TITLE);

    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(displayFunc);

    initGL();

    glutMainLoop();
    return 0;
}
