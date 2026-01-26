#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include "Enemies/Enemy.hpp"      // Certifique-se que existe
#include "Projectile.hpp" // Certifique-se que existe

enum class TowerType {
    Archer,
    Cannon,
    Mage
};

class Tower {
public:
    // Construtor
    Tower(sf::Vector2f position, const std::string& textureName);
    virtual ~Tower() = default;

    // Lógica principal: Retorna um projetil se atirar
    virtual std::optional<Projectile> update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies);
    
    void draw(sf::RenderWindow& window);

protected:
    void updateRangeVisuals();
    // Função auxiliar para encontrar o inimigo mais próximo
    const Enemy* findTarget(const std::vector<std::unique_ptr<Enemy>>& enemies);

protected:
    sf::Sprite mSprite;
    sf::CircleShape mRangeIndicator; 
    
    float mRange;           
    float mFireCooldown;    
    float mTimer;           
    
    int mDamage;                    
    float mProjectileSpeed;         
    std::string mProjectileTexture; 
};