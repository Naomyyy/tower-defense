#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Projectile {
public:
    
    Projectile(sf::Vector2f position, sf::Vector2f direction, float speed, int damage, const std::string& textureName);
    // Updates position based on velocity and delta time
    void update(float dt);
    // Draws the projectile sprite to the window
    void draw(sf::RenderWindow& window);
    
    // Getters e Setters
    const sf::Sprite& getSprite() const;
    sf::Vector2f getPosition() const;
    bool isAlive() const;
    void destroy(); 
    int getDamage() const;

private:
    sf::Sprite sprite;     
    sf::Vector2f velocity;
    bool alive = true;
    int damage;            
};