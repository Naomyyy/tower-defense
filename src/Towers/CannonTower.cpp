#include "Towers/CannonTower.hpp"

CannonTower::CannonTower(sf::Vector2f position) 
    : Tower(position, "cannon_tower")
{
    // Configurações Específicas do Canhão
    mRange = 150.f;        // Alcance menor
    mDamage = 50;          // Dano alto
    mFireCooldown = 3.0f;  // Lento (1 tiro a cada 2s)
    mProjectileSpeed = 300.f;
    mProjectileTexture = "cannonball";
    
    updateRangeVisuals();
}