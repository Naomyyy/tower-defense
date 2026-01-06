#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <cmath>

class Enemy {
public:
    Enemy(sf::Vector2f startPos, const std::string& textureName,
          int health, int damage, float speed, int reward);

    void update(float dt, const std::vector<sf::Vector2f>& path);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    bool isAlive() const;
    void destroy();

    // Métodos para manipular vida e dano
    void takeDamage(int amount);
    int getDamage() const;
    int getReward() const;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;

    int health;
    int damage;
    float speed;
    int reward;

    // variáveis para movimento e estado
    size_t currentPathIndex = 0;
    bool alive = true;
};
