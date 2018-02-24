#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <vector>
#include <string>

#include "Shader.h"

struct Vertex{
    Vertex(glm::vec3 pos, glm::vec2 texCoord, glm::vec3 normal){
        this->pos = pos;
        this->normal = normal;
        this->texCoord = texCoord;
    }
    Vertex(){};

    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture{
    GLuint ID;
    std::string type;
    aiString path;
};

class Mesh
{
    public:
        Mesh(const std::vector<Vertex> vertices,const std::vector<GLuint> indices, std::vector<Texture> textures, float shininess = 32, glm::vec3 emission = glm::vec3(0.0));

        void init();
        void draw(Shader &shader, GLenum primitiveType = GL_TRIANGLES);
        void setEmissionColor(glm::vec3 color);
        ~Mesh();

    private:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        float shininess;
        glm::vec3 emission;

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
};

#endif // MESH_H
