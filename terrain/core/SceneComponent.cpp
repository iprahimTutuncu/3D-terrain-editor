#include "SceneComponent.h"

SceneComponent::SceneComponent(std::string type):
    worldTransform(1.0),
    ID(currID++)
{
    this->type = type;
}

SceneComponent::~SceneComponent()
{
    //dtor
}

std::string SceneComponent::getType()
{
    return type;
}

glm::mat4 SceneComponent::getWorldTransform()
{
    return worldTransform;
}

void SceneComponent::setWorldTransform(glm::mat4 worldTransform)
{
    this->worldTransform = worldTransform;
}

int SceneComponent::getID()
{
    return ID;
}

int SceneComponent::currID = 0;

