#include "PointLight.h"

PointLight::PointLight(glm::vec3 position):
    SceneComponent("PointLight")
{
    lightProperties.isEnable = true;
    lightProperties.isPoint = true;
    lightProperties.isDirection = false;
    lightProperties.isSpot = false;
    lightProperties.isCursor = false;

    lightProperties.position = position;
    lightProperties.attenuationConstant = 1.0;
    lightProperties.attenuationLinear = 0.0;
    lightProperties.attenuationQuadratic = 0.0;
}

void PointLight::enable(bool isEnable)
{
    lightProperties.isEnable = isEnable;
}

void PointLight::setAmbientColor(glm::vec3 color)
{
    lightProperties.ambient = color;
}

void PointLight::setDiffuseColor(glm::vec3 color)
{
    lightProperties.diffuse = color;
}

void PointLight::setSpecularColor(glm::vec3 color)
{
    lightProperties.specular = color;
}

bool PointLight::pointLightHasCursor(bool enable)
{
    lightProperties.isCursor = enable;
}


void PointLight::setAttenuation(float constant, float linear, float quadratic)
{
    lightProperties.attenuationConstant = constant;
    lightProperties.attenuationLinear = linear;
    lightProperties.attenuationQuadratic = quadratic;
}

void PointLight::setPosition(glm::vec3 position)
{
    lightProperties.position = position;
}

LightProperties PointLight::getLightProperties()
{
    return lightProperties;
}
