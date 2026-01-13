#include "Tower.hpp"
#include "AssetManager.hpp"
#include <cmath>

// =========================================================
// CLASSE BASE: TOWER
// =========================================================

Tower::Tower(sf::Vector2f position, const std::string& textureName) {
    // Busca a textura pelo APELIDO (ex: "base_tower")
    const sf::Texture& texture = AssetManager::getInstance().getTexture(textureName);
    mSprite.setTexture(texture);

    // Ajuste de escala
    const float targetSize = 80.0f; 
    float scaleFactor = targetSize / texture.getSize().x;
    mSprite.setScale(scaleFactor, scaleFactor);
    
    // Centraliza
    mSprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f); 
    mSprite.setPosition(position);

    // Valores padrão
    mRange = 100.f;
    mFireCooldown = 1.0f;
    mTimer = 0.f;
    
    // Inicializa o visual do range
    updateRangeVisuals();
}

void Tower::updateRangeVisuals() {
    mRangeIndicator.setRadius(mRange);
    mRangeIndicator.setOrigin(mRange, mRange);
    mRangeIndicator.setPosition(mSprite.getPosition());
    mRangeIndicator.setFillColor(sf::Color::Transparent);
    mRangeIndicator.setOutlineThickness(1.f);
    mRangeIndicator.setOutlineColor(sf::Color(100, 100, 255, 120));
}

std::optional<Projectile> Tower::update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    mTimer -= dt;
    
    if (mTimer > 0) return std::nullopt;

    for (const auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;

        sf::Vector2f diff = enemy->getPosition() - mSprite.getPosition();
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist <= mRange) {
            sf::Vector2f direction = diff / dist; 
            mTimer = mFireCooldown; 
            
            // Cria o projétil com os dados específicos desta torre
            return Projectile(
                mSprite.getPosition(), 
                direction, 
                mProjectileSpeed, 
                mDamage, 
                mProjectileTexture
            );
        }
    }
    return std::nullopt;
}

// ===========================================================
// A FUNÇÃO QUE ESTAVA FALTANDO (CAUSAVA O ERRO DE LINKER)
// ===========================================================
void Tower::draw(sf::RenderWindow& window) {
    // window.draw(mRangeIndicator); // Opcional: ver o alcance
    window.draw(mSprite);
}


// =========================================================
// SUBCLASSES (CORRIGIDAS COM OS APELIDOS DO ASSETMANAGER)
// =========================================================

ArcherTower::ArcherTower(sf::Vector2f position) 
    : Tower(position, "archer_tower") // <--- Usa o ID "archer_tower"
{
    mRange = 200.0f;
    mFireCooldown = 0.5f;   
    mDamage = 15;           
    mProjectileSpeed = 600.f; 
    mProjectileTexture = "arrow";
    
    updateRangeVisuals(); 
}

CannonTower::CannonTower(sf::Vector2f position) 
    : Tower(position, "cannon_tower") // <--- Usa o ID "cannon_tower"
{
    mRange = 150.0f;        
    mFireCooldown = 2.0f;   
    mDamage = 50;           
    mProjectileSpeed = 300.f; 
    mProjectileTexture = "cannonball";
    
    updateRangeVisuals();
}

MageTower::MageTower(sf::Vector2f position) 
    : Tower(position, "mage_tower") // <--- Usa o ID "mage_tower"
{
    mRange = 350.0f;        
    mFireCooldown = 1.0f;   
    mDamage = 30;
    mProjectileSpeed = 400.f;
    mProjectileTexture = "magic_bolt";
    
    updateRangeVisuals();
}