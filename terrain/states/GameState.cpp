#include "GameState.h"
#include "glm/ext.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

GameState::GameState(Game* parent)
:renderer((float) parent->getWidth(),(float) parent->getHeight()),
 collisionHandler(camera)
{


    this->parent = parent;

    terrain = std::make_shared<Terrain>("media/texture/testHM.png", "media/texture/testHM.png");
    terrain->addTerrainTexture("media/texture/terrain/grass_green_d.jpg", "media/texture/terrain/grass_ground_s.jpg","media/texture/terrain/grass_green_n.jpg" , 2);
    terrain->addTerrainTexture("media/texture/terrain/jungle_stone_d.jpg", "media/texture/terrain/jungle_stone_s.jpg", "media/texture/terrain/jungle_stone_n.jpg", 1);
    terrain->addTerrainTexture("media/texture/terrain/desert_sand_d.jpg", "media/texture/terrain/desert_sand_s.jpg" , "media/texture/terrain/desert_sand_n.jpg", 3);

    water = std::make_shared<Water>(parent->getWidth(), parent->getHeight());
    water->setHeight(5.0f);
    sceneManager.init(parent->getWidth(), parent->getHeight());

    sun = sceneManager.makeDirectionalLight();
    sun->setDirection(glm::vec3(1.0,0.1,0.3));
    sun->setAmbientColor(glm::vec3(0.1, 0.1,0.2));
    sun->setDiffuseColor(glm::vec3(0.5,0.3,0.0));
    sun->setSpecularColor(glm::vec3(0.7,0.7,0.3));
    sun->enable(true);
    camera = sceneManager.getCamera();
    camera->setPosition(glm::vec3(0.0, 2.0, 0.0));
    camera->setSpeed(5);

    std::shared_ptr<PointLight> p1 = sceneManager.createPointLight();
    std::shared_ptr<PointLight> p2 = sceneManager.createPointLight();

    std::shared_ptr<Model> m1 = sceneManager.createModel("media/models/nanosuit/nanosuit.obj");
    std::shared_ptr<Model> m2 = sceneManager.createModel("media/models/tree/treeHR.obj");
    //line3D = sceneManager.createLine3D(glm::vec3(0.0,0.0,0.0), glm::vec3(0.0,2.0,0.0), glm::vec3(1.0,0.0,0.0));

    grid = sceneManager.createGrid(terrain->getGridSize()/10, 10.0);
    cursorLight = sceneManager.createPointLight();
    cursorLight->setAmbientColor(glm::vec3(1.0,0.0,1.0));
    cursorLight->setDiffuseColor(glm::vec3(1.0,0.0,1.0));
    cursorLight->setSpecularColor(glm::vec3(1.0,0.0,1.0));
    cursorLight->pointLightHasCursor(true);

    std::shared_ptr<SceneNode> n = sceneManager.createNode();
    std::shared_ptr<SceneNode> n2 = sceneManager.createNode();
    std::shared_ptr<SceneNode> n3 = sceneManager.createNode();

    //n->addComponent(m1);
    //n->addComponent(p1);
    //n->addComponent(grid);
    //n->setScale(glm::vec3(0.2, 0.2, 0.2));
    n->setPosition(glm::vec3(0.0,0.0,0.0));

    //n3->addComponent(m2);
    //n3->addComponent(p2);
    //n3->setPosition(glm::vec3(0.0,1.0,0.0));
    //n3->setScale(glm::vec3(0.2,0.2,0.2));

    n2->addComponent(cursorLight);

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
    renderer.setTerrain(terrain);
    renderer.setWater(water);

    mouseRay.init(camera);


}

