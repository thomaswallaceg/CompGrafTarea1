#ifndef PELOTA_H
#define PELOTA_H
#include <vector>
using namespace std;

class Pelota
{
    public:
        Pelota(int id);
        virtual ~Pelota();

        vector<double> getPos();
        void setPos(double x, double y);

        vector<double> getVel();
        void setVel(double x, double y);

        void actualizarPosYVel();
        void chequearBordes();
        void dibujarPelota();

    private:
        int id;
        vector<double> pos, vel;

        void drawHalfSphere(int lats, int longs, GLfloat r);
};

#endif // PELOTA_H
