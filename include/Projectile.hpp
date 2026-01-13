#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Projectile {
public:
    // Construtor atualizado: Agora aceita velocidade, dano e nome da textura
    Projectile(sf::Vector2f position, sf::Vector2f direction, float speed, int damage, const std::string& textureName);
    
    void update(float dt);
    void draw(sf::RenderWindow& window);
    
    // Getters e Setters
    sf::Vector2f getPosition() const;
    bool isAlive() const;
    void destroy(); 

    // O método que estava faltando:
    int getDamage() const;

private:
    sf::Sprite mSprite;     // Mudamos de CircleShape para Sprite (para usar imagens)
    sf::Vector2f mVelocity;
    bool mAlive = true;
    int mDamage;            // Variável para guardar o dano
};