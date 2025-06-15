#include "CreditsScreen.h"

CreditsScreen::CreditsScreen(const sf::Font& f)
    : font(f)
{
    std::vector<std::string> info = {
        "JumpKing Clone",
        "Autor: Twoje Imię",
        "Silnik: SFML + C++17",
        "DB: SQLite3",
        "",
        "Dziękuję za grę!"
    };
    for (int i = 0; i < (int)info.size(); ++i) {
        sf::Text t;
        t.setFont(font);
        t.setString(info[i]);
        t.setCharacterSize(28);
        t.setPosition(600, 150 + i * 50);
        lines.push_back(t);
    }
    backText.setFont(font);
    backText.setString("Wróć (Esc)");
    backText.setCharacterSize(24);
    backText.setPosition(50, 850);
}

void CreditsScreen::handleEvent(const sf::Event& ev, GameState& state) {
    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
        state = GameState::MainMenu;
    }
}

void CreditsScreen::draw(sf::RenderWindow& win) {
    for (auto& t : lines) win.draw(t);
    win.draw(backText);
}
