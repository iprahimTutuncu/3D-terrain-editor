#ifndef SCENE_H
#define SCENE_H

#include <SFML/System/Time.hpp>

#include "../Game.h"

class BaseState
{
    public:
        virtual void event() = 0;
        virtual void update(const sf::Time &deltaTime) = 0;
        virtual void draw() = 0;
    protected:
        Game *parent;
};

#endif // SCENE_H
