#include "MainMenuState.h"

#include "GameState.h"

using namespace std;

MainMenuState::MainMenuState(Game *parent)
{
    this->parent = parent;
    window.create(sf::VideoMode(640, 480), "config menu");
    parent->getWindow().setVisible(false);
}

void MainMenuState::event()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed){
            window.close();
            parent->getWindow().setVisible(true);
            parent->getWindow().display(); // ??? si je le retire, ca crash :/
            parent->pushState(new GameState(parent));
        }
    }
}

void MainMenuState::update(const sf::Time &deltaTime)
{

}

void MainMenuState::draw()
{
    window.clear();
    window.display();

}

