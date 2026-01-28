#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <string>

#include "Enemies/Enemy.hpp"      
#include "Projectile.hpp" 

enum class TowerType {Archer, Cannon, Mage};

class Tower {
public:
    // Initializes the tower with its position and sets the base texture.
    Tower(sf::Vector2f position, const std::string& textureName);
    virtual ~Tower() = default;
    // Returns a projectile only if the tower fires this frame
    virtual std::optional<Projectile> update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies);
    // Show the tower sprite in the window.
    void draw(sf::RenderWindow& window);

    const sf::Sprite& getSprite() const;

protected:
    // Updates the size and position of the range visualization circle.
    void updateRangeVisuals();
    // Searches for the enemy closest to the end of the path
    const Enemy* findTarget(const std::vector<std::unique_ptr<Enemy>>& enemies); 


protected:
    sf::Sprite sprite;
    sf::CircleShape rangeIndicator; 
    std::string projectileTexture; 

    float range;           
    float fireCooldown;    
    float timer;           
    
    int damage;                    
    float projectileSpeed;         
};