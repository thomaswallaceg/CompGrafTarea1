#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "FreeImage.h"
#include <math.h>
#include "include/Pelota.h"
#include <chrono>
#include <thread>

#define _USE_MATH_DEFINES
#define FPS 60
#define PI 3.14159265


enum TipoCamara {libre, palo, techo};

void dibujarPalo(float angPalo, float &distPalo, double blancaX, double blancaY){
    glPushMatrix();
    glTranslatef(blancaX,blancaY,0);
    glRotatef(angPalo,0,0,1);
    glRotatef(-5,1,0,0);
    glBegin(GL_LINES);
        glVertex3f(0.05,0.4+distPalo,0);
        glVertex3f(0.05,5+distPalo,0);
        glVertex3f(-0.05,+0.4+distPalo,0);
        glVertex3f(-0.05,5+distPalo,0);
    glEnd();
    glColor3ub(255,0,0);
    glBegin(GL_POLYGON);
        glVertex3f(0,0.4+distPalo,0.05);
        glVertex3f(0.05,0.4+distPalo,0);
        glVertex3f(0,0.4+distPalo,-0.05);
        glVertex3f(-0.05,0.4+distPalo,0);
    glEnd();
    glColor3ub(0,0,255);
    glBegin(GL_POLYGON);
        glVertex3f(0,5+distPalo,0.05);
        glVertex3f(0.05,5+distPalo,0);
        glVertex3f(0,5+distPalo,-0.05);
        glVertex3f(-0.05,5+distPalo,0);
    glEnd();

    glPopMatrix();
 }



void actualizarCam(float &x,float &y, float &z, float x_angle, float y_angle,float radius){
    z = cos(y_angle*PI/180) * cos(x_angle*PI/180) * radius;
    x = sin(y_angle*PI/180) * cos(x_angle*PI/180) * radius;
    y = sin(x_angle*PI/180) * radius;
}



void chequearColision(std::vector<Pelota*> pelotas, int i, int j){
    if (!(pelotas[i]->getUltimoChoque() == j && pelotas[j]->getUltimoChoque() == i)) {
        double dx = pelotas[i]->getPos()[0] - pelotas[j]->getPos()[0];
        double dy = pelotas[i]->getPos()[1] - pelotas[j]->getPos()[1];
        double dist = hypot(dx, dy);
        if (dist < 0.4) {
            pelotas[i]->setUltimoChoque(j);
            pelotas[j]->setUltimoChoque(i);
            std::vector<double> normal = {dx/dist, dy/dist};
            std::vector<double> tangente = {-normal[1], normal[0]};
            double vni = pelotas[i]->getVel()[0] * normal[0] + pelotas[i]->getVel()[1] * normal[1];
            double vnj = pelotas[j]->getVel()[0] * normal[0] + pelotas[j]->getVel()[1] * normal[1];
            double vti = pelotas[i]->getVel()[0] * tangente[0] + pelotas[i]->getVel()[1] * tangente[1];
            double vtj = pelotas[j]->getVel()[0] * tangente[0] + pelotas[j]->getVel()[1] * tangente[1];
            pelotas[i]->setVel(normal[0]*vnj + tangente[0]*vti, normal[1]*vnj + tangente[1]*vti);
            pelotas[j]->setVel(normal[0]*vni + tangente[0]*vtj, normal[1]*vni + tangente[1]*vtj);
        }
    }
}

