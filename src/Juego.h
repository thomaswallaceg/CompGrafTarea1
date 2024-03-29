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
        void dibujarJuego();
        int procesarFisica();
        bool salir();

    private:
        SDL_Event evento;
        Recursos* recursos;

            // AJUSTES
        bool pausa = false;
        bool controles = false;

        bool wireframe = false;
        bool mostrarTexturas = true;
        bool facetado = false;

        bool fin=false;

        // CAMARA
        int camara = 0;// techo
        float anga=180; // �ngulo horizontal
        float angb=0; // �ngulo vertical
        float rad=4; // Radio
        float x=0; // Posici�n c�mara libre
        float y=0; // Posici�n c�mara libre
        float z=0; // Posici�n c�mara libre
        float centrox = 2.5; // Centro c�mara libre
        float centroy = 5; // Posici�n c�mara libre
        float centroz = 1; // Posici�n c�mara libre
        float sens = 0.4; // Sensibilidad
        bool mouseCam=false;
        bool luzAmbiente=true;
        int velocidad; //normal
        bool botonIzquierdoApretado=false;

        void viewOrtho();
        void viewPerspective();
        void actualizarCamaraLibre(float x_angle, float y_angle,float radius);
        void actualizarCamaraPalo(float x_angle);
        void loadParedes();

        // JUEGO
        bool chequearFin=false;

        // PALO
        float angPalo=180;
        float distPalo=0;
        bool girarPalo=true;


        // PELOTAS
        float radio = 0.16;
        bool movimientoPelotas;
        std::vector<Pelota*> pelotas;

        void shoot();
        void posicionesIniciales();
        void chequearColision(int i, int j);

        void procesarEntrada();
};
