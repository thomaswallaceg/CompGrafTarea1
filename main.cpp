#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "FreeImage.h"
#include <math.h>       /* sin */
#include "FreeImage.h"
#include "include/Pelota.h"
#include <chrono>
#include <thread>

#define _USE_MATH_DEFINES
#define FPS 60

void actualizarCam(float &x,float &y, float &z, float x_angle, float y_angle,float radius){
    z = cos(y_angle*M_PI/180) * cos(x_angle*M_PI/180) * radius;
    x = sin(y_angle*M_PI/180) * cos(x_angle*M_PI/180) * radius;
    y = sin(x_angle*M_PI/180) * radius;
}


void drawCubito(float x, float y, float z,float lx,float ly,float lz){
    glBegin(GL_POLYGON);
        glVertex3f(x,y,z);
        glVertex3f(x+lx,y,z);
        glVertex3f(x+lx,y+ly,z);
        glVertex3f(x,y+ly,z);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(x,y,z+lz);
        glVertex3f(x+lx,y,z+lz);
        glVertex3f(x+lx,y+ly,z+lz);
        glVertex3f(x,y+ly,z+lz);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(x,y,z);
        glVertex3f(x,y,z+lz);
        glVertex3f(x,y+ly,z+lz);
        glVertex3f(x,y+ly,z);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(x,y,z);
        glVertex3f(x,y,z+lz);
        glVertex3f(x+lx,y,z+lz);
        glVertex3f(x+lx,y,z);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(x+lx,y,z);
        glVertex3f(x+lx,y,z+lz);
        glVertex3f(x+lx,y+ly,z+lz);
        glVertex3f(x+lx,y+ly,z);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(x,y+ly,z);
        glVertex3f(x,y+ly,z+lz);
        glVertex3f(x+lx,y+ly,z+lz);
        glVertex3f(x+lx,y+ly,z);
    glEnd();
}

void drawMesa(){
    glColor3ub(127,127,127);
    //patas
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    drawCubito(0,0,0,0.5,0.5,0.5);
    drawCubito(4.5,0,0,0.5,0.5,0.5);
    drawCubito(4.5,9.5,0,0.5,0.5,0.5);
    drawCubito(0,9.5,0,0.5,0.5,0.5);

    glColor3ub(139,69,19);

    //mesa
    //tapa
    glColor3ub(34,139,34);
    glBegin(GL_POLYGON);
        glVertex3f(-0.25,-0.25,-2);
        glVertex3f(5.25,-0.25,-2);
        glVertex3f(5.25,10.25,-2);
        glVertex3f(-0.25,10.25,-2);
    glEnd();

    //abajo
    glColor3ub(139,69,19);
    glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
        glVertex3f(5,0,0);
        glVertex3f(5,10,0);
        glVertex3f(0,10,0);
    glEnd();

    //costados
    glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
        glVertex3f(5,0,0);
        glVertex3f(5.25,-0.25,-2);
        glVertex3f(-0.25,-0.25,-2);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(5,10,0);
        glVertex3f(5,0,0);
        glVertex3f(5.25,-0.25,-2);
        glVertex3f(5.25,10.25,-2);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(5,10,0);
        glVertex3f(0,10,0);
        glVertex3f(-0.25,10.25,-2);
        glVertex3f(5.25,10.25,-2);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(0,0,0);
        glVertex3f(0,10,0);
        glVertex3f(-0.25,10.25,-2);
        glVertex3f(-0.25,-0.25,-2);
    glEnd();

    glColor3ub(0,0,0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);


    //borde wireframe
    drawCubito(-0.25,-0.25,-2.25,5.5,10.5,0.25);
    drawCubito(0,0,-2.25,5,10,0.25);

    glColor3ub(139,69,19);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //borde pintado
    drawCubito(-0.25,-0.25,-2.25,5.5,0.25,0.25);
    drawCubito(-0.25,10,-2.25,5.5,0.25,0.25);
    drawCubito(-0.25,-0.25,-2.25,0.25,10.5,0.25);
    drawCubito(5,-0.25,-2.25,0.25,10.5,0.25);
}

