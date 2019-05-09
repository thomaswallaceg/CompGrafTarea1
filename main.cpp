#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "src/Pelota.h"
#include "src/Juego.h"
#include <chrono>

#define FPS 60


int main(int argc, char *argv[]) {
    //SDL
    if(SDL_Init(SDL_INIT_VIDEO)<0) {
        std::cerr << "No se pudo iniciar SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }
    atexit(SDL_Quit);

    Uint32 flags = SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_OPENGL;

    if(SDL_SetVideoMode(1280, 720, 32, flags)==NULL) {
        std::cerr << "No se pudo establecer el modo de video: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if(SDL_EnableKeyRepeat(10, 10)<0) {
        std::cerr << "No se pudo establecer el modo key-repeat: " << SDL_GetError() << std::endl;
        exit(1);
    }

    Juego *juego = new Juego();
    juego->inicializar();
    do{
        auto inicio = std::chrono::steady_clock::now();
        juego->mainLoop();

        std::chrono::duration<double,std::micro> transcurrido = std::chrono::steady_clock::now() - inicio;
        while (transcurrido < std::chrono::duration<double,std::micro>(1000000/60)) {
            transcurrido = std::chrono::steady_clock::now() - inicio;
            printf("%d   %d\n",transcurrido,std::chrono::duration<double,std::micro>(1000000/60));
        }
        //Sleep(16.6667 - elapsed_seconds.count());
        //std::this_thread::sleep_for(1);
    }while(!juego->salir());
    return 0;
}
