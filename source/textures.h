#pragma once

#include <unordered_map>
#include <iostream>
#include <memory>

#include <SFML/Graphics/Texture.hpp>

#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H
namespace
{
    struct TextureMap
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;

    public:
        std::shared_ptr<sf::Texture> load_texture(std::string &str)
        {
            std::cout << "Loading '" << str << "'\n";
            if (textures.find(str) != textures.end()) return textures[str];
            auto texture = std::make_shared<sf::Texture>();
            texture->loadFromFile("./" + str + ".png");
            textures[str] = texture;
            return texture;
        }
        void purge()
        {
            for (auto &pair : textures)
                if (pair.second.unique())
                {
                    std::cout << "Unloading '" + pair.first + "'\n";
                    textures.erase(pair.first);
                }
        }
    };
}    // namespace
#endif

static TextureMap texture_cache;
