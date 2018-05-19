#ifndef SHADOW_H
#define SHADOW_H

#include <iostream>
#include <memory>

#include <SFML/Window.hpp>

#include "FrameBuffer.h"
#include "light/DirectionalLight.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Grid.h"

#include "glm/mat4x4.hpp"
#include "glm/gtx/projection.hpp"

class Shadow
{
    public:
        Shadow(float width, float height);

        bool init();

        void update(DirectionalLight& dl, Camera& cam);

        void drawShadow(Model& model, glm::mat4 modelTransform);

        void setDirectionalLight(DirectionalLight& light);
        void setCamera(Camera& camera);
        void setNearPlane(float near_plane);
        void setFarPlane(float far_plane);

        float getNearPlane() const;
        float getFarPlane() const;
        GLuint getDepthTexture() const;

        glm::mat4 getLightViewProjectionMatrix();

        void beginDraw();
        void endDraw();

        void drawScreen(GLuint tex);

        ~Shadow();

    private:
        glm::mat4 lightViewProj;
        glm::vec3 lightDir;
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;

        FrameBuffer fbo;
        GLuint quadVAO;
        GLuint quadVBO;

        float near_plane;
        float far_plane;

        float width;
        float height;

        std::shared_ptr<Shader> p_shaderDepth;
        std::shared_ptr<Shader> p_shaderShadowMap;
};

#endif // SHADOW_H
