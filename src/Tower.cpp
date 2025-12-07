#include "Tower.hpp"
#include "AssetManager.hpp"
#include <cmath>

Tower::Tower(sf::Vector2f position, const std::string& textureName) {
    // Configuração do Sprite
    const sf::Texture& texture = AssetManager::getInstance().getTexture(textureName);
    mSprite.setTexture(texture);

    const float targetSize = 80.0f; 
    float scaleFactor = targetSize / texture.getSize().x;
    mSprite.setScale(scaleFactor, scaleFactor);
    // Define a origem e a posição central
    mSprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f); 
    mSprite.setPosition(position);

    // Configuração visual do alcance
    mRangeIndicator.setRadius(mRange);
    mRangeIndicator.setOrigin(mRange, mRange);
    mRangeIndicator.setPosition(position);
    mRangeIndicator.setFillColor(sf::Color::Transparent);
    mRangeIndicator.setOutlineThickness(1.f);
    mRangeIndicator.setOutlineColor(sf::Color(100, 100, 255, 120));
}

std::optional<Projectile> Tower::update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    mTimer -= dt;
    
    // Se ainda está recarregando, não faz nada
    if (mTimer > 0) return std::nullopt;

    // Procura inimigo no alcance
    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        sf::Vector2f diff = enemy->getPosition() - mSprite.getPosition();
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist <= mRange) {
            // Atirar!
            sf::Vector2f direction = diff / dist; // Normaliza
            mTimer = mFireCooldown; // Reinicia cooldown
            return Projectile(mSprite.getPosition(), direction);
        }
    }

    return std::nullopt;
}

void Tower::draw(sf::RenderWindow& window) {
    window.draw(mRangeIndicator);
    window.draw(mSprite);
}