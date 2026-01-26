#include "Game.hpp"
#include "AssetManager.hpp"
#include <iostream>
#include <algorithm>

#include "Enemies/Golem.hpp"
#include "Enemies/Ghost.hpp"
#include "Enemies/Minotaur.hpp"

#include "Towers/Tower.hpp" 
#include "Towers/ArcherTower.hpp"
#include "Towers/CannonTower.hpp"
#include "Towers/MageTower.hpp"

// --- INCLUDES DOS MENUS ---
#include "Menus/DifficultyMenu.hpp"
#include "Menus/PauseMenu.hpp"
#include "Menus/MainMenu.hpp"
#include "Menus/GameOverMenu.hpp"
#include "Menus/WinMenu.hpp"

// ------------------------

#ifndef TILE_SIZE
#define TILE_SIZE 40 
#define GRID_WIDTH 20
#define GRID_HEIGHT 15
#endif

Game::Game()
    : mWindow(sf::VideoMode(800, 600), "Tower Defense - Modular"),
      mSpawnInterval(2.f),
      mSpawnTimer(0.f),
      mSpawnedCount(0),
      mMaxSpawn(20),
      mGameState(MenuState::MainMenu),
      mSelectedTower(TowerType::Archer),
      mMoney(200),
      mIsTowerSelected(false),
      mDifficulty(Difficulty::Normal),
      mLives(20)
{
    mWindow.setFramerateLimit(60);

    AssetManager& assets = AssetManager::getInstance();
    // Inimigos
    assets.loadTexture("base_enemy",   "assets/golem.png");
    assets.loadTexture("minotaur_texture", "assets/minotaur.png");
    assets.loadTexture("ghost_texture",    "assets/ghost.png");

    // Torres
    assets.loadTexture("archer_tower", "assets/torre1.png");
    assets.loadTexture("cannon_tower", "assets/torre2.png");
    assets.loadTexture("mage_tower",   "assets/torre3.png");

    // Projéteis
    assets.loadTexture("arrow",        "assets/stone.png");
    assets.loadTexture("cannonball",   "assets/lightning.png");
    assets.loadTexture("magic_bolt",   "assets/barrel.png");

    mFont.loadFromFile("assets/font.ttf");
    mNumberFont.loadFromFile("assets/numberFont.ttf");
    
    // UI GOLD
    mMoneyUI.setFont(mNumberFont);
    mMoneyUI.setCharacterSize(20);
    mMoneyUI.setFillColor(sf::Color::Yellow);
    mMoneyUI.setPosition(550.f, 20.f);
    mMoneyUI.setString("GOLD: " + std::to_string(mMoney));

    // UI LIVES
    mLivesUI.setFont(mNumberFont);
    mLivesUI.setCharacterSize(20);
    mLivesUI.setFillColor(sf::Color::Red);
    mLivesUI.setPosition(550.f, 45.f);
    mLivesUI.setString("LIVES: " + std::to_string(mLives));

    // Botões
    mShopButtons.push_back(std::make_unique<Button>("50",  sf::Vector2f(550.f, 550.f), mNumberFont, 15));
    mShopButtons.push_back(std::make_unique<Button>("100", sf::Vector2f(620.f, 550.f), mNumberFont, 15));
    mShopButtons.push_back(std::make_unique<Button>("150", sf::Vector2f(690.f, 550.f), mNumberFont, 15));

    mCurrentMenu = std::make_unique<MainMenu>(mWindow);
}

void Game::run() {
    sf::Clock clock;
    while (mWindow.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event ev;
        while (mWindow.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) mWindow.close();
            else {
                if (mCurrentMenu) mCurrentMenu->handleEvent(ev, mWindow);
                else processEvents(ev);
            }
        }

        if (mCurrentMenu) {
            mCurrentMenu->update(dt, mWindow);
            MenuState nextState = mCurrentMenu->getNextState();

            if (nextState != MenuState::None && nextState != mGameState) {
                MenuState previousState = mGameState;
                mGameState = nextState;

                switch (mGameState) {
                    case MenuState::MainMenu:  
                        mCurrentMenu = std::make_unique<MainMenu>(mWindow); 
                        break;
                    
                    case MenuState::Pause:     
                        mCurrentMenu = std::make_unique<PauseMenu>(mWindow); 
                        break;
                    
               
                    
                    
                    case MenuState::Gameplay: {
                        if (auto diffMenu = dynamic_cast<DifficultyMenu*>(mCurrentMenu.get())) {
                            int choice = diffMenu->getSelectedDifficulty();
                            mCurrentMenu.reset();
                            setupDifficulty(choice);
                        } else if (dynamic_cast<PauseMenu*>(mCurrentMenu.get())) {
                            mCurrentMenu.reset();
                        } else {
                            mCurrentMenu.reset();
                        }
                        break;
                    }
                    
                    case MenuState::GameOver:  
                        mCurrentMenu = std::make_unique<GameOverMenu>(mWindow);
                        break;
                    
                    case MenuState::Win:
                        mCurrentMenu = std::make_unique<WinMenu>(mWindow);
                        break;
                    
                    case MenuState::Difficulty:
                        mCurrentMenu = std::make_unique<DifficultyMenu>(mWindow);
                        break;
                    
                    default: break;
                }
            }
        } else update(dt);
        render();
    }
}

