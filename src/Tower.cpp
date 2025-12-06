#include "Tower.hpp"
#include <cmath>

Tower::Tower(sf::Vector2f position) {
    mShape.setRadius(14.f);
    mShape.setOrigin(14.f, 14.f);
    mShape.setPosition(position);
    mShape.setFillColor(sf::Color::Blue);

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

        sf::Vector2f diff = enemy->getPosition() - mShape.getPosition();
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist <= mRange) {
            // Atirar!
            sf::Vector2f direction = diff / dist; // Normaliza
            mTimer = mFireCooldown; // Reinicia cooldown
            return Projectile(mShape.getPosition(), direction);
        }
    }

    return std::nullopt;
}

void Tower::draw(sf::RenderWindow& window) {
    window.draw(mRangeIndicator);
    window.draw(mShape);
}