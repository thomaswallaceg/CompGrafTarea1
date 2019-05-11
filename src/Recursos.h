#include <vector>
#include <string>
#include "SDL/SDL_opengl.h"
#include <../glm/glm/vec2.hpp>
#include <../glm/glm/vec3.hpp>

class Recursos
{
    public:
        Recursos();
        ~Recursos();
        void cargarModelo(int modelo); // 1:Mesa,2:Palo
        void dibujarModelo(int modelo); // 1:Mesa,2:Palo
        GLuint cargarTexturaPelota(int id);
        void cargarHUD();
        void texturaHUD(int i);

    private:

        std::vector< glm::vec3 > verticesMesa;
        std::vector< glm::vec2 > uvsMesa;
        std::vector< glm::vec3 > normalesMesa;
        GLuint texMesa=0;

        std::vector< glm::vec3 > verticesPalo;
        std::vector< glm::vec2 > uvsPalo;
        std::vector< glm::vec3 > normalesPalo;
        GLuint texPalo=0;

        std::vector<GLuint> texHUD;

        void drawObj(GLuint text,std::vector<glm::vec3> vertices,std::vector<glm::vec2> uvs,std::vector<glm::vec3> normals);
        bool loadObj(const char *path,std::vector<glm::vec3> &out_vertices,std::vector<glm::vec2> &out_uvs,std::vector<glm::vec3> &out_normals);
        void loadText(GLuint &text,std::string archivo);
};