void chequearColision(std::vector<Pelota*> pelotas, int i, int j){
    double dx = pelotas[i]->getPos()[0] - pelotas[j]->getPos()[0];
    double dy = pelotas[i]->getPos()[1] - pelotas[j]->getPos()[1];
    double dist = hypot(dx, dy);
    if (dist < 0.4) {
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

float x,y,z;
int main(int argc, char *argv[]) {
    float anga=0;
    float angb=0;
    float angulo=0;
    float rad=-5;
    bool moverCam=false;
    if(SDL_Init(SDL_INIT_VIDEO)<0) {
        cerr << "No se pudo iniciar SDL: " << SDL_GetError() << endl;
        exit(1);
    }
    x=0;
    y=0;
    z=rad;
    atexit(SDL_Quit);

    Uint32 flags = SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_OPENGL;

    if(SDL_SetVideoMode(1280, 720, 32, flags)==NULL) {
        cerr << "No se pudo establecer el modo de video: " << SDL_GetError() << endl;
        exit(1);
    }

    if(SDL_EnableKeyRepeat(10, 10)<0) {
        cerr << "No se pudo establecer el modo key-repeat: " << SDL_GetError() << endl;
        exit(1);
    }glColor3ub(34,139,34);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glMatrixMode(GL_PROJECTION);

    float color = 0;
    glClearColor(color, color, color, 1);

    gluPerspective(45, 1280/720.f, 0.1, 100);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    // CARGAR TEXTURA BOLA
    char* archivo = new char[20];
    archivo = "canon.png";

    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
    FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
    bitmap = FreeImage_ConvertTo24Bits(bitmap);

    int w = FreeImage_GetWidth(bitmap);
    int h = FreeImage_GetHeight(bitmap);

    void* datos = FreeImage_GetBits(bitmap);

    GLuint textura;
    glGenTextures(1, &textura);

    glBindTexture(GL_TEXTURE_2D, textura);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);

    delete datos;

    bool fin=false;
    SDL_Event evento;

    float aspecto = 1024/768.f;

    float escala = 1;

    float pos[] = {0.0,0.0,0.0,1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    //glTranslatef(0.0,0.0,0.0);
    glRotatef(90,1.0,0.0,0.0);


    // VECTOR DE BOLAS
    glColor3f(0.5,0,0);
    std::vector<Pelota*> pelotas;
    for (int i=0; i < 1; i++) pelotas.push_back(new Pelota(i));

    //for (int i=0; i < 19; i++) pelotas[i]->setPos(0.8,0.5+i*0.5);
    //for (int i=19; i < 38; i++) pelotas[i]->setPos(1.4,0.5+(i-19)*0.5);
    //for (int i=38; i < 57; i++) pelotas[i]->setPos(2,0.5+(i-38)*0.5);
    //for (int i=57; i < 76; i++) pelotas[i]->setPos(3,0.5+(i-57)*0.5);
    //for (int i=76; i < 95; i++) pelotas[i]->setPos(4,0.5+(i-76)*0.5);

    //for (int i=0; i < 19; i++) pelotas[i]->setVel(0,1);

    pelotas[0]->setPos(2.5,5);
    pelotas[0]->setVel(0,0);




    glTranslatef(2.5,5,-2.2);

    do{
        auto inicioTimer = chrono::steady_clock::now();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        //gluLookAt(x,y,-z,0,0,0,0,1,0);
        gluLookAt(x,y,-z,2.5,0,5,0,1,0);


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


        glTranslatef(0,0,-0.2);
        glColor3f(0.5,0,0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textura);

        for (int i=0; i < pelotas.size(); i++){
            pelotas[i]->actualizarPosYVel();
            pelotas[i]->chequearBordes();
            for (int j=i+1; j < pelotas.size(); j++)
                chequearColision(pelotas,i,j);
            pelotas[i]->dibujarPelota();
        }

        glPopMatrix();
        glEnd();
        //actualizarCam(x,y,z,angb,anga,rad);
        float ip,jp;
        int xm,ym,xn,yn;
        SDL_GetMouseState(&xm, &ym);
            //semi-esfera
         glDisable(GL_TEXTURE_2D);
        while(SDL_PollEvent(&evento)){
            switch(evento.type){

            case SDL_MOUSEBUTTONDOWN:
                if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(3)){
                    moverCam=true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                moverCam=false;
                break;
            case SDL_MOUSEMOTION:
                if(moverCam){
                    if (evento.motion.yrel<0 && angb < 80 )
                        angb-=evento.motion.yrel*0.4;//factor de ajuste: 0,4
                    else if (evento.motion.yrel>=0 && angb > -80)
                        angb-=evento.motion.yrel*0.4;//factor de ajuste: 0,4
                    anga+=evento.motion.xrel*0.4;//factor de ajuste: 0,4
                    actualizarCam(x,y,z,angb,anga,rad);
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
                    if(angulo<45)
                        angulo += 1;
                    break;
                case SDLK_RIGHT:
                    if(angulo>-45)
                        angulo -= 1;
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

        auto finTimer = chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = finTimer - inicioTimer;
        Sleep(16.6667 - elapsed_seconds.count());
    }while(!fin);
    return 0;
}







////HOLA GIMENA PUEDE PUSHEAR
