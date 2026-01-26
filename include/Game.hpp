#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "Enemies/Golem.hpp"
#include "Enemies/Ghost.hpp"
#include "Enemies/Minotaur.hpp"

#include "Towers/Tower.hpp" 
#include "Towers/ArcherTower.hpp"
#include "Towers/CannonTower.hpp"
#include "Towers/MageTower.hpp"

#include "Projectile.hpp"
#include "Maps/Map.hpp"

#include "Menus/MenuScreen.hpp"
#include "Menus/MainMenu.hpp"
#include "Menus/PauseMenu.hpp"
#include "Menus/Button.hpp"
#include "Menus/GameOverMenu.hpp"
#include "Menus/WinMenu.hpp"  
#include "Menus/DifficultyMenu.hpp"  

enum class Difficulty {
    Easy,
    Normal,
    Hard
};


class Game {
public:
    Game();

    void run();

private:
    void processEvents(const sf::Event& ev);
    void update(float dt);
    void handleCollisions();
    void render();
    void setupDifficulty(int choice);

private:
    TowerType mSelectedTower;
    sf::RenderWindow mWindow;
    std::vector<sf::Vector2f> mPath;

    std::vector<std::unique_ptr<Enemy>> mEnemies;
    std::vector<std::unique_ptr<Tower>> mTowers;
    std::vector<Projectile> mProjectiles;
    Map mMap;
    bool mIsTowerSelected;

    float mSpawnTimer = 0.f;
    float mSpawnInterval = 2.f; // intervalo entre spawns (segundos)
    int mSpawnedCount = 0;
    int mMaxSpawn = 20;
    float mEnemySpeedMultiplier;

    MenuState mGameState = MenuState::MainMenu; // Começa no menu principal
    std::unique_ptr<MenuScreen> mCurrentMenu;

    int mMoney;
    sf::Font mFont; 
    sf::Font mNumberFont;
    sf::Text mMoneyUI;
    std::vector<std::unique_ptr<Button>> mShopButtons;
    
    int mLives;
    sf::Text mLivesUI;

    Difficulty mDifficulty;

};
