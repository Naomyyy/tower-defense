#include "Game.hpp"
#include "AssetManager.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>

#include "Enemies/Golem.hpp"
#include "Enemies/Ghost.hpp"
#include "Enemies/Minotaur.hpp"

#include "Towers/Tower.hpp" 
#include "Towers/ArcherTower.hpp"
#include "Towers/CannonTower.hpp"
#include "Towers/MageTower.hpp"

#include "Menus/DifficultyMenu.hpp"
#include "Menus/PauseMenu.hpp"
#include "Menus/MainMenu.hpp"
#include "Menus/GameOverMenu.hpp"
#include "Menus/WinMenu.hpp"

#ifndef TILE_SIZE
#define TILE_SIZE 40 
#define GRID_WIDTH 20
#define GRID_HEIGHT 15
#endif

Game::Game()
    : window(sf::VideoMode(800, 600), "Tower Defense - Modular"),
      spawnInterval(2.f),
      spawnTimer(0.f),
      spawnedCount(0),
      maxSpawn(20),
      gameState(MenuState::MainMenu),
      selectedTower(TowerType::Archer),
      money(200),
      isTowerSelected(false),
      difficulty(Difficulty::Normal),
      lives(20)
{
    window.setFramerateLimit(60);

    AssetManager& assets = AssetManager::getInstance();
    assets.loadTexture("base_enemy",   "assets/golem.png");
    assets.loadTexture("minotaur_texture", "assets/minotaur.png");
    assets.loadTexture("ghost_texture",    "assets/ghost.png");

    assets.loadTexture("archer_tower", "assets/torre1.png");
    assets.loadTexture("cannon_tower", "assets/torre2.png");
    assets.loadTexture("mage_tower",   "assets/torre3.png");

    assets.loadTexture("arrow",        "assets/stone.png");
    assets.loadTexture("cannonball",   "assets/lightning.png");
    assets.loadTexture("magic_bolt",   "assets/barrel.png");

    font.loadFromFile("assets/font.ttf");
    numberFont.loadFromFile("assets/numberFont.ttf");
    
    moneyUI.setFont(numberFont);
    moneyUI.setCharacterSize(20);
    moneyUI.setFillColor(sf::Color::Yellow);
    moneyUI.setPosition(550.f, 20.f);
    moneyUI.setString("GOLD: " + std::to_string(money));

    livesUI.setFont(numberFont);
    livesUI.setCharacterSize(20);
    livesUI.setFillColor(sf::Color::Red);
    livesUI.setPosition(550.f, 45.f);
    livesUI.setString("LIVES: " + std::to_string(lives));

    shopButtons.push_back(std::make_unique<Button>("50",  sf::Vector2f(550.f, 550.f), numberFont, 15));
    shopButtons.push_back(std::make_unique<Button>("100", sf::Vector2f(620.f, 550.f), numberFont, 15));
    shopButtons.push_back(std::make_unique<Button>("150", sf::Vector2f(690.f, 550.f), numberFont, 15));

    currentMenu = std::make_unique<MainMenu>(window);

    demolishButton = std::make_unique<Button>("X", sf::Vector2f(760.f, 550.f), numberFont, 15);

    isDemolishing = false; 
}

// Main game loop, handling events, updates, and rendering
void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) window.close();
            else {
                if (currentMenu) currentMenu->handleEvent(ev, window);
                else processEvents(ev);
            }
        }

        if (currentMenu) {
            currentMenu->update(dt, window);
            MenuState nextState = currentMenu->getNextState();

            if (nextState != MenuState::None && nextState != gameState) {
                MenuState previousState = gameState;
                gameState = nextState;

                switch (gameState) {
                    case MenuState::MainMenu:  currentMenu = std::make_unique<MainMenu>(window); break;
                    case MenuState::Pause:     currentMenu = std::make_unique<PauseMenu>(window);  break;
                    case MenuState::Gameplay: {
                        if (auto diffMenu = dynamic_cast<DifficultyMenu*>(currentMenu.get())) {
                            setupDifficulty(diffMenu->getSelectedDifficulty());
                        } 
                        currentMenu.reset();
                        break;
                    }
                    case MenuState::GameOver:  currentMenu = std::make_unique<GameOverMenu>(window); break;
                    case MenuState::Win:       currentMenu = std::make_unique<WinMenu>(window); break;
                    case MenuState::Difficulty:currentMenu = std::make_unique<DifficultyMenu>(window); break;
                    default: break;
                }
            }
        } else update(dt);
        render();
    }
}

