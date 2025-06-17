#include "WelcomeScreen.h"

WelcomeScreen::WelcomeScreen(const sf::Font& f, const std::string& bgImagePath)
    : font(f)
{
    // 1) wczytanie tła
    if (!bgTex.loadFromFile(bgImagePath)) {
        sf::Image img; img.create(1540, 920, sf::Color(30, 30, 30));
        bgTex.loadFromImage(img);
    }
    bgSprite.setTexture(bgTex);

    // 2) tytuł gry
    title.setFont(font);
    title.setString("JumpKing");
    title.setCharacterSize(72);
    auto tb = title.getLocalBounds();
    title.setPosition((bgTex.getSize().x - tb.width) / 2.f, 50);

    // 3) etykiety przycisków
    std::vector<std::string> labels = {
        "Graj",
        "Opcje",
        "Tabela wynikow",
        "Credits",
        "Wyjscie"
    };

    const float btnW = 400, btnH = 60;
    float startY = 180;
    for (int i = 0; i < (int)labels.size(); ++i) {
        Button b;
        b.box.setSize({btnW, btnH});
        b.box.setFillColor(sf::Color(70, 70, 70, 200));
        b.box.setPosition(
            (bgTex.getSize().x - btnW) / 2.f,
            startY + i * (btnH + 20)
            );

        b.label.setFont(font);
        b.label.setString(labels[i]);
        b.label.setCharacterSize(28);
        auto lb = b.label.getLocalBounds();
        b.label.setPosition(
            b.box.getPosition().x + (btnW - lb.width) / 2.f,
            b.box.getPosition().y + (btnH - lb.height) / 2.f - lb.top
            );

        buttons.push_back(b);
    }
}

void WelcomeScreen::updateHover(const sf::Vector2f& mp) {
    hovered = -1;
    for (int i = 0; i < (int)buttons.size(); ++i) {
        if (buttons[i].box.getGlobalBounds().contains(mp)) {
            hovered = i;
            break;
        }
    }
    for (int i = 0; i < (int)buttons.size(); ++i) {
        buttons[i].box.setFillColor(
            i == hovered
                ? sf::Color(100, 100, 250, 220)
                : sf::Color(70, 70, 70, 200)
            );
    }
}

void WelcomeScreen::handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow& window) {
    if (ev.type == sf::Event::MouseMoved) {
        updateHover({(float)ev.mouseMove.x, (float)ev.mouseMove.y});
    }
    else if (ev.type == sf::Event::MouseButtonPressed
             && ev.mouseButton.button == sf::Mouse::Left)
    {
        updateHover({(float)ev.mouseButton.x, (float)ev.mouseButton.y});
        if (hovered != -1) {
            switch (hovered) {
            case 0: state = GameState::Auth;        break;
            case 1: state = GameState::Options;     break;
            case 2: state = GameState::ScoreTable;  break;
            case 3: state = GameState::Credits;     break;
            case 4: window.close();                 break;  // ⬅ zamykanie gry
            }
        }
    }
}

void WelcomeScreen::draw(sf::RenderWindow& window) {
    window.draw(bgSprite);
    window.draw(title);
    for (auto& b : buttons) {
        window.draw(b.box);
        window.draw(b.label);
    }
}
