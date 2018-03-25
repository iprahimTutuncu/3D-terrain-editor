#include "Renderer.h"
#include <iostream>

Renderer::Renderer(float width, float height):
    shadow(width, height),
    postProcess(width, height)
{
    init();


}
void Renderer::init()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(!shadow.init())
        std::cout << "Error: failed to init shadow" << std::endl;
    if(!postProcess.init())
        std::cout << "Error: failed to init postProcess" << std::endl;

    //Initialisation des shaders
    //possiblement mettre cela dans un fichier txt.
    std::vector<std::string> shaderName = {
        "light",
        "flatNoLight",
        "terrain"
    };

    std::string shaderLocation[] = {
        "media/shader/light/light.vert",  "media/shader/light/light.frag",
        "media/shader/flatNoLight/flatNoLight.vert",  "media/shader/flatNoLight/flatNoLight.frag",
        "media/shader/terrain/terrain.vert",  "media/shader/terrain/terrain.frag",


    };

    for(unsigned int i = 0; i < shaderName.size(); i+=1){
        int j = i * 2;
        std::shared_ptr<Shader> p_shader;
        p_shader = RessourceManager::ShaderManager::get(shaderLocation[j], shaderLocation[j+1]);
        p_shaders[shaderName[i]] = p_shader;
    }

}

void Renderer::update(const sf::Time& deltaTime)
{
    shadow.update(*directionalLight, *camera);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, shadow.getDepthTexture());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.getID());

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, terrain->getTextureID());

    //std::cout << shadow.getFarPlane() << std::endl;

    //LIGHT(MODEL) SHADER UPDATE
    p_shaders["light"]->use();
    p_shaders["light"]->setMat4("viewProj", camera->getViewProjectionMatrix());
    p_shaders["light"]->setVec3("eyePosition", camera->getPosition());
    p_shaders["light"]->setInt("depth_texture", 4);
    p_shaders["light"]->setMat4("lightSpaceCoordinate", shadow.getLightViewProjectionMatrix());

    p_shaders["light"]->setInt("skybox", 3);

    p_shaders["light"]->addLight(directionalLight->getLightProperties());
    for(auto pl: pointLights)
        p_shaders["light"]->addLight(pl.second->getLightProperties());

    //TERRAIN SHADER UPDATE
    p_shaders["terrain"]->use();
    p_shaders["terrain"]->setMat4("viewProj", camera->getViewProjectionMatrix());
    p_shaders["terrain"]->setVec3("eyePosition", camera->getPosition());
    p_shaders["terrain"]->setInt("depth_texture", 4);
    p_shaders["terrain"]->setMat4("lightSpaceCoordinate", shadow.getLightViewProjectionMatrix());

    p_shaders["terrain"]->setInt("heightmap", 5);

    p_shaders["terrain"]->addLight(directionalLight->getLightProperties());
    for(auto pl: pointLights)
        p_shaders["terrain"]->addLight(pl.second->getLightProperties());

    //FLAT WITH LIGHT SHADER UPDATE
    p_shaders["flatNoLight"]->use();
    p_shaders["flatNoLight"]->setMat4("viewProj", camera->getViewProjectionMatrix());
}

void Renderer::addModel(std::string name, std::shared_ptr<Model>model)
{
    models.insert({name, model});
}

void Renderer::removeModel(std::string name)
{
    if(models.find(name) != models.end())
        models.erase(name);

}

 void Renderer::addGrid(std::string name, std::shared_ptr<Grid>grid)
{
    grids.insert({name, grid});
}

void Renderer::removeGrid(std::string name)
{
    if(grids.find(name) != grids.end())
        grids.erase(name);
}

void Renderer::setRenderMode(RenderMode renderMode)
{
    this->currRenderMode = renderMode;
    if(renderMode == RenderMode::FILL)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else if(renderMode == RenderMode::LINE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if(renderMode == RenderMode::POINT)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

}

void Renderer::setPostProcessMode(PostProcessMode postProcessMode)
{
    postProcess.setPostProcessMode(postProcessMode);
}

void Renderer::setCamera(std::shared_ptr<Camera> camera)
{
    this->camera = camera;
}

void Renderer::setCubeMapFace(FACE face, std::string fileName)
{
    cubeMap.setFace(face, fileName);
}

void Renderer::addLine3D(int ID, std::shared_ptr<Line3D>line3d)
{
    lines3D.insert({ID, line3d});
}

void Renderer::removeLine3D(int ID)
{
    if(lines3D.find(ID) != lines3D.end())
        lines3D.erase(ID);
}

void Renderer::addPointLight(std::string name, std::shared_ptr<PointLight> pointLight)
{
    pointLights.insert({name, pointLight});
}

void Renderer::setDirectionalLight(std::shared_ptr<DirectionalLight>directionalLight)
{
    this->directionalLight = directionalLight;
}

void Renderer::removePointLight(std::string name)
{
    if(pointLights.find(name) != pointLights.end())
        pointLights.erase(name);
}


void Renderer::enableShadowMap(bool enable)
{
    //à finir
}

void Renderer::setTerrain(std::shared_ptr<Terrain>terrain)
{
    this->terrain = terrain;
}


void Renderer::render()
{
    //dessine l'ombre des models
    shadow.beginDraw();
    for(auto m: models)
        shadow.drawShadow(*m.second, m.second->getWorldTransform());
    shadow.endDraw();

    //dessine l'ecran sur la texture pour le post-processing
    postProcess.beginDraw();

    glClearColor(0.0f, 0.5f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    cubeMap.draw(camera->getProjectionMatrix(), camera->getViewMatrix());

    //draw terrain
    p_shaders["terrain"]->use();
    p_shaders["terrain"]->setMat4("model", terrain->getWorldTransform());
    p_shaders["terrain"]->setMat4("modelView", camera->getViewMatrix() * terrain->getWorldTransform());
    terrain->draw(*p_shaders["terrain"]);


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(auto g: grids){
        p_shaders["flatNoLight"]->use();
        p_shaders["flatNoLight"]->setMat4("model", g.second->getWorldTransform());
        g.second->draw(*p_shaders["flatNoLight"]);
    }
    setRenderMode(this->currRenderMode);



    for(auto m: models){
        p_shaders["light"]->use();
        p_shaders["light"]->setMat4("model", m.second->getWorldTransform());
        p_shaders["light"]->setMat4("modelView", camera->getViewMatrix() * m.second->getWorldTransform());

        m.second->Draw(*p_shaders["light"]);
    }

    for(auto l: lines3D){
        p_shaders["flatNoLight"]->use();
        p_shaders["flatNoLight"]->setMat4("model", l.second->getWorldTransform());

        l.second->draw(*p_shaders["flatNoLight"]);
    }
    postProcess.endDraw();

    //dessine les ecrans
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    postProcess.drawScreen();
    //shadow.drawScreen();
    setRenderMode(this->currRenderMode);
}




Renderer::~Renderer()
{
    //dtor
}
