#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "Pelota.h"
#include "Recursos.h"

class Juego
{
    public:
        Juego();
        ~Juego();
        void inicializar();
        void mainLoop();
        bool salir();

    private:
        SDL_Event evento;
        Recursos* recursos;

            // AJUSTES
        bool pausa = false;

        bool wireframe = false;
        bool mostrarTexturas = true;
        bool facetado = false;

        bool fin=false;

        // CAMARA
        int camara = 0;// techo
        float anga=180; // �ngulo horizontal
        float angb=0; // �ngulo vertical
        float rad=5; // Radio
        float x=0; // Posici�n c�mara libre
        float y=0; // Posici�n c�mara libre
        float z=0; // Posici�n c�mara libre
        float centrox = 2.5; // Centro c�mara libre
        float centroy = 5; // Posici�n c�mara libre
        float centroz = 1; // Posici�n c�mara libre
        float sens = 0.4; // Sensibilidad
        bool mouseCam=false;
        bool luzAmbiente=true;

        // PALO
        float angPalo=180;
        float distPalo=0;
        bool girarPalo=true;

        bool botonIzquierdoApretado=false;

        // PELOTAS
        float radio = 0.16;
        bool movimientoPelotas;
        std::vector<Pelota*> pelotas;
        std::vector<std::vector<bool>> colisiones;

        void viewOrtho();
        void viewPerspective();
        void shoot();
        void procesarEntrada();
        void posicionesIniciales();
        void chequearColision(int i, int j);
        void actualizarCamaraLibre(float x_angle, float y_angle,float radius);
        void actualizarCamaraPalo(float x_angle);
};
