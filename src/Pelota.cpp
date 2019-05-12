#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "Pelota.h"
#include <iostream>
#include <math.h>

#define PI 3.14159265


Pelota::Pelota(int id,float radio,GLuint tex)
{
    this->id = id;
    this->radio = radio;
    this->tex = tex;
    pos=std::vector<double>(2);
    vel=std::vector<double>(2);
    lastPos=std::vector<double>(2);
    ejeRotacion=std::vector<double>(2);
    vel[0]=vel[1]=pos[0]=pos[1]=lastPos[0]=lastPos[1]=ejeRotacion[0]=ejeRotacion[1]=0;
    ultimoChoque = -1;
}

Pelota::~Pelota(){}

/* MOVIMIENTO */
void Pelota::dibujarPelota(){
    glPushMatrix();
    glTranslatef(pos[0],pos[1],0);
    glRotatef(angulo,ejeRotacion[0],ejeRotacion[1],0);
    for (int i=0;i<rotaciones.size();i++)
        glRotatef(rotaciones[i][0],rotaciones[i][1],rotaciones[i][2],0);
    glBindTexture(GL_TEXTURE_2D, tex);
    drawHalfSphere(30, 30, radio);
    glPopMatrix();
}

void Pelota::actualizarPosYVel(){
    // POSICION
    setLastPos(pos[0],pos[1]);
    setPos(pos[0]+vel[0],pos[1]+vel[1]);

    // VELOCIDAD
    double normVel=hypot(vel[0],vel[1]);
    normVel *= 0.995;
    normVel -= 0.000003;
    if (normVel <= 0) vel[0]=vel[1]=0;
    else {
        double aux = hypot(vel[0],vel[1]);
        setVel(vel[0]*(normVel/aux),vel[1]*(normVel/aux));
    }

    // GIRO
    if (lastPos[0] != pos[0] || lastPos[1] != pos[1]){
        ejeRotacion={lastPos[1] - pos[1], pos[0] - lastPos[0]};
        double dist = hypot(pos[0]-lastPos[0],pos[1]-lastPos[1]);
        angulo += (180/(radio * PI)) * dist;
    }

}

void Pelota::chequearBordes(){
    if (pos[0] <= radio && vel[0] < 0) {
        if (pos[1] < 5.32 && pos[1] > 4.68) metida=true;
        vel[0]=-vel[0];
        ultimoChoque = -1;
        guardarRotacion();
    }
    if (pos[0] >= 5-radio && vel[0] > 0) {
        if (pos[1] < 5.32 && pos[1] > 4.68) metida=true;
        vel[0]=-vel[0];
        ultimoChoque = -1;
        guardarRotacion();
    }
    if (pos[1] <= radio && vel[1] < 0) {vel[1]=-vel[1]; ultimoChoque = -1;guardarRotacion();}
    if (pos[1] >= 10-radio && vel[1] > 0) {vel[1]=-vel[1]; ultimoChoque = -1;guardarRotacion();}

    if ((pos[0] <= radio+0.2 && pos[1] <= radio+0.2)   || (pos[0] <= radio+0.2 && pos[1] >= 9.8-radio) ||
        (pos[0] >= 4.8-radio && pos[1] <= radio+0.2) || (pos[0] >= 4.8-radio && pos[1] >= 9.8-radio))
            metida = true;
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

bool Pelota::getMetida(){
    return metida;
}

void Pelota::setMetida(bool met){
    metida = met;
}


// AUXILIARES
void Pelota::drawHalfSphere(int lats, int longs, GLfloat r) {
   int i, j;
    int halfLats = lats;
    for(i = 0; i <= halfLats; i++)
    {
        double lat0 = PI * (-0.5 + (double) (i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++)
        {
            double lng = 2 * PI * (double) (j - 1) / longs;
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

void Pelota::guardarRotacion(){
    std::vector<double> aux = {angulo, ejeRotacion[0], ejeRotacion[1]};
    rotaciones.insert(rotaciones.begin(),aux);
    angulo=0;
}
