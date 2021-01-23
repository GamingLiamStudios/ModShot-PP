#include "character.h"

#include <SFML/Graphics/Rect.hpp>

Character::Character(std::string name, std::string variant, int frames)
    : name(name), variant(variant), frames(frames), direction(Direction::Up), frame(0)
{
    if (variant.empty())
        sheet.loadFromFile("./Graphics/Characters/" + name + ".png");
    else
        sheet.loadFromFile("./Graphics/Characters/" + name + "_" + variant + ".png");
}

Character::Character(const Character &other)
{
}

Character Character::operator=(const Character &other)
{
    this->name    = other.name;
    this->variant = other.variant;
    this->frame   = other.frame;
    this->frames  = other.frames;
    set_direction(other.direction);
    this->sheet = other.sheet;

    return *this;
}

void Character::set_direction(Direction dir)
{
    this->direction = dir;
}

Direction Character::get_direction()
{
    return direction;
}

void Character::inc_frame()
{
    frame++;
    while (frame >= frames) frame -= frames;
}

sf::Sprite Character::get_sprite()
{
    const int y = (sheet.getSize().y / 4);
    const int x = (sheet.getSize().x / frames);
    return sf::Sprite(sheet, sf::IntRect(x * frame, y * (int) direction, x, y));
}
