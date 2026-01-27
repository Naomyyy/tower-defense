#include "Towers/CannonTower.hpp"

CannonTower::CannonTower(sf::Vector2f position) 
    : Tower(position, "cannon_tower")
{
    // Tower with a high damage but slow fire fire rate 
    range = 150.f;        
    damage = 50;         
    fireCooldown = 3.0f;  
    projectileSpeed = 300.f;
    projectileTexture = "cannonball";
    
    updateRangeVisuals();
}