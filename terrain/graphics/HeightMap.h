#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <SFML/Graphics/Image.hpp>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class HeightMap
{
    public:
        HeightMap(std::string path = "media/texture/default.png", std::string texPath = "media/texture/default.png");

        GLuint getTextureID();
        GLuint getTextureMapID();
        float getRedColor(int i, int j);
        void reload();
        void addCircle(float height, int x, int y, float attLinear,float attQuad, float r);
        void addAveragingCircle(int x, int y, float attLinear,float attQuad);
        void addSmoothCircle(int x, int y, float attLinear,float attQuad);
        void addCircleOnTextureMap(float height, int x, int y, float attLinear,float attQuad);
        glm::vec2 getSize();
        ~HeightMap();
    private:
        GLuint mTextureID;
        GLuint mTextureMapID;

        sf::Image mImage;
        sf::Image mTexImage;

        float data[512*512]{0.0f};
        float dataTex[512*512]{0.0f};

        std::string mPath;
        std::string mTexPath;

        glm::vec2 mSize;
};

#endif // HEIGHTMAP_H
