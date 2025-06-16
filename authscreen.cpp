#include "AuthScreen.h"

AuthScreen::AuthScreen(const sf::Font& f) : font(f) {
    usernameLabel.setFont(font);
    usernameLabel.setString("Użytkownik:");
    usernameLabel.setCharacterSize(28);
    usernameLabel.setPosition(570, 250);

    passwordLabel.setFont(font);
    passwordLabel.setString("Hasło:");
    passwordLabel.setCharacterSize(28);
    passwordLabel.setPosition(570, 340);

    usernameBox.setSize({400, 40});
    usernameBox.setPosition(570, 280);
    usernameBox.setFillColor(sf::Color::White);

    passwordBox.setSize({400, 40});
    passwordBox.setPosition(570, 370);
    passwordBox.setFillColor(sf::Color::White);

    usernameText.setFont(font);
    usernameText.setCharacterSize(24);
    usernameText.setFillColor(sf::Color::Black);
    usernameText.setPosition(580, 285);

    passwordText.setFont(font);
    passwordText.setCharacterSize(24);
    passwordText.setFillColor(sf::Color::Black);
    passwordText.setPosition(580, 375);

    loginButton.setSize({200, 50});
    loginButton.setPosition(670, 460);
    loginButton.setFillColor(sf::Color(100, 200, 100));

    loginText.setFont(font);
    loginText.setString("Zaloguj");
    loginText.setCharacterSize(24);
    loginText.setFillColor(sf::Color::White);
    loginText.setPosition(loginButton.getPosition().x + 40, loginButton.getPosition().y + 10);

    guestButton.setSize({300, 50});
    guestButton.setPosition(620, 530);
    guestButton.setFillColor(sf::Color(100, 100, 250));

    guestText.setFont(font);
    guestText.setString("Kontynuuj bez logowania");
    guestText.setCharacterSize(22);
    guestText.setFillColor(sf::Color::White);
    guestText.setPosition(guestButton.getPosition().x + 20, guestButton.getPosition().y + 12);
}

void AuthScreen::handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow& window) {
    if (ev.type == sf::Event::MouseButtonPressed) {
        auto mouse = sf::Mouse::getPosition(window);
        usernameActive = usernameBox.getGlobalBounds().contains(mouse.x, mouse.y);
        passwordActive = passwordBox.getGlobalBounds().contains(mouse.x, mouse.y);

        if (loginButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
            // Tu: sprawdź dane logowania
            state = GameState::Playing;
        }
        if (guestButton.getGlobalBounds().contains(mouse.x, mouse.y)) {
            state = GameState::Playing;
        }
    }

    if (ev.type == sf::Event::TextEntered) {
        char c = static_cast<char>(ev.text.unicode);
        if (c >= 32 && c <= 126) {
            if (usernameActive) usernameInput += c;
            if (passwordActive) passwordInput += c;
        }
        if (ev.text.unicode == 8) {  // backspace
            if (usernameActive && !usernameInput.empty())
                usernameInput.pop_back();
            if (passwordActive && !passwordInput.empty())
                passwordInput.pop_back();
        }
    }

    usernameText.setString(usernameInput);
    passwordText.setString(std::string(passwordInput.length(), '*')); // ukrycie hasła
}

void AuthScreen::draw(sf::RenderWindow& win) {
    win.draw(usernameLabel);
    win.draw(passwordLabel);
    win.draw(usernameBox);
    win.draw(passwordBox);
    win.draw(usernameText);
    win.draw(passwordText);
    win.draw(loginButton);
    win.draw(loginText);
    win.draw(guestButton);
    win.draw(guestText);
}
