#include "Camera.h"

Camera::Camera(float width,float height, std::shared_ptr<Mesh> parent):
    SceneComponent("Camera")
{
    if(parent == nullptr)
        position = glm::vec3(0);
    else
        position = glm::vec3(0);

    near_plane = 0.1f;
    far_plane =  100.5f;
    FOV = 53.0f;

    proj = glm::perspective(glm::radians(FOV), width/height, near_plane, far_plane);
    view = glm::mat4(1);

    this->width = width;
    this->height = height;

    this->screenRation = width/height;

}
void Camera::input_callback(float mouseX, float mouseY)
{
    static glm::vec2 mouseOldPos = glm::vec2(width/2, height/2);

    glm::vec2 mousePos = glm::vec2(mouseX, mouseY);

    if(mousePos.x > width)
        mousePos.x = width;
    else if(mousePos.x < 0)
        mousePos.x = 0;

    if(mousePos.y > height)
        mousePos.y = height;
    else if(mousePos.y < 0)
        mousePos.y = 0;

    glm::vec2 mouseOffset = glm::vec2(mousePos.x - mouseOldPos.x, mousePos.y - mouseOldPos.y);
    mouseOldPos = glm::vec2(mouseX, mouseY);

    if(mouseOldPos.x > width)
        mouseOldPos.x = width;
    else if(mouseOldPos.x < 0)
        mouseOldPos.x = 0;

    if(mouseOldPos.y > height)
        mouseOldPos.y = height;
    else if(mouseOldPos.y < 0)
        mouseOldPos.y = 0;

    float sensitivity = 0.2;
    mouseOffset *= sensitivity;

    yaw += mouseOffset.x;
    pitch += mouseOffset.y;

    if(pitch > 89.0f)
        pitch = 89.0f;
    else if(pitch < -89.0f)
        pitch = -89.0f;

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    view = glm::lookAt(position,
                       position - glm::normalize(front),
                       glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getViewProjectionMatrix()
{
    return proj * view;
}

glm::mat4 Camera::getViewMatrix()
{
    return view;
}

glm::mat4 Camera::getProjectionMatrix()
{
    return proj;
}

void Camera::move_forward()
{
    float y = position.y;
    position -= front * spd;
    //position.y = y;
}

void Camera::move_back()
{
    float y = position.y;
    position += front * spd;
    //position.y = y;
}

void Camera::turn(float speed)
{
    yaw += speed;
}

glm::vec3 Camera::getPosition()
{
    return position;
}

glm::vec3 Camera::getFront()
{
    return front;
}

glm::vec3 Camera::getUp()
{
    return glm::vec3(0.0, 1.0, 0.0);
}

float Camera::getRatio()
{
    return screenRation;
}

float Camera::getWidth()
{
    return width;
}

float Camera::getHeight()
{
    return height;
}

float Camera::getNearPlane()
{
    return near_plane;
}

float Camera::getFarPlane()
{
    return far_plane;
}

float Camera::getFOV()
{
    return FOV;
}

void Camera::setSpeed(float spd)
{
    this->spd = spd;
}

void Camera::setPosition(glm::vec3 position)
{
    this->position = position;
}



