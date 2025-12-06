#include "Projectile.hpp"

Projectile::Projectile(sf::Vector2f position, sf::Vector2f velocity)
    : mVelocity(velocity) 
{
    mShape.setRadius(4.f);
    mShape.setOrigin(4.f, 4.f);
    mShape.setPosition(position);
    mShape.setFillColor(sf::Color::Yellow);
}

void Projectile::update(float dt) {
    mShape.move(mVelocity * mSpeed * dt);

    // Verifica limites da tela (hardcoded para simplicidade, ideal passar window size)
    sf::Vector2f pos = mShape.getPosition();
    if (pos.x < 0 || pos.x > 2000 || pos.y < 0 || pos.y > 2000) {
        mAlive = false;
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(mShape);
}

sf::Vector2f Projectile::getPosition() const { return mShape.getPosition(); }
bool Projectile::isAlive() const { return mAlive; }
void Projectile::destroy() { mAlive = false; }