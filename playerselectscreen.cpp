#include "PlayerSelectScreen.h"

PlayerSelectScreen::PlayerSelectScreen(const sf::Font& f)
    : font(f)
{
    infos = DatabaseManager::fetchAllPlayers();
    for (auto& pi : infos) {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(28);
        t.setString(pi.username + "  [" + std::to_string(pi.levels_completed) + "]");
        t.setPosition(550, 200 + entries.size() * 50);
        entries.push_back(t);
    }
    updateHighlight();
}

void PlayerSelectScreen::updateEntries() {
    entries.clear();
    infos = DatabaseManager::fetchAllPlayers();
    for (auto& pi : infos) {
        // zbuduj napis
        std::string label = pi.username + "  [" + std::to_string(pi.levels_completed) + "]";
        // sf::Text(konst sf::String& string, const sf::Font& font, unsigned int characterSize)
        sf::Text t(label, font, 28);
        t.setPosition(550, 200 + entries.size() * 50);
        entries.push_back(t);
    }
    updateHighlight();
}


void PlayerSelectScreen::updateHighlight() {
    for (int i = 0; i < (int)entries.size(); ++i) {
        entries[i].setFillColor(i == selected ? sf::Color::Yellow : sf::Color::White);
    }
}

void PlayerSelectScreen::handleEvent(const sf::Event& ev, GameState& state) {
    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Up) {
            if (--selected < 0) selected = entries.size()-1;
            updateHighlight();
        }
        else if (ev.key.code == sf::Keyboard::Down) {
            if (++selected >= (int)entries.size()) selected = 0;
            updateHighlight();
        }
        else if (ev.key.code == sf::Keyboard::Enter && !entries.empty()) {
            // zapamiętujemy id i startujemy grę
            selectedId = infos[selected].id;
            state = GameState::Playing;
        }
        else if (ev.key.code == sf::Keyboard::Escape) {
            state = GameState::MainMenu;
        }
    }
}

void PlayerSelectScreen::draw(sf::RenderWindow& win) {
    if (entries.empty()) {
        sf::Text no("Brak graczy. Wróć do menu i utwórz nowe konto.", font, 24);
        no.setPosition(400, 300);
        win.draw(no);
    } else {
        for (auto& t : entries) win.draw(t);
    }
}
