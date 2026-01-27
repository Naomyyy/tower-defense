#include "Towers/ArcherTower.hpp"

ArcherTower::ArcherTower(sf::Vector2f position) 
    : Tower(position, "archer_tower") 
{
    // Tower with high attack speed but low damage
    range = 200.f;
    damage = 10;
    fireCooldown = 0.5f; 
    projectileSpeed = 500.f;
    projectileTexture = "arrow";
    
    updateRangeVisuals(); 
    
}