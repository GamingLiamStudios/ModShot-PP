#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "world/character.h"
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
    bool moving;

public:
    Game(int width, int height, const char *title);

    bool update();
    bool render();

    void set_window_title(std::string title);
    void close();

    bool running;
};