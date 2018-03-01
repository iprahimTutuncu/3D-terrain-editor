#include "GameState.h"
#include "glm/ext.hpp"

#include <iostream>

using namespace std;

GameState::GameState(Game* parent)
:renderer((float) parent->getWidth(),(float) parent->getHeight()),
 collisionHandler(camera),
 heightMap("media/texture/testHM.png")
{
    this->parent = parent;

    sceneManager.init(parent->getWidth(), parent->getHeight());

    sun = sceneManager.makeDirectionalLight();
    sun->setDirection(glm::vec3(0.0,1.0,0));
    sun->enable(true);
    camera = sceneManager.getCamera();

    std::shared_ptr<PointLight> p1 = sceneManager.createPointLight();
    std::shared_ptr<PointLight> p2 = sceneManager.createPointLight();

    std::shared_ptr<Model> m1 = sceneManager.createModel("media/models/nanosuit/nanosuit.obj");
    std::shared_ptr<Model> m2 = sceneManager.createModel("media/models/bunny/bunny.obj");
    line3D = sceneManager.createLine3D(glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,2.0,0.0), glm::vec3(1.0,0.0,0.0));

    grid = sceneManager.createGrid(100, 1.0);
    cursorLight = sceneManager.createPointLight();
    cursorLight->setAmbientColor(glm::vec3(1.0,0.0,1.0));
    cursorLight->setDiffuseColor(glm::vec3(1.0,0.0,1.0));
    cursorLight->setSpecularColor(glm::vec3(1.0,0.0,1.0));

    std::shared_ptr<SceneNode> n = sceneManager.createNode();
    std::shared_ptr<SceneNode> n2 = sceneManager.createNode();
    std::shared_ptr<SceneNode> n3 = sceneManager.createNode();

    //n->addComponent(m1);
    //n->addComponent(p1);
    n->addComponent(grid);
    //n->setScale(glm::vec3(0.2, 0.2, 0.2));
    n->setPosition(glm::vec3(0.0,0.0,0.0));

    n3->addComponent(m2);
    //n3->addComponent(p2);
    n3->setPosition(glm::vec3(10.0,2.0,0.0));

    n2->addComponent(cursorLight);
    n2->addComponent(line3D);

    sceneManager.getRootNode()->addChild(n);
    sceneManager.getRootNode()->addChild(n3);
    sceneManager.getRootNode()->addChild(n2);

    sceneManager.getRootNode()->updateAll();

    sceneManager.fillRenderer(renderer);

    renderer.setCubeMapFace(FACE::BACK,   "media/models/skybox/HQ_sea_sky/back.jpg");
    renderer.setCubeMapFace(FACE::BOTTOM, "media/models/skybox/HQ_sea_sky/bottom.jpg");
    renderer.setCubeMapFace(FACE::FRONT,  "media/models/skybox/HQ_sea_sky/front.jpg");
    renderer.setCubeMapFace(FACE::LEFT,   "media/models/skybox/HQ_sea_sky/left.jpg");
    renderer.setCubeMapFace(FACE::RIGHT,  "media/models/skybox/HQ_sea_sky/right.jpg");
    renderer.setCubeMapFace(FACE::TOP,    "media/models/skybox/HQ_sea_sky/top.jpg");

    //renderer.setPostProcessMode(PostProcessMode::GRAY_SCALE);
    renderer.setRenderMode(RenderMode::FILL);
    renderer.setHeightMapTextureID(heightMap.getTextureID());
    mouseRay.init(camera);

}

void GameState::event()
{
    sf::Event event;
    while (parent->getWindow().pollEvent(event))
    {
        if (event.type == sf::Event::Closed){
            parent->getWindow().close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
            parent->getWindow().close();
        }
    }

    float mousex = sf::Mouse::getPosition(parent->getWindow()).x;
    float mousey = sf::Mouse::getPosition(parent->getWindow()).y;

    camera->input_callback(mousex, mousey);
    mouseRay.update(mousex, mousey);


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        camera->move_forward();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        camera->move_back();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        camera->turn(-1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        camera->turn(1);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){

        int x = cursorLight->getLightProperties().position.x * heightMap.getSize().x;
        x /= grid->getSize();
        x+=heightMap.getSize().x/2;
        int z = cursorLight->getLightProperties().position.z * heightMap.getSize().y;
        z /= grid->getSize();
        z+=heightMap.getSize().y/2;
        std::cout << "x: " << x << ", y: " << z << std::endl;
        heightMap.addCircle(10.0f, x, z,0.022, 0.0022);

    }




}

void GameState::update(const sf::Time& deltaTime)
{
    //std::cout << 1 / deltaTime.asSeconds() << std::endl;
    static float average = 0;
    average += 1 / deltaTime.asSeconds();
    static int counter = 0;
    counter++;

    renderer.update(deltaTime);

    glm::vec3 intersection;
    glm::vec3 beginPos = camera->getPosition() - glm::normalize(camera->getFront());
    line3D->setBeginPoint( beginPos);

    glm::vec3 endPos = beginPos - 100.0f * glm::normalize(camera->getFront()) ;
    line3D->setEndPoint(endPos);

    collisionHandler.setRayDirection(endPos);
    collisionHandler.setRayOrigin(beginPos);
    collisionHandler.setRayLength(100.0);
    collisionHandler.setGrid(grid);

    static float t = 0.001;
    t+=0.01;
    sun->setDirection(glm::vec3(2.0, 1.0, 0.0));
    if(collisionHandler.isRayOnTerrain(intersection)){
        cursorLight->setPosition(intersection);
    }
}

void GameState::draw()
{
    renderer.render();

    parent->getWindow().display();
}

