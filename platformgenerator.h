#ifndef PLATFORM_GENERATOR_H
#define PLATFORM_GENERATOR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Tworzy platformy na podstawie tekstowej mapy (znaki # = platforma)
std::vector<sf::RectangleShape>
createPlatformsFromMap(const std::vector<std::string>& map,
                       float tileWidth, float tileHeight);


#endif
