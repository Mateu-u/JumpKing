// PauseScreen.h
#pragma once
#include <SFML/Graphics.hpp>

class PauseScreen {
public:
    PauseScreen(sf::Font& font, sf::Vector2u winSize);

    void handleEvent(sf::Event& event, sf::RenderWindow& window, bool& resume, bool& exit);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape overlay;
    sf::Text resumeText;
    sf::Text exitText;
};
