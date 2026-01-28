#include "Enemies/Ghost.hpp"

Ghost::Ghost(sf::Vector2f startPos) // High-speed, low-health
    : Enemy(
        startPos, // Position
        "ghost_texture", // Texture Name
        80,     // Health
        5,     // Damage
        110.f,  // Speed
        25      // Gold Reward 
      )
{
}
