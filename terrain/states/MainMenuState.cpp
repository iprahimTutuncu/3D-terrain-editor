#include "MainMenuState.h"

#include "GameState.h"

using namespace std;

MainMenuState::MainMenuState(Game *parent)
{
    this->parent = parent;
    window.create(sf::VideoMode(640, 480), "config menu");
    parent->getWindow().setVisible(false);

        // Display the list of all the video modes available for fullscreen
std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
for (std::size_t i = 0; i < modes.size(); ++i)
{
    sf::VideoMode mode = modes[i];
    std::cout << "Mode #" << i << ": "
              << mode.width << "x" << mode.height << " - "
              << mode.bitsPerPixel << " bpp" << std::endl;
}
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

