#include "CubeMap.h"
#include <SFML/Graphics/Image.hpp>
#include <iostream>

CubeMap::CubeMap()
{
    textures[static_cast<int>(FACE::BACK)]   = "";
    textures[static_cast<int>(FACE::BOTTOM)] = "";
    textures[static_cast<int>(FACE::FRONT)]  = "";
    textures[static_cast<int>(FACE::LEFT)]   = "";
    textures[static_cast<int>(FACE::RIGHT)]  = "";
    textures[static_cast<int>(FACE::TOP)]    = "";

    float vertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenTextures(1, &ID);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    p_shader = RessourceManager::ShaderManager::get("media/shader/skybox/skybox.vert", "media/shader/skybox/skybox.frag");
}

void CubeMap::setFace(FACE face, std::string fileName)
{
    textures[static_cast<int>(face)] = fileName;


    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    sf::Image image;
    if(image.loadFromFile(fileName)){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + static_cast<int>(face), 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
    else
        std::cout << "Cubemap failed to load, " << fileName << " doesn't existe" << std::endl;
}

void CubeMap::draw(glm::mat4 cameraProjection, glm::mat4 cameraView)
{
    p_shader->use();
    p_shader->setMat4("proj", cameraProjection);
    glm::mat4 view = glm::mat4(glm::mat3(cameraView));
    p_shader->setMat4("view", view);
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

GLuint CubeMap::getID()
{
    return ID;
}
