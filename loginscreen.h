#pragma once
#include <SFML/Graphics.hpp>
#include "databasemanager.h"
#include "GameState.h"

class LoginScreen {
public:
    explicit LoginScreen(const sf::Font& font);

    // eventy: wpisywanie, backspace, Tab, Enter
    void handleEvent(const sf::Event& event, GameState& state);
    // rysowanie wszystkich elementów logowania
    void draw(sf::RenderWindow& window);

private:
    const sf::Font&      font;
    sf::Text             title;

    sf::Text             labelUser, labelPass;
    sf::RectangleShape   boxUser, boxPass;
    sf::Text             textUser, textPass;
    std::string          username, password;
    bool                 activeUserField = true;

    sf::Text             btnLogin, btnNewUser;
    sf::Text             errorText;

    void updateInputText();
};
