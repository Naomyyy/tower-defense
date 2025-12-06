#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.hpp"
#include "Tower.hpp"
#include "Projectile.hpp"
#include "Menus/MenuScreen.hpp"
#include "Menus/MainMenu.hpp"
#include "Menus/PauseMenu.hpp"
#include "Menus/MenuMapEditor.hpp"

class Game {
public:
    Game();

    void run();

private:
    void processEvents(const sf::Event& ev);
    void update(float dt);
    void handleCollisions();
    void render();

private:
    sf::RenderWindow mWindow;
    std::vector<sf::Vector2f> mPath;

    std::vector<std::unique_ptr<Enemy>> mEnemies;
    Tower mTower;
    std::vector<Projectile> mProjectiles;

    float mSpawnTimer = 0.f;
    float mSpawnInterval = 2.f; // intervalo entre spawns (segundos)
    int mSpawnedCount = 0;
    int mMaxSpawn = 20;

    MenuState mGameState;
    std::unique_ptr<MenuScreen> mCurrentMenu;
};