// Handles user inputs during gameplay
void Game::processEvents(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
        if (isDemolishing) {
            isDemolishing = false;
            return;
        }
        gameState = MenuState::Pause;
        currentMenu = std::make_unique<PauseMenu>(window);
        return;
    }

    if (demolishButton->clicked(window, ev)) {
        isDemolishing = !isDemolishing;
        isTowerSelected = false; 
        return;
    }

    for (size_t i = 0; i < shopButtons.size(); ++i) {
        if (shopButtons[i]->clicked(window, ev)) {
            int cost = (i == 0) ? 50 : (i == 1) ? 100 : 150;
            if (money >= cost) {
                selectedTower = (i == 0) ? TowerType::Archer : (i == 1) ? TowerType::Cannon : TowerType::Mage;
                isTowerSelected = true;
                isDemolishing = false; 
            }
            return; 
        }
    }

    if(ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left){

        sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        
        if (pos.x > 580 && pos.y > 450) return;

        if (isDemolishing) {
            for (size_t i = 0; i < towers.size(); ++i) {
                if (towers[i]->getSprite().getGlobalBounds().contains(pos)) {
                    towers.erase(towers.begin() + i);
                    isDemolishing = false; 
                    return;
                }
            }
        }
        else if (isTowerSelected) {
            int tileX = (int)pos.x / TILE_SIZE;
            int tileY = (int)pos.y / TILE_SIZE;

            if (tileX >= 0 && tileX < GRID_WIDTH && tileY >= 0 && tileY < GRID_HEIGHT) {
                if (map.isBuildable(tileX, tileY)) { 
                    int cost = (selectedTower == TowerType::Archer) ? 50 : 
                               (selectedTower == TowerType::Cannon) ? 100 : 150;

                    sf::Vector2f towerPos = {(float)tileX * TILE_SIZE + 20.f, (float)tileY * TILE_SIZE + 20.f };
                    
                    if (selectedTower == TowerType::Archer) towers.push_back(std::make_unique<ArcherTower>(towerPos));
                    else if (selectedTower == TowerType::Cannon) towers.push_back(std::make_unique<CannonTower>(towerPos));
                    else towers.push_back(std::make_unique<MageTower>(towerPos));

                    money -= cost;
                    isTowerSelected = false; 
                }
            }
        }
    }
    else if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Right) {
        isTowerSelected = false;
        isDemolishing = false;
    }
}

// Updates game elements each frame
void Game::update(float dt) {
    spawnTimer -= dt;
    const std::vector<sf::Vector2f>& path = map.getPath();

    if (spawnTimer <= 0.f && spawnedCount < maxSpawn && !path.empty()) {
        if (difficulty == Difficulty::Easy) {
            enemies.push_back(std::make_unique<Golem>(path.front()));
        } else if (difficulty == Difficulty::Normal) {
            int r = rand() % 2;
            if (r == 0) enemies.push_back(std::make_unique<Golem>(path.front()));
            else enemies.push_back(std::make_unique<Ghost>(path.front()));
        } else {
            int r = rand() % 3;
            if (r == 0) enemies.push_back(std::make_unique<Golem>(path.front()));
            else if (r == 1) enemies.push_back(std::make_unique<Ghost>(path.front()));
            else enemies.push_back(std::make_unique<Minotaur>(path.front()));
        }
        spawnedCount++;
        spawnTimer = spawnInterval;
    }

    for (auto& enemy : enemies) {
        enemy->update(dt, path);

        sf::Vector2f diff = enemy->getPosition() - path.back();

        if (enemy->isAlive() && (diff.x * diff.x + diff.y * diff.y) < 100.f) {

            lives -= enemy->getDamage();
            enemy->destroy(); 
    
            if (lives <= 0) {
                gameState = MenuState::GameOver;
                currentMenu = std::make_unique<GameOverMenu>(window);
                return;
            }
        }
    }

    for (auto& tower : towers) {
        if (auto proj = tower->update(dt, enemies)) {
            projectiles.push_back(*proj);
        }
    }

    for (auto& proj : projectiles) proj.update(dt);
    for (auto& btn : shopButtons) btn->update(window);
    demolishButton->update(window);

    handleCollisions();

    if (spawnedCount >= maxSpawn && enemies.empty()) {
        gameState = MenuState::Win;
        currentMenu = std::make_unique<WinMenu>(window);
        return;
    }
    moneyUI.setString("GOLD: " + std::to_string(money));
    livesUI.setString("LIFE " + std::to_string(lives));
}
// Manages collisions between projectiles and enemies
void Game::handleCollisions() {
    for (auto& p : projectiles) {
        if (!p.isAlive()) continue;
        for (auto& e : enemies) {
            if (!e->isAlive()) continue;
            if (p.getSprite().getGlobalBounds().intersects(e->getSprite().getGlobalBounds())) {
                e->takeDamage(p.getDamage()); 
                if (!e->isAlive()) money += e->getReward();
                p.destroy(); 
                break; 
            }
        }
    }

    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const Projectile& p){return !p.isAlive();}), projectiles.end());
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e){return !e->isAlive();}), enemies.end());
}

