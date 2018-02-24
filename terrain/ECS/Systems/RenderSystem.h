#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <memory>
#include <string>
#include <unordered_map>
#include <SFML/Window.hpp>

#include "../../graphics/Shader.h"
#include "../../graphics/Model.h"
#include "../../graphics/light/PointLight.h"
#include "../../graphics/light/DirectionalLight.h"
#include "../../graphics/CubeMap.h"
#include "../../graphics/Camera.h"
#include "../../graphics/FrameBuffer.h"

#include "SystemBase.h"

namespace ECS{
    class RenderSystem: public SystemBase
    {
        public:
            RenderSystem(std::shared_ptr<sf::Window> window);

            void init(std::vector<std::shared_ptr<Entity>> entities) override;
            void update(const sf::Time &deltaTime) override;

        private:
            void initShader(const std::string &shaderName, ECS::Transformable &modelTrans);

            std::shared_ptr<sf::Window> p_window;

            std::unordered_map<std::string, std::shared_ptr<Shader>> p_shaders;
            std::vector<Model> models;
            std::vector<PointLight> Pointlights;
            std::vector<DirectionalLight> directionalLights;

            //position en parallèle avec les models.
            //est shared à cause que transformable est modifié dans d'autre system
            std::vector<std::shared_ptr<ECS::Transformable>> model_transforms;

            CubeMap cubemap;
            Camera camera;

    };
}
#endif // RENDERSYSTEM_H
