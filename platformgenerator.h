// PlatformGenerator.h
#ifndef PLATFORM_GENERATOR_H
#define PLATFORM_GENERATOR_H

#include <SFML/Graphics.hpp>
#include <vector>

// Tworzy wektor platform typu Jump King
// windowWidth, windowHeight – rozmiary okna gry
std::vector<sf::RectangleShape>
createJumpKingPlatforms(int windowWidth, int windowHeight);

#endif // PLATFORM_GENERATOR_H
