#include "LoginScreen.h"
#include <iostream>

LoginScreen::LoginScreen(const sf::Font& f)
    : font(f)
{
    title.setFont(font);
    title.setString("Logowanie");
    title.setCharacterSize(48);
    title.setPosition(600,  50);

    labelUser.setFont(font);
    labelUser.setString("User:");
    labelUser.setCharacterSize(24);
    labelUser.setPosition(500, 200);
    boxUser.setPosition(580, 200);
    boxUser.setSize({300,30});
    boxUser.setFillColor({100,100,100,200});

    labelPass.setFont(font);
    labelPass.setString("Pass:");
    labelPass.setCharacterSize(24);
    labelPass.setPosition(500, 260);
    boxPass.setPosition(580, 260);
    boxPass.setSize({300,30});
    boxPass.setFillColor({100,100,100,200});

    textUser.setFont(font);
    textUser.setCharacterSize(20);
    textUser.setPosition(585, 205);

    textPass.setFont(font);
    textPass.setCharacterSize(20);
    textPass.setPosition(585, 265);

    btnLogin.setFont(font);
    btnLogin.setString("Zaloguj (Enter)");
    btnLogin.setCharacterSize(22);
    btnLogin.setPosition(580, 330);

    btnNewUser.setFont(font);
    btnNewUser.setString("Nowy (N)");
    btnNewUser.setCharacterSize(22);
    btnNewUser.setPosition(580, 370);

    errorText.setFont(font);
    errorText.setCharacterSize(20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(500, 430);
}

void LoginScreen::updateInputText() {
    textUser.setString(username);
    // maskujemy hasło:
    std::string masked(password.size(), '*');
    textPass.setString(masked);
}

void LoginScreen::handleEvent(const sf::Event& ev, GameState& state) {
    if (ev.type == sf::Event::TextEntered) {
        auto c = ev.text.unicode;
        if (c == 8) { // Backspace
            if (activeUserField && !username.empty()) username.pop_back();
            else if (!activeUserField && !password.empty()) password.pop_back();
        }
        else if (c >= 32 && c < 127) {
            if (activeUserField)    username.push_back(char(c));
            else                    password.push_back(char(c));
        }
        updateInputText();
    }
    else if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Tab) {
            activeUserField = !activeUserField;
        }
        else if (ev.key.code == sf::Keyboard::Enter) {
            // próba logowania
            if (DatabaseManager::authenticate(username, password)) {
                state = GameState::MainMenu;
                errorText.setString("");
            } else {
                errorText.setString("Błąd logowania!");
            }
        }
        else if (ev.key.code == sf::Keyboard::N) {
            // nowy user
            if (DatabaseManager::createUser(username, password)) {
                errorText.setString("Utworzono konto. Zaloguj się.");
            } else {
                errorText.setString("Błąd tworzenia konta!");
            }
        }
    }
}

void LoginScreen::draw(sf::RenderWindow& win) {
    win.draw(title);
    win.draw(labelUser);
    win.draw(boxUser);
    win.draw(textUser);
    win.draw(labelPass);
    win.draw(boxPass);
    win.draw(textPass);
    win.draw(btnLogin);
    win.draw(btnNewUser);
    if (!errorText.getString().isEmpty())
        win.draw(errorText);
}