void shoot(Pelota* blanca,float angPalo, float distPalo){
    double velx = cos((angPalo-90)*PI/180);
    double vely = sin((angPalo-90)*PI/180);
    blanca->setVel((velx*distPalo/2)/3,(vely*distPalo/2)/3);
    distPalo = 0;
}

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
    }glColor3ub(34,139,34);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glMatrixMode(GL_PROJECTION);

    float color = 0;
    glClearColor(color, color, color, 1);

    gluPerspective(45, 1280/720.f, 0.1, 100);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    bool fin=false;
    SDL_Event evento;

    float pos[] = {0.0,0.0,0.0,1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    //ANGULOS CAMARA
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

    //BOOLEANOS CAMARA
    bool moverCam=false;
    bool girarPalo=true;

    TipoCamara camara = libre;

    bool botonIzquierdoApretado=false;

    // PELOTAS
    bool movimientoPelotas;

    std::vector<Pelota*> pelotas;

    pelotas.push_back(new Pelota(0));
    for (int i=1; i < 16; i++){
        pelotas.push_back(new Pelota(i));
        pelotas[i]->setVel(0,0);
    }

    pelotas[0]->setPos(2.5,2.5);
    pelotas[1]->setPos(2.5,6.6392);
    pelotas[2]->setPos(2.3,7.0696);
    pelotas[3]->setPos(2.7,7.0696);
    pelotas[4]->setPos(2.1,7.5);
    pelotas[5]->setPos(2.5,7.5);
    pelotas[6]->setPos(2.9,7.5);
    pelotas[7]->setPos(1.9,7.9304);
    pelotas[8]->setPos(2.3,7.9304);
    pelotas[9]->setPos(2.7,7.9304);
    pelotas[10]->setPos(3.1,7.9304);
    pelotas[11]->setPos(1.7,8.3608);
    pelotas[12]->setPos(2.1,8.3608);
    pelotas[13]->setPos(2.5,8.3608);
    pelotas[14]->setPos(2.9,8.3608);
    pelotas[15]->setPos(3.3,8.3608);

    pelotas[0]->setVel(0,0);

    //pelotas.push_back(new Pelota(3));
    //pelotas[0]->setVel(0.1,0.1);

    do{
        auto inicioTimer = std::chrono::steady_clock::now();
        //glTranslatef(2.5,5,-2.2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        if (camara == libre)
            gluLookAt(x+centrox,y+centroy,-z+centroz,centrox,centroy,centroz,0,1,0);
        else if (camara == palo) {
            gluLookAt(pelotas[0]->getPos()[0]+x,1,pelotas[0]->getPos()[1]-z,pelotas[0]->getPos()[0],0.5,pelotas[0]->getPos()[1],0,1,0);
        } else {
            gluLookAt(2.5,10,5,2.5,0,5,1,0,0);
        }

        float pos[] = {0.0,0.0,0.0,1.0};
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        glRotatef(90,1.0,0.0,0.0);   // angle, x-axis, y-axis, z-axis

        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

        // "MESA"
        glColor3ub(34,139,34);
        glBegin(GL_POLYGON);
            glVertex3f(-0.2,-0.2,0);
            glVertex3f(5.2,-0.2,0);
            glVertex3f(5.2,10.2,0);
            glVertex3f(-0.2,10.2,0);
        glEnd();
        glColor3ub(255,255,255);
        glBegin(GL_POLYGON);
            glVertex3f(-0.2,-0.2,2);
            glVertex3f(5.2,-0.2,2);
            glVertex3f(5.2,10.2,2);
            glVertex3f(-0.2,10.2,2);
        glEnd();


        glTranslatef(0,0,-0.2);
        glColor3f(0.5,0,0);

        movimientoPelotas = false;

        for (int i=0; i < pelotas.size(); i++){
            pelotas[i]->actualizarPosYVel();
            if (pelotas[i]->getVel()[0] != 0 || pelotas[i]->getVel()[1] != 0) movimientoPelotas = true;
            pelotas[i]->chequearBordes();
            for (int j=i+1; j < pelotas.size(); j++)
                chequearColision(pelotas,i,j);
            pelotas[i]->dibujarPelota();
        }
        if (!movimientoPelotas && camara!=libre){
            dibujarPalo(angPalo,distPalo,pelotas[0]->getPos()[0],pelotas[0]->getPos()[1]);
        }


        glPopMatrix();
        glEnd();
        float ip,jp;
        int xm,ym,xn,yn;
        SDL_GetMouseState(&xm, &ym);
        glDisable(GL_TEXTURE_2D);
        while(SDL_PollEvent(&evento)){
            switch(evento.type){

            case SDL_MOUSEBUTTONDOWN:
                if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_LEFT)){
                    girarPalo=false;
                    botonIzquierdoApretado=true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (camara!=libre){
                    if (botonIzquierdoApretado){
                        girarPalo=true;
                        botonIzquierdoApretado=false;
                        if (!movimientoPelotas) shoot(pelotas[0],angPalo,distPalo);
                        distPalo=0;
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                if (camara==libre){
                    if(moverCam){
                        if (evento.motion.yrel<0 && angb < 80 )
                            angb-=evento.motion.yrel*sens;//factor de ajuste: 0,4
                        else if (evento.motion.yrel>=0 && angb > -80)
                            angb-=evento.motion.yrel*sens;//factor de ajuste: 0,4
                        anga+=evento.motion.xrel*sens;//factor de ajuste: 0,4
                        actualizarCam(x,y,z,angb,anga,rad);
                    }
                } else {
                    if(girarPalo){
                        angPalo += evento.motion.xrel*sens;
                        anga=angPalo;
                        actualizarCam(x,y,z,angb,anga,rad);
                    } else {
                        distPalo += evento.motion.yrel*sens/3;
                        if(distPalo>=2) distPalo=2;
                        if(distPalo<=0) distPalo=0;
                    }
                }
                break;
            case SDL_QUIT:
                fin = true;
                break;
            case SDL_KEYDOWN:
                switch(evento.key.keysym.sym){
                case SDLK_ESCAPE:
                    fin = true;
                    break;
                case SDLK_LEFT:
                    if (camara!=libre){
                        angPalo+=1;
                    }
                    anga += 1;
                    actualizarCam(x,y,z,angb,anga,rad);
                    break;
                case SDLK_RIGHT:
                    if (camara!=libre){
                        angPalo-=1;
                    }
                    anga -= 1;
                    actualizarCam(x,y,z,angb,anga,rad);
                    break;
                case SDLK_UP:
                        distPalo -= 0.02;
                        if(distPalo<=0) distPalo=0;
                    break;
                case SDLK_DOWN:
                        distPalo += 0.02;
                        if(distPalo>=2) distPalo=2;
                    break;
                case SDLK_s:
                    rad-=.05;//factor de ajuste: 0,05
                    actualizarCam(x,y,z,angb,anga,rad);
                    break;
                case SDLK_w:
                    if(rad<0)
                        rad+=.05;//factor de ajuste: 0,05
                    actualizarCam(x,y,z,angb,anga,rad);
                    break;
                case SDLK_v:
                    if (camara==libre) camara=palo;
                    else if (camara==palo) camara=techo;
                    else camara=libre;

                    if (camara == palo){
                        anga=angPalo;
                        angb=0;
                        distPalo=0;
                    }
                    actualizarCam(x,y,z,angb,anga,rad);
                    break;
                case SDLK_SPACE:
                    if (camara!=libre and !movimientoPelotas){
                        shoot(pelotas[0],angPalo,distPalo);
                        distPalo=0;
                    }
                    break;
                case SDLK_b:
                    moverCam=!moverCam;
                    break;
                case SDLK_q:
                    if (camara==libre && moverCam) {
                        centroy+=0.1;
                        if (centroy > 5) centroy=5;
                    }
                    break;
                case SDLK_a:
                    if (camara==libre && moverCam) {
                        centroy-=0.1;
                        if (centroy < 0) centroy=0;
                    }
                    break;
                case SDLK_l:
                    if (camara==libre && moverCam) {
                        centroz+=0.1;
                        if (centroz > 13) centroz=13;
                    }
                    break;
                case SDLK_j:
                    if (camara==libre && moverCam) {
                        centroz-=0.1;
                        if (centroz < -3) centroz=-3;
                    }
                    break;
                case SDLK_i:
                    if (camara==libre && moverCam) {
                        centrox+=0.1;
                        if (centrox > 8) centrox=8;
                    }
                    break;
                case SDLK_k:
                    if (camara==libre && moverCam) {
                        centrox-=0.1;
                        if (centrox < -3) centrox=-3;
                    }
                    break;

                case SDLK_F11:
                    flags ^= SDL_FULLSCREEN;
                    if(SDL_SetVideoMode(1280, 720, 32, flags)==NULL) {
                        fprintf(stderr, "No se pudo establecer el modo de video: %s\n", SDL_GetError());
                        exit(1);
                    }

                    glMatrixMode(GL_PROJECTION);

                    float color = 0;
                    glClearColor(color, color, color, 1);

                    gluPerspective(45, 1280/720.f, 0.1, 100);

                    glMatrixMode(GL_MODELVIEW);

                    break;

                }
                break;
            }
        }
        SDL_GL_SwapBuffers();

        auto finTimer = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = finTimer - inicioTimer;
        Sleep(16.6667 - elapsed_seconds.count());
//        std::this_thread::sleep_for(1);
    }while(!fin);
    return 0;
}
