#include "Game.h"
#include "states/GameState.h"
#include "states/MainMenuState.h"

Game::Game():
    window()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2;

    settings.majorVersion = 3;
    settings.minorVersion = 2;
    settings.attributeFlags = sf::ContextSettings::Core;

    window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "OpenGL", sf::Style::Close, settings);
    window.setActive(false);
    window.setVerticalSyncEnabled(true);
    glewExperimental = GL_TRUE;
    glewInit();

   // this->states.push(new MainMenuState(this));
   this->states.push(new GameState(this));
}

float Game::getWidth()
{
    return SCREEN_WIDTH;
}

float Game::getHeight()
{
    return SCREEN_HEIGHT;
}

sf::Window& Game::getWindow()
{
    return window;
}

void Game::pushState(BaseState* state)
{
    this->states.push(state);
}

void Game::popState()
{
    delete this->states.top();
    this->states.pop();
}

void Game::changeState(BaseState* state)
{
    if(!this->states.empty())
        popState();
    pushState(state);
}

BaseState* Game::peekState()
{
    if(this->states.empty())
        return nullptr;
    return this->states.top();
}



void Game::run()
{
    //gestion du temps
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.f / 60.f);

    while(states.size() != 0){
        states.top()->event();

        //faire la loop des update seulement si le compteur fait le temps requis
        /*
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate > timePerFrame){
            timeSinceLastUpdate -= timePerFrame;
            scenes.top()->update(timePerFrame);
        }
        */
        sf::Time timePassed = clock.getElapsedTime();
        states.top()->update(timePassed);
        clock.restart();
        states.top()->draw();
    }
}


