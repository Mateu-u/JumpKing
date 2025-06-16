#include "PauseScreen.h"

PauseScreen::PauseScreen(sf::Font& font, sf::Vector2u winSize) {
    overlay.setSize(sf::Vector2f(winSize));
    overlay.setFillColor(sf::Color(0, 0, 0, 150)); // półprzezroczysta nakładka

    resumeText.setFont(font);
    resumeText.setString("Kontynuuj");
    resumeText.setCharacterSize(40);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setPosition(winSize.x / 2.f - 100.f, winSize.y / 2.f - 50.f);

    exitText.setFont(font);
    exitText.setString("Wyjscie");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(winSize.x / 2.f - 100.f, winSize.y / 2.f + 20.f);
}

void PauseScreen::handleEvent(sf::Event& event, sf::RenderWindow& window, bool& resume, bool& exit) {
    if (event.type == sf::Event::MouseButtonPressed) {
        auto mousePos = sf::Mouse::getPosition(window);
        if (resumeText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            resume = true;
        }
        else if (exitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            exit = true;
        }
    }
}

void PauseScreen::draw(sf::RenderWindow& window) {
    window.draw(overlay);
    window.draw(resumeText);
    window.draw(exitText);
}
