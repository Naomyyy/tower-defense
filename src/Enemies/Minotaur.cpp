#include "Enemies/Minotaur.hpp"

Minotaur::Minotaur(sf::Vector2f startPos)
    : Enemy(
        startPos,
        "minotaur_texture",
        350,    // vida média-alta
        30,     // dano alto
        55.f,   // velocidade média
        50      // recompensa alta
      )
{
}
