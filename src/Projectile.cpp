#include "Projectile.hpp"
#include "AssetManager.hpp" 
#include <cmath>            

Projectile::Projectile(sf::Vector2f position, sf::Vector2f direction, float speed, int damage, const std::string& textureName)
    : damage(damage), alive(true)
{
    const sf::Texture& texture = AssetManager::getInstance().getTexture(textureName);
    sprite.setTexture(texture);

    // Adjusts the sprite to 20 px
    float targetSize = 20.0f;
    float scaleFactor = targetSize / (float)texture.getSize().x;
    sprite.setScale(scaleFactor, scaleFactor);

    // Set origin to the center for correct rotation logic
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(position);
    // Calculate Velocity
    velocity = direction * speed;
    // We use atan2(y, x) to find the angle of the direction vector in radian, we convert it to degrees (180/PI) for SFML.
    //This ensures the projectile "faces" the enemy it's traveling toward.
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle);
}

void Projectile::update(float dt) {
    sprite.move(velocity * dt);
}

void Projectile::draw(sf::RenderWindow& window) {
    if (alive) {
        window.draw(sprite);
    }
}


sf::Vector2f Projectile::getPosition() const {
    return sprite.getPosition();
}

const sf::Sprite& Projectile::getSprite() const{
    return sprite;
}

bool Projectile::isAlive() const {
    return alive;
}

void Projectile::destroy() {
    alive = false;
}

int Projectile::getDamage() const {
    return damage;
}