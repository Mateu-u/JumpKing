#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameState.h"

class CreditsScreen {
public:
    CreditsScreen(const sf::Font& font);
    void handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow& window);
    void draw(sf::RenderWindow& win);

private:
    const sf::Font& font;
    std::vector<sf::Text> lines;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::RectangleShape exitButton;
    sf::Text exitText;
};
