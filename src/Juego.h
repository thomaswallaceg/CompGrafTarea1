#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "Pelota.h"

#include <../glm/glm/vec2.hpp>
#include <../glm/glm/vec3.hpp>


enum TipoCamara {libre, palo, techo};

class Juego
{
    public:
        Juego();
        ~Juego();
        void inicializar();
        void mainLoop();
        bool salir();

    private:
        bool fin=false;
        SDL_Event evento;

            // AJUSTES
        bool pausa = false;

        bool wireframe = false;
        bool mostrarTexturas = true;

        int contadorFrames = 0;
        bool teclaApretada = false;

        // CAMARA
        float anga=0;
        float angb=0;
        float rad=-5;

        float x=0;
        float y=0;
        float z=rad;
        float centrox = 2.5;
        float centroy = 1;
        float centroz = 12;

        float angPalo=180;
        float distPalo=0;

        float sens = 0.4;

        bool moverCam=false;
        bool girarPalo=true;

        TipoCamara camara = techo;

        bool botonIzquierdoApretado=false;

        // PELOTAS
        bool movimientoPelotas;
        std::vector<Pelota*> pelotas;

        std::vector< glm::vec3 > vertices;
        std::vector< glm::vec2 > uvs;
        std::vector< glm::vec3 > normals;
        std::vector<unsigned int> vertexIndices;

        GLuint texMesa=0;

        void shoot();
        void chequearColision(int i, int j);
        void actualizarCam(float x_angle, float y_angle,float radius);
        void dibujarPalo();
        void apretarTecla();
        bool loadObj(const char *path,std::vector<glm::vec3> &out_vertices,std::vector<glm::vec2> &out_uvs,std::vector<glm::vec3> &out_normals,std::vector< unsigned int > &vertexIndices);

};
