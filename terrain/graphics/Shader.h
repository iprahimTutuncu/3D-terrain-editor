#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "light/Light.h"
#include "Material.h"

class Shader
{
    public:
        GLuint ID;

        Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
        Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* tessellationControlPath, const GLchar* tessellationEvaluationPath);
        void use();

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, glm::mat4 value) const;
        void setVec4(const std::string& name, glm::vec4 value) const;
        void setVec3(const std::string& name, glm::vec3 value) const;
        void addLight(LightProperties light);
        void setMaterial(MaterialProperties material);
        short getAmountOfLight();

    private:
        const short maxLight = 10;
        short lightCounter;
};

#endif // SHADER_H
