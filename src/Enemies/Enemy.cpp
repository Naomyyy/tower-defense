#include "Enemies/Enemy.hpp"
#include "AssetManager.hpp" 

Enemy::Enemy(sf::Vector2f startPos, const std::string& textureName,
             int health, int damage, float speed, int reward)
    : mPosition(startPos), health(health), damage(damage), speed(speed), reward(reward), alive(true)
{
    const sf::Texture& texture = AssetManager::getInstance().getTexture(textureName);
    mSprite.setTexture(texture);
    // Escala: Ajusta o tamanho do sprite
    const float targetSize = 50.0f; 
    float scaleFactor = targetSize / texture.getSize().x;
    mSprite.setScale(scaleFactor, scaleFactor);
    // Define a origem do sprite para o centro (para rotação e posicionamento).
    mSprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    mSprite.setPosition(startPos);

    // Configura as barras de vida 
    maxHealth = health; // Guarda o valor inicial
    mHealthBarBack.setSize(sf::Vector2f(40.f, 5.f)); // Largura de 40px, altura de 5px
    mHealthBarBack.setFillColor(sf::Color::Red);
    mHealthBarFront.setSize(sf::Vector2f(40.f, 5.f));
    mHealthBarFront.setFillColor(sf::Color::Green);
}

void Enemy::update(float dt, const std::vector<sf::Vector2f>& path) {
    if (!alive || path.empty()) return;

    if (currentPathIndex < path.size()) {
        sf::Vector2f target = path[currentPathIndex];
        sf::Vector2f direction = target - mPosition;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < speed * dt) {
            mPosition = target;
            currentPathIndex++;
        } else {
            direction /= distance; // normaliza
            mPosition += direction * speed * dt;
        }
        mSprite.setPosition(mPosition);
    } else {
        // Chegou no fim do caminho - você pode decidir o que acontece
        destroy();
    }

    sf::Vector2f barPos = mPosition + sf::Vector2f(-20.f, -35.f); 
    mHealthBarBack.setPosition(barPos);
    mHealthBarFront.setPosition(barPos);

    // 1. Cálculo da proporção com precisão
    float healthPercentage = static_cast<float>(health) / static_cast<float>(maxHealth);

    // Garante que a porcentagem nunca seja menor que zero para não inverter a barra
    if (healthPercentage < 0.0f) healthPercentage = 0.0f;

    // 2. Ajuste do tamanho da barra verde (Front)
    // Se a vida for 1, a barra ainda terá um "tiquinho" de verde
    mHealthBarFront.setSize(sf::Vector2f(40.f * healthPercentage, 5.f));

    // 3. Lógica de Cores revisada
    // A barra só fica puramente vermelha se a parte verde sumir. 
    // Vamos mudar a cor da parte VERDE (Front) para indicar perigo:
    if (healthPercentage > 0.5f) {
        mHealthBarFront.setFillColor(sf::Color::Green);
    } else if (healthPercentage > 0.15f) { // Ainda tem um pouco de vida
        mHealthBarFront.setFillColor(sf::Color::Yellow);
    } else if (healthPercentage > 0.0f) { // Vida crítica, mas vivo
        mHealthBarFront.setFillColor(sf::Color(255, 140, 0)); // Laranja (muito perto de morrer)
    } else {
        mHealthBarFront.setFillColor(sf::Color::Red);
    }

    if (!path.empty()) {
        sf::Vector2f start = path.front();
        sf::Vector2f end = path.back();
        sf::Vector2f pos = getPosition();

        float totalDist = std::hypot(end.x - start.x, end.y - start.y);
        float traveled = std::hypot(pos.x - start.x, pos.y - start.y);
        mProgress = totalDist > 0 ? traveled / totalDist : 0.f;
        if (mProgress > 1.f) mProgress = 1.f;
    }

}

void Enemy::draw(sf::RenderWindow& window) {
    if (alive) {
        window.draw(mSprite);
        window.draw(mHealthBarBack);  // Desenha o fundo vermelho
        window.draw(mHealthBarFront); // Desenha a vida verde por cima
    }
}

sf::Vector2f Enemy::getPosition() const {
    return mPosition;
}

const sf::Sprite& Enemy::getSprite() const{
    return mSprite;
}

bool Enemy::isAlive() const {
    return alive;
}

void Enemy::destroy() {
    alive = false;
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        alive = false;
    }
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getReward() const {
    return reward;
}
