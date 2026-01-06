#include "Enemy.hpp"
#include "AssetManager.hpp" 

Enemy::Enemy(sf::Vector2f startPos, const std::string& textureName,
             int health, int damage, float speed, int reward)
    : position(startPos), health(health), damage(damage), speed(speed), reward(reward), alive(true)
{
    const sf::Texture& texture = AssetManager::getInstance().getTexture(textureName);
    sprite.setTexture(texture);
    // Escala: Ajusta o tamanho do sprite
    const float targetSize = 50.0f; 
    float scaleFactor = targetSize / texture.getSize().x;
    sprite.setScale(scaleFactor, scaleFactor);
    // Define a origem do sprite para o centro (para rotação e posicionamento).
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setPosition(startPos);
}

void Enemy::update(float dt, const std::vector<sf::Vector2f>& path) {
    if (!alive || path.empty()) return;

    if (currentPathIndex < path.size()) {
        sf::Vector2f target = path[currentPathIndex];
        sf::Vector2f direction = target - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < speed * dt) {
            position = target;
            currentPathIndex++;
        } else {
            direction /= distance; // normaliza
            position += direction * speed * dt;
        }
        sprite.setPosition(position);
    } else {
        // Chegou no fim do caminho - você pode decidir o que acontece
        destroy();
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    if (alive) {
        window.draw(sprite);
    }
}

sf::Vector2f Enemy::getPosition() const {
    return position;
}

bool Enemy::isAlive() const {
    return alive;
}

void Enemy::destroy() {
    alive = false;
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        alive = false;
    }
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getReward() const {
    return reward;
}
