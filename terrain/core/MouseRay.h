#ifndef MOUSERAY_H
#define MOUSERAY_H

#include "../graphics/Camera.h"
#include "glm/glm.hpp"

class MouseRay
{
    public:
        MouseRay();
        ~MouseRay() = default;

        void init(std::shared_ptr<Camera> camera);
        void update(float x, float y);
        glm::vec3 getCurrRay();

    private:
        glm::vec3 calcMouseRay(float x, float y);
        glm::vec2 calcNDC(float x, float y);
        glm::vec4 calcEyeCoords(glm::vec4 clipCoords);
        glm::vec3 calcWorldCoords(glm::vec4 eyeCoords);

        float width;
        float height;
        glm::mat4 view;
        glm::mat4 proj;
        std::shared_ptr<Camera> camera;

        glm::vec3 currRay;
};

#endif // MOUSERAY_H
