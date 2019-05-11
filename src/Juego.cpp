#include "Juego.h"
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include <math.h>

#define PI 3.14159265
#define TECHO 0
#define PALO 1
#define LIBRE 2
#define MESA 2

Juego::Juego(){
    recursos = new Recursos();
    // PELOTAS
    for (int i=0; i<16;i++){
        GLuint tex = recursos->cargarTexturaPelota(i);        pelotas.push_back(new Pelota(i,radio,tex));
        std::vector<bool> aux;
        colisiones.push_back(aux);
        for (int j=0; j<pelotas.size();j++){
            colisiones[i].push_back(false);
        }
    }

    posicionesIniciales();

}

bool Juego::salir(){
return fin;
}

void Juego::inicializar(){
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glMatrixMode(GL_PROJECTION);
    glClearColor(0,0,0,1);

    gluPerspective(45, 1280/720.f, 0.1, 100);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_NORMALIZE);

    // CARGAR MODELOS
    recursos->cargarModelo(PALO);
    recursos->cargarModelo(MESA);
}

void Juego::mainLoop(){
    if (!mostrarTexturas) glDisable(GL_TEXTURE_2D);
    else glEnable(GL_TEXTURE_2D);

    if (wireframe) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    if (facetado) glShadeModel(GL_FLAT);
    else glShadeModel(GL_SMOOTH);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //HABILITAR LUZ 0
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // habilita la luz 0

    GLfloat luz_0_ambiente[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat luz_0_difusa[]  = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat luz_0_especular[] = {10.0f, 10.0f, 10.0f, 1.0f};
    GLfloat luz_0_posicion[] = {2.5f, 5.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_0_ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_0_difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_0_especular);
    glLightfv(GL_LIGHT0, GL_POSITION, luz_0_posicion);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3f(1,1,1);

    if (camara == LIBRE) {
        actualizarCamaraLibre(anga,angb,rad);
        gluLookAt(x+centrox,y+centroy,z+centroz,centrox,centroy,centroz,0,0,1);
    }
    if (camara == PALO) {
        actualizarCamaraPalo(anga);
        gluLookAt(pelotas[0]->getPos()[0]+x,pelotas[0]->getPos()[1]+y,1,pelotas[0]->getPos()[0],pelotas[0]->getPos()[1],0.5,0,0,1);
    }
    if (camara == TECHO) {
        gluLookAt(2.5,5,10,2.5,5,0,-1,0,0);
    }

    //MESA
    glPushMatrix();
    glTranslatef(2.5,5,-radio);
    glScalef(0.1125,0.1022,0.105);
    glRotatef(90,1,0,0);
    glMateriali(GL_FRONT_AND_BACK, GL_SPECULAR, 0);
    glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, 30);
    glMaterialf(GL_FRONT_AND_BACK, GL_AMBIENT, 100);
    recursos->dibujarModelo(MESA);
    glPopMatrix();

    glBegin(GL_LINES);
        glVertex3f(-2,9.6,0);
        glVertex3f(7,9.6,0);
        glVertex3f(-2,5.32,0);
        glVertex3f(7,5.3,0);
        glVertex3f(-2,0.4,0);
        glVertex3f(7,0.4,0);
    glEnd();

    glMateriali(GL_FRONT_AND_BACK, GL_SPECULAR, 500);
    glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, 70);
    glMaterialf(GL_FRONT_AND_BACK, GL_AMBIENT, 50);

    if (pelotas[0]->getMetida() && !movimientoPelotas){
        pelotas[0]->setMetida(false);
        pelotas[0]->setVel(0,0);
        pelotas[0]->setPos(2.5,2.5);
        pelotas[0]->setUltimoChoque(-1);
        pelotas[0]->setAngulos(0,0);
        pelotas[0]->setLastPos(2.5,2.5);
    }

    movimientoPelotas = false;
    for (int i=0; i < pelotas.size(); i++){
        if (!pelotas[0]->getMetida()){
            if (!pausa) {
                pelotas[i]->actualizarPosYVel();
                if (pelotas[i]->getVel()[0] != 0 || pelotas[i]->getVel()[1] != 0) movimientoPelotas = true;
                pelotas[i]->chequearBordes();
                for (int j=i+1; j < pelotas.size(); j++)
                    chequearColision(i,j);
            }
            pelotas[i]->dibujarPelota(pausa);
        } else {
            for (int k=0; k<pelotas.size(); k++){
                colisiones[i][k]=false;
                colisiones[k][i]=false;
            }
        }
    }

    //PALO
    if (!movimientoPelotas && !pausa && camara!=LIBRE){
        glPushMatrix();
        glTranslatef(pelotas[0]->getPos()[0],pelotas[0]->getPos()[1],0);
        glRotatef(angPalo+180,0,0,1);
        glRotatef(-12,1,0,0);
        glTranslatef(0,-radio-distPalo-0.03,0);
        glScalef(0.5,0.35,0.5);
        glRotatef(-270,1,0,0);
        glMateriali(GL_FRONT_AND_BACK, GL_SPECULAR, 0);
        glMaterialf(GL_FRONT_AND_BACK, GL_DIFFUSE, 30);
        glMaterialf(GL_FRONT_AND_BACK, GL_AMBIENT, 100);
        recursos->dibujarModelo(PALO);
        glPopMatrix();
    }
    // HUD
    viewOrtho();
    /*
    glBegin(GL_TRIANGLES);
        glVertex2f(0,0);
        glVertex2f(0,1280);
        glVertex2f(720,0);
    glEnd();
    */
    viewPerspective();

    glDisable(GL_TEXTURE_2D);

    procesarEntrada();

    SDL_GL_SwapBuffers();
}


