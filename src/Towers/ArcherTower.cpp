#include "Towers/ArcherTower.hpp"

ArcherTower::ArcherTower(sf::Vector2f position) 
    : Tower(position, "archer_tower") 
{
    // Tower with high attack speed but low damage
    range = 175.f;
    damage = 10;
    fireCooldown = 0.5f; 
    projectileSpeed = 450.f;
    projectileTexture = "arrow";
    
    updateRangeVisuals(); 
    
}