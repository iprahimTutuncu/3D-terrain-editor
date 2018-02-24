#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "SceneNode.h"
#include "../graphics/light/PointLight.h"
#include "../graphics/light/DirectionalLight.h"
#include "../graphics/Camera.h"
#include "../graphics/CubeMap.h"
#include "../graphics/Model.h"
#include "../graphics/PostProcess.h"
#include "../graphics/Shadow.h"
#include "../graphics/Grid.h"
#include "../graphics/Renderer.h"
#include "../core/CollisionHandler.h"

class SceneManager
{
    public:
        SceneManager();
        ~SceneManager();

        void init(float width, float height);

        std::shared_ptr<PointLight> createPointLight();
        std::shared_ptr<Grid> createGrid(int gridSize, float tileSize);
        std::shared_ptr<Model> createModel(std::string fileLocation);
        std::shared_ptr<Line3D> createLine3D(glm::vec3 point1, glm::vec3 point2, glm::vec3 color = glm::vec3(0.0,1.0,0.0));
        std::shared_ptr<DirectionalLight> makeDirectionalLight();

        std::unique_ptr<SceneNode>::pointer getRootNode();
        std::shared_ptr<Camera> getCamera();
        std::shared_ptr<SceneNode> createNode();

        void fillCollisionHandler(CollisionHandler collisionHandler);
        void fillRenderer(Renderer &renderer);
        void render();

    private:
        std::shared_ptr<Camera> camera;
        std::shared_ptr<DirectionalLight> directionalLight;

        std::vector<std::shared_ptr<PointLight>> pointLights;
        std::vector<std::shared_ptr<Model>> models;
        std::vector<std::shared_ptr<Grid>> grids;
        std::vector<std::shared_ptr<Line3D>> lines3D;

        std::unique_ptr<SceneNode> root;
        std::vector<std::shared_ptr<SceneNode>> nodes;
};

#endif // SCENEMANAGER_H
