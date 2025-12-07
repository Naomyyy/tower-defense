#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include "Enemy.hpp"
#include "Projectile.hpp"

class Tower {
public:
    Tower(sf::Vector2f position, const std::string& textureName);

    // Retorna um Projétil se atirar, ou std::nullopt se estiver em cooldown
    std::optional<Projectile> update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies);
    
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite mSprite;
    sf::CircleShape mRangeIndicator; // Visualizar o alcance
    float mRange = 120.f;
    float mFireCooldown = 0.8f;
    float mTimer = 0.f;
};