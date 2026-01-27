#include "Enemies/Ghost.hpp"

Ghost::Ghost(sf::Vector2f startPos) // High-speed, low-health
    : Enemy(
        startPos, // Position
        "ghost_texture", // Texture Name
        80,     // Health
        10,     // Damage for player
        110.f,  // Speed
        25      // Gold Reward 
      )
{
}
