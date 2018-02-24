#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"
#include "../../core/SceneComponent.h"

class PointLight: public SceneComponent
{
    public:
        PointLight(glm::vec3 position = glm::vec3(0.0));
        void enable(bool isEnable);

        void setAmbientColor(glm::vec3 color);
        void setDiffuseColor(glm::vec3 color);
        void setSpecularColor(glm::vec3 color);

        void setAttenuation(float constant, float linear, float quadratic);
        void setPosition(glm::vec3 position);

        virtual void update(){
            glm::vec4 temp = glm::vec4(lightProperties.position, 1.0);
            temp = getWorldTransform() * temp;

            lightProperties.position.x = temp.x;
            lightProperties.position.y = temp.y;
            lightProperties.position.z = temp.z;
        }

        LightProperties getLightProperties();

    private:
        LightProperties lightProperties;

};

#endif // POINTLIGHT_H
