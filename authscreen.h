#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "databasemanager.h"

class AuthScreen {
public:
    explicit AuthScreen(const sf::Font& font);
    void handleEvent(const sf::Event& ev, GameState& state);
    void draw(sf::RenderWindow& win);
private:
    const sf::Font& font;
    sf::Text labelUser, labelPass, textUser, textPass;
    sf::RectangleShape boxUser, boxPass;
    sf::Text btnLogin, btnGuest, errorMsg;
    std::string user, pass;
    bool activeUser = true;
    void updateInputText();
};
