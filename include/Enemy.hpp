#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Enemy {
public:
    Enemy(sf::Vector2f startPos);

    void update(float dt, const std::vector<sf::Vector2f>& path);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    bool isAlive() const;
    void destroy();

private:
    sf::CircleShape mShape;
    float mSpeed = 60.f;
    int mCurrentWaypointIndex = 0;
    bool mAlive = true;
};