void GameState::event()
{
    static float elevationScale = 1.f;
    static float elevationRadiusScale = 1.f;

    sf::Event event;
    while (parent->getWindow().pollEvent(event))
    {
        if (event.type == sf::Event::Closed){
            parent->getWindow().close();
            exit(0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
            parent->getWindow().close();
            exit(0);
        }
        if(event.type == sf::Event::MouseWheelMoved){
            //ELEVATION SPEED
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
                if(event.mouseWheel.delta == 1)
                    elevationRadiusScale += 0.1f;
                else if(event.mouseWheel.delta == -1)
                    elevationRadiusScale -= 0.1f;

                if (elevationRadiusScale > 10.f)
                    elevationRadiusScale = 10.f;
                else if(elevationRadiusScale < 0.1f)
                    elevationRadiusScale  = 0.1f;
            }else{
                float multY = 1.0;

                if(event.mouseWheel.delta == 1)
                    multY = 1.1;
                else if(event.mouseWheel.delta == -1)
                    multY = 0.9;

                cursorLight->setAttenuation(1.0,
                                            cursorLight->getLightProperties().attenuationLinear * multY,
                                            cursorLight->getLightProperties().attenuationQuadratic * multY);
                }
        }
    }

    float mousex = sf::Mouse::getPosition(parent->getWindow()).x;
    float mousey = sf::Mouse::getPosition(parent->getWindow()).y;

    mouseRay.update(mousex, mousey);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
       sf::Keyboard::isKeyPressed(sf::Keyboard::O)){
        isWalkOnly = true;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
       sf::Keyboard::isKeyPressed(sf::Keyboard::I)){
        isWalkOnly = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        camera->move_forward();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)){
                terrain->saveHeightMap("heightmap0.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
                terrain->saveHeightMap("heightmap1.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
                terrain->saveHeightMap("heightmap2.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
                terrain->saveHeightMap("heightmap3.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)){
                terrain->saveHeightMap("heightmap4.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)){
                terrain->saveHeightMap("heightmap5.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)){
                terrain->saveHeightMap("heightmap6.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)){
                terrain->saveHeightMap("heightmap7.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)){
                terrain->saveHeightMap("heightmap8.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)){
                terrain->saveHeightMap("heightmap9.terrain");
            }else{
                std::cout << "Not a valid input for saving" << std::endl;
            }
        }else
            camera->move_back();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        camera->turn(-2);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        camera->turn(2);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)){
                terrain->loadHeightMap("heightmap0.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
                terrain->loadHeightMap("heightmap1.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){
                terrain->loadHeightMap("heightmap2.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){
                terrain->loadHeightMap("heightmap3.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)){
                terrain->loadHeightMap("heightmap4.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)){
                terrain->loadHeightMap("heightmap5.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)){
                terrain->loadHeightMap("heightmap6.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)){
                terrain->loadHeightMap("heightmap7.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)){
                terrain->loadHeightMap("heightmap8.terrain");
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)){
                terrain->loadHeightMap("heightmap9.terrain");
            }else{
                std::cout << "Not a valid input for loading" << std::endl;
            }
        }
    }

    camera->input_callback(mousex, mousey);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        //terrain.elevateWithCursorPosition(10.0f, cursorLight->getLightProperties().position);
        int x = cursorLight->getLightProperties().position.x * terrain->getGridSize();
        x /= grid->getSize();
        x+=terrain->getGridSize()/2;
        int z = cursorLight->getLightProperties().position.z * terrain->getGridSize();
        z /= grid->getSize();
        z+=terrain->getGridSize()/2;

        float extrude = 0.003 * elevationScale;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            terrain->addCircleOnTextureMap(100.f, x, z,cursorLight->getLightProperties().attenuationLinear, cursorLight->getLightProperties().attenuationQuadratic);
        else
            terrain->addCircle(extrude, x, z,cursorLight->getLightProperties().attenuationLinear, cursorLight->getLightProperties().attenuationQuadratic, elevationRadiusScale);
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        //terrain.elevateWithCursorPosition(10.0f, cursorLight->getLightProperties().position);
        int x = cursorLight->getLightProperties().position.x * terrain->getGridSize();
        x /= grid->getSize();
        x+=terrain->getGridSize()/2;
        int z = cursorLight->getLightProperties().position.z * terrain->getGridSize();
        z /= grid->getSize();
        z+=terrain->getGridSize()/2;


        float extrude = -0.003 * elevationScale;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){
            terrain->addCircleOnTextureMap(-100.f, x, z,cursorLight->getLightProperties().attenuationLinear, cursorLight->getLightProperties().attenuationQuadratic);
        }
        else
            terrain->addCircle(extrude, x, z,cursorLight->getLightProperties().attenuationLinear, cursorLight->getLightProperties().attenuationQuadratic, elevationRadiusScale);

    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        //terrain.elevateWithCursorPosition(10.0f, cursorLight->getLightProperties().position);
        int x = cursorLight->getLightProperties().position.x * terrain->getGridSize();
        x /= grid->getSize();
        x+=terrain->getGridSize()/2;
        int z = cursorLight->getLightProperties().position.z * terrain->getGridSize();
        z /= grid->getSize();
        z+=terrain->getGridSize()/2;

        terrain->addAveragingCircle(x, z,cursorLight->getLightProperties().attenuationLinear, cursorLight->getLightProperties().attenuationQuadratic);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){

        int x = cursorLight->getLightProperties().position.x * terrain->getGridSize();
        x /= grid->getSize();
        x+=terrain->getGridSize()/2;
        int z = cursorLight->getLightProperties().position.z * terrain->getGridSize();
        z /= grid->getSize();
        z+=terrain->getGridSize()/2;


        terrain->addSmoothCircle(x, z,cursorLight->getLightProperties().attenuationLinear, cursorLight->getLightProperties().attenuationQuadratic);
    }

    //CAMERA SPEED
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::M)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){
            camera->setSpeed(0.1f);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            camera->setSpeed(0.5f);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            camera->setSpeed(1.0f);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            camera->setSpeed(2.0f);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
            camera->setSpeed(4.0f);
    }

    //ELEVATION SPEED
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            elevationScale += 0.005f;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            elevationScale -= 0.005f;
        }

        if (elevationScale > 8.f)
            elevationScale = 8.f;
        else if(elevationScale < 0.2f)
            elevationScale  = 0.2f;


        //cursorLight->setDiffuseColor(cursorLight->getLightProperties().diffuse + );
        //cursorLight->setSpecularColor(cursorLight->getLightProperties().specular + );
    }


    glm::vec3 newAmbient(elevationScale, 0.f, elevationRadiusScale*0.618033f);
    cursorLight->setAmbientColor(newAmbient);

    glm::vec3 newDiffuse(0.f, 0.f, elevationRadiusScale);
    cursorLight->setDiffuseColor(newDiffuse);


}

void GameState::update(const sf::Time& deltaTime)
{
    terrain->addCircle(0, 1, 1,cursorLight->getLightProperties().attenuationLinear, cursorLight->getLightProperties().attenuationQuadratic, 10.f);

    std::cout << 1 / deltaTime.asSeconds() << std::endl;
    static float average = 0;
    average += 1 / deltaTime.asSeconds();
    static int counting = 0;
    counting++;
    std::cout << "average: " << average/counting << std::endl;
    static float counter = 0.001;
    counter+= 0.01;


    int x = 0;
    int z = 0;
    float terrainY = 0.f;
    if(isWalkOnly){
        float terrainSize = terrain->getGridSize();

        for(float i = -0.5; i <= 0.5; i+=0.05){
            for(float j = -0.5; j <= 0.5; j+=0.05){
                x  = (camera->getPosition().x+i)  * terrainSize;
                x /= terrainSize;
                x += terrainSize/2;

                z  = (camera->getPosition().z+j) * terrainSize;
                z /= terrainSize;
                z += terrainSize/2;

                terrainY += terrain->getPosition(x,z).y + terrain->getHeight(glm::vec2(x,z)) * 69;
            }
        }

        terrainY /= 400.f;


        glm::vec3 newCamPos(camera->getPosition());
        newCamPos.y = terrainY + 2.0f;
        camera->setPosition(newCamPos);

        float mousex = sf::Mouse::getPosition(parent->getWindow()).x;
        float mousey = sf::Mouse::getPosition(parent->getWindow()).y;
        camera->input_callback(mousex, mousey);

    }
    //SUN MOVE!
    sun->setDirection(glm::vec3(cos(counter) * 3.0, 1.0, sin(counter)* 3.0));

    glm::vec3 intersection;
    glm::vec3 beginPos = camera->getPosition();
    glm::vec3 endPos = beginPos + camera->getFarPlane() * mouseRay.getCurrRay();

    collisionHandler.setTerrain(terrain);
    collisionHandler.setRayDirection(mouseRay.getCurrRay());
    collisionHandler.setRayOrigin(beginPos);
    collisionHandler.setRayLength(camera->getFarPlane());

    static float t = 0.001;
    t+=0.01;

    if(collisionHandler.isRayOnTerrain(intersection)){
        cursorLight->setPosition(intersection);

        int x = cursorLight->getLightProperties().position.x * terrain->getGridSize();
        x /= grid->getSize();
        x+=terrain->getGridSize()/2;
        int z = cursorLight->getLightProperties().position.z * terrain->getGridSize();
        z /= grid->getSize();
        z+=terrain->getGridSize()/2;

        float y = terrain->getHeight(glm::vec2(x,z));
        y *= 69;
        //std::cout << y << std::endl;
        glm::vec3 height = glm::vec3(0.0,y,0.0);
        cursorLight->setPosition(cursorLight->getLightProperties().position + height);
    }

    renderer.update(deltaTime);

}

void GameState::draw()
{
    renderer.render();
    parent->getWindow().display();
}

GameState::~GameState()
{
    delete parent;
}

