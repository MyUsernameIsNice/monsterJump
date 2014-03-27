/*
 ING. Ma. Guadalupe Roque Diaz de Leon
 Objetivo: Dise“o de una animacion de un TREN que se mueve en 4 direcciones y crece
 el tren siempre se estara moviendo, las teclas indicadas solo cambian la direccion
 del tren.
 */
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <SOIL.h>
#include <cstdio>

static int MENU = 0;
static int CONFIG = 1;
const int X_MAX=1;
const int X_MIN=-1;
const int Y_MAX=1;
const int Y_MIN=-1;
const double DELTA= 0.1;
double dirX=DELTA;//derecha x=-1,Y=0 IZQUIERDA, X=0;Y=-1 ABAJO, X=0,Y=1 ARRIBA
double dirY=0.0;
int opcion = 1,screen=0;
GLuint tex, textureMonst, textureHigh, textureConf,
textureSonido, textureMusica;

static void inicializa()
{
    glClearColor(1.0,1.0,1.0,1);//DEFINE EL COLOR DEL FONDO DE TU PANTALLA
    glEnable(GL_DEPTH_TEST);
}


void displayText( float xRas, float yRas, const char *string ) {
	int j = strlen( string );

	glRasterPos3f( xRas, yRas, -0.6);
	for( int i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, string[i] );
	}
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

