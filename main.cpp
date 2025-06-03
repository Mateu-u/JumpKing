#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

// -----------------------------------------------------------------------------
// Parametry gry
// -----------------------------------------------------------------------------
const float GRAVITY         = 0.5f;
const float MOVE_SPEED      = 4.0f;     // prędkość przesuwania na ziemi
const float JUMP_CHARGE_SP  = 0.7f;     // szybkość ładowania siły skoku
const float MAX_JUMP_FORCE  = 18.0f;    // maksymalna siła skoku
const int   WINDOW_WIDTH    = 1280;
const int   WINDOW_HEIGHT   = 720;

// -----------------------------------------------------------------------------
// Klasa gracza
// -----------------------------------------------------------------------------
class Player {
public:
    sf::Sprite        sprite;
    sf::Texture       texture;
    sf::Vector2f      velocity;
    bool              isOnGround     = false;
    float             jumpCharge     = 0.0f;
    bool              chargingJump   = false;
    bool              facingRight    = true;
    int               lastLandedPlat = -1;

    Player(float x, float y, float scale = 0.15f) {
        if (!texture.loadFromFile("tekstury/testPlayer.png")) {
            sf::Image placeholder;
            placeholder.create(200, 200, sf::Color::Red);
            texture.loadFromImage(placeholder);
        }
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        sprite.setScale(scale, scale);
        sprite.setPosition(x, y);
    }
    // Skok
    void handleInput() {
        if (!chargingJump && isOnGround) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                sprite.move(-MOVE_SPEED, 0);
                facingRight = false;
                sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                sprite.move(MOVE_SPEED, 0);
                facingRight = true;
                sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (isOnGround) {
                chargingJump = true;
                jumpCharge += JUMP_CHARGE_SP;
                if (jumpCharge > MAX_JUMP_FORCE)
                    jumpCharge = MAX_JUMP_FORCE;
            }
        }
        else if (chargingJump) {
            velocity.y = -jumpCharge;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                velocity.x = -jumpCharge / 2.0f;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                velocity.x = jumpCharge / 2.0f;
            }
            else {
                velocity.x = 0.f;
            }
            jumpCharge   = 0;
            chargingJump = false;
            isOnGround   = false;
        }


    }

    void applyGravity() {
        if (!isOnGround)
            velocity.y += GRAVITY;
        sprite.move(0, velocity.y);
    }

        void applyVelocityX() {
            sprite.move(velocity.x, 0);
            if (isOnGround)
                velocity.x *= 0.1f;
        }

        int checkCollisions(const sf::FloatRect& prevBounds,
                            const sf::FloatRect& currBounds,
                            std::vector<sf::RectangleShape>& platforms)
        {
            isOnGround = false;
            int landedIndex = -1;

            for (int i = 0; i < static_cast<int>(platforms.size()); ++i) {
                const auto& plat = platforms[i];
                sf::FloatRect platBounds = plat.getGlobalBounds();

                if (currBounds.intersects(platBounds)) {
                    float prevBottom = prevBounds.top + prevBounds.height;
                    float currBottom = currBounds.top + currBounds.height;

                    float prevTop = prevBounds.top;
                    float currTop = currBounds.top;

                    // Lądowanie na platformie (kolizja od góry)
                    if (prevBottom <= platBounds.top && velocity.y >= 0) {
                        sprite.setPosition(sprite.getPosition().x, platBounds.top - currBounds.height / 2.f);
                        velocity.y = 0.f;
                        isOnGround = true;
                        landedIndex = i;
                    }

                    // Uderzenie od dołu (głową)
                    else if (prevTop >= platBounds.top + platBounds.height && velocity.y < 0) {
                        sprite.setPosition(sprite.getPosition().x, platBounds.top + platBounds.height + currBounds.height / 2.f);
                        velocity.y = 0.f;
                    }
                }
            }

            return landedIndex;
        }



    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};

// -----------------------------------------------------------------------------
// Generowanie platform
// -----------------------------------------------------------------------------
std::vector<sf::RectangleShape> createJumpKingPlatforms()
{
    std::vector<sf::RectangleShape> platforms;
    platforms.reserve(100);

    {
        sf::RectangleShape floor;
        floor.setSize({ static_cast<float>(WINDOW_WIDTH), 20.f });
        floor.setFillColor(sf::Color(100, 100, 100));
        floor.setPosition(0.f, static_cast<float>(WINDOW_HEIGHT - 20));
        platforms.push_back(floor);
    }

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    float baseY = static_cast<float>(WINDOW_HEIGHT - 20);
    for (int i = 1; i <= 100; ++i) {
        sf::RectangleShape plat;
        float y = baseY - i * 150.f;
        float x = static_cast<float>(std::rand() % (WINDOW_WIDTH - 250) + 50);
        plat.setSize({ 200.f, 20.f });
        plat.setFillColor(sf::Color(100, 200, 100));
        plat.setPosition(x, y);
        platforms.push_back(plat);
    }

    return platforms;
}

// -----------------------------------------------------------------------------
// Główna funkcja
// -----------------------------------------------------------------------------
int main()
{
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Jump King",
        sf::Style::Titlebar | sf::Style::Close
        );
    window.setFramerateLimit(60);

    float initialX = WINDOW_WIDTH / 2.0f;
    float initialY = WINDOW_HEIGHT - 120;
    Player player(initialX, initialY, 0.15f);

    auto platforms = createJumpKingPlatforms();

    sf::View view(sf::FloatRect(0, 0, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)));

    sf::Text scoreText;
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    int score = 0;

    sf::RectangleShape jumpBarBack({ 200.f, 20.f });
    jumpBarBack.setFillColor(sf::Color(50, 50, 50, 200));
    jumpBarBack.setPosition(10.f, 40.f);

    sf::RectangleShape jumpBarFront({ 0.f, 20.f });
    jumpBarFront.setFillColor(sf::Color(150, 150, 250, 200));
    jumpBarFront.setPosition(10.f, 40.f);

    sf::FloatRect prevBounds = player.sprite.getGlobalBounds();

    while (window.isOpen()) {
        sf::Clock clock;
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.handleInput();
        prevBounds = player.sprite.getGlobalBounds();

        player.applyVelocityX();
        player.applyGravity();

        // Sprawdzenie kolizji
        sf::FloatRect currBounds = player.sprite.getGlobalBounds();
        int landedIndex = player.checkCollisions(prevBounds, currBounds, platforms);
        if (landedIndex != -1 && landedIndex != player.lastLandedPlat) {
            score += 10;
            player.lastLandedPlat = landedIndex;
        }

        // Reset, jeśli gracz spadł poza dół ekranu
        if (player.sprite.getPosition().y - (currBounds.height / 2.0f) > WINDOW_HEIGHT) {
            player.sprite.setPosition(initialX, initialY);
            player.velocity = { 0.f, 0.f };
            player.jumpCharge = 0.f;
            player.chargingJump = false;
            player.isOnGround = false;
            player.lastLandedPlat = -1;
            score = 0;

            view.setCenter(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
        }


        window.clear(sf::Color(30, 30, 50));
        window.setView(view);

        for (auto& plat : platforms) {
            window.draw(plat);
        }
        player.draw(window);
        window.setView(window.getDefaultView());

        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        // Pasek ładowania skoku
        window.draw(jumpBarBack);
        float barWidth = (player.jumpCharge / MAX_JUMP_FORCE) * jumpBarBack.getSize().x;
        jumpBarFront.setSize({ barWidth, jumpBarFront.getSize().y });
        window.draw(jumpBarFront);

        window.display();
    }

    return 0;
}
