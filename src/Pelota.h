#ifndef PELOTA_H
#define PELOTA_H
#include "SDL/SDL_opengl.h"
#include <vector>
#include <string>

class Pelota
{
    public:
        Pelota(int id,float radio);
        virtual ~Pelota();

        std::vector<double> getPos();
        void setPos(double x, double y);

        std::vector<double> getLastPos();
        void setLastPos(double x, double y);

        std::vector<double> getAngulos();
        void setAngulos(double x, double y);

        std::vector<double> getVel();
        void setVel(double x, double y);

        int getUltimoChoque();
        void setUltimoChoque(int i);

        void actualizarPosYVel();
        void chequearBordes();
        void dibujarPelota(bool pausa);
        void cargarTextura();

    private:
        float radio;
        int id;
        std::vector<double> pos, lastPos, vel, angulos;
        GLuint tex;
        int ultimoChoque;

        void drawHalfSphere(int lats, int longs, GLfloat r);
};

#endif // PELOTA_H