///////////////////////////////////// AUXILIARES //////////////////////////////////////////////////////////
void Juego::actualizarCamaraLibre(float x_angle, float y_angle,float radius){
    x = cos(x_angle*PI/180) * cos(y_angle*PI/180) * radius;
    y = -sin(x_angle*PI/180) * cos(y_angle*PI/180) * radius;
    z = sin(y_angle*PI/180) * radius;
}

void Juego::actualizarCamaraPalo(float x_angle){
    x = cos((x_angle+90)*PI/180) * 3;
    y = sin((x_angle+90)*PI/180) * 3;
    z = 1.5;
}

void Juego::chequearColision(int i, int j){
    if (!pelotas[i]->getMetida() && !pelotas[j]->getMetida()){
        double dx = pelotas[i]->getPos()[0] - pelotas[j]->getPos()[0];
        double dy = pelotas[i]->getPos()[1] - pelotas[j]->getPos()[1];
        double dist = hypot(dx, dy);
        if (dist < 2*radio) {
            if ((!colisiones[i][j]) && ((pelotas[i]->getUltimoChoque()!=i) || (pelotas[j]->getUltimoChoque()!=i))){
                pelotas[i]->setUltimoChoque(j);
                pelotas[j]->setUltimoChoque(i);
                colisiones[i][j]=true;
                std::vector<double> normal = {dx/dist, dy/dist};
                std::vector<double> tangente = {-normal[1], normal[0]};
                double vni = pelotas[i]->getVel()[0] * normal[0] + pelotas[i]->getVel()[1] * normal[1];
                double vnj = pelotas[j]->getVel()[0] * normal[0] + pelotas[j]->getVel()[1] * normal[1];
                double vti = pelotas[i]->getVel()[0] * tangente[0] + pelotas[i]->getVel()[1] * tangente[1];
                double vtj = pelotas[j]->getVel()[0] * tangente[0] + pelotas[j]->getVel()[1] * tangente[1];
                pelotas[i]->setVel(normal[0]*vnj + tangente[0]*vti, normal[1]*vnj + tangente[1]*vti);
                pelotas[j]->setVel(normal[0]*vni + tangente[0]*vtj, normal[1]*vni + tangente[1]*vtj);
            }
        } else colisiones[i][j]=false;
    }
}

void Juego::shoot(){
    double velx = cos((angPalo-90)*PI/180);
    double vely = sin((angPalo-90)*PI/180);
    pelotas[0]->setVel((velx*distPalo/2)/3,(vely*distPalo/2)/3);
    distPalo = 0;
}

void Juego::viewOrtho()	{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, 720, 0 , 1280, -1, 1 );
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void Juego::viewPerspective(){
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
    glDepthMask(GL_TRUE);
}

void Juego::posicionesIniciales(){
    for (int i=0; i < 16; i++){
        pelotas[i]->setVel(0,0);
        for (int j=0; j < 16;j++) colisiones[i][j]=false;
    }

    double centroTximpar = 2.5;
    double centroTy = 7.5;
    double espacioy = 0.355;
    double espaciox = 0.41;
    double mult = 0.9;

    double centroTxpar = centroTximpar + (espaciox*mult)/2;

    pelotas[0]->setPos(2.5,2.5);

    pelotas[1] ->setPos(centroTximpar                  , centroTy-2*mult*espacioy);

    pelotas[9] ->setPos(centroTxpar                    , centroTy-mult*espacioy);
    pelotas[10] ->setPos(centroTxpar-mult*espaciox     , centroTy-mult*espacioy);

    pelotas[2] ->setPos(centroTximpar+mult*espaciox    , centroTy);
    pelotas[8] ->setPos(centroTximpar                  , centroTy);
    pelotas[3] ->setPos(centroTximpar-mult*espaciox    , centroTy);

    pelotas[11] ->setPos(centroTxpar+mult*espaciox     , centroTy+mult*espacioy);
    pelotas[7] ->setPos(centroTxpar                    , centroTy+mult*espacioy);
    pelotas[15] ->setPos(centroTxpar-mult*espaciox     , centroTy+mult*espacioy);
    pelotas[12]->setPos(centroTxpar-2*mult*espaciox    , centroTy+mult*espacioy);

    pelotas[4]->setPos(centroTximpar+2*mult*espaciox   , centroTy+2*mult*espacioy);
    pelotas[13]->setPos(centroTximpar+mult*espaciox    , centroTy+2*mult*espacioy);
    pelotas[5]->setPos(centroTximpar                   , centroTy+2*mult*espacioy);
    pelotas[14]->setPos(centroTximpar-mult*espaciox    , centroTy+2*mult*espacioy);
    pelotas[6]->setPos(centroTximpar-2*mult*espaciox   , centroTy+2*mult*espacioy);
}

