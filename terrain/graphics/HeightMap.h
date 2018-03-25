#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <SFML/Graphics/Image.hpp>
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class HeightMap
{
    public:
        HeightMap(std::string path = "media/texture/default.png");

        GLuint getTextureID();
        glm::vec3 getPixelColor(int i, int j);
        void reload();
        void addCircle(float height, int x, int y, float attLinear,float attQuad);
        glm::vec2 getSize();
        ~HeightMap();
    private:
        GLuint mTextureID;
        sf::Image mImage;
        std::string mPath;
        glm::vec2 mSize;
};

#endif // HEIGHTMAP_H
