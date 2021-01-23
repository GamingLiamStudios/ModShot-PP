#include <iostream>
#include <chrono>

#include "oneshot.h"
#include <boxer/boxer.h>

int main()
{
    Game oneshot(640, 480, "OneShot");

    while (oneshot.running)
    {
        // Handles Updates to game(Water tiles, Movement, Dialogue, etc)
        if (!oneshot.update())
        {
            boxer::show("Failed to Tick!", "OneShot", boxer::Style::Error);
            oneshot.close();
            break;
        }
        // Actually renders the game
        if (!oneshot.render())
        {
            boxer::show("Failed to Render!", "OneShot", boxer::Style::Error);
            oneshot.close();
        }
    }

    return 0;
}

Game::Game(int width, int height, const char *title)
{
    window.create(sf::VideoMode(width, height), title);
    window.setFramerateLimit(40);    // Fixed FPS
    running = window.isOpen();

    player        = Character("niko", "bulb");
    moving        = 0;
    next_dir.held = false;
}

bool Game::update()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed: close(); break;

        // Input Handling
        case sf::Event::KeyPressed:
        {
            auto old = next_dir.held;
            switch (event.key.code)
            {
            case sf::Keyboard::Key::Up:
            case sf::Keyboard::W: next_dir.held |= 0b0001; break;
            case sf::Keyboard::Key::Down:
            case sf::Keyboard::S: next_dir.held |= 0b0010; break;
            case sf::Keyboard::Key::Left:
            case sf::Keyboard::A: next_dir.held |= 0b0100; break;
            case sf::Keyboard::Key::Right:
            case sf::Keyboard::D: next_dir.held |= 0b1000; break;
            }
            if (old != next_dir.held)    // Input update
            {
                bool move = true;

                // Check if W and S are both pressed
                if (((next_dir.held & 0b0011) - 0b0011) == 0)
                    // Check if A or D are pressed and are both not pressed
                    if (next_dir.held & 0b1100 && ((next_dir.held & 0b1100) - 0b1100))
                        next_dir.dir = (next_dir.held >> 3) ? Direction::Right : Direction::Left;
                    else
                        move = false;

                // Check if A and D are both pressed
                if (((next_dir.held & 0b1100) - 0b1100) == 0)
                    // Check if W or S are pressed and are both not pressed
                    if (next_dir.held & 0b0011 && ((next_dir.held & 0b0011) - 0b0011))
                    {
                        next_dir.dir = (next_dir.held >> 1) ? Direction::Down : Direction::Up;
                        move         = true;
                    }
                    else
                        move = false;

                if (move) switch (next_dir.held & ~old)
                    {
                    case 0b0001: next_dir.dir = Direction::Up; break;
                    case 0b0010: next_dir.dir = Direction::Down; break;
                    case 0b0100: next_dir.dir = Direction::Left; break;
                    case 0b1000: next_dir.dir = Direction::Right; break;
                    }

                moving &= ~1;
                moving |= move ? 1 : 0;
            }
        }
        break;
        case sf::Event::KeyReleased:
        {
            auto old = next_dir.held;
            switch (event.key.code)
            {
            case sf::Keyboard::Key::Up:
            case sf::Keyboard::W: next_dir.held &= ~0b0001; break;
            case sf::Keyboard::Key::Down:
            case sf::Keyboard::S: next_dir.held &= ~0b0010; break;
            case sf::Keyboard::Key::Left:
            case sf::Keyboard::A: next_dir.held &= ~0b0100; break;
            case sf::Keyboard::Key::Right:
            case sf::Keyboard::D: next_dir.held &= ~0b1000; break;
            }

            if (old != next_dir.held)    // Input update
            {
                bool move = true;
                if (((next_dir.held & 0b0011) - 0b0011) == 0)    // Check if W and S are both
                                                                 // pressed
                    // Check if A or D are pressed and are both not pressed
                    if (next_dir.held & 0b1100 && ((next_dir.held & 0b1100) - 0b1100))
                        next_dir.dir = (next_dir.held >> 3) ? Direction::Right : Direction::Left;
                    else
                        move = false;

                if (((next_dir.held & 0b1100) - 0b1100) == 0)    // Check if A and D are both
                                                                 // pressed
                    // Check if W or S are pressed and are both not pressed
                    if (next_dir.held & 0b0011 && ((next_dir.held & 0b0011) - 0b0011))
                    {
                        next_dir.dir = (next_dir.held >> 1) ? Direction::Down : Direction::Up;
                        move         = true;
                    }
                    else
                        move = false;
                if (next_dir.held == 0) move = false;

                if (move) switch (old & ~next_dir.held)
                    {
                    case 0b0001: next_dir.dir = Direction::Down; break;
                    case 0b0010: next_dir.dir = Direction::Up; break;
                    case 0b0100: next_dir.dir = Direction::Right; break;
                    case 0b1000: next_dir.dir = Direction::Left; break;
                    }

                moving &= ~1;
                moving |= move ? 1 : 0;
            }
        }
        break;
        }
    }

    if (next_dir.held && !(moving >> 1)) player.set_direction(next_dir.dir);
    if (moving)
    {
        const int walk_speed = 16;    // Magic number
        if (((moving >> 1) % (walk_speed / 2)) == 0) player.inc_frame();

        switch (player.get_direction())
        {
        case Direction::Up: world.move(sf::Vector2f(0, 48 / float(walk_speed))); break;
        case Direction::Down: world.move(sf::Vector2f(0, -(48 / float(walk_speed)))); break;
        case Direction::Right: world.move(sf::Vector2f(32 / float(walk_speed), 0)); break;
        case Direction::Left: world.move(sf::Vector2f(-(32 / float(walk_speed)), 0)); break;
        }

        moving += 0b10;

        if ((moving >> 1) >= walk_speed) moving &= 1;
    }

    return true;
}

bool Game::render()
{
    window.clear(sf::Color::Black);

    window.draw(world);

    auto player_sprite = player.get_sprite();
    player_sprite.setPosition(
      640 / 2 - player_sprite.getTextureRect().width / 2,
      480 / 2 - player_sprite.getTextureRect().height / 2);
    window.draw(player_sprite);

    window.display();
    return true;
}

void Game::close()
{
    window.close();
    running = false;
}