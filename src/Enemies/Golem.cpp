#include "Enemies/Golem.hpp"

Golem::Golem(sf::Vector2f startPos)
    : Enemy(
        startPos, // Position
        "base_enemy", // Texture name
        200,    // Health
        2,     // Damage for Player
        40.f,   // Velocity 
        10      // Gold reward 
      )
{
}