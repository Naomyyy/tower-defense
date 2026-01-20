#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <string>
#include "Enemy.hpp"
#include "Projectile.hpp"

enum class TowerType {
    Archer,
    Cannon,
    Mage
};

// =========================================================
// CLASSE BASE: TOWER
// =========================================================
class Tower {
public:
    // Construtor: Recebe posição e o nome da textura para buscar no AssetManager
    Tower(sf::Vector2f position, const std::string& textureName);
    
    // Destrutor virtual (Obrigatório para herança funcionar bem com unique_ptr)
    virtual ~Tower() = default;

    // Atualiza a torre (mira e atira)
    // Retorna um Projétil (std::optional) se atirar neste frame
    virtual std::optional<Projectile> update(float dt, const std::vector<std::unique_ptr<Enemy>>& enemies);
    
    // Desenha a torre
    void draw(sf::RenderWindow& window);

protected:
    // --- Métodos Auxiliares para as Filhas ---
    // Atualiza o círculo visual (range) baseada no valor de mRange
    void updateRangeVisuals();

protected:
    // --- Componentes Visuais ---
    sf::Sprite mSprite;
    sf::CircleShape mRangeIndicator; // Círculo que mostra o alcance
    
    // --- Atributos de Jogo (Game Design) ---
    float mRange;           // Alcance do tiro
    float mFireCooldown;    // Tempo entre tiros
    float mTimer;           // Contador interno para o cooldown
    
    int mDamage;                    // Dano que a torre causa
    float mProjectileSpeed;         // Velocidade do projétil disparado
    std::string mProjectileTexture; // Nome da textura do projétil (ex: "arrow", "cannonball")
};

// =========================================================
// SUBCLASSES (TIPOS DE TORRES)
// =========================================================

// 1. Archer: Rápida, dano baixo, projétil veloz
class ArcherTower : public Tower {
public:
    ArcherTower(sf::Vector2f position);
};

// 2. Cannon: Lenta, dano alto, projétil lento
class CannonTower : public Tower {
public:
    CannonTower(sf::Vector2f position);
};

// 3. Mage: Alcance longo, dano médio
class MageTower : public Tower {
public:
    MageTower(sf::Vector2f position);
};