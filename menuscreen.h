#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

class MenuScreen {
public:
    explicit MenuScreen(const sf::Font& font);
    void handleEvent(const sf::Event& event, GameState& state);
    void draw(sf::RenderWindow& window);

private:
    const sf::Font& font;
    std::vector<sf::Text> items;
    int selected = 0;

    void updateHighlight();
};
