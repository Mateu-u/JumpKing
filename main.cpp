#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <filesystem>

#include "GameState.h"
#include "databasemanager.h"
#include "WelcomeScreen.h"
#include "AuthScreen.h"
#include "OptionsScreen.h"
#include "ScoreTableScreen.h"
#include "CreditsScreen.h"
#include "Player.h"
#include "PlatformGenerator.h"

const int WINDOW_WIDTH = 1540;
const int WINDOW_HEIGHT = 920;
const float MAX_JUMP_FORCE = 18.0f;
const int NUM_LEVELS = 5;

const std::string RES_DIR = "C:/Users/CP24/Documents/JumpKing/build/Desktop_Qt_6_9_0_MinGW_64_bit-Debug/debug/tekstury/";

void loadLevel(int levelIndex,
               const std::vector<std::vector<std::string>>& maps,
               const sf::Texture& platformTex,
               const std::vector<sf::Texture>& backgrounds,
               sf::Sprite& bgSprite,
               std::vector<sf::RectangleShape>& shapePlatforms,
               std::vector<sf::Sprite>& platforms,
               float tileW,
               float tileH)
{
    if (levelIndex >= 0 && levelIndex < (int)backgrounds.size())
        bgSprite.setTexture(backgrounds[levelIndex]);

    shapePlatforms = createPlatformsFromMap(maps[levelIndex], tileW, tileH);
    platforms.clear();
    for (auto& s : shapePlatforms) {
        sf::Sprite spr(platformTex);
        auto sz = s.getSize();
        spr.setTextureRect({0, 0, (int)sz.x, (int)sz.y});
        spr.setPosition(s.getPosition());
        platforms.push_back(spr);
    }
}

