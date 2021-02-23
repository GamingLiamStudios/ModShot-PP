#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "world/character.h"
#include "util/types.h"
#include "textures.h"

class Game
{
private:
    sf::RenderWindow window;
    sf::Vector2i     center;
    bool             shake;

    struct
    {
        struct
        {
            int         autotileCount;
            sf::Texture texture;
        } atlas;    // Contains all AutoTiles & currently loaded Tilemap
        sf::VertexArray render;
    } map;

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