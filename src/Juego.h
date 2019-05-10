#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "Pelota.h"

#include <../glm/glm/vec2.hpp>
#include <../glm/glm/vec3.hpp>


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
        bool facetado = false;

        int contadorFrames = 0;
        bool teclaApretada = false;

        // CAMARA
        float anga=0;
        float angb=0;
        float rad=5;

        float x=0;
        float y=0;
        float z=0;
        float centrox = 2.5;
        float centroy = 5;
        float centroz = 1;

        float angPalo=180;
        float distPalo=0;

        float sens = 0.4;

        bool moverCam=false;
        bool girarPalo=true;

        int camara = 0;// techo

        bool botonIzquierdoApretado=false;

        // PELOTAS
        float radio = 0.16;
        bool movimientoPelotas;
        std::vector<Pelota*> pelotas;
        std::vector<std::vector<bool>> colisiones;

        std::vector< glm::vec3 > vertices;
        std::vector< glm::vec2 > uvs;
        std::vector< glm::vec3 > normals;
        std::vector<unsigned int> vertexIndices;
        std::vector< glm::vec3 > verticesPalo;
        std::vector< glm::vec2 > uvsPalo;
        std::vector< glm::vec3 > normalsPalo;
        std::vector<unsigned int> vertexIndicesPalo;

        GLuint texMesa=0;
        GLuint texPalo=0;

        void shoot();
        void posicionesIniciales();
        void chequearColision(int i, int j);
        void actualizarCam(float x_angle, float y_angle,float radius);
        void actualizarCamaraPalo(float x_angle);
        void dibujarPalo();
        void apretarTecla();
        bool loadObj(const char *path,std::vector<glm::vec3> &out_vertices,std::vector<glm::vec2> &out_uvs,std::vector<glm::vec3> &out_normals,std::vector< unsigned int > &vertexIndices);
        void cargarText(GLuint &texMesa,std::string archivo);
        void dibujarObj(GLuint text,float escalaX,float escalaY,float escalaZ,float translX,float translY,float translZ,std::vector<glm::vec3> vertices,std::vector<glm::vec2> uvs,std::vector<glm::vec3> normals,std::vector<unsigned int> vertexIndices,float angulo,bool alrevez);
};
