#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "FreeImage.h"
#include "../include/Pelota.h"
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#define PI 3.14159265


Pelota::Pelota(int i)
{
    id = i;
    pos=std::vector<double>(2);
    vel=std::vector<double>(2);
    lastPos=std::vector<double>(2);
    angulos=std::vector<double>(2);
    vel[0]=vel[1]=pos[0]=pos[1]=lastPos[0]=lastPos[1]=angulos[0]=angulos[1]=0;
    tex = 0;
    ultimoChoque = -1;
}

Pelota::~Pelota(){}

/* MOVIMIENTO */
void Pelota::dibujarPelota(){
    if (lastPos[0] != pos[0] || lastPos[1] != pos[1]){
        angulos[0] = angulos[0] + 900/3.141592 * (pos[0]-lastPos[0]);
        angulos[1] = angulos[1] + 900/3.141592 * (pos[1]-lastPos[1]);
        while (angulos[0] > 360) angulos[0] = angulos[0] - 360;
        while (angulos[1] > 360) angulos[1] = angulos[1] - 360;
    }
    if (tex==0) cargarTextura();

    glPushMatrix();

    glTranslatef(pos[0],pos[1],0);
    glRotatef(angulos[1],1,0,0);
    glRotatef(-angulos[0],0,1,0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    drawHalfSphere(10, 10, 0.2);

    glPopMatrix();
}

void Pelota::actualizarPosYVel(){
    setLastPos(pos[0],pos[1]);

    pos[0] += vel[0];
    pos[1] += vel[1];

    vel[0] *= 0.99;
    vel[1] *= 0.99;
    vel[0] -= ((vel[0] > 0) - (vel[0] < 0)) * 0.00003;
    vel[1] -= ((vel[1] > 0) - (vel[1] < 0)) * 0.00003;

    if (vel[0] < 0.0001 and vel[0] > -0.0001) vel[0]=0;
    if (vel[1] < 0.0001 and vel[1] > -0.0001) vel[1]=0;
}

void Pelota::chequearBordes(){
    if (pos[0] <= 0.2 && vel[0] < 0) {vel[0]=-vel[0]; ultimoChoque = -1;}
    if (pos[0] >= 4.8 && vel[0] > 0) {vel[0]=-vel[0]; ultimoChoque = -1;}
    if (pos[1] <= 0.2 && vel[1] < 0) {vel[1]=-vel[1]; ultimoChoque = -1;}
    if (pos[1] >= 9.8 && vel[1] > 0) {vel[1]=-vel[1]; ultimoChoque = -1;}
}


/* GETTERS Y SETTERS */
std::vector<double> Pelota::getPos(){
    return pos;
}
void Pelota::setPos(double x, double y){
    pos[0] = x;
    pos[1] = y;
}

std::vector<double> Pelota::getLastPos(){
    return lastPos;
}
void Pelota::setLastPos(double x, double y){
    lastPos[0] = x;
    lastPos[1] = y;
}

std::vector<double> Pelota::getVel(){
    return vel;
}

void Pelota::setVel(double x, double y){
    vel[0] = x;
    vel[1] = y;
}

int Pelota::getUltimoChoque(){
    return ultimoChoque;
}

void Pelota::setUltimoChoque(int i){
    ultimoChoque = i;
}


// AUXILIARES
void Pelota::drawHalfSphere(int lats, int longs, GLfloat r) {
   int i, j;
    int halfLats = lats;
    for(i = 0; i <= halfLats; i++)
    {
        double lat0 = 3.141592 * (-0.5 + (double) (i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = 3.141592 * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++)
        {
            double lng = 2 * 3.141592 * (double) (j - 1) / longs;
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

 void Pelota::cargarTextura(){
     // CARGAR TEXTURA BOLA
    std::stringstream str;
    str << id;
    std::string archivo = "tex/pelotas/" + str.str() + ".png";

    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo.c_str());
    FIBITMAP* bitmap = FreeImage_Load(fif, archivo.c_str());
    bitmap = FreeImage_ConvertTo24Bits(bitmap);

    int w = FreeImage_GetWidth(bitmap);
    int h = FreeImage_GetHeight(bitmap);

    void* datos = FreeImage_GetBits(bitmap);

    GLuint textura;
    glGenTextures(1, &textura);

    glBindTexture(GL_TEXTURE_2D, textura);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);

    tex = textura;

    delete datos;
 }
