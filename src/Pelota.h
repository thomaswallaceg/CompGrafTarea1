#ifndef PELOTA_H
#define PELOTA_H
#include "SDL/SDL_opengl.h"
#include <vector>
#include <string>

class Pelota
{
    public:
        Pelota(int id,float radio,GLuint tex);
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

        bool getMetida();
        void setMetida(bool met);

        void actualizarPosYVel();
        void chequearBordes();
        void dibujarPelota();
        void cargarTextura();
        void guardarRotacion();

    private:
        bool metida = false;
        float radio;
        int id;
        std::vector<double> pos, lastPos, vel, ejeRotacion;
        std::vector<std::vector<double>> rotaciones;
        double angulo = 0;

        GLuint tex;
        int ultimoChoque = -1;

        void drawHalfSphere(int lats, int longs, GLfloat r);
};

#endif // PELOTA_H
