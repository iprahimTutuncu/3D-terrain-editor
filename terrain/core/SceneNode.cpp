#include "SceneNode.h"

#include <iostream>

SceneNode::SceneNode():
    scale(1.0),
    rotation(1.0),
    position(0.0),
    parent(nullptr)
{

}

SceneNode::~SceneNode()
{

}

void SceneNode::addChild(std::shared_ptr<SceneNode> node)
{
    if(parent != node)
        childs.emplace_back(node);
}

void SceneNode::addComponent(std::shared_ptr<SceneComponent> comp)
{
    components.emplace_back(comp);
}


void SceneNode::removeChild(std::string name)
{
    for(int i = 0; i < childs.size(); i++)
        if(childs[i]->getName() == name)
            childs.erase(childs.begin() + i);
}

std::vector<std::shared_ptr<SceneNode>> SceneNode::getChilds()
{
    return childs;
}



std::vector<std::shared_ptr<SceneComponent>> SceneNode::getComponents(std::string type)
{
    std::vector<std::shared_ptr<SceneComponent>> vec;
    for(auto c: components)
        if(c->getType() == type)
            vec.push_back(c);

    return vec;
}

std::vector<std::shared_ptr<SceneComponent>> SceneNode::getComponents()
{
    return components;
}

std::string SceneNode::getName()
{
    return name;
}



void SceneNode::update(){
    worldTransform = glm::mat4(1.0);
    worldTransform = glm::scale(worldTransform, scale);
    worldTransform = glm::translate(worldTransform, position);

    //se gatrder une semaine pour appliquer les quaternions
    //worldTransform = glm::rotate(worldTransform, glm::radians(10.0f), rotation);

    for(auto c: components){
        c->setWorldTransform(c->getWorldTransform() * worldTransform);
        c->update();
    }
}


void SceneNode::updateAll()
{
    update();

    for(auto &node: childs){
        node->setPosition(node->getPosition() + position);
        node->setRotation(node->getRotation() + rotation);
        node->setScale(node->getScale() * scale);

        node->updateAll();
    }


}

glm::vec3 SceneNode::setScale(glm::vec3 scale)
{
    this->scale = scale;
}

glm::vec3 SceneNode::setRotation(glm::vec3 rotation)
{
    this->rotation = rotation;
}

glm::vec3 SceneNode::setPosition(glm::vec3 position)
{
    this->position = position;
}

glm::vec3 SceneNode::getScale()
{
    return scale;
}

glm::vec3 SceneNode::getRotation()
{
    return rotation;
}

glm::vec3 SceneNode::getPosition()
{
    return position;
}

glm::mat4 SceneNode::getModelTransform()
{
    return worldTransform;
}
