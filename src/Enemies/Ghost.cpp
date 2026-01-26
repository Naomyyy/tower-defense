#include "Enemies/Ghost.hpp"

Ghost::Ghost(sf::Vector2f startPos)
    : Enemy(
        startPos,
        "ghost_texture",
        80,     // pouca vida
        10,     // dano baixo
        110.f,  // muito rápido
        25      // recompensa média
      )
{
}
