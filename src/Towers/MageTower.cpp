#include "Towers/MageTower.hpp"

MageTower::MageTower(sf::Vector2f position) 
    : Tower(position, "mage_tower")
{
    // Tower with long range , fast but low damage
    range = 300.f;       
    damage = 30;          
    fireCooldown = 1.0f;  
    projectileSpeed = 400.f;
    projectileTexture = "magic_bolt";
    
    updateRangeVisuals();
}