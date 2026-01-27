#include "Enemies/Golem.hpp"

Golem::Golem(sf::Vector2f startPos)
    : Enemy(
        startPos, // Position
        "base_enemy", // Texture name
        200,    // Health
        20,     // Damage for Player
        40.f,   // Velocity 
        40      // Gold reward 
      )
{
}