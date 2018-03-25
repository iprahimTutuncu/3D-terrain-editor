#include "CollisionHandler.h"
#include "glm/ext.hpp"
CollisionHandler::CollisionHandler(std::shared_ptr<Camera> camera)
{

}

CollisionHandler::~CollisionHandler()
{
    //dtor
}


bool CollisionHandler::isRayOnTerrain(glm::vec3& intersection)
{
    glm::vec3 v = this->currRay.direction * currRay.length;
    glm::vec3 w = terrain->getPosition(1,1) - currRay.origin;

    float k = glm::dot(w, glm::vec3(0.0, 1.0, 0.0)) / glm::dot(v, glm::vec3(0.0, 1.0, 0.0));

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

void CollisionHandler::setTerrain(std::shared_ptr<Terrain> terrain)
{
    this->terrain = terrain;
}


void CollisionHandler::moveCameraOutOfTerrain()
{

}
