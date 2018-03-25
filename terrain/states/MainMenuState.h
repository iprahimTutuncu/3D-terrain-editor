#ifndef MAINMENU_H
#define MAINMENU_H

#include <TGUI/TGUI.hpp>

#include "BaseState.h"

class MainMenuState : public BaseState
{
    public:
        MainMenuState(Game* parent);

        void event() override;
        void update(const sf::Time &deltaTime) override;
        void draw() override;
    private:
        sf::RenderWindow window;
};

#endif // MAINMENU_H
