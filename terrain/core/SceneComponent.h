#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H

#include "glm/mat4x4.hpp"
#include <string>

class SceneComponent
{
    public:
        SceneComponent(std::string type);
        ~SceneComponent();

        virtual void update(){};

        glm::mat4 getWorldTransform();
        void setWorldTransform(glm::mat4 worldTransform);
        std::string getType();
        int getID();

    protected:
        int ID;
        std::string type;
        glm::mat4 worldTransform;
    private:
        static int currID;

};

#endif // SCENECOMPONENT_H
