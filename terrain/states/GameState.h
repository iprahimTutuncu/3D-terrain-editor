#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "../graphics/Shader.h"
#include "../graphics/Model.h"
#include "../graphics/light/DirectionalLight.h"
#include "../graphics/light/PointLight.h"
#include "../graphics/Camera.h"
#include "../graphics/CubeMap.h"
#include "../graphics/Material.h"
#include "../graphics/HeightMap.h"
#include "../ECS/Systems/RenderSystem.h"
#include "../graphics/PostProcess.h"
#include "../graphics/Shadow.h"
#include "../core/SceneManager.h"
#include "BaseState.h"
#include "../core/MouseRay.h"
#include "../core/CollisionHandler.h"
#include "../core/Water.h"
#include "../vendor/imgui.h"

class GameState : public BaseState
{
    public:
        GameState(Game* parent);

        void event() override;
        void update(const sf::Time &deltaTime) override;
        void draw() override;

        ~GameState();
    private:
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Grid> grid;
        std::shared_ptr<PointLight> cursorLight;
        std::shared_ptr<DirectionalLight> sun;
        std::shared_ptr<Line3D> line3D; //debug purpose
        std::shared_ptr<Terrain> terrain;
        std::shared_ptr<Water> water;

        CollisionHandler collisionHandler;
        SceneManager sceneManager;
        Renderer renderer;
        MouseRay mouseRay;

        bool my_tool_active{true};

};


#endif // MAINSCENE_H
