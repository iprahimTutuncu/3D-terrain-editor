#include "Shader.h"

using namespace std;

string readFile(const char* fileName){
    ifstream file(fileName);

	if(file.good())
		return string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
		);

    cout << "file failed to load..." << fileName << " not found" << endl;
    exit(1);

}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    GLuint vertexShader;
    GLuint fragShader;
    GLint success;
    string temp;
    const GLchar* adapter[1];
    char infoLog[512];
    lightCounter = 0;

    //vertex shader
    temp = readFile(vertexPath);
    adapter[0] = temp.c_str();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, adapter, nullptr);
    glCompileShader(vertexShader);

    // retourner un messafe d'erreur s'il y a lieu
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Erreur: dans compilation vertex shader " << vertexPath << "\n" << infoLog << std::endl;
    }

    //fragment shader
    temp = readFile(fragmentPath);
    adapter[0] = temp.c_str();

    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader,1, adapter, nullptr);
    glCompileShader(fragShader);


    // retourner un messafe d'erreur s'il y a lieu
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "Erreur: dans compilation fragment shader " << fragmentPath << "\n" << infoLog << std::endl;
    }

    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

}

void Shader::use()
{
    lightCounter = 0;
    GLint curr = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &curr);

    if(curr != ID)
        glUseProgram(ID);
}

void Shader::setBool(const std::string& name,bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name,int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name,float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, glm::vec4 value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()),1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()),1, glm::value_ptr(value));
}

void Shader::addLight(LightProperties light)
{
    if(lightCounter > maxLight){
        std::cout << "Maximum Light qt. reached, unable to add more" << std::endl;
        return;
    }

    setBool("light[" + std::to_string(lightCounter) + "].isEnable", light.isEnable);
    setBool("light[" + std::to_string(lightCounter) + "].isPoint", light.isPoint);
    setBool("light[" + std::to_string(lightCounter) + "].isDirection", light.isDirection);
    setBool("light[" + std::to_string(lightCounter) + "].isCursor", light.isCursor);

    setVec3("light[" + std::to_string(lightCounter) + "].ambient", light.ambient);
    setVec3("light[" + std::to_string(lightCounter) + "].diffuse", light.diffuse);
    setVec3("light[" + std::to_string(lightCounter) + "].specular", light.specular);
    setVec3("light[" + std::to_string(lightCounter) + "].direction", light.direction);
    setVec3("light[" + std::to_string(lightCounter) + "].position", light.position);

    setFloat("light[" + std::to_string(lightCounter) + "].attenuationConstant", light.attenuationConstant);
    setFloat("light[" + std::to_string(lightCounter) + "].attenuationLinear", light.attenuationLinear);
    setFloat("light[" + std::to_string(lightCounter) + "].attenuationQuadratic", light.attenuationQuadratic);


    lightCounter++;
}

void Shader::setMaterial(MaterialProperties material)
{
    setVec3("material.emission", material.emission);
    setFloat("material.shininess", material.shininess);
}

