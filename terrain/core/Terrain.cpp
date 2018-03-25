#include "Terrain.h"

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

void Terrain::draw(Shader& shader)
{
    shader.setFloat("terrainWidth", getGridSize());
    shader.setFloat("terrainHeight", heighmap.getSize().y);
    grid.draw(shader);
}



