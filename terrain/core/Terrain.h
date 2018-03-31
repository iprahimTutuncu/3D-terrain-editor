#ifndef TERRAIN_H
#define TERRAIN_H

#include "../graphics/Grid.h"
#include "../graphics/HeightMap.h"

#include <string>

struct TextureTerrainData{
    std::shared_ptr<GLuint> textureID_diffuse;
    std::shared_ptr<GLuint> textureID_specular;

    float affected_normal_level; // entre 0.0 et 1.0 inclusive

    TextureTerrainData(std::shared_ptr<GLuint> textureID_diffuse, std::shared_ptr<GLuint> textureID_specular, float affected_normal_level)
        {
        this->textureID_diffuse = textureID_diffuse;
        this->textureID_specular = textureID_specular;
        this->affected_normal_level = affected_normal_level;
    }
};

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

        void addTerrainTexture(const std::string &path_diffuse, const std::string &path_specular, const float affected_normal_level);
        std::vector<std::shared_ptr<GLuint>> getTerrainTextures();

    private:
        Grid grid;
        HeightMap heighmap;
        const unsigned int MAX_TEXTURE_ON_MAP = 10;
        std::vector<TextureTerrainData> textures;
};

#endif // TERRAIN_H
