#include "Projectile.hpp"
#include "AssetManager.hpp" // Importante: inclua seu gerenciador de assets
#include <cmath>            // Necessário para calcular o ângulo (atan2)

// Construtor Atualizado
Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction, float speed, int damage, const std::string& textureName)
    : mDamage(damage), mAlive(true)
{
    // 1. Configurar a Textura (Pegando do Gerenciador)
    const sf::Texture& texture = AssetManager::getInstance().getTexture(textureName);
    mSprite.setTexture(texture);

    // 2. Definir a origem no centro da imagem
    // Isso é crucial para que o projétil gire em torno do próprio eixo e não do canto
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // 3. Posicionamento
    mSprite.setPosition(position);

    // 4. Calcular Velocidade
    mVelocity = direction * speed;

    // 5. Calcular Rotação (Matemática para "olhar" para a direção)
    // atan2 devolve o ângulo em radianos, multiplicamos por 180/PI para ter graus
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    mSprite.setRotation(angle);
}

void Projectile::update(float dt) {
    // Move o sprite baseado na velocidade e tempo
    mSprite.move(mVelocity * dt);
    
    // Opcional: Destruir se sair muito longe da tela (para economizar memória)
    // if (mSprite.getPosition().x > 2000 || mSprite.getPosition().x < -100) destroy();
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(mSprite);
}

// --- Getters e Setters ---

sf::Vector2f Projectile::getPosition() const {
    return mSprite.getPosition();
}

bool Projectile::isAlive() const {
    return mAlive;
}

void Projectile::destroy() {
    mAlive = false;
}

int Projectile::getDamage() const {
    return mDamage;
}