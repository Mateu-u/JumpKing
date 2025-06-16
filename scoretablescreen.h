#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "GameState.h"
#include "databasemanager.h"

class ScoreTableScreen {
public:
    ScoreTableScreen(const sf::Font& font);
    void handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    const sf::Font& font;
    std::vector<sf::Text> entries;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::RectangleShape exitButton;
    sf::Text exitText;
};
