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
    const sf::Sprite& getSprite() const;
    bool isAlive() const;
    void destroy();

    // Métodos para manipular vida e dano
    void takeDamage(int amount);
    int getDamage() const;
    int getReward() const;
    float getProgress() const { return mProgress; } 


private:
    sf::Sprite mSprite;
    sf::Vector2f mPosition;

    int health;
    int damage;
    float speed;
    int reward;

    float mProgress = 0.f;

    // variáveis para movimento e estado
    size_t currentPathIndex = 0;
    bool alive = true;

    // variaveis para representar a vida do inimigo
    sf::RectangleShape mHealthBarBack;
    sf::RectangleShape mHealthBarFront;
    int maxHealth;
};
