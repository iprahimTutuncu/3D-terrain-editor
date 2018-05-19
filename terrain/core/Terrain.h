#ifndef TERRAIN_H
#define TERRAIN_H

#include "../graphics/Grid.h"
#include "../graphics/HeightMap.h"

#include <string>

struct TextureTerrainData{
    std::shared_ptr<GLuint> textureID_diffuse;
    std::shared_ptr<GLuint> textureID_specular;

    short location; // entre 0.0 et 1.0 inclusive

    TextureTerrainData(std::shared_ptr<GLuint> textureID_diffuse, std::shared_ptr<GLuint> textureID_specular, float location)
        {
        this->textureID_diffuse = textureID_diffuse;
        this->textureID_specular = textureID_specular;
        this->location = location;
    }
};

class Terrain
{
    public:
        Terrain(const std::string& path = "media/texture/default.png", const std::string& texPath = "media/texture/default.png");
        ~Terrain() = default;

        void init(unsigned int gridSize);
        void build();
        void draw(Shader &shader);

        void loadHeightMap(const std::string& path, const std::string& texPath);
        /**
          height -> 0.f - 1.0f
          x -> 0 - maxGrid
          y -> 0 - maxGrid
          attLinear -> 0.f - 1.0f (use the properties of a point light)
          attQuad -> 0.f - 1.0f (use the properties of a point light)
          r -> 0.f  - 10.f
        **/
        void addCircle(float height, int x, int y, float attLinear,float attQuad, float r);
        void addAveragingCircle(int x, int y, float attLinear,float attQuad);
        void addCircleOnTextureMap(float height, int x, int y, float attLinear,float attQuad);
        void addSmoothCircle(int x, int y, float attLinear, float attQuad);

        glm::vec3 getPosition(unsigned int i,unsigned int j);
        glm::mat4 getWorldTransform();

        GLuint getTextureID();
        GLuint getTextureMapID();
        float getHeight(glm::vec2 position);
        float getGridSize();

        void addTerrainTexture(const std::string &path_diffuse, const std::string &path_specular, short location);
        std::vector<std::shared_ptr<GLuint>> getTerrainTextures();

    private:
        Grid grid;
        HeightMap heighmap;
        const unsigned int MAX_TEXTURE_ON_MAP = 10;
        std::vector<TextureTerrainData> textures;
};

#endif // TERRAIN_H
