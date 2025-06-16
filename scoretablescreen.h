#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "databasemanager.h"

class ScoreTableScreen {
public:
    explicit ScoreTableScreen(const sf::Font& font);
    void handleEvent(const sf::Event& ev, GameState& state);
    void draw(sf::RenderWindow& win);
private:
    const sf::Font& font;
    std::vector<sf::Text> entries;
};
