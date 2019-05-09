#include "Juego.h"
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "FreeImage.h"
#include <math.h>

#define PI 3.14159265

Juego::Juego(){
    // PELOTAS
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

    float pos[] = {0.0,0.0,0.0,1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);

    bool res = loadObj("mod/pool.obj", vertices, uvs, normals,vertexIndices);



}

void Juego::mainLoop(){
    if (!mostrarTexturas) glDisable(GL_TEXTURE_2D);
    else glEnable(GL_TEXTURE_2D);

    if (wireframe) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    else glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /*
    GLfloat luz_posicion[]  = {2.5f, 10.0f, 5.0f, 1.0f};
    GLfloat luz_ambiente[]  = {20.0f, 20.0f, 20.0f, 1.0f};
    GLfloat luz_difusa[]    = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat luz_especular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat luz_direccion[] = {0.0f, 0.0f, -1.0f};

    //HABILITAR LUZ 0
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0); // habilita la luz 0
    glEnable(GL_LIGHT1); // habilita la luz 1
    glEnable(GL_LIGHT2); // habilita la luz 1
    glEnable(GL_LIGHT3); // habilita la luz 1
    glEnable(GL_LIGHT4); // habilita la luz 1
    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion);
    glLightfv(GL_LIGHT2, GL_POSITION, luz_difusa);
    glLightfv(GL_LIGHT3, GL_POSITION, luz_especular);
    glLightfv(GL_LIGHT4, GL_POSITION, luz_direccion);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    */

    if (camara == libre)
        gluLookAt(x+centrox,y+centroy,-z+centroz,centrox,centroy,centroz,0,1,0);
    else if (camara == palo) {
        gluLookAt(pelotas[0]->getPos()[0]+x,1,pelotas[0]->getPos()[1]-z,pelotas[0]->getPos()[0],0.5,pelotas[0]->getPos()[1],0,1,0);
    } else {
        gluLookAt(2.5,10,5,2.5,0,5,1,0,0);
    }

    glRotatef(90,1.0,0.0,0.0);   // angle, x-axis, y-axis, z-axis

    if (texMesa==0) {

    std::string archivo = "tex/pool.jpg";

    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo.c_str());
    FIBITMAP* bitmap = FreeImage_Load(fif, archivo.c_str());
    bitmap = FreeImage_ConvertTo24Bits(bitmap);

    int w = FreeImage_GetWidth(bitmap);
    int h = FreeImage_GetHeight(bitmap);

    void* datos = FreeImage_GetBits(bitmap);

    glGenTextures(1, &texMesa);

    glBindTexture(GL_TEXTURE_2D, texMesa);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);

    delete datos;
    }


    glPushMatrix();
//    glDisable(GL_TEXTURE_2D);
    glTranslatef(2.2,5,4);
    glRotatef(180,1,0,0);
    glScalef(0.048,0.048,0.048);
    //glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, texMesa);
    glBegin(GL_QUADS);
    for(int i =0;i< vertexIndices.size();i++){
        glTexCoord2d(uvs[i][0],uvs[i][1]);
        glNormal3f(normals[i][0],normals[i][1],normals[i][2]);
        glVertex3f(vertices[i][0],vertices[i][1],vertices[i][2]);
    }
    glEnd();
