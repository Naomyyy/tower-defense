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

enum class Difficulty {Easy,Normal,Hard};


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
    
    sf::RenderWindow window;
    Map map;
    // Entity Containers
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<Projectile> projectiles;
    // Spawning System
    float spawnTimer = 0.f;
    float spawnInterval = 2.f; 
    int spawnedCount = 0;
    int maxSpawn = 20;
    float enemySpeedMultiplier;
    // UI and Menus
    MenuState gameState = MenuState::MainMenu; // Começa no menu principal
    std::unique_ptr<MenuScreen> currentMenu;
    sf::Font font; 
    sf::Font numberFont;
    sf::Text moneyUI;
    std::vector<std::unique_ptr<Button>> shopButtons;
    sf::Text livesUI;
    // Game 
    Difficulty difficulty;
    TowerType selectedTower;
    std::vector<sf::Vector2f> path;
    bool isTowerSelected;
    int lives;
    int money;
};
