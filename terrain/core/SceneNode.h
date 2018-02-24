#ifndef SCENENODE_H
#define SCENENODE_H

#include <vector>
#include <memory>

#include "SceneComponent.h"
#include "glm/vec3.hpp"
#include "glm/gtx/transform.hpp"

class SceneNode
{
    public:
        SceneNode();
        ~SceneNode();

        void addChild(std::shared_ptr<SceneNode> node);
        void removeChild(std::string name);

        void addComponent(std::shared_ptr<SceneComponent> comp);

        std::vector<std::shared_ptr<SceneNode>> getChilds();
        std::vector<std::shared_ptr<SceneComponent>> getComponents(std::string type);
        std::vector<std::shared_ptr<SceneComponent>> getComponents();

        void update();
        void updateAll();

        glm::vec3 setScale(glm::vec3 scale);
        glm::vec3 setRotation(glm::vec3 rotation);
        glm::vec3 setPosition(glm::vec3 position);

        std::string getName();
        glm::vec3 getScale();
        glm::vec3 getRotation();
        glm::vec3 getPosition();
        glm::mat4 getModelTransform();

    private:
        std::string name;

        glm::vec3 scale;
        glm::vec3 rotation;
        glm::vec3 position;

        glm::mat4 worldTransform;

        std::shared_ptr<SceneNode> parent;
        std::vector<std::shared_ptr<SceneNode>> childs;
        std::vector<std::shared_ptr<SceneComponent>> components;
};

#endif // SCENENODE_H
