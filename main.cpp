#include <iostream>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "FreeImage.h"
#include <math.h>
#include "include/Pelota.h"
#include <chrono>
#include <thread>
#include <.././glm/glm/vec3.hpp>
#include <.././glm/glm/vec2.hpp>

#define _USE_MATH_DEFINES
#define FPS 60

 //PRUEBA OBJETOS

/*bool loadObj(const char * path,std::vector < glm::vec3 > & out_vertices,std::vector < glm::vec2 > & out_uvs,std::vector < glm::vec3 > & out_normals){
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
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
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
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
*/
/*GLuint elephant;

void loadObj(char *fname)
{
    FILE *fp;
    int read;
    GLfloat x, y, z;
    char ch;
    elephant=glGenLists(1);
    fp=fopen(fname,"r");
    if (!fp)
    {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    glPointSize(2.0);
    glNewList(elephant, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_POINTS);
        while(!(feof(fp)))
        {
            read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
            if(read==4&&ch=='v')
            {
                glVertex3f(x,y,z);
            }
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
    fclose(fp);
}

*/
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
    if (pelotas[i]->getUltimoChoque() != j && pelotas[j]->getUltimoChoque() != i) {
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
    pelotas.push_back(new Pelota(10));
    pelotas[0]->setPos(2.5,5);
    pelotas[0]->setVel(0.5,0);

    /*
    for (int i=0; i < 16; i++){
        pelotas.push_back(new Pelota(i));
        pelotas[i]->cargarTextura();
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

    pelotas[0]->setVel(0,1);
    */
   /*     std::vector< glm::vec3 > vertices;
        std::vector< glm::vec2 > uvs;
        std::vector< glm::vec3 > normals; // No las usaremos por ahora
        //bool res = loadObj("mesa/mesa.obj", vertices, uvs, normals);
        loadObj("mesa/mesa.obj");*/
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








        for (int i=0; i < pelotas.size(); i++){
            pelotas[i]->actualizarPosYVel();
            pelotas[i]->chequearBordes();
            for (int j=i+1; j < pelotas.size(); j++)
                chequearColision(pelotas,i,j);
            pelotas[i]->dibujarPelota();
        }


        //PRUEBA IMPORT OBJETOS

       // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

   /*     glEnableVertexAttribArray(attribute_v_coord);
  // Describe our vertices array to OpenGL (it can't guess its format automatically)
  glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
  glVertexAttribPointer(
    attribute_v_coord,  // attribute
    4,                  // number of elements per vertex, here (x,y,z,w)
    GL_FLOAT,           // the type of each element
    GL_FALSE,           // take our values as-is
    0,                  // no extra data between each position
    0                   // offset of first element
  );

  glEnableVertexAttribArray(attribute_v_normal);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
  glVertexAttribPointer(
    attribute_v_normal, // attribute
    3,                  // number of elements per vertex, here (x,y,z)
    GL_FLOAT,           // the type of each element
    GL_FALSE,           // take our values as-is
    0,                  // no extra data between each position
    0                   // offset of first element
  );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_mesh_elements);
  int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
*/

        //PRUEBA IMPORT OBJETOS

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
