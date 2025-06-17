#include "Player.h"
#include <cmath>

namespace {
constexpr float GRAVITY         = 0.5f;
constexpr float MOVE_SPEED      = 4.0f;
constexpr float JUMP_CHARGE_SP  = 0.7f;
constexpr float MAX_JUMP_FORCE  = 18.0f;
}

Player::Player(float x, float y,
               const std::map<std::string, sf::Texture>& textures,
               float scale)
    : texMap(textures)
    , currentLevel(0)
    , velocity(0.f, 0.f)
    , jumpCharge(0.f)
    , chargingJump(false)
    , facingRight(true)
    , lastLandedPlat(-1)
    , isOnGround(false)
{
    setState("idle");
    sprite.setOrigin(
        sprite.getTextureRect().width / 2.f,
        sprite.getTextureRect().height / 2.f
        );
    sprite.setScale(scale, scale);
    sprite.setPosition(x, y);
}

void Player::setState(const std::string& state) {
    sprite.setTexture(texMap.at(state));
}

void Player::handleInput() {
    if (!chargingJump && isOnGround) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            sprite.move(-MOVE_SPEED, 0);
            facingRight = false;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            sprite.move(MOVE_SPEED, 0);
            facingRight = true;
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
        if      (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocity.x = -jumpCharge/2.f;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocity.x =  jumpCharge/2.f;
        else                                                    velocity.x = 0.f;

        jumpCharge   = 0.f;
        chargingJump = false;
        isOnGround   = false;
    }
}

void Player::applyGravity() {
    if (!isOnGround) velocity.y += GRAVITY;
    sprite.move(0, velocity.y);
}

void Player::applyVelocityX() {
    sprite.move(velocity.x, 0);
    if (isOnGround) velocity.x *= 0.1f;
}

int Player::checkCollisions(const sf::FloatRect& prevBounds,
                            const sf::FloatRect& currBounds,
                            std::vector<sf::RectangleShape>& platforms)
{
    isOnGround = false;
    int landedIndex = -1;

    for (int i = 0; i < (int)platforms.size(); ++i) {
        const auto& plat = platforms[i];
        sf::FloatRect platformBounds = plat.getGlobalBounds();

        if (currBounds.intersects(platformBounds)) {
            // Obliczamy wszystkie potrzebne pozycje
            float playerBottom = currBounds.top + currBounds.height;
            float playerTop = currBounds.top;
            float playerLeft = currBounds.left;
            float playerRight = currBounds.left + currBounds.width;

            float platformTop = platformBounds.top;
            float platformBottom = platformBounds.top + platformBounds.height;
            float platformLeft = platformBounds.left;
            float platformRight = platformBounds.left + platformBounds.width;

            // Obliczamy głębokości penetracji z każdej strony
            float overlapLeft = playerRight - platformLeft;
            float overlapRight = platformRight - playerLeft;
            float overlapTop = playerBottom - platformTop;
            float overlapBottom = platformBottom - playerTop;

            // Znajdujemy najmniejszą penetrację
            float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

            // Kolizja od góry (lądowanie)
            if (minOverlap == overlapTop && velocity.y >= 0) {
                sprite.setPosition(sprite.getPosition().x, platformTop - currBounds.height/2.0f);
                velocity.y = 0.0f;
                isOnGround = true;
                landedIndex = i;
            }
            // Kolizja od dołu (głową w sufit)
            else if (minOverlap == overlapBottom && velocity.y < 0) {
                sprite.setPosition(sprite.getPosition().x, platformBottom + currBounds.height/2.0f);
                velocity.y = 0.0f;
            }
            // Kolizja z lewej strony
            else if (minOverlap == overlapLeft) {
                sprite.setPosition(platformLeft - currBounds.width / 2.0f, sprite.getPosition().y);

                // Jeśli gracz nie stoi na ziemi (czyli skacze) — odbij się
                if (!isOnGround && velocity.x > 0) {
                    velocity.x = -velocity.x * 0.4f;  // odbicie w lewo z tłumieniem
                } else {
                    velocity.x = 0.0f;
                }
            }
            else if (minOverlap == overlapRight) {
                sprite.setPosition(platformRight + currBounds.width / 2.0f, sprite.getPosition().y);

                if (!isOnGround && velocity.x < 0) {
                    velocity.x = -velocity.x * 0.4f;  // odbicie w prawo z tłumieniem
                } else {
                    velocity.x = 0.0f;
                }
            }

        }
    }

    return landedIndex;
}

void Player::updateAnimationState() {
    if      (chargingJump)           setState("jump_charge");
    else if (velocity.y < 0.f)       setState("jump");
    else if (velocity.y > 1.f)       setState("fall");
    else                              setState("idle");

    float sx = std::abs(sprite.getScale().x);
    sprite.setScale(
        facingRight ?  sx : -sx,
        sprite.getScale().y
        );
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
