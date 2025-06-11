#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>

class Player {
public:
    Player(float x, float y,
           const std::map<std::string, sf::Texture>& textures,
           float scale = 0.15f);

    void handleInput();
    void applyGravity();
    void applyVelocityX();
    int  checkCollisions(const sf::FloatRect& prevBounds,
                        const sf::FloatRect& currBounds,
                        std::vector<sf::RectangleShape>& platforms);
    void updateAnimationState();
    void draw(sf::RenderWindow& window);

    sf::Sprite    sprite;
    sf::Vector2f  velocity;
    bool          isOnGround;
    float         jumpCharge;
    bool          chargingJump;
    bool          facingRight;
    int           lastLandedPlat;

private:
    void setState(const std::string& state);
    const std::map<std::string, sf::Texture>& texMap;
};

#endif // PLAYER_H
