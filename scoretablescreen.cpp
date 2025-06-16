#include "ScoreTableScreen.h"
#include <algorithm>

ScoreTableScreen::ScoreTableScreen(const sf::Font& f)
    : font(f)
{
    auto players = DatabaseManager::fetchAllPlayers();
    std::sort(players.begin(), players.end(),
              [](auto& a, auto& b){
                  return a.levels_completed > b.levels_completed;
              });
    int y = 200;
    for (int i = 0; i < (int)players.size() && i < 10; ++i) {
        auto& p = players[i];
        std::string line = std::to_string(i+1)
                           + ". " + p.username
                           + " - " + std::to_string(p.levels_completed);
        sf::Text t(line, font, 28);
        t.setPosition(500, y);
        y += 40;
        entries.push_back(t);
    }
}

void ScoreTableScreen::handleEvent(const sf::Event& ev, GameState& state) {
    if (ev.type == sf::Event::KeyPressed &&
        ev.key.code == sf::Keyboard::Escape)
    {
        state = GameState::Welcome;
    }
}

void ScoreTableScreen::draw(sf::RenderWindow& win) {
    for (auto& t : entries) win.draw(t);
    sf::Text back("Powrot (Esc)", font, 24);
    back.setPosition(50, 900);
    win.draw(back);
}
