// main.cpp

#include <SFML/Graphics.hpp>
#include <iostream>
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
const int NUM_LEVELS = 3;

// Ścieżka do folderu z grafikami
const std::string RES_DIR   = "tekstury/";

int main()
{
    // Okno
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Jump King"
        );
    window.setFramerateLimit(60);

    // 1) Tło
    std::vector<sf::Texture> backgroundTextures;
    for (int i = 0; i < NUM_LEVELS; ++i) {
        sf::Texture bgTex;
        if (!bgTex.loadFromFile(RES_DIR + "background_" + std::to_string(i+1) + ".png")) {
            std::cerr << "ERROR loading background for level " << i+1 << "\n";
            sf::Image img; img.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color(20,20,40 + i*20));
            bgTex.loadFromImage(img);
        }
        backgroundTextures.push_back(bgTex);
    }

    sf::Sprite bgSprite(backgroundTextures[0]); // Początkowe tło

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
    if (!platformTex.loadFromFile(RES_DIR + "testWall.png")) {
        std::cerr << "ERROR loading platform.png\n";
    }
    platformTex.setRepeated(true);

    // 4) Pierwotne kształty platform (kolorowe, tylko pozycje+rozmiary)
    std::vector<std::vector<std::string>> level = {
    {
        "##             #",
        "##       #######",
        "##           ###",
        "##             #",
        "####           #",
        "########       #",
        "#####          #",
        "##             #",
        "##           ###",
        "#        #######",
        "#         ######",
        "#         ######",
        "#####     ######",
        "################"
    },
    {
        "#              #",
        "###      #######",
        "#              #",
        "#              #",
        "#              #",
        "#  #####       #",
        "#              #",
        "#           ####",
        "####           #",
        "#              #",
        "#        ####  #",
        "#              #",
        "#####          #",
        "#              #"
    },
    {
        "################",
        "##       #######",
        "##           ###",
        "##             #",
        "####           #",
        "#####          #",
        "########       #",
        "##             #",
        "##           ###",
        "#         ######",
        "#       ########",
        "#              #",
        "####       #####",
        "#              #"
    }
    };

    float tileH = WINDOW_HEIGHT / static_cast<float>(level[0].size());
    float tileW = WINDOW_WIDTH / static_cast<float>(level[0][0].size());
    auto shapePlatforms = createPlatformsFromMap(level[0], tileW, tileH);


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
        WINDOW_HEIGHT - 220.f,
        textures,
        0.10f
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

// Przy przejściu do wyższego poziomu:
if (player.sprite.getPosition().y + player.sprite.getGlobalBounds().height/2 < 0) {
    int newLevel = player.getCurrentLevel() + 1;
    if (newLevel < NUM_LEVELS) {
        player.setCurrentLevel(newLevel);

        // Zmień tło
        bgSprite.setTexture(backgroundTextures[newLevel]);

        // Załaduj nowe platformy
        shapePlatforms = createPlatformsFromMap(level[newLevel], tileW, tileH);
        platforms.clear();
        for (auto& shape : shapePlatforms) {
            sf::Sprite spr(platformTex);
            spr.setTextureRect({0, 0, static_cast<int>(shape.getSize().x),
                              static_cast<int>(shape.getSize().y)});
            spr.setPosition(shape.getPosition());
            platforms.push_back(spr);
        }

        // Zachowaj prędkość i pozycję X, tylko przenieś w dół ekranu
        player.sprite.setPosition(
            player.sprite.getPosition().x, // Zachowaj X
            WINDOW_HEIGHT - 10.f // Ustaw na samym dole ekranu
        );
    }
}

// Przy spadaniu na niższy poziom:
if (player.sprite.getPosition().y - player.sprite.getGlobalBounds().height/2 > WINDOW_HEIGHT) {
    int newLevel = player.getCurrentLevel() - 1;
    if (newLevel >= 0) {
        player.setCurrentLevel(newLevel);

        // Zmień tło
        bgSprite.setTexture(backgroundTextures[newLevel]);

        // Załaduj nowe platformy
        shapePlatforms = createPlatformsFromMap(level[newLevel], tileW, tileH);
        platforms.clear();
        for (auto& shape : shapePlatforms) {
            sf::Sprite spr(platformTex);
            spr.setTextureRect({0, 0, static_cast<int>(shape.getSize().x),
                              static_cast<int>(shape.getSize().y)});
            spr.setPosition(shape.getPosition());
            platforms.push_back(spr);
        }

        // Zachowaj prędkość i pozycję X, tylko przenieś na górę ekranu
        player.sprite.setPosition(
            player.sprite.getPosition().x, // Zachowaj X
            10.f // Ustaw na samej górze ekranu
        );
    }
}

        // Reset po upadku
        if (player.sprite.getPosition().y - currBounds.height/2.f > WINDOW_HEIGHT) {
            player.sprite.setPosition(WINDOW_WIDTH/2.f, WINDOW_HEIGHT - 220.f);
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
