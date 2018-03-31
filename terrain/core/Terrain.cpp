#include "Terrain.h"
#include "../ressourceManager/TextureManager.h"

Terrain::Terrain(const std::string& path):
    heighmap(path)
{
    init(heighmap.getSize().x);
}

void Terrain::init(unsigned int gridSize)
{
    grid.init(200, 1.0);
}

void Terrain::build()
{

}

void Terrain::loadHeightMap(const std::string& path)
{
}

void Terrain::addCircle(float height, int x, int y, float attLinear,float attQuad)
{
    heighmap.addCircle(height,x,y,attLinear,attQuad);
}

float Terrain::getHeight(glm::vec2 position)
{
    return heighmap.getPixelColor(position.x, position.y).r;
}

float Terrain::getGridSize()
{
    return heighmap.getSize().x;
}

GLuint Terrain::getTextureID()
{
    return heighmap.getTextureID();
}

glm::mat4 Terrain::getWorldTransform()
{
    return grid.getWorldTransform();
}


glm::vec3 Terrain::getPosition(unsigned int i, unsigned int j)
{
    return grid.getPosition(i,j);
}

void Terrain::addTerrainTexture(const std::string &path_diffuse, const std::string &path_specular, float affected_normal_level)
{
    if(affected_normal_level > 1.0)
        affected_normal_level = 1.0;
    else if(affected_normal_level < 0.0)
        affected_normal_level = 0.0;

    textures.push_back(TextureTerrainData(RessourceManager::TextureManager::get(path_diffuse),
                                          RessourceManager::TextureManager::get(path_specular),
                                          affected_normal_level));

    std::sort(textures.begin(), textures.end(), [](const TextureTerrainData &ttdA, const TextureTerrainData &ttdB){
                    return ttdA.affected_normal_level < ttdB.affected_normal_level;
    });

    for(TextureTerrainData ttd: textures){
        std::cout << "txture: " << *ttd.textureID_diffuse << ", " << ttd.affected_normal_level << std::endl;
    }
}
std::vector<std::shared_ptr<GLuint>> Terrain::getTerrainTextures()
{
    std::vector<std::shared_ptr<GLuint>> texturesID;
    for(int i = 0; i < textures.size(); i++){
        texturesID.emplace_back(textures[i].textureID_diffuse);
        texturesID.emplace_back(textures[i].textureID_specular);
    }

    return texturesID;
}




void Terrain::draw(Shader& shader)
{
    for(int i = 0; i < textures.size() && i < MAX_TEXTURE_ON_MAP; i++){
        glActiveTexture(GL_TEXTURE0 + *textures[i].textureID_diffuse);
        shader.setInt("materials[" + std::to_string(i) + "].texture_diffuse", *textures[i].textureID_diffuse);
        glBindTexture(GL_TEXTURE_2D, *textures[i].textureID_diffuse);

        glActiveTexture(GL_TEXTURE0 + *textures[i].textureID_specular);
        shader.setInt("materials[" + std::to_string(i) + "].texture_specular", *textures[i].textureID_specular);
        glBindTexture(GL_TEXTURE_2D, *textures[i].textureID_specular);

        shader.setFloat("affected_normal_levels[" + std::to_string(i) + "]", textures[i].affected_normal_level);

    }

    grid.draw(shader);
}



