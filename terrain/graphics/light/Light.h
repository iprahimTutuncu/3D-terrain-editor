#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include <glm/vec3.hpp>

struct LightProperties{
    bool isEnable;
    bool isDirection;
    bool isPoint;
    bool isSpot;

    glm::vec3 position;
    glm::vec3 direction;

    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

#endif // LIGHT_H_INCLUDED
