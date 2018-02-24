#include "MouseRay.h"

MouseRay::MouseRay()
{
}

void MouseRay::init(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
    this->width = camera->getWidth();
    this->height = camera->getHeight();
}

void MouseRay::update(float x, float y)
{
    this->view = camera->getViewMatrix();
    this->proj = camera->getProjectionMatrix();
    this->currRay = calcMouseRay(x, y);

}

glm::vec3 MouseRay::calcMouseRay(float x, float y)
{
    glm::vec2 NDCoords = calcNDC(x, y);
    glm::vec4 clipCoords = glm::vec4(NDCoords.x, NDCoords.y, -1.0f, 1.0f);
    glm::vec4 eyeCoords = calcEyeCoords(clipCoords);
    glm::vec3 worldRay = calcWorldCoords(eyeCoords);
    return worldRay;
}

glm::vec3 MouseRay::getCurrRay()
{
    return this->currRay;
}

glm::vec2 MouseRay::calcNDC(float x, float y)
{
    glm::vec2 NDC;
    NDC.x = (2.0f * x) / width - 1.0f;
    NDC.y = 1.0f - (2.0f * y) / height;
    return NDC;
}

glm::vec4 MouseRay::calcEyeCoords(glm::vec4 clipCoords)
{
    glm::vec4 eyeCoord = glm::inverse(proj) * clipCoords;
    return glm::vec4(eyeCoord.x, eyeCoord.y, -1.0, 1.0);
}

glm::vec3 MouseRay::calcWorldCoords(glm::vec4 eyeCoords)
{
    glm::vec4 worldCoord = glm::inverse(view) * eyeCoords;
    glm::vec3 rayWorld = glm::vec3(worldCoord.x, worldCoord.y, worldCoord.z);
    return glm::normalize(rayWorld);
}
