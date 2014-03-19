/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <SOIL.h>
#include <cstdio>
#endif

#include <stdlib.h>

/* GLUT callback Handlers */
GLuint tex, texture1, texture2, texture3;
double offset,offsetPtf;
static float ver[8][3]=
{
    {-4.0,-1.0,-4.0},
    {-4.0,1.0,-4.0},
    {4.0,1.0,-4.0},
    {4.0,-1.0,-4.0},
    {-4.0,-1.0,4.0},
    {-4.0,1.0,4.0},
    {4.0,1.0,4.0},
    {4.0,-1.0,4.0}
};
static int rndmX[7];

static void timer(int i)
{
    offset+=1;
    offsetPtf+=25;
    if(offsetPtf>1000)
    {
        offsetPtf=-900;
        for(int i=0; i<7; i++)
        {
            rndmX[i] = 100*(rand()%12)+100;
        }
    }
    glutTimerFunc(50,timer,1);
    glutPostRedisplay();
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho (0, 1366.0, 0, 768.0, -200.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

GLint LoadGLTexture(const char *filename, int width, int height)
{
     GLuint texture;

    //CODE SEGMENT 2
    texture = SOIL_load_OGL_texture // load an image file directly as a new OpenGL texture
	(
		filename,
		SOIL_LOAD_RGBA,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
	);


    // check for an error during the load process
    if(texture == 0)
    {
	    printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }

    glGenTextures(1, &tex); // allocate a texture name
    glBindTexture(GL_TEXTURE_2D, texture); // select our current texture
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_DECAL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);  // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when texture area is large, bilinear filter the first mipmap
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // texture should tile
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   //j gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texture);
    return texture;
}

void drawBackground()
{
    glColor4f(1.0,1.0,1.0,1.0);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture1 );
    glBegin( GL_QUADS );
    glTexCoord2d(0.0,0.0); glVertex3d(0.0,0-offset,-0.6);
    glTexCoord2d(1,0.0); glVertex3d(1366,0-offset,-0.6);
    glTexCoord2d(1,1); glVertex3d(1366,8000-offset,-0.6);
    glTexCoord2d(0.0,1); glVertex3d(0.0,8000-offset,-0.6);
    glEnd();
}

void quad(int a,int b,int c,int d, bool tex)
{
    glBegin(GL_QUADS);

    if(tex)
    {
        glTexCoord2d(1,0.0);
        glVertex3fv(ver[a]);
        glTexCoord2d(1.0,1.0);
        glVertex3fv(ver[b]);
        glTexCoord2d(0.0,1.0);
        glVertex3fv(ver[c]);
        glTexCoord2d(0.0,0.0);
        glVertex3fv(ver[d]);
    }
    else
    {
        glVertex3fv(ver[a]);
        glVertex3fv(ver[b]);
        glVertex3fv(ver[c]);
        glVertex3fv(ver[d]);
    }
    glEnd();
}

void drawCube(/*int scale*/)
{
    /*for(int i=0; i<8; i++)
    {
        for(int j=0; j<3; j++)
        {
            ver[i][j]*=scale;
        }
    }*/
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture3 );
    quad(0,1,2,3,true);//front
    quad(3,7,8,4,false);//right
    quad(5,6,7,8,false);//back
    quad(2,6,5,1,false);//left
    quad(2,6,7,3,false);//top
    quad(1,5,8,4,false);//bottom
}

void display(void)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    drawBackground();

  /*  glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture2 );
    glBegin( GL_QUADS );
    glTexCoord2d(0.0,0.0); glVertex2d(0.5,0.5);
    glTexCoord2d(1,0.0); glVertex2d(0.6,0.5);
    glTexCoord2d(1,1); glVertex2d(0.6,0.678);
    glTexCoord2d(0.0,1); glVertex2d(0.5,0.678);
    glEnd();*/
    glPushMatrix();
    glTranslatef(rndmX[0],100-offsetPtf,50);
    glScalef(20,20,0);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(rndmX[1],200-offsetPtf,50);
    glScalef(20,20,0);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(rndmX[2],400-offsetPtf,50);
    glScalef(20,20,0);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(rndmX[3],100-offsetPtf,50);
    glScalef(20,20,0);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(rndmX[4],700-offsetPtf,50);
    glScalef(20,20,0);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(rndmX[5],850-offsetPtf,50);
    glScalef(20,20,0);
    drawCube();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(rndmX[6],1000-offsetPtf,50);
    glScalef(20,20,0);
    drawCube();
    glPopMatrix();

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'j' :
        case 'J':
            offset += 10;
            glutPostRedisplay();
            break;
        case 27:
        case 'q':
            exit(0);
            break;

    }

    glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1366,768);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);

    glClearColor(1,0,1,1);

    texture1 = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\fondo.jpg", 1366, 8000);
    texture2 = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\PowerUpStar.png", 128, 128);
    texture3 = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\platformVenomSide.png", 256, 64);

        for(int i=0; i<7; i++)
        {
            rndmX[i] = 100*(rand()%12);
        }

    glutTimerFunc(50,timer,1);
    glutMainLoop();

    return EXIT_SUCCESS;
}
