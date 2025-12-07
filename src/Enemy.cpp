#include "Enemy.hpp"
#include "AssetManager.hpp"

Enemy::Enemy(sf::Vector2f startPos, const std::string& textureName) {
  const sf::Texture& texture = AssetManager::getInstance().getTexture(textureName);
  mSprite.setTexture(texture);
  // Escala: Ajusta o tamanho do sprite
  const float targetSize = 50.0f; 
  float scaleFactor = targetSize / texture.getSize().x;
  mSprite.setScale(scaleFactor, scaleFactor);
  // Define a origem do sprite para o centro (para rotação e posicionamento).
  mSprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
  mSprite.setPosition(startPos);
}

void Enemy::update(float dt, const std::vector<sf::Vector2f>& path) {
    if (!mAlive || path.empty()) return;

    // Chegou ao fim do caminho?
    if (mCurrentWaypointIndex >= (int)path.size()) {
        mAlive = false;
        return;
    }

    sf::Vector2f target = path[mCurrentWaypointIndex];
    sf::Vector2f currentPos = mSprite.getPosition();
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
    mSprite.move(velocity * dt);
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(mSprite);
}

sf::Vector2f Enemy::getPosition() const { return mSprite.getPosition(); }
bool Enemy::isAlive() const { return mAlive; }
void Enemy::destroy() { mAlive = false; }