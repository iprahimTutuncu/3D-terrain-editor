#ifndef WATER_H
#define WATER_H

#include "Shader.h"
#include "Grid.h"
#include "FrameBuffer.h"

class Water
{
    public:
        Water(int width, int height);

        void init(float length);
        void build();
        void draw(Shader &shader);

        glm::mat4 getWorldTransform();

        void bindReflection();
        void bindRefraction();

        void unbind();

        float getHeight();
        void setHeight(float height); //to build

        GLuint getReflectionTexture();
        GLuint getRefractionTexture();
        GLuint getRefractionDepthTexture();

        ~Water();
    private:
        Grid grid;
        FrameBuffer reflectionFBO;
        FrameBuffer refractionFBO;

        GLuint reflectionTexID;
        GLuint refractionTexID;
        GLuint refractionDepthTexID;

        float height;
        std::string DUDVpath{"media/texture/waterDUDV.png"};
        std::string normalmapPath{"media/texture/waterNormalMap.png"};
        std::shared_ptr<GLuint> DUDV;
        std::shared_ptr<GLuint> normalmap;

};

#endif // WATER_H
