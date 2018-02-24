#include "CollisionHandler.h"
#include "glm/ext.hpp"
CollisionHandler::CollisionHandler(std::shared_ptr<Camera> camera)
{
    //ctor
}

CollisionHandler::~CollisionHandler()
{
    //dtor
}


bool CollisionHandler::isRayOnTerrain(glm::vec3& intersection)
{
    glm::vec3 v = this->currRay.direction * currRay.length - currRay.origin;
    glm::vec3 w = grid->getPosition(1,1) - currRay.origin;

    float k = glm::dot(w, glm::vec3(0.0, 1.0, 0.0)) / glm::dot(v, glm::vec3(0.0, 1.0, 0.0));
    std::cout << glm::to_string(this->currRay.direction) << "le K" <<  std::endl;

    intersection = currRay.origin + k * v;

    return k >= 0 && k <= 1;
}

bool CollisionHandler::isRayOnWater(glm::vec3& intersection)
{

}

bool CollisionHandler::isRayOnMesh()
{

}

bool CollisionHandler::isRayOnGrass()
{

}

int CollisionHandler::currGrass()
{

}

int CollisionHandler::currMesh()
{

}

void CollisionHandler::setRayDirection(glm::vec3 direction)
{
    this->currRay.direction = direction;
}

void CollisionHandler::setRayLength(float length)
{
    this->currRay.length = length;
}

void CollisionHandler::setRayOrigin(glm::vec3 origin)
{
    this->currRay.origin = origin;
}
void CollisionHandler::addMeshPosition(int ID, glm::vec3 position)
{
    meshTerrainPosition.insert({ID, position});

}

void CollisionHandler::addGrassPosition(int ID, glm::vec3 position)
{
    grassTerrainPosition.insert({ID, position});
}

void CollisionHandler::setWaterLevel(float waterLevel)
{

}

void CollisionHandler::setGrid(std::shared_ptr<Grid>grid)
{
    this->grid = grid;
}


void CollisionHandler::moveCameraOutOfTerrain()
{

}
