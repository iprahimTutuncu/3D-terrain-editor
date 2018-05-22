#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

#include "../core/SceneComponent.h"


class Camera: public SceneComponent
{
    public:
        Camera(float width,float height, std::shared_ptr<Mesh> parent = nullptr);
        void input_callback(float mouseX, float MouseY);
        void move_forward();
        void move_back();
        void moveSide(float spd);
        void turn(float speed);
        glm::mat4 getViewProjectionMatrix();
        glm::mat4 getProjectionMatrix();
        glm::mat4 getViewMatrix();
        glm::vec3 getPosition();
        glm::vec3 getFront();
        glm::vec3 getUp();
        float getWidth();
        float getHeight();
        float getNearPlane();
        float getFarPlane();
        float getRatio();
        float getFOV();

        void recalcView();
        void inversePitch();


        void setSpeed(float spd);
        void setPosition(glm::vec3 position);

    private:
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 up;

        glm::vec3 front;

        glm::mat4 proj;
        glm::mat4 view;

        float width;
        float height;
        float screenRation;
        float near_plane;
        float far_plane;
        float FOV;

        float spd = 0.1;
        float yaw = 0;
        float pitch = 0;
};

#endif // CAMERA_H
