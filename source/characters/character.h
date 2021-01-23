#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "util/types.h"

class Character
{
private:
    std::string name;
    std::string variant;
    Direction   direction;
    int         frame;

    sf::Texture sheet;
    int         frames;

public:
    Character() = default;
    Character(std::string name, std::string variant, int frames = 4);

    Character(const Character &);
    Character operator=(const Character &);

    void      set_direction(Direction dir);
    Direction get_direction();
    void      inc_frame();

    sf::Sprite get_sprite();
};