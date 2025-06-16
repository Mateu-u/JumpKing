#include "CreditsScreen.h"

CreditsScreen::CreditsScreen(const sf::Font& f)
    : font(f)
{
    if (backgroundTexture.loadFromFile("tekstury/credits_background.png")) {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(1540.f / backgroundTexture.getSize().x,
                                  920.f / backgroundTexture.getSize().y);
    }

    std::vector<std::string> info = {
        "JumpKing Clone",
        "Autor: Wiktor Witczak, Mateusz Tomaszewski",
        "Silnik: SFML + C++17",
        "DB: SQLite3",
        "",
        "Dziekuje za gre!"
    };

    for (int i = 0; i < (int)info.size(); ++i) {
        sf::Text t;
        t.setFont(font);
        t.setString(info[i]);
        t.setCharacterSize(28);
        sf::FloatRect textRect = t.getLocalBounds();
        t.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
        t.setPosition(1540.f / 2.f, 200 + i * 50);
        t.setFillColor(sf::Color::White);
        lines.push_back(t);
    }

    exitButton.setSize({200.f, 60.f});
    exitButton.setFillColor(sf::Color(100, 100, 250, 200));
    exitButton.setPosition(670.f, 800.f);

    exitText.setFont(font);
    exitText.setString("Wyjście");
    exitText.setCharacterSize(28);
    exitText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = exitText.getLocalBounds();
    exitText.setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
    exitText.setPosition(
        exitButton.getPosition().x + exitButton.getSize().x / 2.f,
        exitButton.getPosition().y + exitButton.getSize().y / 2.f
        );
}

void CreditsScreen::handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow& window) {
    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
        state = GameState::Welcome;
    }

    if (ev.type == sf::Event::MouseButtonPressed) {
        auto mousePos = sf::Mouse::getPosition(window);
        if (exitButton.getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y)) {
            state = GameState::Welcome;
        }
    }
}

void CreditsScreen::draw(sf::RenderWindow& win) {
    win.draw(backgroundSprite);
    for (auto& t : lines) win.draw(t);
    win.draw(exitButton);
    win.draw(exitText);
}
