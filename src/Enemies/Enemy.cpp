#include "Enemies/Enemy.hpp"
#include "AssetManager.hpp" 
#include <cmath>

Enemy::Enemy(sf::Vector2f startPos, const std::string& textureName,
             int health, int damage, float speed, int reward)
    : position(startPos), health(health), damage(damage), speed(speed), reward(reward), alive(true), maxHealth(health), progress(0.f)
{
    const sf::Texture& texture = AssetManager::getInstance().getTexture(textureName);
    sprite.setTexture(texture);
    // Center origin for rotation and movement logic
    const float targetSize = 50.0f; 
    float scaleFactor = targetSize / texture.getSize().x;
    sprite.setScale(scaleFactor, scaleFactor);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(startPos);

    // Initialize health bar background (Red)
    healthBarBack.setSize(sf::Vector2f(40.f, 5.f)); 
    healthBarBack.setFillColor(sf::Color::Red);
    // Initialize health bar foreground (Green)
    healthBarFront.setSize(sf::Vector2f(40.f, 5.f));
    healthBarFront.setFillColor(sf::Color::Green);
}

void Enemy::update(float dt, const std::vector<sf::Vector2f>& path) {
    if (!alive || path.empty()) return;

    if (currentPathIndex < path.size()) {
        sf::Vector2f target = path[currentPathIndex];
        sf::Vector2f direction = target - position;
        // Calculate distance to the next waypoint
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < speed * dt) {
            position = target;
            currentPathIndex++;
            progress+=distance;
        } else {
            // Move towards the target point
            direction /= distance; // Normalize the direction
            position += direction * speed * dt;
            progress+=speed * dt;
        }
        sprite.setPosition(position);
    } else {
        destroy();
    }
    
    // Update health bar position and scale
    sf::Vector2f barPos = position + sf::Vector2f(-20.f, -35.f); 

    healthBarBack.setPosition(barPos);
    healthBarFront.setPosition(barPos);
    float healthPercentage = (float)health / (float)maxHealth;
    if (healthPercentage < 0.0f) healthPercentage = 0.0f;

    healthBarFront.setSize(sf::Vector2f(40.f * healthPercentage, 5.f));

    if (healthPercentage > 0.5f)  healthBarFront.setFillColor(sf::Color::Green);
    else if (healthPercentage > 0.15f) healthBarFront.setFillColor(sf::Color::Yellow);
    else if (healthPercentage > 0.0f) healthBarFront.setFillColor(sf::Color(255, 140, 0)); // Orange
    else healthBarFront.setFillColor(sf::Color::Red);
    
}

void Enemy::draw(sf::RenderWindow& window) {
    if (alive) {
        window.draw(sprite);
        window.draw(healthBarBack);  
        window.draw(healthBarFront); 
    }
}


void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        alive = false;
    }
}


sf::Vector2f Enemy::getPosition() const {return position;}

const sf::Sprite& Enemy::getSprite() const{return sprite;}

bool Enemy::isAlive() const {return alive;}

void Enemy::destroy() {alive = false;}

int Enemy::getDamage() const {return damage;}

int Enemy::getReward() const {return reward;}

float Enemy::getProgress() const {return progress;}


