/*
Noé Aubin-Cadot.
Codé en novembre 2008.
Adapté pour SDL2 en juin 2017.
Déposé sur GitHub en septembre 2019.
C'est une simulation de l'équation d'onde 2D par une surface qui ondule transversalement.
*/
// Pour compiler :
// gcc -o main main.c -framework SDL2 -framework SDL2_image -framework OpenGL -framework GLUT -Wno-deprecated
// ou encore :
// gcc $(sdl2-config --cflags) -Wall -o main  main.c $(sdl2-config --libs) -lSDL2_image -framework OpenGL -framework GLUT -Wno-deprecated

// Pour C :
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// Pour SDL :
#include <SDL2/SDL.h>
//#include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_image.h>

// Pour OpenGL :
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

void Dessiner_sin(double *EQonde);

double angleX = 0;
double angleY = 0;
double angleZ = 170;
double vueX = 25.0;
double vueY = 25.0;
double vueZ = 25.0;
int L=100;

SDL_Window *fenetre;
SDL_GLContext contexte;

int main(int argc, char *argv[])
{
    // Initialisation de SDL
    SDL_Init(SDL_INIT_VIDEO);
    
    // Création d'une fenêtre
    fenetre = SDL_CreateWindow("Onde transversale 2D par l'equation d'onde usuelle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
    contexte = SDL_GL_CreateContext(fenetre);
    SDL_SetWindowResizable(fenetre,SDL_TRUE);
	
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)640/480,1,1000);

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
    
    Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;
	
	int i,j;
	int M=L;
	int N=L;
	int TOURNE=0;
	int ONDEgo=1;
	double eps=0.01;
	double *EQonde0;EQonde0=(double *) malloc((M*N)*sizeof(double));
	double *EQonde1;EQonde1=(double *) malloc((M*N)*sizeof(double));
	double *EQonde2;EQonde2=(double *) malloc((M*N)*sizeof(double));
	for (i=0;i<M;i++)
	{
		for (j=0;j<N;j++)
		{
			EQonde0[i*M+j]=sin(2*M_PI*i/(N-1))*sin(2*M_PI*j/(M-1));
			EQonde1[i*M+j]=sin(2*M_PI*i/(N-1))*sin(2*M_PI*j/(M-1));
			EQonde2[i*M+j]=sin(2*M_PI*i/(N-1))*sin(2*M_PI*j/(M-1));
		}
	}
	
	SDL_Event event;
	while (1)
    {
        start_time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
					exit(0);
					break;
				case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
				{
                    case SDLK_ESCAPE: // Veut arreter le jeu
                        exit(0);
						break;
					case SDLK_SPACE: // Stop ou arrete le fait que le plan d'onde tourne
                        {int EVENT=0;
						if (TOURNE)
							{TOURNE=0;EVENT=1;}
						if (!TOURNE && !EVENT)
							{TOURNE=1;}
						break;
						}
					case SDLK_RETURN: // Stop ou arrete le fait que l'onde Žvolue
                        {int EVENT=0;
						if (ONDEgo)
							{ONDEgo=0;EVENT=1;}
						if (!ONDEgo && !EVENT)
							{ONDEgo=1;}
						break;
						}
					case SDLK_UP:
						vueX=(3.0/4.0)*vueX;
						vueY=(3.0/4.0)*vueY;
						vueZ=(3.0/4.0)*vueZ;
						break;
					case SDLK_DOWN:
						vueX=(4.0/3.0)*vueX;
						vueY=(4.0/3.0)*vueY;
						vueZ=(4.0/3.0)*vueZ;
						break;
					case SDLK_RIGHT:
						angleZ-=30.0;
						break;
					case SDLK_LEFT:
						angleZ+=30.0;
						break;
					case SDLK_q:
						angleX+=10.0;
						break;
					case SDLK_w:
						angleY+=10.0;
						break;
					case SDLK_e:
						angleZ+=10.0;
						break;
				}
                break;
            }
        }
        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;
		
		if (TOURNE)
		{
			angleZ += 0.01 * ellapsed_time;
		}
        //angleY += 0.01 * ellapsed_time;
		//eangleX += 0.01 * ellapsed_time;
		
        Dessiner_sin(EQonde0);
		if (ONDEgo)
		{
			for (i=1;i<M-1;i++)
			{
				for (j=1;j<N-1;j++)
				{
					EQonde2[i*M+j]=EQonde1[i*M+j];
					EQonde1[i*M+j]=EQonde0[i*M+j];
				}
			}
			for (i=1;i<M-1;i++)
			{
				for (j=1;j<N-1;j++)
				{
					EQonde0[i*M+j]=2*EQonde1[i*M+j]-EQonde2[i*M+j]+eps*(-4*EQonde1[i*M+j]+ EQonde1[(i+1)*M+j]+ EQonde1[(i-1)*M+j]+ EQonde1[i*M+j+1]+ EQonde1[i*M+j-1]);
				}
			}
		}
        ellapsed_time = SDL_GetTicks() - start_time;
        if (ellapsed_time < 10)
        {
            SDL_Delay(10 - ellapsed_time);
        }
    }
    return 0;
}

void Dessiner_sin(double *EQonde)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

	int M=L;
	int N=L;
    gluLookAt(vueX,vueY,vueZ,0,0,0,0,0,1);

    glRotated(angleZ,0,0,1);
    glRotated(angleY,0,1,0);
	glRotated(angleX,1,0,0);
	
    glBegin(GL_QUADS);
	int i,j;
	for (i=0;i<M-1;i++)
	{
		for (j=0;j<N-1;j++)
		{
			glColor3ub(255*(fabs(atan(EQonde[i*M+j]))),100*sin(M_PI*i/10)+40-N,255*sin(M_PI*i/10));
			
			glVertex3d((double)i-M/2,(double)j-N/2,20.0*EQonde[i*M+j]);
			glVertex3d((double)i-M/2,(double)(j+1)-N/2,20.0*EQonde[i*M+j+1]);
			glVertex3d((double)(i+1)-M/2,(double)(j+1)-N/2,20.0*EQonde[(i+1)*M+j+1]);
			glVertex3d((double)(i+1)-M/2,(double)j-N/2,20.0*EQonde[(i+1)*M+j]);
		}
	}
	glPushMatrix();
	glColor3f(255, 255, 255);
	glRasterPos2f(0,0);
	glPopMatrix();
	glutSwapBuffers();

	glEnd();
    glFlush();
    SDL_GL_SwapWindow(fenetre);
}