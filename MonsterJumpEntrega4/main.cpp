
//
//  main.cpp
//  CuboConFigurasGC
//
//  Created by Maria  Roque on 18/09/12.
//  Copyright (c) 2012 Tecnológico de Monterrey. All rights reserved.
//
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <cstdio>
#include <SOIL.h>

float x=1.0;
float x1=0.0;
float x2=0.0;
using namespace std;
const float medida = 20;  // Mitad del tamaño de cada lado del cubo
int borderLeft = 0;
int borderRight = 1366;
int borderBottom = 0;
int borderTop = 700;
int borderNear = -200;
int borderFar = 200;
float rotateX = 20.0;
float rotateY = 20.0;
float posY[24];
float posX[24];
float scale=1.0;
float transX1=0.0;
float transY1=0.0;
float transX2=0.0;
float transY2=0.0;
float offset=0.0;
bool hidden[24];
GLuint tex, platformSide, platformTop, platformBottom, background,
            platformVenomSide, platformVenomTop, platformVenomBottom;


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

void init(void)
{
    int _posY=0;
    int _posX=160;

    for(int i = 0; i < 6; i++)
    {
        posX[i*6]=_posX;
        posY[i*6]=_posY;

        posX[i*6+1]=_posX+200;
        posY[i*6+1]=_posY;

        posX[i*6+2]=_posX+400;
        posY[i*6+2]=_posY;

        posX[i*6+3]=_posX+600;
        posY[i*6+3]=_posY;

        posX[i*6+4]=_posX+800;
        posY[i*6+4]=_posY;

        posX[i*6+5]=_posX+1000;
        posY[i*6+5]=_posY;

        _posY += 200;
    }

    srand(timeGetTime());
    for(int i = 0; i< 24; i++)
    {
       // hidden[i] = (rand()%8 < 3);
    }
    glClearColor (0.0, 0.0, 1.0, 0.0);
    glShadeModel (GL_SMOOTH );//sombreado plano
    //glShadeModel (GL_FLAT );
}

void changeHiddenInRow(int row)
{
    hidden[row]= (rand()%8 < 3);
}

void myTimer(int v)
{
    for(int i = 0; i <  24; i++)
    {
        posY[i] -= 5;
        if(posY[i]<borderBottom-medida*2)
        {
            posY[i] = borderTop + medida*2;
            //changeHiddenInRow(i);
        }
    }

    offset += 0.5;

    glutPostRedisplay();
    glutTimerFunc(30, myTimer, 1);

}
void drawPlatform(int top, int bottom, int side)
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, top );
    glBegin(GL_QUADS);

    //Arriba
    glColor4f(1.0, 1.0, 1.0,1.0);
    glTexCoord2d(0.0,0.0);
    glVertex3f( -4*medida, medida, -4*medida );
    glTexCoord2d(0.0,1.0);
    glVertex3f( -4*medida, medida,  4*medida );
    glTexCoord2d(1.0,1.0);
    glVertex3f(  4*medida, medida,  4*medida );
    glTexCoord2d(1.0,0.0);
    glVertex3f(  4*medida, medida, -4*medida );

    glEnd();
    glBindTexture( GL_TEXTURE_2D, bottom );
    glBegin(GL_QUADS);

    //Abajo
    glTexCoord2d(0.0,0.0);
    glVertex3f( -4*medida, -medida, -4*medida );
    glTexCoord2d(1.0,0.0);
    glVertex3f(  4*medida, -medida, -4*medida );
    glTexCoord2d(1.0,1.0);
    glVertex3f(  4*medida, -medida,  4*medida );
    glTexCoord2d(0.0,1.0);
    glVertex3f( -4*medida, -medida,  4*medida );

    glEnd();
    glBindTexture( GL_TEXTURE_2D, side );
    glBegin(GL_QUADS);

    //Izquierda
    glTexCoord2d(0.0,1.0);
    glVertex3f( -4*medida,  medida,  4*medida );
    glTexCoord2d(1.0,1.0);
    glVertex3f( -4*medida,  medida, -4*medida );
    glTexCoord2d(1.0,0.0);
    glVertex3f( -4*medida, -medida, -4*medida );
    glTexCoord2d(0.0,0.0);
    glVertex3f( -4*medida, -medida,  4*medida );

    //Derecha
    glTexCoord2d(0.0,0.0);
    glVertex3f( 4*medida, -medida, -4*medida );
    glTexCoord2d(0.0,1.0);
    glVertex3f( 4*medida,  medida, -4*medida );
    glTexCoord2d(1.0,1.0);
    glVertex3f( 4*medida,  medida,  4*medida );
    glTexCoord2d(1.0,0.0);
    glVertex3f( 4*medida, -medida,  4*medida );

    //Frente
    glTexCoord2d(1.0,0.0);
    glVertex3f( -4*medida, -medida, 4*medida );
    glTexCoord2d(0.0,0.0);
    glVertex3f(  4*medida, -medida, 4*medida );
    glTexCoord2d(0.0,1.0);
    glVertex3f(  4*medida,  medida, 4*medida );
    glTexCoord2d(1.0,1.0);
    glVertex3f( -4*medida,  medida, 4*medida );

    //Atrás
    glTexCoord2d(0.0,0.0);
    glVertex3f( -4*medida, -medida, -4*medida );
    glTexCoord2d(0.0,1.0);
    glVertex3f( -4*medida,  medida, -4*medida );
    glTexCoord2d(1.0,1.0);
    glVertex3f(  4*medida,  medida, -4*medida );
    glTexCoord2d(1.0,0.0);
    glVertex3f(  4*medida, -medida, -4*medida );

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void drawBackground()
{
    glColor4f(1.0,1.0,1.0,1.0);
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, background );
    glBegin( GL_QUADS );
    glTexCoord2d(0.0,0.0); glVertex3d(0.0,0-offset,borderNear+1);
    glTexCoord2d(1,0.0); glVertex3d(1366,0-offset,borderNear+1);
    glTexCoord2d(1,1); glVertex3d(1366,8000-offset,borderNear+1);
    glTexCoord2d(0.0,1); glVertex3d(0.0,8000-offset,borderNear+1);
    glEnd();
}

