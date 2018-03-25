#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <SFML/System/Time.hpp>

#include "Camera.h"
#include "CubeMap.h"
#include "PostProcess.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"
#include "Model.h"
#include "Shadow.h"
#include "Grid.h"
#include "Line3D.h"
#include "../core/Terrain.h"

enum class RenderMode {FILL,
                       LINE,
                       POINT};

class Renderer
{
    public:
        Renderer(float width, float height);
        ~Renderer();

        void init();

        void update(const sf::Time &deltaTime);

        void addModel(std::string name, std::shared_ptr<Model> model);
        void removeModel(std::string name);

        void addGrid(std::string name, std::shared_ptr<Grid> grid);
        void removeGrid(std::string name);

        void addLine3D(int ID, std::shared_ptr<Line3D> line3d);
        void removeLine3D(int ID);

        void setRenderMode(RenderMode renderMode);
        void setPostProcessMode(PostProcessMode postProcessMode);

        void setCamera(std::shared_ptr<Camera> camera);
        void setCubeMapFace(FACE face, std::string fileName);

        void addPointLight(std::string name, std::shared_ptr<PointLight> pointLight);
        void setDirectionalLight(std::shared_ptr<DirectionalLight> directionalLight);

        void removePointLight(std::string name);
        void removeDirectionalLight(std::string name);

        void enableShadowMap(bool enable);

        void setTerrain(std::shared_ptr<Terrain> terrain);

        void render();

    private:
        Shadow shadow;
        CubeMap cubeMap;
        PostProcess postProcess;
        RenderMode currRenderMode;

        std::shared_ptr<Terrain> terrain;

        std::shared_ptr<Camera> camera;
        std::shared_ptr<DirectionalLight> directionalLight;

        std::unordered_map<std::string, std::shared_ptr<Shader>> p_shaders;
        std::unordered_map<std::string, std::shared_ptr<PointLight>> pointLights;
        std::unordered_map<std::string, std::shared_ptr<Model>> models;
        std::unordered_map<std::string, std::shared_ptr<Grid>> grids;
        std::unordered_map<int, std::shared_ptr<Line3D>> lines3D;


};

#endif // RENDERER_H
