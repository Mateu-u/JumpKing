#include "PlatformGenerator.h"
#include <cstdlib>
#include <ctime>

std::vector<sf::RectangleShape> createPlatformsFromMap(
    const std::vector<std::string>& map,
    float tileWidth,
    float tileHeight)
{
    std::vector<sf::RectangleShape> platforms;
    const int rows = (int)map.size();
    const int cols = map.empty() ? 0 : (int)map[0].size();

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (map[y][x] == '#') {
                sf::RectangleShape plat;
                plat.setSize({ tileWidth, tileHeight });
                plat.setFillColor(sf::Color(100, 200, 100));
                plat.setPosition(x * tileWidth, y * tileHeight);
                platforms.push_back(plat);
            }
        }
    }

    return platforms;
}

