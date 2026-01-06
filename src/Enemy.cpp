#include "Enemy.hpp"

Enemy::Enemy(sf::Vector2f startPos, const std::string& textureName,
             int health, int damage, float speed, int reward)
    : position(startPos), health(health), damage(damage), speed(speed), reward(reward), alive(true)
{
    if (!texture.loadFromFile(textureName)) {
        // tratar erro de carregamento
    }
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Enemy::update(float dt, const std::vector<sf::Vector2f>& path) {
    if (!alive) return;

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