//    glEnable(GL_TEXTURE_2D);
    glPopMatrix();

    glColor3f(1,1,1);
    movimientoPelotas = false;

    for (int i=0; i < pelotas.size(); i++){
        pelotas[i]->actualizarPosYVel();
        if (pelotas[i]->getVel()[0] != 0 || pelotas[i]->getVel()[1] != 0) movimientoPelotas = true;
        pelotas[i]->chequearBordes();
        for (int j=i+1; j < pelotas.size(); j++)
            chequearColision(i,j);
        pelotas[i]->dibujarPelota();
    }
    glDisable(GL_TEXTURE_2D);
    if (!movimientoPelotas && camara!=libre){
        dibujarPalo();
    }
    // "MESA"
    glTranslatef(0,0,0.2);
    glColor3ub(34,139,34);
    glNormal3f(0,1,0);
    glBegin(GL_POLYGON);
        glVertex3f(-0.2,-0.2,0);
        glVertex3f(5.2,-0.2,0);
        glVertex3f(5.2,10.2,0);
        glVertex3f(-0.2,10.2,0);
    glEnd();
    glColor3ub(127,127,127);
    glBegin(GL_POLYGON);
        glVertex3f(-0.2,-0.2,2);
        glVertex3f(5.2,-0.2,2);
        glVertex3f(5.2,10.2,2);
        glVertex3f(-0.2,10.2,2);
    glEnd();


    glPopMatrix();
    glEnd();

    int xm,ym;
    SDL_GetMouseState(&xm, &ym);
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
                    if (!movimientoPelotas) shoot();
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
                    actualizarCam(angb,anga,rad);
                }
            } else {
                if(girarPalo){
                    angPalo += evento.motion.xrel*sens;
                    anga=angPalo;
                    actualizarCam(angb,anga,rad);
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
            if (teclaApretada) {
                contadorFrames++;
                if (contadorFrames >= 10) teclaApretada=false;
            }
            switch(evento.key.keysym.sym){
                case SDLK_q:
                    fin = true;
                    break;
                case SDLK_LEFT:
                    if (camara!=libre){
                        angPalo+=1;
                    }
                    anga += 1;
                    actualizarCam(angb,anga,rad);
                    break;
                case SDLK_RIGHT:
                    if (camara!=libre){
                        angPalo-=1;
                    }
                    anga -= 1;
                    actualizarCam(angb,anga,rad);
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
                    actualizarCam(angb,anga,rad);
                    break;
                case SDLK_w:
                    if(rad<0)
                        rad+=.05;//factor de ajuste: 0,05
                    actualizarCam(angb,anga,rad);
                    break;
                case SDLK_v:
                    if (!teclaApretada){
                        if (camara==libre) camara=palo;
                        else if (camara==palo) camara=techo;
                        else camara=libre;

                        if (camara == palo){
                            anga=angPalo;
                            angb=0;
                            distPalo=0;
                        }
                        actualizarCam(angb,anga,rad);
                        apretarTecla();
                    }
                    break;
                case SDLK_SPACE:
                    if (camara!=libre and !movimientoPelotas){
                        shoot();
                        distPalo=0;
                    }
                    break;
                case SDLK_b:
                    if (!teclaApretada){
                        moverCam=!moverCam;
                        apretarTecla();
                    }
                    break;
                case SDLK_e:
                    if (camara==libre && moverCam) {
                        centroy+=0.1;
                        if (centroy > 5) centroy=5;
                    }
                    break;
                case SDLK_d:
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
                case SDLK_p:
                    if (!teclaApretada){
                        pausa=!pausa;
                        apretarTecla();
                    }
                    break;

                case SDLK_F9:
                    if (!teclaApretada){
                        mostrarTexturas = !mostrarTexturas;
                        apretarTecla();
                    }
                    break;
                case SDLK_F10:
                    if (!teclaApretada){
                        wireframe = !wireframe;
                        apretarTecla();
                    }
                    break;
                }
                break;
        }
    }
    SDL_GL_SwapBuffers();
}

void Juego::apretarTecla(){
    teclaApretada=true;
    contadorFrames=0;
}

void Juego::dibujarPalo(){
    glPushMatrix();
    glTranslatef(pelotas[0]->getPos()[0],pelotas[0]->getPos()[1],0);
    glRotatef(angPalo,0,0,1);
    glRotatef(-5,1,0,0);
    glBegin(GL_LINES);
        glVertex3f(0.05,0.4+distPalo,0);
        glVertex3f(0.05,5+distPalo,0);
        glVertex3f(-0.05,+0.4+distPalo,0);
        glVertex3f(-0.05,5+distPalo,0);
    glEnd();
    glColor3ub(255,255,255);
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



void Juego::actualizarCam(float x_angle, float y_angle,float radius){
    z = cos(y_angle*PI/180) * cos(x_angle*PI/180) * radius;
    x = sin(y_angle*PI/180) * cos(x_angle*PI/180) * radius;
    y = sin(x_angle*PI/180) * radius;
}



void Juego::chequearColision(int i, int j){
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

void Juego::shoot(){
    double velx = cos((angPalo-90)*PI/180);
    double vely = sin((angPalo-90)*PI/180);
    pelotas[0]->setVel((velx*distPalo/2)/3,(vely*distPalo/2)/3);
    distPalo = 0;
}


bool Juego::loadObj(const char *path,std::vector<glm::vec3> &out_vertices,std::vector<glm::vec2> &out_uvs,std::vector<glm::vec3> &out_normals, std::vector< unsigned int > &vertexIndices){
    std::vector< unsigned int > uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file !\n");
        return false;
    }
    while( 1 ){

        char lineHeader[128];
        // Lee la primera palabra de la línea
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File, es decir, el final del archivo. Se finaliza el ciclo.
        else if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]  );
            if (matches != 12){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            vertexIndices.push_back(vertexIndex[3]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            uvIndices.push_back(uvIndex[3]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            normalIndices.push_back(normalIndex[3]);
        }
    }
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
        out_vertices.push_back(vertex);
    }
    for( unsigned int i=0; i<uvIndices.size(); i++ ){
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = temp_uvs[ uvIndex-1 ];
        out_uvs.push_back(uv);
    }
    for( unsigned int i=0; i<normalIndices.size(); i++ ){
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[ normalIndex-1 ];
        out_normals.push_back(normal);
    }
}

