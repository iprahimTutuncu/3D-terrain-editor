#ifndef GAME_H
#define GAME_H

#include <SFML/Window.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include <stack>
#include <memory>

class BaseState;

class Game
{
    public:
        Game();
        void run();
        float getWidth();
        float getHeight();
        sf::Window& getWindow();

        void pushState(BaseState* state);
        void popState();
        void changeState(BaseState* state);
        BaseState* peekState();

    private:
        sf::Window window;
        std::stack<BaseState*> states;

        const unsigned int SCREEN_WIDTH  = 960;
        const unsigned int SCREEN_HEIGHT = 540;

        //variable jettez dans le tat




};

#endif // GAME_H
