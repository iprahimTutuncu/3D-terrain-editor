#ifndef WATER_H
#define WATER_H

#include "Shader.h"
#include "Grid.h"
#include "FrameBuffer.h"

class Water
{
    public:
        Water();

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

        ~Water();
    private:
        Grid grid;
        FrameBuffer reflectionFBO;
        FrameBuffer refractionFBO;

        const int REFLECTION_WIDTH{800};
        const int REFLECTION_HEIGHT{600};

        const int REFRACTION_WIDTH{800};
        const int REFRACTION_HEIGHT{600};

        GLuint reflectionTexID;
        GLuint refractionTexID;

        float height;
        std::string DUDVpath{"media/texture/waterDUDV.png"};
        std::string normalmapPath{"media/texture/waterNormalMap.png"};
        std::shared_ptr<GLuint> DUDV;
        std::shared_ptr<GLuint> normalmap;

};

#endif // WATER_H
