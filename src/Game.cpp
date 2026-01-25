#include "Game.hpp"
#include "AssetManager.hpp"
#include <iostream>
#include <algorithm>
#include "Tower.hpp" 

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
      mIsTowerSelected(false) // rastreia se estamos "carregando" uma torre
{
    mWindow.setFramerateLimit(60);

    AssetManager& assets = AssetManager::getInstance();
    // Inimigo
    assets.loadTexture("base_enemy",   "assets/golem.png");

    // Torres
    assets.loadTexture("archer_tower", "assets/torre1.png");
    assets.loadTexture("cannon_tower", "assets/torre2.png");
    assets.loadTexture("mage_tower",   "assets/torre3.png");

    // Projeteis
    assets.loadTexture("arrow",        "assets/stone.png");
    assets.loadTexture("cannonball",   "assets/lightning.png");
    assets.loadTexture("magic_bolt",   "assets/barrel.png");

    mFont.loadFromFile("assets/font.ttf");
    mNumberFont.loadFromFile("assets/numberFont.ttf");
    
    mMoneyUI.setFont(mNumberFont);
    mMoneyUI.setCharacterSize(20);
    mMoneyUI.setString("GOLD: " + std::to_string(mMoney));
    mMoneyUI.setFillColor(sf::Color::Yellow);
    mMoneyUI.setPosition(550.f, 20.f); 

    // Botões no Canto Inferior Direito
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
                mGameState = nextState;
                switch (mGameState) {
                    case MenuState::MainMenu:  
                        mCurrentMenu = std::make_unique<MainMenu>(mWindow); 
                        mGameState = MenuState::MainMenu;  
                        break;
                    case MenuState::Pause:     
                        mCurrentMenu = std::make_unique<PauseMenu>(mWindow); 
                        break;
                    case MenuState::MapEditor: 
                        mCurrentMenu = std::make_unique<MenuMapEditor>(mWindow); 
                        break;
                    case MenuState::Gameplay: {
                        int choice = 1; 
                        if (mCurrentMenu) {
                            DifficultyMenu* diffMenu = dynamic_cast<DifficultyMenu*>(mCurrentMenu.get());
                            if (diffMenu) {
                                choice = diffMenu->getSelectedDifficulty();
                            }
                        }
                        mCurrentMenu.reset(); 
                        setupDifficulty(choice); 
                        break;
                    }
                    case MenuState::GameOver:  
                        mCurrentMenu = std::make_unique<GameOverMenu>(mWindow);
                        mTowers.clear();
                        mEnemies.clear();
                        mProjectiles.clear();
                        mSpawnedCount = 0;
                        mMoney = 200; 
                        break;
                    case MenuState::Win:
                        mCurrentMenu = std::make_unique<WinMenu>(mWindow);
                        mTowers.clear(); mEnemies.clear(); 
                        mProjectiles.clear();
                        mSpawnedCount = 0; 
                        mMoney = 200; 
                        break;
                    case MenuState::Difficulty: {
                        mCurrentMenu = std::make_unique<DifficultyMenu>(mWindow);
                        mTowers.clear();
                        mEnemies.clear();
                        mProjectiles.clear();
                        mSpawnedCount = 0;
                        mSpawnTimer = 0.f;
                        mMoney = 200;  
                        mIsTowerSelected = false;
                        break;
                    }
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

    // 1. Verifica clique na Loja
    for (size_t i = 0; i < mShopButtons.size(); ++i) {
        if (mShopButtons[i]->clicked(mWindow, ev)) {
            int cost = (i == 0) ? 50 : (i == 1) ? 100 : 150;
            if (mMoney >= cost) {
                if (i == 0) mSelectedTower = TowerType::Archer;
                else if (i == 1) mSelectedTower = TowerType::Cannon;
                else if (i == 2) mSelectedTower = TowerType::Mage;
                
                mIsTowerSelected = true; // Agora uma torre está selecionada para posicionar
            }
            return; 
        }
    }

    // 2. Colocar Torre (Segundo Clique)
    if(ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left){
        sf::Vector2f mPos = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow));
        
        // Bloqueia clique se estiver em cima da interface da loja (evita colocar torre embaixo do botão)
        if ( (mPos.x > 580 && mPos.y > 450)) return;

        if (mIsTowerSelected) {
            int tileX = (int)mPos.x / TILE_SIZE;
            int tileY = (int)mPos.y / TILE_SIZE;

            if (tileX >= 0 && tileX < GRID_WIDTH && tileY >= 0 && tileY < GRID_HEIGHT) {
                if (mMap.getTileId(mPos.x, mPos.y) == 0) { 
                    int cost = (mSelectedTower == TowerType::Archer) ? 50 : 
                               (mSelectedTower == TowerType::Cannon) ? 100 : 150;

                    sf::Vector2f towerPos = {(float)tileX * TILE_SIZE + 20.f, (float)tileY * TILE_SIZE + 20.f };
                    
                    if (mSelectedTower == TowerType::Archer) mTowers.push_back(std::make_unique<ArcherTower>(towerPos));
                    else if (mSelectedTower == TowerType::Cannon) mTowers.push_back(std::make_unique<CannonTower>(towerPos));
                    else mTowers.push_back(std::make_unique<MageTower>(towerPos));

                    mMoney -= cost;
                    mIsTowerSelected = false; // Reset: torre posicionada
                }
            }
        }
    }
    // Clique direito cancela a seleção
    else if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Right) {
        mIsTowerSelected = false;
    }
}

