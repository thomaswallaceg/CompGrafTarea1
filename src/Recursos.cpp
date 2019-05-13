#include "Recursos.h"
#include "FreeImage.h"
#include <sstream>
#include <stdio.h>

Recursos::Recursos(){}
Recursos::~Recursos(){}

void Recursos::cargarModelo(int modelo){
    if (modelo==1) {
        bool res2 = loadObj("mod/cue.obj",verticesPalo,uvsPalo,normalesPalo);
        std::string archivoPalo = "tex/poolPalo.jpg";
        loadText(texPalo,archivoPalo);
    } else if (modelo==2) {
        bool res = loadObj("mod/mesa.obj", verticesMesa, uvsMesa, normalesMesa);
        std::string archivoMesa = "tex/mesaAzul.png";
        loadText(texMesa,archivoMesa);
    }
}

void Recursos::dibujarModelo(int modelo){
    if (modelo==1) {
        drawObj(texPalo,verticesPalo,uvsPalo,normalesPalo);
    } else if (modelo==2) {
        drawObj(texMesa,verticesMesa,uvsMesa,normalesMesa);
    }
}

GLuint Recursos::cargarTexturaPelota(int id){
    GLuint res;
    std::stringstream str;
    str << id;
    std::string archivo = "tex/pelotas/" + str.str() + ".png";
    loadText(res,archivo);
    return res;
}

void Recursos::cargarHUD(){
    for (int i=0; i<17; i++){
        GLuint aux;
        texHUD.push_back(aux);
        std::stringstream str;
        str << i;
        std::string archivo = "tex/menus/hud" + str.str() + ".png";
        loadText(texHUD[i],archivo);
    }
}

void Recursos::texturaHUD(int i){
    glBindTexture(GL_TEXTURE_2D,texHUD[i]);
}

void Recursos::drawObj(GLuint text,std::vector<glm::vec3> vertices,std::vector<glm::vec2> uvs,std::vector<glm::vec3> normals){
    glBindTexture(GL_TEXTURE_2D, text);
    glBegin(GL_QUADS);
    for(int i =0;i< vertices.size();i++){
        glTexCoord2d(uvs[i][0],uvs[i][1]);
        glNormal3f(normals[i][0],normals[i][1],normals[i][2]);
        glVertex3f(vertices[i][0],vertices[i][1],vertices[i][2]);
    }
    glEnd();
}

bool Recursos::loadObj(const char *path,std::vector<glm::vec3> &out_vertices,std::vector<glm::vec2> &out_uvs,std::vector<glm::vec3> &out_normals){
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



void Recursos::loadText(GLuint &text,std::string archivo){
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo.c_str());
    FIBITMAP* bitmap = FreeImage_Load(fif, archivo.c_str());
    bitmap = FreeImage_ConvertTo24Bits(bitmap);

    int w = FreeImage_GetWidth(bitmap);
    int h = FreeImage_GetHeight(bitmap);

    void* datos = FreeImage_GetBits(bitmap);

    glGenTextures(1, &text);

    glBindTexture(GL_TEXTURE_2D, text);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, datos);

    delete datos;
}



void Recursos::cargarTextParedes(){
    for (int i=0; i<4; i++){
        GLuint aux;
        texParedes.push_back(aux);
        std::stringstream str;
        str << i;
        std::string archivo = "tex/paredes/pared" + str.str() + ".png";
        loadText(texParedes[i],archivo);
    }
}

void Recursos::texturaParedes(int i){
    glBindTexture(GL_TEXTURE_2D,texParedes[i]);
}
