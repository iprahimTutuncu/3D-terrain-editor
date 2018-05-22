#include "Terrain.h"
#include "../ressourceManager/TextureManager.h"

Terrain::Terrain(const std::string& path, const std::string& texPath):
    heighmap(path, texPath)
{
    init(heighmap.getSize().x);
}

void Terrain::init(unsigned int gridSize)
{
    grid.init(gridSize, 1.0);
}

void Terrain::build()
{

}

void Terrain::loadHeightMap(const std::string& path, const std::string& texPath)
{
}

void Terrain::addCircle(float height, int x, int y, float attLinear,float attQuad, float r)
{
    heighmap.addCircle(height,x,y,attLinear,attQuad, r);
}

void Terrain::addAveragingCircle(int x, int y, float attLinear, float attQuad)
{
    heighmap.addAveragingCircle(x,y,attLinear,attQuad);
}

void Terrain::addSmoothCircle(int x, int y, float attLinear, float attQuad){
 heighmap.addSmoothCircle(x,y,attLinear,attQuad);
}

void Terrain::addCircleOnTextureMap(float height, int x, int y, float attLinear, float attQuad)
{
    heighmap.addCircleOnTextureMap(height,x,y,attLinear,attQuad);
}

float Terrain::getHeight(glm::vec2 position)
{
    return heighmap.getRedColor(position.x, position.y); //heightmap internalformat = R32F
}

float Terrain::getGridSize()
{
    return heighmap.getSize().x;
}

GLuint Terrain::getTextureID()
{
    return heighmap.getTextureID();
}

GLuint Terrain::getTextureMapID()
{
    return heighmap.getTextureMapID();
}

glm::mat4 Terrain::getWorldTransform()
{
    return grid.getWorldTransform();
}


glm::vec3 Terrain::getPosition(unsigned int i, unsigned int j)
{
    return grid.getPosition(i,j);
}

void Terrain::addTerrainTexture(const std::string &path_diffuse, const std::string &path_specular, const std::string &path_normal, short location)
{
    textures.push_back(TextureTerrainData(RessourceManager::TextureManager::get(path_diffuse),
                                          RessourceManager::TextureManager::get(path_specular),
                                          RessourceManager::TextureManager::get(path_normal),
                                          location));

    std::sort(textures.begin(), textures.end(), [](const TextureTerrainData &ttdA, const TextureTerrainData &ttdB){
                    return ttdA.location < ttdB.location;
    });

    for(TextureTerrainData ttd: textures){
        std::cout << "txture: " << *ttd.textureID_diffuse << ", " << ttd.location << ", normal " << *ttd.textureID_normal << std::endl;
    }
}
std::vector<std::shared_ptr<GLuint>> Terrain::getTerrainTextures()
{
    std::vector<std::shared_ptr<GLuint>> texturesID;
    for(int i = 0; i < textures.size(); i++){
        texturesID.emplace_back(textures[i].textureID_diffuse);
        texturesID.emplace_back(textures[i].textureID_specular);
        texturesID.emplace_back(textures[i].textureID_normal);
    }

    return texturesID;
}

void Terrain::loadHeightMap(const std::string& path)
{
    heighmap.load(path);
}

void Terrain::saveHeightMap(const std::string& path)
{
    heighmap.save(path);
}


void Terrain::draw(Shader& shader)
{
    static float time = 0.01;
    time += 0.0001;
    if(time > 1.0){
        time = 0.0f;
    }

    shader.use();

    glActiveTexture(GL_TEXTURE12);
    glBindTexture(GL_TEXTURE_2D, getTextureID());

    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, getTextureMapID());

    glActiveTexture(GL_TEXTURE14);
    glBindTexture(GL_TEXTURE_2D, *RessourceManager::TextureManager::get("media/texture/waterDUDV.png"));
    shader.setInt("DUDV", 14);

    shader.setInt("heightmap", 12);
    shader.setInt("texturemap", 13);
    shader.setFloat("elapsedTime", time);

    for(int i = 0; i < textures.size() && i < MAX_TEXTURE_ON_MAP; i++){
        int j = i +1;
        glActiveTexture(GL_TEXTURE0 + j * 3);
        shader.setInt("materials[" + std::to_string(i) + "].texture_diffuse", j * 3);
        glBindTexture(GL_TEXTURE_2D, *textures[i].textureID_diffuse);

        glActiveTexture(GL_TEXTURE0 + 1 + j * 3);
        shader.setInt("materials[" + std::to_string(i) + "].texture_specular", 1 + j * 3);
        glBindTexture(GL_TEXTURE_2D, *textures[i].textureID_specular);

        glActiveTexture(GL_TEXTURE0 + 2 + j * 3);
        shader.setInt("materials[" + std::to_string(i) + "].texture_normalMap", 2 + j * 3);
        glBindTexture(GL_TEXTURE_2D, *textures[i].textureID_normal);
    }

    grid.draw(shader);
}



