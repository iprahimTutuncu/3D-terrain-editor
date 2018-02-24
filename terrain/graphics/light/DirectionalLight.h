#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"
#include "../../core/SceneComponent.h"

class DirectionalLight: public SceneComponent
{
    public:
        DirectionalLight(glm::vec3 direction = glm::vec3(0.0,1.0,0.0));
        void enable(bool isEnable);

        void setAmbientColor(glm::vec3 color);
        void setDiffuseColor(glm::vec3 color);
        void setSpecularColor(glm::vec3 color);

        void setDirection(glm::vec3 direction);

        LightProperties getLightProperties();
    private:
        LightProperties lightProperties;
};

#endif // DIRECTIONALLIGHT_H