void Game::update(float dt) {
    mSpawnTimer -= dt;
    const std::vector<sf::Vector2f>& path = mMap.getPath();
    if (mSpawnTimer <= 0.f && mSpawnedCount < mMaxSpawn) {
        if (!path.empty()) {
            float speed = 60.f * mEnemySpeedMultiplier;
            mEnemies.push_back(std::make_unique<Enemy>(path.front(), "base_enemy", 100, 10, speed, 20));
            mSpawnedCount++;
            mSpawnTimer = mSpawnInterval;
        } 
    }
    for (auto it = mEnemies.begin(); it != mEnemies.end(); ) {
        (*it)->update(dt, path);

        // Verifica se o inimigo chegou ao fim do caminho
        sf::Vector2f enemyPos = (*it)->getPosition();
        sf::Vector2f lastWaypoint = path.back();
        
        float diffX = lastWaypoint.x - enemyPos.x;
        float diffY = lastWaypoint.y - enemyPos.y;
        float distSq = diffX * diffX + diffY * diffY;

        // Se a distância for menor que 10 pixels (100 = 10 ao quadrado)
        if (distSq < 100.f && (*it)->isAlive()) {
            mGameState = MenuState::GameOver;
            mCurrentMenu = std::make_unique<GameOverMenu>(mWindow);
            return; 
        }

        if (!(*it)->isAlive()) {
            it = mEnemies.erase(it); // Remove se estiver morto (por tiro)
        } else {
            ++it;
        }
    }
    for (auto& tower : mTowers) {
        if (auto proj = tower->update(dt, mEnemies)) mProjectiles.push_back(*proj);
    }
    for (auto& proj : mProjectiles) proj.update(dt);
    for (auto& btn : mShopButtons) btn->update(mWindow);
    // verifica se houve vitoria 
    if (mSpawnedCount >= mMaxSpawn && mEnemies.empty()) {
        mGameState = MenuState::Win;
        mCurrentMenu = std::make_unique<WinMenu>(mWindow);
        return; 
    }
    handleCollisions();
    mMoneyUI.setString("GOLD: " + std::to_string(mMoney));
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
    mProjectiles.erase(std::remove_if(mProjectiles.begin(), mProjectiles.end(), [](const Projectile& p){return !p.isAlive();}), mProjectiles.end());
    mEnemies.erase(std::remove_if(mEnemies.begin(), mEnemies.end(), [](const std::unique_ptr<Enemy>& e){return !e->isAlive();}), mEnemies.end());
}

void Game::render() {
    mWindow.clear(sf::Color::Black); // Limpa com preto para não estragar o menu

    if (mCurrentMenu) {
        mCurrentMenu->draw(mWindow);
    } else {

        mMap.draw(mWindow); // O mapa desenha a grama verde e a estrada preta
        for (auto& e : mEnemies) e->draw(mWindow);
        for (auto& t : mTowers) t->draw(mWindow);
        for (auto& p : mProjectiles) p.draw(mWindow);

        // UI DA LOJA
        AssetManager& assets = AssetManager::getInstance();
        std::vector<std::string> textures = {"archer_tower", "cannon_tower", "mage_tower"};
        for (size_t i = 0; i < mShopButtons.size(); ++i) {
            mShopButtons[i]->draw(mWindow);
            sf::Sprite icon(assets.getTexture(textures[i]));
            icon.setScale(0.4f, 0.4f);
            icon.setPosition(mShopButtons[i]->getPosition().x + 5, mShopButtons[i]->getPosition().y - 45);
            mWindow.draw(icon);
        }

        mMoneyUI.setString("GOLD: " + std::to_string(mMoney));
        mWindow.draw(mMoneyUI);

        // Feedback de Posicionamento (Ghosting)
        if (mIsTowerSelected) {
            sf::Vector2i mPosI = sf::Mouse::getPosition(mWindow);
            sf::Vector2f mPosF = mWindow.mapPixelToCoords(mPosI);

            // Definindo raio de alcance 
            float range = 0.f;
            if (mSelectedTower == TowerType::Archer) range = 200.f;
            else if (mSelectedTower == TowerType::Cannon) range = 150.f;
            else if (mSelectedTower == TowerType::Mage) range = 350.f;

            // Desenho do raio de alcance
            sf::CircleShape rangeCircle(range);
            rangeCircle.setOrigin(range, range); // Centraliza a origem do círculo
            rangeCircle.setPosition(mPosF);
            rangeCircle.setFillColor(sf::Color(255, 255, 255, 40)); // Branco bem transparente
            rangeCircle.setOutlineThickness(1.5f);
            rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 120));
            mWindow.draw(rangeCircle);
            
            std::string texName = (mSelectedTower == TowerType::Archer) ? "archer_tower" : 
                                 (mSelectedTower == TowerType::Cannon) ? "cannon_tower" : "mage_tower";
            
            sf::Sprite ghost(assets.getTexture(texName));
            ghost.setScale(1.0f, 1.0f);
            ghost.setOrigin(ghost.getLocalBounds().width/2, ghost.getLocalBounds().height/2);
            ghost.setPosition(mPosF);
            ghost.setColor(sf::Color(255, 255, 255, 150)); // Semitransparente
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

    if (choice == 0) { // EASY
        mMap.load("assets/map_easy.txt");
        mMoney = 200;
        mMaxSpawn = 20;
        mEnemySpeedMultiplier = 0.8f; // Crie essa variável para afetar os inimigos
    } 
    else if (choice == 2) { // HARD
        mMap.load("assets/map_hard.txt");
        mMoney = 100;
        mMaxSpawn = 100;
        mEnemySpeedMultiplier = 1.3f;
    } 
    else { // NORMAL
        mMap.load("assets/map_normal.txt");
        mMoney = 150;
        mMaxSpawn = 50;
        mEnemySpeedMultiplier = 1.0f;
    }
}