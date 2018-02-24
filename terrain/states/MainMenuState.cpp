#include "MainMenuState.h"

#include "GameState.h"

using namespace std;

MainMenuState::MainMenuState(Game *parent)
{
    this->parent = parent;
}

void MainMenuState::event()
{
    sf::Event event;
    while (parent->getWindow().pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            parent->getWindow().close();
    }
}

void MainMenuState::update(const sf::Time &deltaTime)
{

}

void MainMenuState::draw()
{
    parent->getWindow().display();
    parent->pushState(new GameState(parent));
}

