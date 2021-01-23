#pragma once

#include <SFML/Graphics.hpp>

#include "characters/character.h"
#include "util/types.h"

class Game
{
private:
    sf::RenderWindow window;

    sf::Sprite world;

    Character player;
    struct
    {
        u8        held : 4;
        Direction dir : 4;
    } next_dir;
    u8 moving;

public:
    Game(int width, int height, const char *title);

    bool update();
    bool render();

    void close();

    bool running;
};