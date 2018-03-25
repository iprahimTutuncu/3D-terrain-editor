#include "Mesh.h"

#include <glm/gtc/type_ptr.hpp>

using namespace std;

Mesh::Mesh(const std::vector<Vertex> vertices, const std::vector<GLuint> indices, std::vector<Texture> textures, float shininess, glm::vec3 emission)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->shininess = shininess;
    this->emission = emission;
    this->init();

}


void Mesh::init()
{
    /// gestion du Vertex Buffer Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    std::size_t verticesSize = vertices.size() * sizeof(vertices[0]);

    std::size_t positionSize   = sizeof(vertices[0].pos);
    std::size_t normalSize     = sizeof(vertices[0].normal);
    std::size_t texCoordSize   = sizeof(vertices[0].texCoord);

    std::size_t vertexSize     = sizeof(vertices[0]);
    std::size_t counter        = 0;

    glBufferData(GL_ARRAY_BUFFER,
                 verticesSize,
                 nullptr,
                 GL_STATIC_DRAW);

    for(Vertex &vertex: vertices){
        //positions des vertex
        glBufferSubData(GL_ARRAY_BUFFER,
                        counter * vertexSize,
                        positionSize,
                        glm::value_ptr(vertex.pos));

        //normales des vertex
        glBufferSubData(GL_ARRAY_BUFFER,
                        counter * vertexSize + positionSize,
                        normalSize,
                        glm::value_ptr(vertex.normal));

        //coordonnée des textures des vertex
        glBufferSubData(GL_ARRAY_BUFFER,
                        counter * vertexSize + normalSize + positionSize,
                        texCoordSize,
                        glm::value_ptr(vertex.texCoord));

        counter++;
    }

    /// gestion du Vertex Array Object



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void *)(positionSize));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void *)(positionSize + normalSize));
    glEnableVertexAttribArray(2);

    /// gestion du Element Buffer Object

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::draw(Shader &shader, GLenum primitiveType)
{
    shader.use();


    for(unsigned short i = 0; i < textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        string name = textures[i].type;
        shader.setInt("material." + name, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }

    shader.setVec3("material.emission", emission);
    shader.setFloat("material.shininess", shininess);

    glBindVertexArray(VAO);
    glDrawElements(primitiveType, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);

}

void Mesh::setEmissionColor(glm::vec3 color)
{
    emission = color;
}

Mesh::~Mesh()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VAO);
}
