#ifndef MAINMENU_H
#define MAINMENU_H

#include "SFML/Graphics.hpp"
#include "BaseState.h"

class MainMenuState : public BaseState
{
    public:
        MainMenuState(Game* parent);

        void event() override;
        void update(const sf::Time &deltaTime) override;
        void draw() override;

        ~MainMenuState();

    private:
        sf::RenderWindow window;
};

#endif // MAINMENU_H
