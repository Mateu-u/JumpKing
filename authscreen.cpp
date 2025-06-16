#include "AuthScreen.h"

AuthScreen::AuthScreen(const sf::Font& f)
    : font(f)
{
    // user
    labelUser = sf::Text("User:", font, 24);
    labelUser.setPosition(500,200);
    boxUser.setPosition(580,200); boxUser.setSize({300,30});
    boxUser.setFillColor({100,100,100,200});
    textUser = sf::Text("", font, 20);
    textUser.setPosition(585,205);

    // pass
    labelPass = sf::Text("Pass:", font, 24);
    labelPass.setPosition(500,260);
    boxPass.setPosition(580,260); boxPass.setSize({300,30});
    boxPass.setFillColor({100,100,100,200});
    textPass = sf::Text("", font, 20);
    textPass.setPosition(585,265);

    // buttons
    btnLogin = sf::Text("Zaloguj (Enter)", font, 22);
    btnLogin.setPosition(580,330);
    btnGuest = sf::Text("Graj bez logowania (G)", font, 22);
    btnGuest.setPosition(580,370);

    // error
    errorMsg = sf::Text("", font, 20);
    errorMsg.setFillColor(sf::Color::Red);
    errorMsg.setPosition(500,420);
}

void AuthScreen::updateInputText() {
    textUser.setString(user);
    textPass.setString(std::string(pass.size(), '*'));
}

void AuthScreen::handleEvent(const sf::Event& ev, GameState& state) {
    if (ev.type == sf::Event::TextEntered) {
        char c = ev.text.unicode;
        if (c == 8) {
            if      (activeUser && !user.empty()) user.pop_back();
            else if (!activeUser && !pass.empty()) pass.pop_back();
        }
        else if (c >= 32 && c < 127) {
            if (activeUser) user.push_back(c);
            else            pass.push_back(c);
        }
        updateInputText();
    }
    else if (ev.type == sf::Event::KeyPressed) {
        if      (ev.key.code == sf::Keyboard::Tab)   activeUser = !activeUser;
        else if (ev.key.code == sf::Keyboard::Enter) {
            if (DatabaseManager::authenticate(user, pass)) {
                state = GameState::Playing;
            } else {
                errorMsg.setString("Błąd logowania!");
            }
        }
        else if (ev.key.code == sf::Keyboard::G) {
            state = GameState::Playing; // gra jako gość
        }
    }
}

void AuthScreen::draw(sf::RenderWindow& win) {
    win.draw(labelUser);
    win.draw(boxUser);
    win.draw(textUser);
    win.draw(labelPass);
    win.draw(boxPass);
    win.draw(textPass);
    win.draw(btnLogin);
    win.draw(btnGuest);
    if (!errorMsg.getString().isEmpty())
        win.draw(errorMsg);
}