int main()
{
    if (!DatabaseManager::init("SQL/Database.sqlite")) {
        std::cerr << "B\u0142\u0105d inicjalizacji bazy danych\n";
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jump King");
    window.setFramerateLimit(60);

    sf::Font uiFont;
    if (!uiFont.loadFromFile("C:/Users/CP24/Documents/JumpKing/Rolie Twily.otf")) {
        std::cerr << "ERROR loading Rolie Twily.otf\n";
        return -1;
    }

    WelcomeScreen welcome(uiFont, RES_DIR + "background_0.png");
    AuthScreen auth(uiFont);
    OptionsScreen options(uiFont, { (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT });
    ScoreTableScreen scoreTable(uiFont);
    CreditsScreen credits(uiFont);

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
            "#              #",
            "#              #",
            "#        #######",
            "#              #",
            "###            #",
            "#####          #",
            "########       #",
            "##             #",
            "##           ###",
            "#         ######",
            "#       ########",
            "#              #",
            "####       #####",
            "#              #"
        },
        {
            "#              #",
            "#        #######",
            "#            ###",
            "###            #",
            "####           #",
            "#####          #",
            "########       #",
            "##             #",
            "##           ###",
            "#         ######",
            "#       ########",
            "#              #",
            "#          #####",
            "#####          #"
        },
        {
            "################",
            "##       #######",
            "##           ###",
            "##             #",
            "####           #",
            "#####          #",
            "########       #",
            "###            #",
            "##           ###",
            "#         ######",
            "#       ###   ##",
            "#              #",
            "####           #",
            "#         ###  #"
        }
    };

    std::vector<sf::Texture> backgroundTextures;
    for (int i = 0; i < NUM_LEVELS; ++i) {
        sf::Texture tex;
        if (!tex.loadFromFile(RES_DIR + "background_" + std::to_string(i + 1) + ".png")) {
            sf::Image img;
            img.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color(20, 20, 40 + i * 20));
            tex.loadFromImage(img);
        }
        backgroundTextures.push_back(std::move(tex));
    }
    sf::Sprite bgSprite(backgroundTextures[0]);

    std::map<std::string, sf::Texture> textures;
    for (auto name : {"idle", "jump_charge", "jump", "fall"}) {
        sf::Texture t;
        if (!t.loadFromFile(RES_DIR + "player_" + std::string(name) + ".png")) {
            sf::Image ph; ph.create(256, 256, sf::Color::Red);
            t.loadFromImage(ph);
        }
        t.setSmooth(false);
        textures[name] = std::move(t);
    }

    sf::Texture platformTex;
    if (!platformTex.loadFromFile(RES_DIR + "testWall.png"))
        std::cerr << "ERROR loading testWall.png\n";
    platformTex.setRepeated(true);

    float tileH = WINDOW_HEIGHT / (float)levelMaps[0].size();
    float tileW = WINDOW_WIDTH / (float)levelMaps[0][0].size();

    std::vector<sf::RectangleShape> shapePlatforms;
    std::vector<sf::Sprite> platforms;
    loadLevel(0, levelMaps, platformTex, backgroundTextures, bgSprite, shapePlatforms, platforms, tileW, tileH);

    Player player(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 220.f, textures, 0.10f);
    sf::View view(sf::FloatRect(0, 0, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    sf::FloatRect prevBounds = player.sprite.getGlobalBounds();
    int score = 0;

    sf::Font hudFont;
    if (!hudFont.loadFromFile("C:/Users/CP24/Documents/JumpKing/Rolie Twily.otf"))
        std::cerr << "ERROR loading arial.ttf\n";

    sf::Text scoreText("Score: 0", hudFont, 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    sf::RectangleShape barBack({200, 20});
    barBack.setFillColor(sf::Color(50, 50, 50, 200));
    barBack.setPosition(10, 40);
    sf::RectangleShape barFront({0, 20});
    barFront.setFillColor(sf::Color(150, 150, 250, 200));
    barFront.setPosition(10, 40);

    GameState state = GameState::Welcome;

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();

            switch (state) {
            case GameState::Welcome: welcome.handleEvent(ev, state, window); break;
            case GameState::Auth: auth.handleEvent(ev, state); break;
            case GameState::Options: options.handleEvent(ev, state, window); break;
            case GameState::ScoreTable: scoreTable.handleEvent(ev, state); break;
            case GameState::Credits: credits.handleEvent(ev, state); break;
            case GameState::Playing: player.handleInput(); break;
            }
        }

        if (state == GameState::Playing) {
            prevBounds = player.sprite.getGlobalBounds();
            player.applyVelocityX();
            player.applyGravity();
            auto currBounds = player.sprite.getGlobalBounds();

            int landed = player.checkCollisions(prevBounds, currBounds, shapePlatforms);
            if (landed != -1 && landed != player.lastLandedPlat) {
                score += 10;
                player.lastLandedPlat = landed;
            }

            if (player.sprite.getPosition().y + currBounds.height / 2 < 0) {
                int nl = player.getCurrentLevel() + 1;
                if (nl < NUM_LEVELS) {
                    score++;
                    player.setCurrentLevel(nl);
                    loadLevel(nl, levelMaps, platformTex, backgroundTextures, bgSprite, shapePlatforms, platforms, tileW, tileH);
                    player.sprite.setPosition(player.sprite.getPosition().x, WINDOW_HEIGHT - 10.f);
                }
            }
            else if (player.sprite.getPosition().y - currBounds.height / 2 > WINDOW_HEIGHT) {
                int nl = player.getCurrentLevel() - 1;
                if (nl >= 0) {
                    player.setCurrentLevel(nl);
                    loadLevel(nl, levelMaps, platformTex, backgroundTextures, bgSprite, shapePlatforms, platforms, tileW, tileH);
                    player.sprite.setPosition(player.sprite.getPosition().x, 10.f);
                } else {
                    player.sprite.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 220.f);
                    player.velocity = {0, 0};
                    player.jumpCharge = 0.f;
                    player.chargingJump = false;
                    player.isOnGround = false;
                    player.lastLandedPlat = -1;
                    score = 0;
                    view.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
                }
            }
            player.updateAnimationState();
        }

        window.clear();
        switch (state) {
        case GameState::Welcome: welcome.draw(window); break;
        case GameState::Auth: auth.draw(window); break;
        case GameState::Options: options.draw(window); break;
        case GameState::ScoreTable: scoreTable.draw(window); break;
        case GameState::Credits: credits.draw(window); break;
        case GameState::Playing:
            window.setView(window.getDefaultView());
            window.draw(bgSprite);
            for (auto& s : platforms) window.draw(s);
            window.setView(view);
            player.draw(window);
            window.setView(window.getDefaultView());
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
            window.draw(barBack);
            float w = (player.jumpCharge / MAX_JUMP_FORCE) * barBack.getSize().x;
            barFront.setSize({w, barFront.getSize().y});
            window.draw(barFront);
            break;
        }
        window.display();
    }

    return 0;
}
