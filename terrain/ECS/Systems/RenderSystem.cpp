#include "RenderSystem.h"

#include "../../ressourceManager/ShaderManager.h"

namespace ECS{

    RenderSystem::RenderSystem(std::shared_ptr<sf::Window> window)
    :p_window(window),
     camera((float) window->getSize().x,(float) window->getSize().y),
     postProcess(window->getSize().x, window->getSize().y)
    {
        camera.setSpeed(10);
        camera.move_forward();
    }

    void RenderSystem::init(std::vector<std::shared_ptr<Entity>> entities)
    {
        //Recherche des entités légitimes pour le rendering
        for(auto p_entity: entities)
            if(p_entity->hasComponent(ECS::COMPONENT::RENDERABLE))
                this->entities.emplace_back(p_entity);

        //Initialisation des shaders
        //possiblement mettre cela dans un fichier txt.
        std::vector<std::string> shaderName = {
            "light",
            "outlineModel",
            "postProcess",
            "skybox"
        };

        std::string shaderLocation[] = {
            "media/shader/light/light.vert",  "media/shader/light/light.frag",
            "media/shader/outlineModel/outlineModel.vert", "media/shader/outlineModel/outlineModel.frag",
            "media/shader/postProcess/postProcess.vert", "media/shader/postProcess/postProcess.frag",
            "media/shader/skybox/skybox.vert", "media/shader/skybox/skybox.frag"
        };

        for(int i = 0; i < shaderName.size(); i+=1){
            int j = i * 2;
            std::shared_ptr<Shader> p_shader;
            p_shader = RessourceManager::ShaderManager::get(shaderLocation[j], shaderLocation[j+1]);
            p_shaders[shaderName[i]] = p_shader;
        }

        //initialisation des objets du jeux
        for(auto p_entity: entities){
            auto render = p_entity->getComponent<ECS::Renderable>(ECS::COMPONENT::RENDERABLE);

            //initialsation des models
            if(render->type == "model"){
                models.emplace_back(Model(render->path));
                if(p_entity->hasComponent(ECS::COMPONENT::TRANSFORMABLE)){
                    auto trans = p_entity->getComponent<ECS::Transformable>(ECS::COMPONENT::TRANSFORMABLE);
                    modelTransforms.emplace_back(trans);
                    std::cout << "wow" << std::endl;
                }
            }
            //initialisation des lumières
            else if(render->type == "light")
                if(p_entity->hasComponent(ECS::COMPONENT::LIGHT)){
                    auto light = p_entity->getComponent<ECS::LightComponent>(ECS::COMPONENT::LIGHT);

                    if(light->isDirection){
                        DirectionalLight dir;
                        dir.setAmbientColor(light->ambient);
                        dir.setDiffuseColor(light->diffuse);
                        dir.setSpecularColor(light->specular);
                        directionalLights.emplace_back(dir);
                    }

                    else if(light->isPoint){
                        PointLight point;
                        point.setAmbientColor(light->ambient);
                        point.setDiffuseColor(light->diffuse);
                        point.setSpecularColor(light->specular);
                        point.setAttenuation(light->attenuationConstant, light->attenuationLinear, light->attenuationQuadratic);
                        Pointlights.emplace_back(point);
                    }
                }
            else if(render->type == "cubemap"){
                if(p_entity->hasComponent(ECS::COMPONENT::CUBEMAP)){
                    auto cube = p_entity->getComponent<ECS::Cubemap>(ECS::COMPONENT::CUBEMAP);

                    cubemap.setFace(FACE::BACK,   cube->backPath);
                    cubemap.setFace(FACE::BOTTOM, cube->bottomPath);
                    cubemap.setFace(FACE::FRONT,  cube->frontPath);
                    cubemap.setFace(FACE::LEFT,   cube->leftPath);
                    cubemap.setFace(FACE::RIGHT,  cube->rightPath);
                    cubemap.setFace(FACE::TOP,    cube->topPath);
                }
            }
        }
    }

    void RenderSystem::update(const sf::Time& deltaTime)
    {
        p_window->display();

    }

    void RenderSystem::initShader(const std::string& shaderName, ECS::Transformable& modelTrans)
    {
        MaterialProperties mat;
        mat.shininess = 32;
        mat.emission = glm::vec3(0.0,0.0,0.1);

        glm::mat4 mod = modelTrans.rotation * modelTrans.scale * modelTrans.transformation;

        p_shaders[shaderName]->use();
        p_shaders[shaderName]->setMat4("viewProj", this->camera.getViewProjectionMatrix());
        p_shaders[shaderName]->setMat4("modelView", this->camera.getViewMatrix() * mod);
        p_shaders[shaderName]->setMat4("model", mod);
        p_shaders[shaderName]->setVec3("eyePosition", this->camera.getPosition());
        p_shaders[shaderName]->setInt("skybox", 3);
        p_shaders[shaderName]->setFloat("mat.shininess", mat.shininess);
        p_shaders[shaderName]->setVec3("mat.emission", mat.emission);

        for(DirectionalLight light: directionalLights)
            p_shaders[shaderName]->addLight(light.getLightProperties());
        for(PointLight light: Pointlights)
            p_shaders[shaderName]->addLight(light.getLightProperties());

    }


}
