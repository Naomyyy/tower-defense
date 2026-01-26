#include "Enemies/Golem.hpp"

Golem::Golem(sf::Vector2f startPos)
    : Enemy(
        startPos,
        "base_enemy",
        200,    // alta vida
        20,     // dano médio
        40.f,   // velocidade lenta
        40      // recompensa alta
      )
{
}