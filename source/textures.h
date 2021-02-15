#pragma once

#include <unordered_map>
#include <string>
#include <utility>
#include <cassert>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>

#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H
namespace
{
    struct TextureMap
    {
    private:
        // Why pair? To keep track of who owns the texture ptr
        std::vector<std::pair<sf::Texture *, int>> textures;
        std::unordered_map<std::string, int>       indexer;

    public:
        int push_texture(std::string &str)
        {
            std::cout << "Loading " << str << "\n";
            if (indexer.find(str) != indexer.end())
            {
                auto index = indexer[str];
                textures[index].second++;
                return index;
            }
            auto texture = new sf::Texture();
            texture->loadFromFile("./" + str + ".png");
            textures.push_back(std::make_pair(texture, int(1)));
            indexer[str] = textures.size() - 1;
            std::cout << "Loaded at index " << textures.size() - 1 << "\n";
            return textures.size() - 1;
        }
        void pop_texture(int index)
        {
            if (textures.size() <= index) return;
            textures[index].second--;
            if (textures[index].second == 0)
            {
                std::cout << "Removing index " << index << "\n";
                delete textures[index].first;
                textures.erase(textures.begin() + index);

                auto it = std::find_if(
                  indexer.begin(),
                  indexer.end(),
                  [index](const std::unordered_map<std::string, int>::value_type &vt) {
                      return vt.second == index;
                  });
                if (it != indexer.end()) indexer.erase(it);
            }
        }
        inline sf::Texture *operator[](int index)
        {
            assert(index < textures.size() && "Index out of bounds!");
            textures[index].second++;
            return textures[index].first;
        }
    };
}    // namespace
#endif

static TextureMap loaded_textures;
