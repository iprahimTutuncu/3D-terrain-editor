#ifndef COMPONENT_H
#define COMPONENT_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace ECS{
    enum class COMPONENT: unsigned int {
        NONE          = 0,
        RENDERABLE    = 1,
        TRANSFORMABLE = 2,
        LIGHT         = 3,
        CUBEMAP       = 4
    };


    struct Component{};

    struct Renderable: public Component{
        Renderable(const std::string &path,const std::string &type){
            this->path = path;
            this->type = type;
        }

        Renderable(Renderable&& renderable){
            this->type = renderable.type;
            this->path = renderable.path;
        };

        std::string type;
        std::string path;
    };

    struct Transformable: public Component{
        Transformable(glm::mat4 scale, glm::mat4 rotation, glm::mat4 transformation){
            this->scale = scale;
            this->rotation = rotation;
            this->transformation = transformation;
        }

        Transformable(Transformable&& transformable) = default;

        glm::mat4 scale;
        glm::mat4 rotation;
        glm::mat4 transformation;
    };

    struct LightComponent: public Component{
        LightComponent(bool isEnable, bool isDirection, bool isPoint, bool isSpot,
                       glm::vec3 position, glm::vec3 direction,
                       float attenuationConstant, float attenuationLinear, float attenuationQuadratic,
                       glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){

            this->isEnable = isEnable;
            this->isDirection = isDirection;
            this->isPoint = isPoint;
            this->isSpot = isSpot;
            this->position = position;
            this->direction = direction;
            this->attenuationConstant = attenuationConstant;
            this->attenuationLinear = attenuationLinear;
            this->attenuationQuadratic = attenuationQuadratic;
        }
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

    struct Cubemap: public Component{
        std::string rightPath;
        std::string leftPath;
        std::string topPath;
        std::string bottomPath;
        std::string backPath;
        std::string frontPath;
    };

    struct CameraComponent: public Component{
        CameraComponent(float width, float height){
            this->width = width;
            this->height = height;
        }

        float width;
        float height;
    };
}
#endif //COMPONENT_H
