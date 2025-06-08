// main.cpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>      // wymaga C++17
#include <map>
#include "Player.h"
#include "PlatformGenerator.h"

// -----------------------------------------------------------------------------
// Parametry gry
// -----------------------------------------------------------------------------
const int   WINDOW_WIDTH    = 1540;
const int   WINDOW_HEIGHT   = 920;
const float MAX_JUMP_FORCE  = 18.0f;

// Ścieżka do folderu z grafikami
const std::string RES_DIR   = "debug/tekstury/";

int main()
{
    // Debug: sprawdź, gdzie exe szuka plików
    std::cout << "Working dir: " << std::filesystem::current_path() << "\n";

    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Jump King"
        );
    window.setFramerateLimit(60);

    // --- 1) Wczytanie tła ---
    sf::Texture bgTex;
    std::string bgFile = RES_DIR + "background.png";
    if (!bgTex.loadFromFile(bgFile)) {
        std::cerr << "ERROR loading background: " << bgFile << "\n";
        sf::Image img; img.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color(20,20,40));
        bgTex.loadFromImage(img);
    }
    sf::Sprite bgSprite(bgTex);

    // --- 2) Wczytanie tekstur gracza ---
    std::map<std::string, sf::Texture> textures;
    for (auto name : {"idle", "jump_charge", "jump", "fall"}) {
        sf::Texture t;
        std::string file = RES_DIR + "player_" + name + ".png";
        if (!t.loadFromFile(file)) {
            std::cerr << "ERROR loading player texture: " << file << "\n";
            sf::Image ph; ph.create(256,256, sf::Color::Red);
            t.loadFromImage(ph);
        }
        t.setSmooth(false);
        textures[name] = t;
    }

    // --- 3) Utworzenie gracza i platform ---
    Player player(
        WINDOW_WIDTH / 2.f,
        WINDOW_HEIGHT - 120.f,
        textures,
        0.15f
        );
    auto platforms = createJumpKingPlatforms(WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::View view({0,0,(float)WINDOW_WIDTH,(float)WINDOW_HEIGHT});
    sf::FloatRect prevBounds = player.sprite.getGlobalBounds();
    int score = 0;

    // --- 4) Przygotowanie HUD ---
    sf::Font font;
    if (!font.loadFromFile(RES_DIR + "arial.ttf")) {
        std::cerr << "ERROR loading font\n";
    }
    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    sf::RectangleShape barBack({200,20});
    barBack.setFillColor(sf::Color(50,50,50,200));
    barBack.setPosition(10,40);
    sf::RectangleShape barFront({0,20});
    barFront.setFillColor(sf::Color(150,150,250,200));
    barFront.setPosition(10,40);

    // --- 5) Pętla gry ---
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        // logika gracza
        player.handleInput();
        prevBounds = player.sprite.getGlobalBounds();
        player.applyVelocityX();
        player.applyGravity();

        // kolizje i score
        auto currBounds = player.sprite.getGlobalBounds();
        int landed = player.checkCollisions(prevBounds, currBounds, platforms);
        if (landed != -1 && landed != player.lastLandedPlat) {
            score += 10;
            player.lastLandedPlat = landed;
        }

        // reset, jeśli spadnie poza ekran
        if (player.sprite.getPosition().y - currBounds.height/2.f > WINDOW_HEIGHT) {
            player.sprite.setPosition(WINDOW_WIDTH/2.f, WINDOW_HEIGHT - 120.f);
            player.velocity = {0,0};
            player.jumpCharge   = 0.f;
            player.chargingJump = false;
            player.isOnGround   = false;
            player.lastLandedPlat = -1;
            score = 0;
            view.setCenter(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);
        }

        player.updateAnimationState();

        // rysowanie
        window.clear();

        // tło (fixed view)
        window.setView(window.getDefaultView());
        window.draw(bgSprite);

        // scena (view)
        window.setView(view);
        for (auto& plat : platforms)
            window.draw(plat);
        player.draw(window);

        // HUD (fixed view)
        window.setView(window.getDefaultView());
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);
        window.draw(barBack);
        float w = (player.jumpCharge / MAX_JUMP_FORCE) * barBack.getSize().x;
        barFront.setSize({ w, barFront.getSize().y });
        window.draw(barFront);

        window.display();
    }

    return 0;
}
