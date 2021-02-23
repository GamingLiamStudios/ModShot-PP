#pragma once

#include <string>
#include <vector>
#include <memory>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "textures.h"
#include "util/types.h"

class Character
{
private:
    int                          frames;
    std::shared_ptr<sf::Texture> sheet;

    sf::Vector2f pos;
    Direction    dir;

    u8 frame;
    u8 moving;
    u8 move_speed;    // In Updates per unit

public:
    Character() = default;
    Character(std::string name, std::string variant, sf::Vector2u pos, int frames = 4);

    void update();
    void move(Direction dir);

    void      set_direction(Direction dir);
    Direction get_direction();

    void running(bool running);

    inline void inc_frame();

    sf::Vector2f get_position();
    sf::Sprite   get_sprite();
};