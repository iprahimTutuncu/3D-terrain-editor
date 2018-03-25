#ifndef TERRAIN_H
#define TERRAIN_H

#include "../graphics/Grid.h"
#include "../graphics/HeightMap.h"

#include <string>

class Terrain
{
    public:
        Terrain(const std::string& path = "media/texture/default.png");
        ~Terrain() = default;

        void init(unsigned int gridSize);
        void build();
        void draw(Shader &shader);

        void loadHeightMap(const std::string& path);
        void addCircle(float height, int x, int y, float attLinear,float attQuad);

        glm::vec3 getPosition(unsigned int i,unsigned int j);
        glm::mat4 getWorldTransform();

        GLuint getTextureID();
        float getHeight(glm::vec2 position);
        float getGridSize();

    private:
        Grid grid;
        HeightMap heighmap;

};

#endif // TERRAIN_H