void Juego::procesarEntrada(){
    int xm,ym;
    SDL_GetMouseState(&xm, &ym);
    while(SDL_PollEvent(&evento)){
        switch(evento.type){
            case SDL_MOUSEBUTTONDOWN:
                if(mouseCam){
                    if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_LEFT)){
                        girarPalo=false;
                        botonIzquierdoApretado=true;
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(mouseCam){
                    if (camara!=LIBRE){
                        if (botonIzquierdoApretado){
                            girarPalo=true;
                            botonIzquierdoApretado=false;
                            if (!movimientoPelotas) shoot();
                            distPalo=0;
                        }
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                if(mouseCam){
                    if (camara==LIBRE){
                            anga+=evento.motion.xrel*sens;//factor de ajuste: 0,4
                            angb-=evento.motion.yrel*sens;//factor de ajuste: 0,4
                            if (angb > 85) angb = 85;
                            if (angb < -85) angb = -85;
                    } else {
                        if(girarPalo){
                            angPalo += evento.motion.xrel*sens;
                            anga=angPalo;
                        } else {
                            distPalo += evento.motion.yrel*sens/3;
                            if(distPalo>=2) distPalo=2;
                            if(distPalo<=0) distPalo=0;
                        }
                    }
                }
                break;
            case SDL_QUIT:
                fin = true;
                break;
            case SDL_KEYDOWN:
                switch(evento.key.keysym.sym){
                    case SDLK_q:
                        fin = true;
                        break;
                    case SDLK_LEFT:
                        anga += 1;
                        if (camara!=LIBRE) angPalo=anga;
                        break;
                    case SDLK_RIGHT:
                        anga -= 1;
                        if (camara!=LIBRE) angPalo=anga;
                        break;
                    case SDLK_UP:
                        angb-=1;
                        if (angb < -80) angb = -80;
                        distPalo -= 0.02;
                        if(distPalo<=0) distPalo=0;
                        break;
                    case SDLK_DOWN:
                        angb+=1;
                        if (angb > 80) angb = 80;
                        distPalo += 0.02;
                        if(distPalo>=2) distPalo=2;
                        break;
                    case SDLK_r:
                        rad-=.05;//factor de ajuste: 0,05
                        if (rad < 0.5) rad = 0.5;
                        break;
                    case SDLK_f:
                        rad+=.05;//factor de ajuste: 0,05
                        if (rad > 12) rad = 12;
                        break;
                    case SDLK_LSHIFT:
                        centroz+=0.1;
                        if (centroz > 10) centroz=10;
                        break;
                    case SDLK_SPACE:
                        centroz-=0.1;
                        if (centroz < 0.5) centroz=0.5;
                        break;
                    case SDLK_w:
                        if (camara==LIBRE){
                            centrox+=0.1;
                            if (centrox > 13) centrox=13;
                        } else {
                            distPalo -= 0.02;
                            if(distPalo<=0) distPalo=0;
                        }
                        break;
                    case SDLK_s:
                        if (camara==LIBRE){
                            centrox-=0.1;
                            if (centrox < -3) centrox=-3;
                        } else {
                            distPalo += 0.02;
                            if(distPalo>=2) distPalo=2;
                        }
                        break;
                    case SDLK_a:
                        if (camara==LIBRE){
                            centroy-=0.1;
                            if (centroy < -3) centroy=-3;
                        } else {
                            anga += 1;
                            angPalo=anga;
                        }
                        break;
                    case SDLK_d:
                        if (camara==LIBRE){
                            centroy+=0.1;
                            if (centroy > 8) centroy=8;
                        } else {
                            anga -= 1;
                            angPalo=anga;
                        }
                        break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(evento.key.keysym.sym){
                        case SDLK_v:
                            camara = (camara+1) % 3;
                            if (camara == LIBRE) {
                                anga=180;
                                angb=35;
                                rad=10;
                                centrox = 2.5;
                                centroy = 5;
                                centroz = 1;
                            }
                            if (camara == PALO) {
                                anga=angPalo;
                            }
                            break;
                        case SDLK_SPACE:
                            if (camara!=LIBRE and !movimientoPelotas){
                                shoot();
                                distPalo=0;
                            }
                            break;
                        case SDLK_p:
                            pausa=!pausa;
                            break;
                        case SDLK_RETURN:
                            posicionesIniciales();
                            break;
                        case SDLK_b:
                            mouseCam=!mouseCam;
                            break;
                        case SDLK_F9:
                            mostrarTexturas = !mostrarTexturas;
                            break;
                        case SDLK_F10:
                            wireframe = !wireframe;
                            break;
                        case SDLK_F11:
                            facetado = !facetado;
                            break;
                }
                break;
            }
    }
}
