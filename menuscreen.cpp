#include "MenuScreen.h"

MenuScreen::MenuScreen(const sf::Font& f)
    : font(f)
{
    std::vector<std::string> labels = {"Graj", "Wybierz gracza", "Credits"};
    for (int i = 0; i < (int)labels.size(); ++i) {
        sf::Text t;
        t.setFont(font);
        t.setString(labels[i]);
        t.setCharacterSize(36);
        t.setPosition(680, 200 + i * 80);
        items.push_back(t);
    }
    updateHighlight();
}

void MenuScreen::updateHighlight() {
    for (int i = 0; i < (int)items.size(); ++i) {
        items[i].setFillColor(i == selected ? sf::Color::Yellow : sf::Color::White);
    }
}

void MenuScreen::handleEvent(const sf::Event& ev, GameState& state) {
    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Up) {
            if (--selected < 0) selected = items.size()-1;
            updateHighlight();
        }
        else if (ev.key.code == sf::Keyboard::Down) {
            if (++selected >= (int)items.size()) selected = 0;
            updateHighlight();
        }
        else if (ev.key.code == sf::Keyboard::Enter) {
            switch (selected) {
            case 0: state = GameState::Playing;      break;
            case 1: state = GameState::PlayerSelect; break;
            case 2: state = GameState::Credits;      break;
            }
        }
    }
}

void MenuScreen::draw(sf::RenderWindow& win) {
    for (auto& t : items) win.draw(t);
}