// Renders the game elements and the current menu
void Game::render() {
    window.clear(sf::Color::Black);

    if (currentMenu) {
        currentMenu->draw(window);
    } else {
        map.draw(window);

        for (auto& e : enemies) e->draw(window);
        for (auto& t : towers) t->draw(window);
        for (auto& p : projectiles) p.draw(window);

        AssetManager& assets = AssetManager::getInstance();
        std::vector<std::string> textures = {"archer_tower", "cannon_tower", "mage_tower"};
        for (size_t i = 0; i < shopButtons.size(); ++i) {
            shopButtons[i]->draw(window);
            sf::Sprite icon(assets.getTexture(textures[i]));
            icon.setScale(0.4f, 0.4f);
            icon.setPosition(shopButtons[i]->getPosition().x + 5, shopButtons[i]->getPosition().y - 45);
            window.draw(icon);
        }

        demolishButton->draw(window);

        if (isDemolishing) { // Show demolish mode warning
            sf::Text warningText;
            warningText.setFont(font);
            warningText.setString("DEMOLISH MODE");
            warningText.setCharacterSize(16);
            warningText.setFillColor(sf::Color::Red);
            warningText.setPosition(740.f, 530.f);
            window.draw(warningText);
        }

        window.draw(moneyUI);
        window.draw(livesUI);
        
        
        // Draw tower ghost and range if a tower is selected
        if (isTowerSelected) {
            sf::Vector2i posI = sf::Mouse::getPosition(window);
            sf::Vector2f posF = window.mapPixelToCoords(posI);

            float range = 0.f;
            if (selectedTower == TowerType::Archer) range = 200.f;
            else if (selectedTower == TowerType::Cannon) range = 150.f;
            else if (selectedTower == TowerType::Mage) range = 350.f;

            sf::CircleShape rangeCircle(range);
            rangeCircle.setOrigin(range, range);
            rangeCircle.setPosition(posF);
            rangeCircle.setFillColor(sf::Color(255, 255, 255, 40));
            rangeCircle.setOutlineThickness(1.5f);
            rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 120));
            window.draw(rangeCircle);
            
            std::string texName = (selectedTower == TowerType::Archer) ? "archer_tower" : 
                                 (selectedTower == TowerType::Cannon) ? "cannon_tower" : "mage_tower";
            
            sf::Sprite ghost(assets.getTexture(texName));
            ghost.setOrigin(ghost.getLocalBounds().width / 2.f, ghost.getLocalBounds().height/ 2.f);
            ghost.setPosition(posF);
            ghost.setColor(sf::Color(255, 255, 255, 150));
            window.draw(ghost);
        }
    }
    window.display();
}

// Configures game parameters based on selected difficulty
void Game::setupDifficulty(int choice) {
    
    towers.clear();
    enemies.clear();
    projectiles.clear();
    spawnedCount = 0;
    spawnTimer = 5.f;

    if (choice == 0) { 
        difficulty = Difficulty::Easy;
        map.load("assets/map_easy.txt");
        money = 150 ;
        lives = 10;
        maxSpawn = 20;
        enemySpeedMultiplier = 0.8f;
    } 
    else if (choice == 2) { 
        difficulty = Difficulty::Hard;
        map.load("assets/map_hard.txt");
        money = 200;
        lives = 10;
        maxSpawn = 80;
        enemySpeedMultiplier = 1.3f;
    } 
    else { 
        difficulty = Difficulty::Normal;
        map.load("assets/map_normal.txt");
        money = 150;
        lives = 10;
        maxSpawn = 50;
        enemySpeedMultiplier = 1.0f;
    }

    moneyUI.setString("GOLD: " + std::to_string(money));
    livesUI.setString("LIFE: " + std::to_string(lives));
}