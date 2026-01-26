#include "Towers/MageTower.hpp"

MageTower::MageTower(sf::Vector2f position) 
    : Tower(position, "mage_tower")
{
    // Configurações Específicas do Mago
    mRange = 300.f;        // Alcance longo
    mDamage = 30;          // Dano baixo
    mFireCooldown = 1.0f;  // Muito rápido (metralhadora mágica)
    mProjectileSpeed = 400.f;
    mProjectileTexture = "magic_bolt";
    
    updateRangeVisuals();
}