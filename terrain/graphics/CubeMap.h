#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include "../ressourceManager/ShaderManager.h"

enum class FACE {RIGHT, LEFT ,TOP , BOTTOM, BACK, FRONT};

class CubeMap
{
    public:
        CubeMap();
        void setFace(FACE face, std::string fileName);
        void draw(glm::mat4 cameraProjection, glm::mat4 cameraView);
        GLuint getID();

    private:
        GLuint VAO;
        GLuint VBO;
        GLuint ID;
        std::shared_ptr<Shader> p_shader;
        std::string textures[6];
};

#endif // CUBEMAP_H
