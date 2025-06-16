#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "GameState.h"

class AuthScreen {
public:
    AuthScreen(const sf::Font& font);
    void handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow& window);
    void draw(sf::RenderWindow& win);

private:
    const sf::Font& font;
    std::string usernameInput;
    std::string passwordInput;
    bool typingUsername = true;
    bool usernameActive = false;
    bool passwordActive = false;

    sf::Text usernameLabel;
    sf::Text passwordLabel;
    sf::RectangleShape usernameBox;
    sf::RectangleShape passwordBox;
    sf::Text usernameText;
    sf::Text passwordText;

    sf::RectangleShape loginButton;
    sf::RectangleShape guestButton;
    sf::Text loginText;
    sf::Text guestText;
};
