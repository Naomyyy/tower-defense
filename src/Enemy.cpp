#include "Enemy.hpp"

Enemy::Enemy(sf::Vector2f startPos) {
    mShape.setRadius(10.f);
    mShape.setOrigin(10.f, 10.f);
    mShape.setPosition(startPos);
    mShape.setFillColor(sf::Color::Red);
}

void Enemy::update(float dt, const std::vector<sf::Vector2f>& path) {
    if (!mAlive || path.empty()) return;

    // Chegou ao fim do caminho?
    if (mCurrentWaypointIndex >= (int)path.size()) {
        mAlive = false;
        return;
    }

    sf::Vector2f target = path[mCurrentWaypointIndex];
    sf::Vector2f currentPos = mShape.getPosition();
    sf::Vector2f direction = target - currentPos;
    
    // Pitágoras para distância
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Se chegou perto o suficiente do waypoint, muda para o próximo
    if (distance < 2.f) {
        mCurrentWaypointIndex++;
        return;
    }

    // Normaliza e move
    sf::Vector2f velocity = (direction / distance) * mSpeed;
    mShape.move(velocity * dt);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(mShape);
}

sf::Vector2f Enemy::getPosition() const { return mShape.getPosition(); }
bool Enemy::isAlive() const { return mAlive; }
void Enemy::destroy() { mAlive = false; }