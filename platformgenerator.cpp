#include "PlatformGenerator.h"
#include <cstdlib>
#include <ctime>

std::vector<sf::RectangleShape>
createJumpKingPlatforms(int windowWidth, int windowHeight)
{
    std::vector<sf::RectangleShape> platforms;
    platforms.reserve(100);

    // podłoga
    sf::RectangleShape floor;
    floor.setSize({ (float)windowWidth, 20.f });
    floor.setFillColor(sf::Color(100,100,100));
    floor.setPosition(0.f, windowHeight - 20.f);
    platforms.push_back(floor);

    std::srand((unsigned)std::time(nullptr));
    float baseY = windowHeight - 20.f;

    for (int i = 1; i <= 100; ++i) {
        sf::RectangleShape plat;
        float y = baseY - i * 150.f;
        float x = static_cast<float>(std::rand() % (windowWidth - 250) + 50);
        plat.setSize({ 200.f, 20.f });
        plat.setFillColor(sf::Color(100,200,100));
        plat.setPosition(x, y);
        platforms.push_back(plat);
    }

    return platforms;
}