void Game::processEvents(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape) {
        mGameState = MenuState::Pause;
        mCurrentMenu = std::make_unique<PauseMenu>(mWindow);
        return;
    }

    for (size_t i = 0; i < mShopButtons.size(); ++i) {
        if (mShopButtons[i]->clicked(mWindow, ev)) {
            int cost = (i == 0) ? 50 : (i == 1) ? 100 : 150;
            if (mMoney >= cost) {
                mSelectedTower = (i == 0) ? TowerType::Archer : (i == 1) ? TowerType::Cannon : TowerType::Mage;
                mIsTowerSelected = true;
            }
            return; 
        }
    }

    if(ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left){
        sf::Vector2f mPos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
        if (mPos.x > 580 && mPos.y > 450) return;

        if (mIsTowerSelected) {
            int tileX = (int)mPos.x / TILE_SIZE;
            int tileY = (int)mPos.y / TILE_SIZE;

            if (tileX >= 0 && tileX < GRID_WIDTH && tileY >= 0 && tileY < GRID_HEIGHT) {
                if (mMap.isBuildable(tileX, tileY)) { 
                    int cost = (mSelectedTower == TowerType::Archer) ? 50 : 
                               (mSelectedTower == TowerType::Cannon) ? 100 : 150;

                    sf::Vector2f towerPos = {(float)tileX * TILE_SIZE + 20.f, (float)tileY * TILE_SIZE + 20.f };
                    
                    if (mSelectedTower == TowerType::Archer) mTowers.push_back(std::make_unique<ArcherTower>(towerPos));
                    else if (mSelectedTower == TowerType::Cannon) mTowers.push_back(std::make_unique<CannonTower>(towerPos));
                    else mTowers.push_back(std::make_unique<MageTower>(towerPos));

                    mMoney -= cost;
                    mIsTowerSelected = false; 
                }
            }
        }
    }
    else if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Right) {
        mIsTowerSelected = false;
    }
}

void Game::update(float dt) {
    mSpawnTimer -= dt;
    const std::vector<sf::Vector2f>& path = mMap.getPath();

    // SPAWN
    if (mSpawnTimer <= 0.f && mSpawnedCount < mMaxSpawn && !path.empty()) {
        if (mDifficulty == Difficulty::Easy) {
            mEnemies.push_back(std::make_unique<Golem>(path.front()));
        } else if (mDifficulty == Difficulty::Normal) {
            int r = rand() % 2;
            if (r == 0) mEnemies.push_back(std::make_unique<Golem>(path.front()));
            else mEnemies.push_back(std::make_unique<Ghost>(path.front()));
        } else {
            int r = rand() % 3;
            if (r == 0) mEnemies.push_back(std::make_unique<Golem>(path.front()));
            else if (r == 1) mEnemies.push_back(std::make_unique<Ghost>(path.front()));
            else mEnemies.push_back(std::make_unique<Minotaur>(path.front()));
        }
        mSpawnedCount++;
        mSpawnTimer = mSpawnInterval;
    }

    // UPDATE INIMIGOS
    for (auto it = mEnemies.begin(); it != mEnemies.end(); ) {
        (*it)->update(dt, path);

        // Chegou ao fim → perde vida
        sf::Vector2f enemyPos = (*it)->getPosition();
        sf::Vector2f lastWaypoint = path.back();
        float dx = lastWaypoint.x - enemyPos.x;
        float dy = lastWaypoint.y - enemyPos.y;

        if ((dx*dx + dy*dy) < 100.f && (*it)->isAlive()) {
            mLives--;
            (*it)->takeDamage(99999); // Mata o inimigo
            it = mEnemies.erase(it);

            if (mLives <= 0) {
                mGameState = MenuState::GameOver;
                mCurrentMenu = std::make_unique<GameOverMenu>(mWindow);
                return;
            }
            continue;
        }

        if (!(*it)->isAlive()) {
            it = mEnemies.erase(it);
        } else {
            ++it;
        }
    }

    // TORRES
    for (auto& tower : mTowers) {
        if (auto proj = tower->update(dt, mEnemies)) {
            mProjectiles.push_back(*proj);
        }
    }

    // PROJÉTEIS
    for (auto& proj : mProjectiles)
        proj.update(dt);

    // SHOP BUTTONS
    for (auto& btn : mShopButtons)
        btn->update(mWindow);

    handleCollisions();

    // Vitória
    if (mSpawnedCount >= mMaxSpawn && mEnemies.empty()) {
        mGameState = MenuState::Win;
        mCurrentMenu = std::make_unique<WinMenu>(mWindow);
        return;
    }

    // Atualiza UI
    mMoneyUI.setString("GOLD: " + std::to_string(mMoney));
    mLivesUI.setString("LIFE " + std::to_string(mLives));
}

