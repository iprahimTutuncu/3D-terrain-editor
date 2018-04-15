#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(glm::vec3 direction):
    SceneComponent("DirectionalLight")
{
    lightProperties.isEnable = true;
    lightProperties.isPoint = false;
    lightProperties.isDirection = true;
    lightProperties.isSpot = false;
    lightProperties.isCursor = false;

    lightProperties.direction = direction;
}

void DirectionalLight::enable(bool isEnable)
{
    lightProperties.isEnable = isEnable;
}

void DirectionalLight::setAmbientColor(glm::vec3 color)
{
    lightProperties.ambient = color;

}

void DirectionalLight::setDiffuseColor(glm::vec3 color)
{
    lightProperties.diffuse = color;
}

void DirectionalLight::setSpecularColor(glm::vec3 color)
{
    lightProperties.specular = color;
}

void DirectionalLight::setDirection(glm::vec3 direction)
{
    lightProperties.direction = direction;
}

LightProperties DirectionalLight::getLightProperties()
{
    return lightProperties;
}
