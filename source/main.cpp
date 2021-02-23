#include <iostream>
#include <chrono>
#include <cassert>

#include "game.h"
#include "textures.h"
#include <boxer/boxer.h>

#ifndef GAME_TITLE
#define GAME_TITLE "OneShot"
#endif

int main()
{
    Game game(640, 480, GAME_TITLE);

    auto        last   = std::chrono::high_resolution_clock::now();
    auto        timer  = std::chrono::high_resolution_clock::now();
    const float ns     = 1000000000 / 40.f;
    float       delta  = 0;
    int         frames = 0;

    // Delta-time not needed due to window.display blocking
    while (game.running)
    {
        auto now = std::chrono::high_resolution_clock::now();
        delta += (now - last).count() / ns;
        last = now;

        while (delta >= 1.f)
        {
            // Handles Updates to game(Water tiles, Movement, Dialogue, etc)
            if (!game.update())
            {
                boxer::show("Failed to Tick!", GAME_TITLE, boxer::Style::Error);
                game.close();
                break;
            }

            delta--;
        }

        // Actually renders the game
        if (!game.render())
        {
            boxer::show("Failed to Render!", GAME_TITLE, boxer::Style::Error);
            game.close();
            break;
        }
        frames++;

        if (now - timer >= std::chrono::seconds(1))
        {
            game.set_window_title(std::string(GAME_TITLE) + " FPS: " + std::to_string(frames));
            frames = 0;
            timer += std::chrono::seconds(1);

            texture_cache.purge();
        }
    }

    return 0;
}

Game::Game(int width, int height, const char *title)
{
    window.create(sf::VideoMode(width, height), title);
    window.setFramerateLimit(80);    // Fixed FPS. Double the normal UPS
    running = window.isOpen();

    center = window.getPosition();
    shake  = false;

    player        = Character("niko", "bulb", { 0, 0 });
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
            player.running(event.key.shift);

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
            case sf::Keyboard::O: shake = true; break;
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

                moving = move;
            }
        }
        break;
        case sf::Event::KeyReleased:
        {
            player.running(event.key.shift);

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
            case sf::Keyboard::O:
            {
                shake = false;
                window.setPosition(center);
            }
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

                moving = move;
            }
        }
        break;
        }
    }

    if (next_dir.held && moving) player.move(next_dir.dir);
    player.update();

    if (shake)
    {
        auto random = sf::Vector2i((std::rand() % 20) - 10, (std::rand() % 20) - 10);
        window.setPosition(center + random);
    }
    else if (window.getPosition() != center)
        center = window.getPosition();

    return true;
}

bool Game::render()
{
    window.clear(sf::Color::Black);

    auto player_sprite = player.get_sprite();
    player_sprite.setPosition(
      640 / 2 - player_sprite.getTextureRect().width / 2,
      480 / 2 - player_sprite.getTextureRect().height / 2);
    player_sprite.move({ player.get_position().x * 48, player.get_position().y * 32 });
    window.draw(player_sprite);

    window.display();
    return true;
}

void Game::set_window_title(std::string title)
{
    window.setTitle(title);
}

void Game::close()
{
    window.close();
    running = false;
}