void Game::handleCollisions() {
    for (auto& p : mProjectiles) {
        if (!p.isAlive()) continue;
        for (auto& e : mEnemies) {
            if (!e->isAlive()) continue;
            if (p.getSprite().getGlobalBounds().intersects(e->getSprite().getGlobalBounds())) {
                e->takeDamage(p.getDamage()); 
                if (!e->isAlive()) mMoney += e->getReward();
                p.destroy(); 
                break; 
            }
        }
    }

    mProjectiles.erase(std::remove_if(mProjectiles.begin(), mProjectiles.end(),
        [](const Projectile& p){return !p.isAlive();}), mProjectiles.end());
    mEnemies.erase(std::remove_if(mEnemies.begin(), mEnemies.end(),
        [](const std::unique_ptr<Enemy>& e){return !e->isAlive();}), mEnemies.end());
}

void Game::render() {
    mWindow.clear(sf::Color::Black);

    if (mCurrentMenu) {
        mCurrentMenu->draw(mWindow);
    } else {
        mMap.draw(mWindow);

        for (auto& e : mEnemies) e->draw(mWindow);
        for (auto& t : mTowers) t->draw(mWindow);
        for (auto& p : mProjectiles) p.draw(mWindow);

        // UI Loja
        AssetManager& assets = AssetManager::getInstance();
        std::vector<std::string> textures = {"archer_tower", "cannon_tower", "mage_tower"};
        for (size_t i = 0; i < mShopButtons.size(); ++i) {
            mShopButtons[i]->draw(mWindow);
            sf::Sprite icon(assets.getTexture(textures[i]));
            icon.setScale(0.4f, 0.4f);
            icon.setPosition(mShopButtons[i]->getPosition().x + 5, mShopButtons[i]->getPosition().y - 45);
            mWindow.draw(icon);
        }

        mWindow.draw(mMoneyUI);
        mWindow.draw(mLivesUI);

        // Feedback Ghosting
        if (mIsTowerSelected) {
            sf::Vector2i mPosI = sf::Mouse::getPosition(mWindow);
            sf::Vector2f mPosF = mWindow.mapPixelToCoords(mPosI);

            float range = 0.f;
            if (mSelectedTower == TowerType::Archer) range = 200.f;
            else if (mSelectedTower == TowerType::Cannon) range = 150.f;
            else if (mSelectedTower == TowerType::Mage) range = 350.f;

            sf::CircleShape rangeCircle(range);
            rangeCircle.setOrigin(range, range);
            rangeCircle.setPosition(mPosF);
            rangeCircle.setFillColor(sf::Color(255, 255, 255, 40));
            rangeCircle.setOutlineThickness(1.5f);
            rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 120));
            mWindow.draw(rangeCircle);
            
            std::string texName = (mSelectedTower == TowerType::Archer) ? "archer_tower" : 
                                 (mSelectedTower == TowerType::Cannon) ? "cannon_tower" : "mage_tower";
            
            sf::Sprite ghost(assets.getTexture(texName));
            ghost.setScale(1.0f, 1.0f);
            ghost.setOrigin(ghost.getLocalBounds().width/2, ghost.getLocalBounds().height/2);
            ghost.setPosition(mPosF);
            ghost.setColor(sf::Color(255, 255, 255, 150));
            mWindow.draw(ghost);
        }
    }
    mWindow.display();
}

void Game::setupDifficulty(int choice) {
    mTowers.clear();
    mEnemies.clear();
    mProjectiles.clear();
    mSpawnedCount = 0;
    mSpawnTimer = 0.f;

    if (choice == 0) { // EASY
        mDifficulty = Difficulty::Easy;
        mMap.load("assets/map_easy.txt");
        mMoney = 100;
        mLives = 20;
        mMaxSpawn = 20;
        mEnemySpeedMultiplier = 0.8f;
    } 
    else if (choice == 2) { // HARD
        mDifficulty = Difficulty::Hard;
        mMap.load("assets/map_hard.txt");
        mMoney = 200;
        mLives = 10;
        mMaxSpawn = 80;
        mEnemySpeedMultiplier = 1.3f;
    } 
    else { // NORMAL
        mDifficulty = Difficulty::Normal;
        mMap.load("assets/map_normal.txt");
        mMoney = 150;
        mLives = 20;
        mMaxSpawn = 50;
        mEnemySpeedMultiplier = 1.0f;
    }

    mMoneyUI.setString("GOLD: " + std::to_string(mMoney));
    mLivesUI.setString("LIFE: " + std::to_string(mLives));
}
