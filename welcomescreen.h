#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

class WelcomeScreen {
public:
    // ścieżka do pliku tła i fontu
    WelcomeScreen(const sf::Font& font, const std::string& bgImagePath);

    // obsługa eventów myszy
    void handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow& window);
    // rysowanie tła, tytułu i przycisków
    void draw(sf::RenderWindow& window);

private:
    // pojedynczy przycisk
    struct Button {
        sf::RectangleShape box;
        sf::Text            label;
    };

    sf::Texture           bgTex;
    sf::Sprite            bgSprite;

    sf::Text              title;
    const sf::Font&       font;
    std::vector<Button>   buttons;
    int                   hovered = -1;

    // uaktualnia hovered na podstawie pozycji myszy
    void updateHover(const sf::Vector2f& mousePos);
};
