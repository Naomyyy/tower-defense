#include "Enemies/Minotaur.hpp"

Minotaur::Minotaur(sf::Vector2f startPos) // High health and reward  
    : Enemy(
        startPos, // Position
        "minotaur_texture", // Texture name
        350,    // Health 
        10,     // Damage to player
        55.f,   // Speed
        50      // Gold Reward
      )
{
}
