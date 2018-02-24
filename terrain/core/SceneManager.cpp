#include "SceneManager.h"
#include <memory>

SceneManager::SceneManager()
{
    root = std::make_unique<SceneNode>();
}

SceneManager::~SceneManager()
{
    //dtor
}

void SceneManager::init(float width, float height){
    this->camera = std::make_shared<Camera>(width, height);
}

std::shared_ptr<PointLight> SceneManager::createPointLight()
{
    std::shared_ptr<PointLight> pl = std::make_shared<PointLight>();
    pl->setAmbientColor(glm::vec3(1.0,0.0,0.1));
    pl->setDiffuseColor(glm::vec3(0.3,1.0,0.3));
    pl->setSpecularColor(glm::vec3(0.5,0.5,1.0));
    pl->setAttenuation(1.0, 0.22, 0.022);
    pointLights.push_back(pl);
    return pl;
}

std::shared_ptr<DirectionalLight> SceneManager::makeDirectionalLight()
{
    directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->setAmbientColor(glm::vec3(0.05,0.05,0.1));
    directionalLight->setDiffuseColor(glm::vec3(0.1,0.1,0.05));
    directionalLight->setSpecularColor(glm::vec3(0.05,0.05,0.05));
    return directionalLight;
}

std::shared_ptr<Line3D> SceneManager::createLine3D(glm::vec3 point1, glm::vec3 point2, glm::vec3 color)
{
    std::shared_ptr<Line3D> line3D = std::make_shared<Line3D>(point1, point2, color);
    lines3D.emplace_back(line3D);
    return line3D;
}

std::shared_ptr<Model> SceneManager::createModel(std::string fileLocation)
{
    std::shared_ptr<Model> m = std::make_shared<Model>(fileLocation);
    models.emplace_back(m);
    return m;
}

std::shared_ptr<Grid> SceneManager::createGrid(int gridSize, float tileSize)
{
    std::shared_ptr<Grid> g = std::make_shared<Grid>();
    g->init(gridSize, tileSize);
    grids.emplace_back(g);
    return g;
}


std::shared_ptr<Camera> SceneManager::getCamera(){
    return camera;
}

std::unique_ptr<SceneNode>::pointer SceneManager::getRootNode()
{
    return root.get();
}

std::shared_ptr<SceneNode> SceneManager::createNode()
{
    std::shared_ptr<SceneNode> n = std::make_shared<SceneNode>();
    nodes.emplace_back(n);
    return n;
}

void SceneManager::fillCollisionHandler(CollisionHandler collisionHandler)
{
    for(auto node: nodes){
        for(auto comp: node->getComponents())
            if(comp->getType() == "Model")
                collisionHandler.addMeshPosition(comp->getID(), node->getPosition());
    }
}

void SceneManager::fillRenderer(Renderer &renderer){
    int counter = 0;
    for(auto node: nodes){
        for(auto comp: node->getComponents())
            if(comp->getType() == "Model")
                renderer.addModel("Model" + std::to_string(counter++), std::static_pointer_cast<Model>(comp));
            else if(comp->getType() == "PointLight")
                renderer.addPointLight("PointLight" + std::to_string(counter++), std::static_pointer_cast<PointLight>(comp));
            else if(comp->getType() == "Grid")
                renderer.addGrid("Grid" + std::to_string(counter++), std::static_pointer_cast<Grid>(comp));
           else if(comp->getType() == "line3D")
                renderer.addLine3D(comp->getID(), std::static_pointer_cast<Line3D>(comp));
    }

    renderer.setCamera(camera);
    renderer.setDirectionalLight(directionalLight);

}


