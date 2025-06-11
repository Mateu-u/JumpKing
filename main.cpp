// main.cpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>      // wymaga C++17
#include <map>
#include <vector>
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
    // Debug: sprawdź working directory
    std::cout << "Working dir: " << std::filesystem::current_path() << "\n";

    // Okno
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Jump King"
        );
    window.setFramerateLimit(60);

    // 1) Tło
    sf::Texture bgTex;
    if (!bgTex.loadFromFile(RES_DIR + "background.png")) {
        std::cerr << "ERROR loading background\n";
        sf::Image img; img.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color(20,20,40));
        bgTex.loadFromImage(img);
    }
    sf::Sprite bgSprite(bgTex);

    // 2) Tekstury gracza
    std::map<std::string, sf::Texture> textures;
    for (auto name : {"idle","jump_charge","jump","fall"}) {
        sf::Texture t;
        if (!t.loadFromFile(RES_DIR + "player_" + name + ".png")) {
            std::cerr << "ERROR loading player texture: " << name << "\n";
            sf::Image ph; ph.create(256,256, sf::Color::Red);
            t.loadFromImage(ph);
        }
        t.setSmooth(false);
        textures[name] = t;
    }

    // 3) Wczytanie tekstury platform
    sf::Texture platformTex;
    if (!platformTex.loadFromFile(RES_DIR + "platform.png")) {
        std::cerr << "ERROR loading platform.png\n";
    }
    platformTex.setRepeated(true);

    // 4) Pierwotne kształty platform (kolorowe, tylko pozycje+rozmiary)
    auto shapePlatforms = createJumpKingPlatforms(WINDOW_WIDTH, WINDOW_HEIGHT);

    // 5) Konwersja na sprite'y z teksturą
    std::vector<sf::Sprite> platforms;
    platforms.reserve(shapePlatforms.size());
    for (auto& shape : shapePlatforms) {
        sf::Sprite spr(platformTex);
        // ustaw region tekstury równy rozmiarowi shape'a
        auto sz = shape.getSize();
        spr.setTextureRect({0, 0,
                            static_cast<int>(sz.x),
                            static_cast<int>(sz.y)});
        // ustaw pozycję
        spr.setPosition(shape.getPosition());
        platforms.push_back(spr);
    }

    // 6) Gracz
    Player player(
        WINDOW_WIDTH/2.f,
        WINDOW_HEIGHT - 120.f,
        textures,
        0.15f
        );

    sf::View view({0,0,(float)WINDOW_WIDTH,(float)WINDOW_HEIGHT});
    sf::FloatRect prevBounds = player.sprite.getGlobalBounds();
    int score = 0;

    // 7) HUD
    sf::Font font;
    if (!font.loadFromFile(RES_DIR + "arial.ttf"))
        std::cerr << "ERROR loading font\n";
    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    sf::RectangleShape barBack({200,20});
    barBack.setFillColor(sf::Color(50,50,50,200));
    barBack.setPosition(10,40);
    sf::RectangleShape barFront({0,20});
    barFront.setFillColor(sf::Color(150,150,250,200));
    barFront.setPosition(10,40);

    // 8) Pętla gry
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e))
            if (e.type == sf::Event::Closed)
                window.close();

        // Logika
        player.handleInput();
        prevBounds = player.sprite.getGlobalBounds();
        player.applyVelocityX();
        player.applyGravity();

        // Kolizje i score
        auto currBounds = player.sprite.getGlobalBounds();
        int landed = player.checkCollisions(prevBounds, currBounds, shapePlatforms);
        if (landed != -1 && landed != player.lastLandedPlat) {
            score += 10;
            player.lastLandedPlat = landed;
        }

        // Reset po upadku
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

        // Render
        window.clear();

        // Tło (fixed view)
        window.setView(window.getDefaultView());
        window.draw(bgSprite);

        // Platformy (fixed view)
        for (auto& spr : platforms)
            window.draw(spr);

        // Gracz (kamera)
        window.setView(view);
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
