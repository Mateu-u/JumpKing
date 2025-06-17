#include "ScoreTableScreen.h"
#include <algorithm>

ScoreTableScreen::ScoreTableScreen(const sf::Font& f)
    : font(f)
{
    // Załaduj tło
    if (backgroundTexture.loadFromFile("tekstury/score_background.png")) {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            1540.f / backgroundTexture.getSize().x,
            920.f / backgroundTexture.getSize().y
            );
    }

    // Przygotuj listę wyników
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
        t.setFillColor(sf::Color::White);
        entries.push_back(t);
        y += 40;
    }

    // Przygotuj przycisk „Wyjście”
    exitButton.setSize({200.f, 60.f});
    exitButton.setFillColor(sf::Color(100, 100, 250, 200));
    exitButton.setPosition(670.f, 800.f); // środek przy szerokości 1540

    exitText.setFont(font);
    exitText.setString("Wyjscie");
    exitText.setCharacterSize(28);
    exitText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = exitText.getLocalBounds();
    exitText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
    exitText.setPosition(
        exitButton.getPosition().x + exitButton.getSize().x / 2.f,
        exitButton.getPosition().y + exitButton.getSize().y / 2.f
        );
}

void ScoreTableScreen::handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow& window) {
    if (ev.type == sf::Event::KeyPressed &&
        ev.key.code == sf::Keyboard::Escape) {
        state = GameState::Welcome;
    }

    if (ev.type == sf::Event::MouseButtonPressed) {
        auto mousePos = sf::Mouse::getPosition(window);
        if (exitButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
            state = GameState::Welcome;
        }
    }
}

void ScoreTableScreen::draw(sf::RenderWindow& win) {
    win.draw(backgroundSprite);

    for (auto& t : entries)
        win.draw(t);

    win.draw(exitButton);
    win.draw(exitText);
}
