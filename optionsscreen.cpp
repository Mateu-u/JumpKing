#include "optionsscreen.h"
#include <iostream>

OptionsScreen::OptionsScreen(const sf::Font& f, const sf::Vector2f& windowSize)
    : font(f), winSize(windowSize)
{
    // --- bg preview (na razie puste)
    bgPreviewTex.create((unsigned)winSize.x, (unsigned)winSize.y);
    bgPreview.setTexture(bgPreviewTex);
    bgPreview.setColor(sf::Color(255,255,255,100)); // półprzezroczysty

    // --- suwaki ---
    auto makeSlider = [&](Slider& s, const std::string& txt, float y){
        s.track.setSize({300, 6});
        s.track.setFillColor(sf::Color(200,200,200));
        s.track.setPosition((winSize.x-300)/2, y);

        s.thumb.setSize({12,24});
        s.thumb.setFillColor(sf::Color(150,150,250));
        // thumb na środku tracka
        float tx = (winSize.x-300)/2 + (s.value/100.f)*300 - 6;
        s.thumb.setPosition(tx, y-9);

        s.label.setFont(font);
        s.label.setString(txt);
        s.label.setCharacterSize(20);
        s.label.setPosition((winSize.x-300)/2, y-40);
    };

    makeSlider(gameSlider,  "Game Volume", 300.f);
    makeSlider(musicSlider, "Music Volume", 400.f);

    // --- przycisk Wczytaj tło ---
//    createButton(loadBgBtn, "Wczytaj tło", {(winSize.x-200)/2, 500.f});
    // --- przycisk Wróć ---
    createButton(backBtn,   "Wyjscie",       {(winSize.x-200)/2, 580.f});
}

void OptionsScreen::createButton(Button& b, const std::string& txt, const sf::Vector2f& pos) {
    b.box.setSize({200, 40});
    b.box.setFillColor(sf::Color(70,70,70,200));
    b.box.setPosition(pos);

    b.label.setFont(font);
    b.label.setString(txt);
    b.label.setCharacterSize(22);
    // centrowanie tekstu
    auto lb = b.label.getLocalBounds();
    b.label.setPosition(
        pos.x + (200 - lb.width)/2.f,
        pos.y + (40 - lb.height)/2.f - lb.top
        );
}

void OptionsScreen::updateHover(const sf::Vector2f& mp) {
    hoveredBtn = -1;
    if (loadBgBtn.box.getGlobalBounds().contains(mp)) hoveredBtn = 0;
    if (backBtn.box.getGlobalBounds().contains(mp))   hoveredBtn = 1;
    // kolorowanie
    loadBgBtn.box.setFillColor( hoveredBtn==0
                                   ? sf::Color(100,100,250) : sf::Color(70,70,70,200) );
    backBtn.box.setFillColor(   hoveredBtn==1
                                 ? sf::Color(100,100,250) : sf::Color(70,70,70,200) );
}

void OptionsScreen::updateSliders(const sf::Vector2f& mp) {
    auto moveThumb = [&](Slider& s){
        float x0 = s.track.getPosition().x;
        float rel = std::clamp((mp.x - x0)/300.f, 0.f, 1.f);
        s.value = rel * 100.f;
        s.thumb.setPosition(x0 + rel*300.f - 6, s.track.getPosition().y-9);
    };
    if (draggingGame)  moveThumb(gameSlider);
    if (draggingMusic) moveThumb(musicSlider);
}

void OptionsScreen::handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow&) {
    sf::Vector2f mp;
    if (ev.type == sf::Event::MouseMoved) {
        mp = {(float)ev.mouseMove.x,(float)ev.mouseMove.y};
        updateHover(mp);
        updateSliders(mp);
    }
    else if (ev.type == sf::Event::MouseButtonPressed
             && ev.mouseButton.button == sf::Mouse::Left)
    {
        mp = {(float)ev.mouseButton.x,(float)ev.mouseButton.y};
        updateHover(mp);
        updateSliders(mp);

        // sprawdź suwaki
        if (gameSlider.thumb.getGlobalBounds().contains(mp)) {
            draggingGame = true;
        } else if (musicSlider.thumb.getGlobalBounds().contains(mp)) {
            draggingMusic = true;
        }
        // przyciski
        else if (hoveredBtn == 0) {
            // wczytaj nowe tło (stub)
            backgroundPath = "debug/custom_bg.png";
            if (!bgPreviewTex.loadFromFile(backgroundPath))
                std::cerr << "Failed to load custom_bg.png\n";
            bgPreview.setTexture(bgPreviewTex, true);
        }
        else if (hoveredBtn == 1) {
            state = GameState::Welcome;
        }
    }
    else if (ev.type == sf::Event::MouseButtonReleased
             && ev.mouseButton.button == sf::Mouse::Left)
    {
        draggingGame = draggingMusic = false;
    }
}

void OptionsScreen::draw(sf::RenderWindow& window) {
    // tło – półprzezroczyste
    window.draw(bgPreview);

    // suwaki
    window.draw(gameSlider.track);
    window.draw(gameSlider.thumb);
    window.draw(gameSlider.label);

    window.draw(musicSlider.track);
    window.draw(musicSlider.thumb);
    window.draw(musicSlider.label);

    // przyciski
    window.draw(loadBgBtn.box);
    window.draw(loadBgBtn.label);
    window.draw(backBtn.box);
    window.draw(backBtn.label);
}
