#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(sf::Vector2f position, sf::Vector2f velocity);
    
    void update(float dt);
    void draw(sf::RenderWindow& window);
    
    // Getters e Setters úteis
    sf::Vector2f getPosition() const;
    bool isAlive() const;
    void destroy(); // Marca como morto

private:
    sf::CircleShape mShape;
    sf::Vector2f mVelocity;
    float mSpeed = 300.f;
    bool mAlive = true;
};