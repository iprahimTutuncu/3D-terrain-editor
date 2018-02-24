#include "GameState.h"
#include "glm/ext.hpp"

#include <iostream>

using namespace std;

GameState::GameState(Game* parent)
:renderer((float) parent->getWidth(),(float) parent->getHeight()),
    collisionHandler(camera)
{
    this->parent = parent;

    sceneManager.init(parent->getWidth(), parent->getHeight());

    std::shared_ptr<DirectionalLight> d1 = sceneManager.makeDirectionalLight();
    d1->setDirection(glm::vec3(-2.0,2.0,-1.0));
    d1->enable(true);
    camera = sceneManager.getCamera();

    std::shared_ptr<PointLight> p1 = sceneManager.createPointLight();
    std::shared_ptr<PointLight> p2 = sceneManager.createPointLight();

    std::shared_ptr<Model> m1 = sceneManager.createModel("media/models/nanosuit/nanosuit.obj");
    std::shared_ptr<Model> m2 = sceneManager.createModel("media/models/bunny/bunny.obj");
    line3D = sceneManager.createLine3D(glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,2.0,0.0), glm::vec3(1.0,0.0,0.0));

    grid = sceneManager.createGrid(500, 1.0);
    cursorLight = sceneManager.createPointLight();

    std::shared_ptr<SceneNode> n = sceneManager.createNode();
    std::shared_ptr<SceneNode> n2 = sceneManager.createNode();
    std::shared_ptr<SceneNode> n3 = sceneManager.createNode();

    n->addComponent(m1);
    n->addComponent(p1);
    n->addComponent(grid);
    n->setScale(glm::vec3(0.2, 0.2, 0.2));
    n->setPosition(glm::vec3(0.0,0.0,0.0));

    n3->addComponent(m2);
    n3->addComponent(p2);
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



}

void GameState::update(const sf::Time& deltaTime)
{
    //std::cout << 1 / deltaTime.asSeconds() << std::endl;
    static float average = 0;
    average += 1 / deltaTime.asSeconds();
    static int counter = 0;
    counter++;

    renderer.update(deltaTime);
    std::cout << glm::to_string(mouseRay.getCurrRay()) << "le cursor point" << std::endl;
    std::cout << glm::to_string(camera->getPosition()) << "origine" << std::endl;
    //std::cout << glm::to_string(cursorLight->getLightProperties().position) << "le cursor lumiere est sur" << std::endl;
    glm::vec3 intersection;
    glm::vec3 beginPos = camera->getPosition() - glm::normalize(camera->getFront());
    line3D->setBeginPoint( beginPos);

    glm::vec3 endPos = beginPos - 100.0f * glm::normalize(camera->getFront()) ;
    line3D->setEndPoint(endPos);


    collisionHandler.setRayDirection(endPos);
    collisionHandler.setRayOrigin(beginPos);
    collisionHandler.setRayLength(100.0);
    collisionHandler.setGrid(grid);

    if(collisionHandler.isRayOnTerrain(intersection)){
        std::cout << "wew" << std::endl;
        cursorLight->setPosition(intersection);
    }

    std::cout << glm::to_string(intersection) << " intersection" << std::endl;
}

void GameState::draw()
{
    renderer.render();

    parent->getWindow().display();
}

