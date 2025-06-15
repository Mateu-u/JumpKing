// main.cpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <map>
#include <vector>

#include "GameState.h"
#include "databasemanager.h"
#include "LoginScreen.h"
#include "MenuScreen.h"
#include "PlayerSelectScreen.h"
#include "CreditsScreen.h"
#include "Player.h"
#include "PlatformGenerator.h"

// ---------------------------------------------------------------------------
// Parametry gry
// ---------------------------------------------------------------------------
const int   WINDOW_WIDTH   = 1540;
const int   WINDOW_HEIGHT  = 920;
const float MAX_JUMP_FORCE = 18.0f;
const int   NUM_LEVELS     = 3;

// Ścieżka do folderu z grafikami (bez zmian)
const std::string RES_DIR =
    "C:/Users/wikto/Documents/JumpKing/"
    "build/Desktop_Qt_6_8_3_MinGW_64_bit-Debug/debug/tekstury/";

int main()
{
    // 0) Debug: wypisz working directory i pliki w nim
    std::cout << "Working dir: " << std::filesystem::current_path() << "\n";
    for (auto& entry : std::filesystem::directory_iterator(".")) {
        std::cout << "  " << entry.path().filename().string() << "\n";
    }

    // 1) Inicjalizacja bazy danych
    if (!DatabaseManager::init("SQL/Database.sqlite")) {
        std::cerr << "Błąd inicjalizacji bazy danych\n";
        return -1;
    }

    // 2) Utworzenie okna
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Jump King"
        );
    window.setFramerateLimit(60);

    // 3) Załaduj czcionkę UI (plik obok exe w folderze 'debug')
    sf::Font uiFont;
    if (!uiFont.loadFromFile("debug/JunigardenSwash.otf")) {
        std::cerr << "ERROR loading JunigardenSwash.otf\n";
        return -1;
    }

    // 4) Utworzenie obiektów ekranów
    LoginScreen        loginScreen(uiFont);
    MenuScreen         menuScreen(uiFont);
    PlayerSelectScreen psScreen(uiFont);
    CreditsScreen      creditsScreen(uiFont);

    // 5) Przygotuj dane poziomów – trzy mapy jako wektory stringów
    std::vector<std::vector<std::string>> levelMaps = {
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

    // 6) Załaduj tła poziomów
    std::vector<sf::Texture> backgroundTextures;
    backgroundTextures.reserve(NUM_LEVELS);
    for (int i = 0; i < NUM_LEVELS; ++i) {
        sf::Texture tex;
        if (!tex.loadFromFile(
                RES_DIR + "background_" + std::to_string(i+1) + ".png"))
        {
            // fallback: jednokolorowe tło
            sf::Image img;
            img.create(WINDOW_WIDTH, WINDOW_HEIGHT,
                       sf::Color(20,20,40 + i*20));
            tex.loadFromImage(img);
        }
        backgroundTextures.push_back(tex);
    }
    sf::Sprite bgSprite(backgroundTextures[0]);

    // 7) Załaduj tekstury gracza
    std::map<std::string, sf::Texture> textures;
    for (auto name : {"idle","jump_charge","jump","fall"}) {
        sf::Texture t;
        if (!t.loadFromFile(
                RES_DIR + "player_" + name + ".png"))
        {
            sf::Image ph; ph.create(256,256, sf::Color::Red);
            t.loadFromImage(ph);
        }
        t.setSmooth(false);
        textures[name] = t;
    }

    // 8) Załaduj teksturę platform i skonwertuj mapę z # do sprite'ów
    sf::Texture platformTex;
    if (!platformTex.loadFromFile(RES_DIR + "testWall.png")) {
        std::cerr << "ERROR loading testWall.png\n";
    }
    platformTex.setRepeated(true);

    float tileH = WINDOW_HEIGHT / static_cast<float>(levelMaps[0].size());
    float tileW = WINDOW_WIDTH  / static_cast<float>(levelMaps[0][0].size());
    auto shapePlatforms = createPlatformsFromMap(
        levelMaps[0], tileW, tileH);

    std::vector<sf::Sprite> platforms;
    platforms.reserve(shapePlatforms.size());
    for (auto& shape : shapePlatforms) {
        sf::Sprite spr(platformTex);
        auto sz = shape.getSize();
        spr.setTextureRect({0,0,
                            static_cast<int>(sz.x),
                            static_cast<int>(sz.y)});
        spr.setPosition(shape.getPosition());
        platforms.push_back(spr);
    }

    // 9) Utwórz gracza i widok kamery
    Player player(
        WINDOW_WIDTH/2.f,
        WINDOW_HEIGHT - 220.f,
        textures,
        0.10f
        );
    sf::View view(
        sf::FloatRect(0,0,
                      static_cast<float>(WINDOW_WIDTH),
                      static_cast<float>(WINDOW_HEIGHT))
        );
    sf::FloatRect prevBounds = player.sprite.getGlobalBounds();
    int score = 0;

    // 10) Przygotuj HUD
    sf::Font hudFont;
    if (!hudFont.loadFromFile(RES_DIR + "arial.ttf"))
        std::cerr << "ERROR loading arial.ttf\n";
    sf::Text scoreText("Score: 0", hudFont, 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10,10);
    sf::RectangleShape barBack({200,20});
    barBack.setFillColor(sf::Color(50,50,50,200));
    barBack.setPosition(10,40);
    sf::RectangleShape barFront({0,20});
    barFront.setFillColor(sf::Color(150,150,250,200));
    barFront.setPosition(10,40);

    // 11) Maszyna stanów – start od logowania
    GameState state = GameState::Login;

    // 12) Główna pętla gry
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();

            // rozdzielamy zdarzenia w zależności od stanu
            switch (state) {
            case GameState::Login:
                loginScreen.handleEvent(ev, state);
                break;
            case GameState::MainMenu:
                menuScreen.handleEvent(ev, state);
                break;
            case GameState::PlayerSelect:
                psScreen.handleEvent(ev, state);
                break;
            case GameState::Credits:
                creditsScreen.handleEvent(ev, state);
                break;
            case GameState::Playing:
                player.handleInput();
                break;
            }
        }

        // logika stanu Playing
        if (state == GameState::Playing) {
            prevBounds = player.sprite.getGlobalBounds();
            player.applyVelocityX();
            player.applyGravity();
            auto currBounds = player.sprite.getGlobalBounds();

            int landed = player.checkCollisions(
                prevBounds, currBounds, shapePlatforms);
            if (landed != -1 && landed != player.lastLandedPlat) {
                score += 10;
                player.lastLandedPlat = landed;
            }

            // przejścia między poziomami
            if (player.sprite.getPosition().y + currBounds.height/2 < 0) {
                int nl = player.getCurrentLevel() + 1;
                if (nl < NUM_LEVELS) {
                    player.setCurrentLevel(nl);
                    bgSprite.setTexture(backgroundTextures[nl]);
                    shapePlatforms = createPlatformsFromMap(
                        levelMaps[nl], tileW, tileH);
                    platforms.clear();
                    for (auto& s: shapePlatforms) {
                        sf::Sprite sp(platformTex);
                        auto zs = s.getSize();
                        sp.setTextureRect({0,0,
                                           (int)zs.x,(int)zs.y});
                        sp.setPosition(s.getPosition());
                        platforms.push_back(sp);
                    }
                    player.sprite.setPosition(
                        player.sprite.getPosition().x,
                        WINDOW_HEIGHT - 10.f
                        );
                }
            }
            // reset po spadku poniżej ekranu
            if (player.sprite.getPosition().y - currBounds.height/2 > WINDOW_HEIGHT) {
                player.sprite.setPosition(
                    WINDOW_WIDTH/2.f, WINDOW_HEIGHT - 220.f);
                player.velocity = {0,0};
                player.jumpCharge   = 0.f;
                player.chargingJump = false;
                player.isOnGround   = false;
                player.lastLandedPlat = -1;
                score = 0;
                view.setCenter(WINDOW_WIDTH/2.f, WINDOW_HEIGHT/2.f);
            }

            player.updateAnimationState();
        }

        // render
        window.clear();
        switch (state) {
        case GameState::Login:
            loginScreen.draw(window);
            break;
        case GameState::MainMenu:
            menuScreen.draw(window);
            break;
        case GameState::PlayerSelect:
            psScreen.draw(window);
            break;
        case GameState::Credits:
            creditsScreen.draw(window);
            break;
        case GameState::Playing:
            // tło
            window.setView(window.getDefaultView());
            window.draw(bgSprite);
            // platformy
            for (auto& sp: platforms)
                window.draw(sp);
            // gracz
            window.setView(view);
            player.draw(window);
            // HUD
            window.setView(window.getDefaultView());
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
            window.draw(barBack);
            float w = (player.jumpCharge / MAX_JUMP_FORCE)
                      * barBack.getSize().x;
            barFront.setSize({w, barFront.getSize().y});
            window.draw(barFront);
            break;
        }
        window.display();
    }

    return 0;
}
