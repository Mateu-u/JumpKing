#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"


class CreditsScreen {
public:
    explicit CreditsScreen(const sf::Font& font);
    void handleEvent(const sf::Event& event, GameState& state);
    void draw(sf::RenderWindow& window);

private:
    const sf::Font& font;
    std::vector<sf::Text> lines;
    sf::Text backText;
};
