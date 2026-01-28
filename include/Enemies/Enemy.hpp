#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Enemy {
public:
    Enemy(sf::Vector2f startPos, const std::string& textureName,
          int health, int damage, float speed, int reward);
    // Updates position along the path and health bar status
    void update(float dt, const std::vector<sf::Vector2f>& path);
    // Renders the enemy and its health interface
    void draw(sf::RenderWindow& window);
    // Getters
    sf::Vector2f getPosition() const;
    const sf::Sprite& getSprite() const;
    int getDamage() const;
    int getReward() const;
    float getProgress() const;
    bool isAlive() const;
    // Logic
    void destroy();
    void takeDamage(int amount);

private:
    sf::Sprite sprite;
    sf::Vector2f position;

    int maxHealth;
    int health;
    int damage;
    float speed;
    int reward;

    float progress = 0.f; 

    size_t currentPathIndex = 0;
    bool alive = true;

    // Health bar visuals
    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBarFront;
};
