#pragma once
#include "Enemy.hpp"

class Ghost : public Enemy {
public:
    explicit Ghost(sf::Vector2f startPos);
};
