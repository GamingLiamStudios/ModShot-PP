#include "character.h"

#include <SFML/Graphics/Rect.hpp>
#include <cmath>

Character::Character(std::string name, std::string variant, sf::Vector2u pos, int frames)
    : frames(frames), pos(pos), dir(Direction::Up), frame(0), moving(0), move_speed(16)
{
    std::string path = "Graphics/Characters/" + name;
    if (!variant.empty()) path += "_" + variant;

    sheet = texture_cache.load_texture(path);
}

void Character::update()
{
    if (moving)
    {
        const float speed = 1.f / move_speed;
        if (((moving >> 1) % (move_speed / 2)) == 0) inc_frame();

        switch (dir)
        {
        case Direction::Up: pos.y -= speed; break;
        case Direction::Down: pos.y += speed; break;
        case Direction::Right: pos.x += speed; break;
        case Direction::Left: pos.x -= speed; break;
        }

        moving += 0b10;
        if ((moving >> 1) >= move_speed) moving = 0;
    }
}

void Character::set_direction(Direction dir)
{
    if (frame % 2 == 0) this->dir = dir;
}

Direction Character::get_direction()
{
    return dir;
}

void Character::running(bool running)
{
    move_speed = running ? 8 : 16;
}

void Character::inc_frame()
{
    frame++;
    while (frame >= frames) frame -= frames;
}

void Character::move(Direction dir)
{
    moving |= 1;
    set_direction(dir);
}

sf::Vector2f Character::get_position()
{
    return pos;
}

sf::Sprite Character::get_sprite()
{
    const int y = (sheet->getSize().y / 4);
    const int x = (sheet->getSize().x / frames);
    return sf::Sprite(*sheet, sf::IntRect(x * frame, y * (int) dir, x, y));
}
