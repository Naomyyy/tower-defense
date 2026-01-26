#include "Towers/Tower.hpp"
#include "AssetManager.hpp"
#include <cmath>
#include <iostream>
#include <optional>

static sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len == 0.f) return {0.f, 0.f};
    return {v.x / len, v.y / len};
}

Tower::Tower(sf::Vector2f position, const std::string& textureName) 
    : mTimer(0.f),
      mRange(150.f),
      mFireCooldown(1.0f),
      mDamage(10),
      mProjectileSpeed(300.f)
{
    mSprite.setTexture(AssetManager::getInstance().getTexture(textureName));
    
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mSprite.setPosition(position);

    updateRangeVisuals();
}

void Tower::updateRangeVisuals() {
    mRangeIndicator.setRadius(mRange);
    mRangeIndicator.setOrigin(mRange, mRange);
    mRangeIndicator.setPosition(mSprite.getPosition());
    mRangeIndicator.setFillColor(sf::Color::Transparent);
    mRangeIndicator.setOutlineColor(sf::Color(255, 255, 255, 50));
    mRangeIndicator.setOutlineThickness(1.f);
}

void Tower::draw(sf::RenderWindow& window) {
    // window.draw(mRangeIndicator); // debug
    window.draw(mSprite);
}

// 🔹 PRIORIDADE: inimigo mais avançado no caminho
const Enemy* Tower::findTarget(const std::vector<std::unique_ptr<Enemy>>& enemies) {
    const Enemy* target = nullptr;
    float maxProgress = -1.f; // inimigo mais avançado

    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        // Verifica se está dentro do alcance
        sf::Vector2f diff = enemy->getPosition() - mSprite.getPosition();
        float distSq = diff.x * diff.x + diff.y * diff.y;
        if (distSq > mRange * mRange) continue;

        // Progressão no caminho (0 = início, 1 = fim)
        float progress = enemy->getProgress(); // 👈 você precisa criar esse método no Enemy

        if (progress > maxProgress) {
            maxProgress = progress;
            target = enemy.get();
        }
    }

    return target;
}

std::optional<Projectile> Tower::update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    mTimer -= dt;

    if (mTimer <= 0.f) {
        const Enemy* target = findTarget(enemies);
        
        if (target) {
            mTimer = mFireCooldown;

            sf::Vector2f towerPos = mSprite.getPosition();
            sf::Vector2f enemyPos = target->getSprite().getPosition();
            sf::FloatRect eBounds = target->getSprite().getGlobalBounds();
            enemyPos.x += eBounds.width / 2.f;
            enemyPos.y += eBounds.height / 2.f;

            sf::Vector2f direction = normalize(enemyPos - towerPos);

            return Projectile(
                towerPos,           // posição inicial
                direction,          // direção NORMALIZADA
                mProjectileSpeed,   // velocidade
                mDamage,            // dano
                mProjectileTexture  // textura
            );
        }
    }
    return std::nullopt;
}
