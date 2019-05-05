#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_opengl.h"
#include "../include/Pelota.h"
#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;

Pelota::Pelota(int i)
{
    id = i;
    pos=vector<double>(2);
    vel=vector<double>(2);
    vel[0]=vel[1]=pos[0]=pos[1]=-1;
}

Pelota::~Pelota(){}

/* MOVIMIENTO */
void Pelota::dibujarPelota(){
    glPushMatrix();
    glTranslatef(pos[0],pos[1],0);
    drawHalfSphere(10, 10, 0.2);
    glPopMatrix();
    /*
    GLUquadric *quad = gluNewQuadric();
    gluQuadricTexture(quad, true);
    gluSphere(quad,0.2,10,10);
    */
}

void Pelota::actualizarPosYVel(){
    pos[0] = pos[0] + vel[0]/32;
    pos[1] = pos[1] + vel[1]/32;

    //vel[0] = vel[0] * 0.9999;
    //vel[1] = vel[1] * 0.9999;
}

void Pelota::chequearBordes(){
    if (pos[0] <= 0.2 && vel[0] < 0) vel[0]=-vel[0];
    if (pos[0] >= 4.8 && vel[0] > 0) vel[0]=-vel[0];
    if (pos[1] <= 0.2 && vel[1] < 0) vel[1]=-vel[1];
    if (pos[1] >= 9.8 && vel[1] > 0) vel[1]=-vel[1];
}


/* GETTERS Y SETTERS */
vector<double> Pelota::getPos(){
    return pos;
}
void Pelota::setPos(double x, double y){
    pos[0] = x;
    pos[1] = y;
}

vector<double> Pelota::getVel(){
    return vel;
}

void Pelota::setVel(double x, double y){
    vel[0] = x;
    vel[1] = y;
}

void Pelota::drawHalfSphere(int lats, int longs, GLfloat r) {
   int i, j;
    int halfLats = lats;
    for(i = 0; i <= halfLats; i++)
    {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++)
        {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            double s1, s2, t;
            s1 = ((double) i) / halfLats;
            s2 = ((double) i + 1) / halfLats;
            t = ((double) j) / longs;

            glTexCoord2d(s1, t);
            glNormal3d(x * zr0, y * zr0, z0);
            glVertex3d(r*x * zr0, r*y * zr0, r*z0);

            glTexCoord2d(s2, t);
            glNormal3d(x * zr1, y * zr1, z1);
            glVertex3d(r*x * zr1,r* y * zr1, r*z1);
        }
        glEnd();
    }
 }
