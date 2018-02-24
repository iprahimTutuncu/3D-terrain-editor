#include "Grid.h"
#include "../ressourceManager/TextureManager.h"
Grid::Grid():
    SceneComponent("Grid")
{
    //ctor
}

void Grid::init(int gridSize, float tileSize)
{
    //generation de spoints pour la grille
    positions.resize(gridSize * gridSize);
    indices.resize(gridSize*gridSize*6);
    this->gridSize = gridSize;

    for(int x = 0; x < gridSize; x++)
        for(int z = 0; z < gridSize; z++)
            positions[x * gridSize + z] = glm::vec3((x * tileSize) - gridSize/2, 1.0f, z * tileSize - gridSize/2);

    unsigned int index = 0;

    for(int x = 0; x < gridSize-1; x++)
        for(int z = 0; z < gridSize-1; z++){
            unsigned int offset = x*gridSize + z;

            indices[index] = offset + 0;
            indices[index + 1] = offset + 1;
            indices[index + 2] = offset + gridSize;
            indices[index + 3] = offset + 1;
            indices[index + 4] = offset + gridSize + 1;
            indices[index + 5] = offset + gridSize;
            index+=6;
        }

    std::vector<Vertex> vertices;
    glm::vec3 normal(0.0, 1.0, 0.0);
    for(glm::vec3 &pos: positions)
        vertices.emplace_back(Vertex(pos, glm::vec2(0,0), normal));

    std::vector<Texture> textures;

    std::string typeName = "texture_reflection";
    auto it = find_if(textures.begin(), textures.end(), [&typeName](const Texture& t) {return t.type == typeName;});
    if (it == textures.end())
        textures.emplace_back(Texture{*RessourceManager::TextureManager::get("media/texture/default.png"), typeName, aiString("None")});

    typeName = "texture_diffuse";
    it = find_if(textures.begin(), textures.end(), [&typeName](const Texture& t) {return t.type == typeName;});
    if (it == textures.end())
        textures.emplace_back(Texture{*RessourceManager::TextureManager::get("media/texture/default.png"), typeName, aiString("None")});


    typeName = "texture_specular";
    it = find_if(textures.begin(), textures.end(), [&typeName](const Texture& t) {return t.type == typeName;});
    if (it == textures.end())
        textures.emplace_back(Texture{*RessourceManager::TextureManager::get("media/texture/default.png"), typeName, aiString("None")});

    //renvoyer le tout dans le buffer
    gridMesh = std::make_unique<Mesh>(vertices, indices, textures, 2, glm::vec3(0.0,0.0,0.0));
}

glm::vec3 Grid::getPosition(int i, int j)
{
    return positions[i * this->gridSize + j];
}



void Grid::draw(Shader& shader)
{
    gridMesh->draw(shader);
}