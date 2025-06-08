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
    , velocity(0.f, 0.f)
    , isOnGround(false)
    , jumpCharge(0.f)
    , chargingJump(false)
    , facingRight(true)
    , lastLandedPlat(-1)
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
        sf::FloatRect tb = plat.getGlobalBounds();

        if (currBounds.intersects(tb)) {
            float prevBottom = prevBounds.top + prevBounds.height;
            float currBottom = currBounds.top + currBounds.height;
            float prevTop    = prevBounds.top;
            float currTop    = currBounds.top;

            // lądowanie
            if (prevBottom <= tb.top && velocity.y >= 0) {
                sprite.setPosition(
                    sprite.getPosition().x,
                    tb.top - currBounds.height / 2.f
                    );
                velocity.y   = 0.f;
                isOnGround   = true;
                landedIndex  = i;
            }
            // uderzenie głową
            else if (prevTop >= tb.top + tb.height && velocity.y < 0) {
                sprite.setPosition(
                    sprite.getPosition().x,
                    tb.top + tb.height + currBounds.height / 2.f
                    );
                velocity.y = 0.f;
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