void dibuja() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawBackground();

    glPushMatrix();

    for(int cnt=0; cnt < 24; cnt++)
    {
        //if(hidden[cnt]!=true)
        {
            //////////////////////////////////
            // Rotación para que se aprecie en 3D, si no, se ve sólo una cara
            // Angulo de rotación = 45, rotando en X=1, Y=1, Z=0.

            glPushMatrix();
            glTranslatef(posX[cnt],posY[cnt],0);
            glRotatef((((posX[cnt]+(medida*4)) - (borderRight/2)) / 533) * rotateX, 0.0, 1.0, 0.0);
            glRotatef((-(((posY[cnt]+(medida)) - (borderTop/2)) / 400) * rotateY), 1.0, 0.0, 0.0);
            //////////////////////////////////


            drawPlatform(platformTop, platformBottom, platformSide);
            glPopMatrix();
        }
    }
    glPushMatrix();
    glTranslatef(100.0,100.0,5.0);
    gluCylinder(gluNewQuadric(), 5,5,20,5,5);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape (int w, int h)
{

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho(borderLeft, borderRight, borderBottom, borderTop, borderNear, borderFar ); //izq, der, abajo, arriba, cerca, lejos
   // glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

void mouse(int button, int state, int x, int y)
{
    if(state==GLUT_DOWN && button==GLUT_LEFT_BUTTON)
    {
        if(x<=300) transX1=-200.0+x/1.5;
        else transX1=x/1.5-200;
        y=400-y/1.5;
        if(y<=300) transY1=-200.0+y;
        else transY1=y-200;

        x1+=40;
        glutPostRedisplay();
    }
    else if(state==GLUT_DOWN && button==GLUT_RIGHT_BUTTON)
    {
        if(x<=300) transX2=-200.0+x/1.5;
        else transX2=x/1.5-200;
        y=400-y/1.5;
        if(y<=300) transY2=-200.0+y;
        else transY2=y-200;

        x2+=40;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (1366, 700);
    glutInitWindowPosition (0, 0);
    glutCreateWindow (argv[0]);
    init ();

    background = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\fondo.jpg", 1366, 8000);
    platformSide = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\platformSide.png", 256, 64);
    platformTop = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\platformTop.png", 256, 256);
    platformBottom = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\platformBottom.png", 256, 256);
    platformVenomSide = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\platformVenomSide.png", 256, 64);
    platformVenomTop = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\platformVenomTop.png", 256, 256);
    platformVenomBottom = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\platformVenomBottom.png", 256, 256);

    glEnable(GL_DEPTH_TEST); //para diferenciar que vertices estan al frente y detras ver ejemplo del documento de word
    glutDisplayFunc(dibuja);
    glutMouseFunc(mouse);
    glutTimerFunc(5, myTimer, 1);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
