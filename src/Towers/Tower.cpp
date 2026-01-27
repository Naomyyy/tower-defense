#include "Towers/Tower.hpp"
#include "AssetManager.hpp"
#include <cmath>

// Turn a vector into a unit vector (length of 1)
static sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len == 0.f) return {0.f, 0.f};
    return {v.x / len, v.y / len};
}

Tower::Tower(sf::Vector2f position, const std::string& textureName) 
    : timer(0.f), range(150.f), fireCooldown(1.0f), damage(10), projectileSpeed(300.f) 
{
    sprite.setTexture(AssetManager::getInstance().getTexture(textureName));
    // Set origin to center for correct rotation and placement
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(position);
    // Draw the tower's attack range
    updateRangeVisuals();
}

void Tower::updateRangeVisuals() {
    rangeIndicator.setRadius(range);
    rangeIndicator.setOrigin(range, range);
    rangeIndicator.setPosition(sprite.getPosition());
    rangeIndicator.setFillColor(sf::Color::Transparent);
    rangeIndicator.setOutlineColor(sf::Color(255, 255, 255, 50));
    rangeIndicator.setOutlineThickness(1.f);
}

void Tower::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

const Enemy* Tower::findTarget(const std::vector<std::unique_ptr<Enemy>>& enemies) {

    const Enemy* target = nullptr;
    float maxProgress = -1.f;

    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;
        // Calculate the difference in X and Y coordinates.
        sf::Vector2f diff = enemy->getPosition() - sprite.getPosition();
        // Pythagorean Theorem: a^2 + b^2 = c^2 to see if the enemy it's in the tower's range
        if ((diff.x * diff.x + diff.y * diff.y) > range * range) continue; 
        // If multiple enemies are in range, we pick the one with most progress.
        float progress = enemy->getProgress();
        if (progress > maxProgress) {
            maxProgress = progress;
            target = enemy.get();
        }
    }
    return target; // Returns an enemy or nullptr if no enemy is found
}

std::optional<Projectile> Tower::update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Subtract the time passed since the last frame
    timer -= dt;
    // The tower can only shoot if the "cooldown" timer has reached zero
    if (timer <= 0.f) {
        const Enemy* target = findTarget(enemies);
        if (target) {
            // Reset the timer to start a new cooldown period
            timer = fireCooldown;
            // Calculate direction vector from tower to enemy 
            sf::Vector2f direction = normalize(target->getPosition() - sprite.getPosition());

            return Projectile(sprite.getPosition(), direction, projectileSpeed, damage, projectileTexture);
        }
    }
    return std::nullopt;
}
