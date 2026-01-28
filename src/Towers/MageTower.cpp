#include "Towers/MageTower.hpp"

MageTower::MageTower(sf::Vector2f position) 
    : Tower(position, "mage_tower")
{
    // Tower with long range, medium damage 
    range = 200.f;       
    damage = 30;          
    fireCooldown = 1.0f;  
    projectileSpeed = 400.f;
    projectileTexture = "magic_bolt";
    
    updateRangeVisuals();
}