#include "Towers/ArcherTower.hpp"

ArcherTower::ArcherTower(sf::Vector2f position) 
    : Tower(position, "archer_tower") // Passa a textura para a base
{
    // Configurações Específicas da Arqueira
    mRange = 200.f;
    mDamage = 10;
    mFireCooldown = 0.5f; // 1 tiro por segundo
    mProjectileSpeed = 500.f;
    mProjectileTexture = "arrow";
    
    updateRangeVisuals(); // Atualiza o tamanho do círculo de alcance

    
}