static void display(void)
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );

    glClearColor(1.0,0.7,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(screen == MENU)
    {
        glPushMatrix();
        glColor3ub(100,100,100);
        displayText(-0.5, 1.7, "Monster Jump");
        glPopMatrix();

        glColor3ub(0,0,0);
        glPushMatrix();
        displayText(-0.5,0.9,"START GAME!");
        glPopMatrix();

        glColor3ub(0,0,0);
        glPushMatrix();
        displayText(-2.45,-2.5,"Configuraciones");
        glPopMatrix();

        glColor3ub(0,0,0);
        glPushMatrix();
        displayText(-0.4,-2,"Monstruos");
        glPopMatrix();

        glColor3ub(0,0,0);
        glPushMatrix();
        displayText(1.5,-2.5,"Highscores");
        glPopMatrix();

        //Start game quads
        glPushMatrix();
        glTranslatef(-0.2,0.8,-1.5);
        glColor3f(0.8,0.4,0);
        glBegin(GL_QUADS);
        glVertex2d(-0.8, 0.68);
        glVertex2d(1.2,0.68);
        glVertex2d(1.2,0.18);
        glVertex2d(-0.8,0.18);
        glEnd();
        glPopMatrix();

        //Configuracion quads
        glPushMatrix();
        glTranslatef(-3.2,-4.45,-2.2);
        glColor3f(0.8,0.4,0);
        glBegin(GL_QUADS);
        glVertex2d(-0.8, 0.68);
        glVertex2d(1.2,0.68);
        glVertex2d(1.2,0.18);
        glVertex2d(-0.8,0.18);
        glEnd();
        glPopMatrix();


        //Configuracion imagen
        glColor3f(1,1,1);
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureConf );
        glPushMatrix();
        glTranslatef(-4,-3.5,-2.2);
        glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2d(0, 0);
        glTexCoord2d(1,0);
        glVertex2d(2,0);
        glTexCoord2d(1,1);
        glVertex2d(2,2);
        glTexCoord2d(0,1);
        glVertex2d(0,2);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        //Highscores quads
        glPushMatrix();
        glTranslatef(2.8,-4.45,-2.2);
        glColor3f(0.8,0.4,0);
        glBegin(GL_QUADS);
        glVertex2d(-0.8, 0.68);
        glVertex2d(1.2,0.68);
        glVertex2d(1.2,0.18);
        glVertex2d(-0.8,0.18);
        glEnd();
        glPopMatrix();

        //Highscores imagen
        glColor3f(1,1,1);
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureHigh );
        glPushMatrix();
        glTranslatef(2,-3.5,-2.2);
        glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2d(0,0);
        glTexCoord2d(1,0);
        glVertex2d(2,0);
        glTexCoord2d(1,1);
        glVertex2d(2,2);
        glTexCoord2d(0,1);
        glVertex2d(0,2);
        glEnd();
        glPopMatrix();
        glDisable( GL_TEXTURE_2D );

        //Monstruos quads
        glPushMatrix();
        glTranslatef(-0.3,-3.65,-2.2);
        glColor3f(0.8,0.4,0);
        glBegin(GL_QUADS);
        glVertex2d(-0.8, 0.68);
        glVertex2d(1.2,0.68);
        glVertex2d(1.2,0.18);
        glVertex2d(-0.8,0.18);
        glEnd();
        glPopMatrix();

        //Monster imagen
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureMonst );
        glPushMatrix();
        glTranslatef(-2.5,-4.45,-5);
        glColor3f(1,1,1);
        glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2d(1, -1);
        glTexCoord2d(1,0);
        glVertex2d(4, -1);
        glTexCoord2d(1,1);
        glVertex2d(4,7);
        glTexCoord2d(0,1);
        glVertex2d(1,7);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glPushMatrix();
        glColor3ub(100,100,100);
        displayText(-0.5, 1.7, "Configuraciones");
        glPopMatrix();

        glColor3ub(0,0,0);
        glPushMatrix();
        displayText(-2.4,-2.5,"Regresa al menu");
        glPopMatrix();

        //Sonido texto
        glPushMatrix();
        glColor3ub(0,0,0);
        displayText(0.8,0, "Sonido on/off");
        glPopMatrix();

        //Musica texto
        glPushMatrix();
        glColor3ub(0,0,0);
        displayText(0.8,-1.4, "Musica on/off");
        glPopMatrix();


        //Regresa al menu squads
        glPushMatrix();
        glTranslatef(-3.2,-4.4,-2.2);
        glColor3f(0.8,0.4,0);
        glBegin(GL_QUADS);
        glVertex2d(-0.8, 0.68);
        glVertex2d(1.2,0.68);
        glVertex2d(1.2,0.18);
        glVertex2d(-0.8,0.18);
        glEnd();
        glPopMatrix();


        //Configuraciones imagen
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureConf );
        glPushMatrix();
        glTranslatef(-10,4,-12);
        glColor3f(1,1,1);
        glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2d(0, 0);
        glTexCoord2d(1,0);
        glVertex2d(8, 0);
        glTexCoord2d(1,1);
        glVertex2d(8,8);
        glTexCoord2d(0,1);
        glVertex2d(0,8);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        //Sonido imagen
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureSonido );
        glPushMatrix();
        glTranslatef(-3,-3,-12);
        glColor3f(1,1,1);
        glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2d(0, 0);
        glTexCoord2d(1,0);
        glVertex2d(6, 0);
        glTexCoord2d(1,1);
        glVertex2d(6,6);
        glTexCoord2d(0,1);
        glVertex2d(0,6);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        //Musica imagen
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, textureMusica );
        glPushMatrix();
        glTranslatef(-3,-10.5,-12);
        glColor3f(1,1,1);
        glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex2d(0, 0);
        glTexCoord2d(1,0);
        glVertex2d(6, 0);
        glTexCoord2d(1,1);
        glVertex2d(6,6);
        glTexCoord2d(0,1);
        glVertex2d(0,6);
        glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }

    glutSwapBuffers();//por default invoca al glFlush();
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if (opcion == 1)
        glFrustum (X_MIN, X_MAX, Y_MIN, Y_MAX, 1, 20);
    else
        glOrtho(X_MIN, X_MAX, Y_MIN, Y_MAX, 1, 20);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity() ;
    if (opcion == 1)
      gluLookAt (0.0,0.1, 2.0, 0, 0, 0, 0.0, 1, 0.0);
    else
      gluLookAt (0, 0, 0.0, 0, 0, 0, 0.0, 1, 0.0);

}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch (theKey)
    {
        //Anade las opciones
        case 'Q': case 'q': case 27:
            exit(-1);
    }
}

void myMouse(int button, int state, int x, int y)
{
    if(screen==MENU && button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && x<200 && y>300)
    {
        screen=CONFIG;
    }
    else if(screen==CONFIG && button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && x<200 && y>430 && y<480)
    {
        screen=MENU;
    }
    glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Pantalla principal");
    glutDisplayFunc(display);
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);
    glutReshapeFunc(reshape);
    textureMonst = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\monstruo.png", 179, 290);
    textureHigh = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\high.png", 179, 290);
    textureConf = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\config.png", 256, 256);
    textureSonido = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\sonido.png", 640, 514);
    textureMusica = LoadGLTexture("C:\\Users\\Manu\\Documents\\Studium\\Tec\\Ene - Mayo 14\\Graficas Computacionales\\Proyecto\\MonsterJump\\Imagenes\\musica.png", 256, 256);
    glEnable(GL_DEPTH_TEST);
    screen=MENU;

    inicializa();
    glutMainLoop();
    return EXIT_SUCCESS;
}
